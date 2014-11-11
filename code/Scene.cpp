#include "Scene.h"

Scene::Scene() 
{
	// light0
	lights[0][0] = 3;
	lights[0][1] = 3;
	lights[0][2] = 0;
	
	// light1
	lights[1][0] = -3;
	lights[1][1] = 0;
	lights[1][2] = 3;
	
	light_num = 2;
	shineness = 15;
}

// Scene::Scene(Vector3d& light, double sn)
// 	:lights(light), shineness(sn) {}

// void Scene::set_lights(const Vector3d& light)
// {
// 	lights = light;
// }

void Scene::add_object(const Object* objP)
{
	objects.push_back(objP);
}