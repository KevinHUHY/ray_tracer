#ifndef SCENE_H
#define SCENE_H

#include "Object.h"
// #include "Image.h"
#include <vector>
#include <Eigen/Dense>
using std::vector;
using namespace Eigen;

typedef vector<const Object*> Objects;

class Scene {
	friend class RayTracer;
public:
	Scene();
	// Scene(Vector3d& light, double sn);
	// ~Scene();

	// void set_lights(const Vector3d& light);
	void add_object(const Object* objP);

private:
	Vector3d lights[2];
	int light_num;
	double shineness;
	Objects objects;
};



#endif