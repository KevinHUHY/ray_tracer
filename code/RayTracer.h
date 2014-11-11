#ifndef RAYTRACER_H
#define RAYTRACER_H
#include "Image.h"
#include "Scene.h"

class RayTracer {
public:
	// RayTracer();
	RayTracer(Image* img, const Scene& scn);
	// void bind_image(Image* img);
	// void bind_scene(const Scene& scn);
	void ray_trace();

private:
	Image* image;
	Scene scene;
	static const int MAX_RECUR_TIME = 10;
	Color compute_point_color(const Ray& ray, const Object* hit_obj, const IntersectPoint& ip);	
	Color compute_reflection_color(const Ray& ray, const IntersectPoint& ip, int times);
	Color compute_refraction_color(const Ray& ray, const IntersectPoint& ip, const Object* hit_obj, int times);	
	Color compute_color(const Ray& ray, int times);
};


#endif