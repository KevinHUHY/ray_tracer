#ifndef OBJECT_H
#define OBJECT_H

#include <Eigen/Dense>
#define EPSILON 0.00001

using namespace Eigen;

struct Ray {
	Vector3d origin;
	Vector3d direction;
};

struct IntersectPoint {
	Vector3d position;
	Vector3d normal;
};

typedef Vector3d Color;

// struct Color {
// 	float r, g, b;
// 	Color() {}
// 	Color(float red, float green, float blue)
// 		:r(red), g(green), b(blue) {}
// };

class Object {
public:
	virtual bool intersect(const Ray& ray, IntersectPoint* ip) const = 0;
	virtual Color get_color() const;
	virtual void set_color(const Color& clr);
	void set_optical_property(bool s, bool t);
	bool is_specular() const;
	bool is_transparent() const;
protected:
	Color color;
	bool specular;
	bool transparent;
};

class Sphere : public Object {
public:
	// Sphere();
	Sphere(const Vector3d& c, double r, const Color& clr);
	virtual bool intersect(const Ray& ray, IntersectPoint* ip) const;

private:
	// Color color;
	Vector3d center;
	double radius;
};

class Plane : public Object {
public:
	Plane(const Vector3d& normal_p, const Vector3d& point, const Vector3d& clr);
	Plane(double a_p, double b_p, double c_p, double d_p, const Color& clr);
	virtual bool intersect(const Ray& ray, IntersectPoint* ip) const;

private:
	double a, b, c, d;
	Vector3d normal;
};



#endif