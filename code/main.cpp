#include <iostream>
// #include "Image.h"
#include "RayTracer.h"

using namespace std;
using namespace Eigen;

int main() {

  Image image(600,600);

  Vector3d c0(0, 0, -4);
  Color color0(0.5, 0.0, 0.0);
  Object* obj0 = new Sphere(c0, 2, color0);
  // obj0 -> set_optical_property(true, false);
  obj0 -> set_optical_property(true, true);

  // Vector3d c1(0, 0, -9);
  // Color color6(0.0, 0.0, 0.5);
  // Object* obj6 = new Sphere(c1, 0.5, color6);
  // // obj0 -> set_optical_property(true, false);
  // obj6 -> set_optical_property(true, false);

  
  // bottom
  Vector3d n0(0, 1, 0);
  Vector3d p0(0, -5, 0);
  Color color1(0.2, 0.0, 0.5);
  Object* obj1 = new Plane(n0, p0, color1);
  obj1 -> set_optical_property(false, false);

  // back
  Vector3d n1(0, 0, 1);
  Vector3d p1(0, 0, -10);
  Color color2(0.2, 0.5, 0.0);
  Object* obj2 = new Plane(n1, p1, color2);
  obj2 -> set_optical_property(false, false);

  // up
  Vector3d n2(0, -1, 0);
  Vector3d p2(0, 5, 0);
  Color color3(0.2, 0.0, 0.5);
  Object* obj3 = new Plane(n2, p2, color3);
  obj3 -> set_optical_property(false, false);

  // left
  Vector3d n3(1, 0, 0);
  Vector3d p3(-5, 0, 0);
  Color color4(0.25, 0.25, 0.44);
  Object* obj4 = new Plane(n3, p3, color4);
  obj4 -> set_optical_property(false, false);

  // right
  Vector3d n4(-1, 0, 0);
  Vector3d p4(5, 0, 0);
  Color color5(0.25, 0.25, 0.44);
  Object* obj5 = new Plane(n4, p4, color5);
  obj5 -> set_optical_property(false, false);

  Scene scene;
  scene.add_object(obj0);
  scene.add_object(obj1);
  scene.add_object(obj2);
  scene.add_object(obj3);
  scene.add_object(obj4);  
  scene.add_object(obj5);
  // scene.add_object(obj6);
  RayTracer ray_tracer(&image, scene);
  ray_tracer.ray_trace();
  image.writePNG("test.png");

  return 0;
}