#include "RayTracer.h"
#include <cstdlib>
#include <iostream>
using std::cerr;
using std::endl;
// RayTracer::RayTracer()
//  	: image(0) {}

RayTracer::RayTracer(Image* img, const Scene& scn)
	:	image(img), scene(scn) {}

// void RayTracer::bind_image(image* )

Ray computePrimRay(int x, int y, double pixel_size)
{
	// use fixed viewport model, can be generalized
	Ray ret;
	// default eye_position is (0, 0, 0);
	ret.origin[0] = 0;
	ret.origin[1] = 0;
	ret.origin[2] = 0;

	ret.direction[0] = (x + 0.5) * pixel_size - 1;
	ret.direction[1] = 1 - (y + 0.5) * pixel_size;
	ret.direction[2] = -1;
	return ret;
}


const Object* closest_hit(Objects objs, const Ray& ray, IntersectPoint* ip_p)
{
	const Object* ret = 0;
	IntersectPoint ip;
	double smallest_dis = 1000000;
	int i = 0;

	for(Objects::iterator it = objs.begin(); it != objs.end(); ++it) {
		if((*it) -> intersect(ray, &ip)) {
			double hit_dis = sqrt((ray.origin - ip.position).squaredNorm());
			if(hit_dis < smallest_dis) {
				smallest_dis = hit_dis;
				ret = (*it);
			}
		}
		i++;
	}
	if(ret != 0) {
		ret -> intersect(ray, ip_p);	
	}
	return ret;
}

bool check_shadow(Objects objs, Ray& ray, double dis2light_square)
{
	IntersectPoint ip;
	int i = 0;
	for(Objects::iterator it = objs.begin(); it != objs.end(); ++it) {
		if((*it) -> intersect(ray, &ip)) {
			double dis2obj_square = (ray.origin - ip.position).squaredNorm();
			if(dis2obj_square < dis2light_square) {
				return true;				
			}
		}
		++i;
	}
	return false;
}

Vector3d reflect(Vector3d in, Vector3d n)
{
	if(in.dot(n) >= 0) {
		n = (-n);
	}
	in = in.normalized();
	n = n.normalized();
	return (in - 2 * (in.dot(n)) * n);
}

Vector3d memberwise_mul(const Vector3d& v1, const Vector3d& v2)
{
	Vector3d v0;
	v0[0] = v1[0] * v2[0];
	v0[1] = v1[1] * v2[1];
	v0[2] = v1[2] * v2[2];
	return v0;
}

Color ambient_mode(const Color& ambient_color)
{
	Vector3d ambient_light(1.0, 1.0, 1.0);
	Color Ia = memberwise_mul(ambient_color, ambient_light);
	return Ia;
}

Color phong_mode(const Color& ambient_diffuse_color, 
								 const Color& specular_color, 
								 const Ray& prim_ray, 
								 const Ray& shadow_ray,
								 const Vector3d& normal,
								 double shineness)
{
	Vector3d v = -(prim_ray.direction.normalized());
	Vector3d l = shadow_ray.direction.normalized();
	Vector3d n = normal.normalized();
	Vector3d r = reflect(-l, n);

	Vector3d ambient_light(1.0, 1.0, 1.0);
	Vector3d diffuse_light(1.0, 1.0, 1.0);
	Vector3d specular_light(1.0, 1.0, 1.0);
	
	Color Ia = memberwise_mul(ambient_diffuse_color, ambient_light);
	Color Id = l.dot(n) * memberwise_mul(ambient_diffuse_color, diffuse_light);
	double dot_rv = r.dot(v);
	if(dot_rv < 0) {
		dot_rv = 0;
	}
	Color Is = pow(dot_rv, shineness) * memberwise_mul(specular_color, specular_light);
	// return Ia;
	// return Id;
	return (Ia + Id + Is);
}

bool refract(Vector3d ri, Vector3d normal, Vector3d* rt, double eta_i, double eta_t)
{
	ri = ri.normalized();
	normal = normal.normalized();
	if(ri.dot(normal) > 0) {
		normal = (-normal);
	}
	double cos_i = -ri.dot(normal);
	double sin_i = sqrt(1 - cos_i * cos_i);
	double sin_t = eta_i * sin_i / eta_t;
	if(sin_t >= 1 + EPSILON) {
		return false;
	}
	(*rt) = -sqrt(1 - pow(eta_i * sin_i / eta_t, 2)) * normal 
					+ eta_i / eta_t * (ri + cos_i * normal);
	return true;
}

bool transmit(const Ray& in, const IntersectPoint& ip, Ray* out, 
							double eta_i, double eta_t, const Object* hit_obj)
{
	Ray inside_ray;
	inside_ray.origin = ip.position;
	if(!refract(in.direction, ip.normal, &(inside_ray.direction), eta_i, eta_t)) {
		return false;
	}
	IntersectPoint out_point;
	hit_obj -> intersect(inside_ray, &out_point);
	if(!refract(inside_ray.direction, out_point.normal, &(out -> direction), eta_t, eta_i)) {
		return false;
	}
	out -> origin = out_point.position;
	return true;
}

Color RayTracer::compute_refraction_color(const Ray& ray, const IntersectPoint& ip,
																					const Object* hit_obj, int times)
{
	if(times >= MAX_RECUR_TIME) {
		return Color(0, 0, 0);
	}
	Ray out_ray;
	if(!transmit(ray, ip, &out_ray, 1, 1.3, hit_obj)) {
		return Color(0, 0, 0);
	}
	return compute_color(out_ray, times+1);
}

Color RayTracer::compute_reflection_color(const Ray& ray, const IntersectPoint& ip, int times)
{
	if(times >= MAX_RECUR_TIME) {
		return Color(0, 0, 0);
	}
	Vector3d reflect_direction = reflect(ray.direction, ip.normal);
	Ray reflected_ray;
	reflected_ray.origin = ip.position;
	reflected_ray.direction = reflect_direction;
	return compute_color(reflected_ray, times+1);
}

Color RayTracer::compute_point_color(const Ray& ray, 
																		 const Object* hit_obj, 
																		 const IntersectPoint& ip)
{
	Color final_color(0, 0, 0);
	for(int i = 0; i < scene.light_num; ++i) {
		Ray shadow_ray;
		shadow_ray.origin = ip.position;
		shadow_ray.direction = scene.lights[i] - ip.position;
		double dis2light_square = shadow_ray.direction.squaredNorm();
		bool is_shadow = check_shadow(scene.objects, shadow_ray, dis2light_square);

		Color color = hit_obj -> get_color();
		if(is_shadow) {
			color = ambient_mode(color);
		} else {
			Color specular_color(0.8, 0.8, 0.8);
			color = phong_mode(color, specular_color, 
												 ray, shadow_ray, ip.normal, 
												 scene.shineness);
		}
		final_color += color;
	}
	return final_color;
}


Color RayTracer::compute_color(const Ray& ray, int times)
{
	IntersectPoint ip;
	const Object* hit_obj = closest_hit(scene.objects, ray, &ip);
	if(hit_obj == 0) {
		return Color(0, 0, 0);
	}
	Color point_color = compute_point_color(ray, hit_obj, ip);
	Color reflection_color(0.0, 0.0, 0.0);
	if(hit_obj -> is_specular()) {
		reflection_color = compute_reflection_color(ray, ip, times);
	}
	Color refraction_color(0.0, 0.0, 0.0);
	if(hit_obj -> is_transparent()) {
		refraction_color = compute_refraction_color(ray, ip, hit_obj, times);
		if(times == 0) {
			return 0.5 * refraction_color + 0.5 * reflection_color;
		}
	}
	return (0.5 * point_color + 0.3 * reflection_color + 0.2 * refraction_color);
}

void RayTracer::ray_trace()
{
	if(image == 0) {
		cerr << "No output image for RayTracer." << endl;
		exit(EXIT_FAILURE);
	}
	
	int image_width = image -> getWidth();	
	int image_height = image -> getHeight();
	if(image_width != image_height) {
		cerr << "Width must be equal to Height in this version." << endl;
		exit(EXIT_FAILURE);
	}
	int image_size = image_width;
	double pixel_size = 2.0 / image_size;
	for(int x = 0; x < image_size; ++x) {
		for(int y = 0; y < image_size; ++y) {
			Ray prim_ray = computePrimRay(x, y, pixel_size);
			Color final_color = compute_color(prim_ray, 0);
			image -> setColor(x, y, final_color[0], final_color[1], final_color[2]);
		}
	}
}