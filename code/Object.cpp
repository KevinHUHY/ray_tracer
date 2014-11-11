#include "Object.h"

#include <iostream>
using namespace std;


Color Object::get_color() const
{
	return color;
}

void Object::set_color(const Color& clr)
{
	color = clr;
	// color[0] = clr[0];
	// color[1] = clr[1];
	// color[2] = clr[2];
}

void Object::set_optical_property(bool s, bool t)
{
	specular = s;
	transparent = t;
}

bool Object::is_specular() const
{
	return specular;
}

bool Object::is_transparent() const
{
	return transparent;
}

Sphere::Sphere(const Vector3d& c, double r, const Color& clr)
	: center(c), radius(r)
{
	set_color(clr);
}

bool Sphere::intersect(const Ray& ray, IntersectPoint* ip) const
{
	double a = ray.direction.squaredNorm();
	double b = 2 * ray.direction.dot(ray.origin - center);
	double c = (ray.origin - center).squaredNorm() - radius * radius;

	double delta = b * b - 4 * a * c;
	
	if(delta < EPSILON) {
		return false;
	}

	double t0 = (-b - sqrt(delta)) / (2 * a);
	double t1 = (-b + sqrt(delta)) / (2 * a);
	double t;
	if(t1 < EPSILON) {
		return false;
	}
	if(t0 > EPSILON) {
		t = t0;
	} else {
		t = t1;
	}

	ip -> position = ray.origin + t * ray.direction;
	ip -> normal = (ip -> position - center).normalized();
	return true;
}

Plane::Plane(const Vector3d& normal_p, const Vector3d& point, const Vector3d& clr)
{
	normal = normal_p;
	a = normal[0];
	b = normal[1];
	c = normal[2];
	d = -normal.dot(point);
	set_color(clr);
}

Plane::Plane(double a_p, double b_p, double c_p, double d_p, const Color& clr)
	: a(a_p), b(b_p), c(c_p), d(d_p)
{
	set_color(clr);
	normal[0] = a;
	normal[1] = b;
	normal[2] = c;
}

bool Plane::intersect(const Ray& ray, IntersectPoint* ip) const
{
	// std::cerr << "Checked???" << std::endl;
	Vector3d v_line(a, b, c);
	double coef = v_line.dot(ray.direction);
	if(coef < EPSILON && coef > -EPSILON) {
		return false;
	}
	double cons = d + v_line.dot(ray.origin);
	double t = -(cons / coef);
	if(t < EPSILON) {
		return false;
	}
	ip -> position = ray.origin + t * ray.direction;
	ip -> normal = normal.normalized();
	// cerr << "ture?" << endl;
	return true;
}