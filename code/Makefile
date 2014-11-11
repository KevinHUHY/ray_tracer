CPP = clang++
LD = clang++
EIGEN_DIR = eigen3.2.2/
CPPFLAGS = -O3 -fPIC -I$(EIGEN_DIR) -DEIGEN_PERMANENTLY_DISABLE_STUPID_WARNINGS -DEIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET
LDFLAGS = -O3
INCLUDE = -Iinclude/
LIB = -lpng
TARGET = ray_tracer.out
OBJS = objs/main.o objs/image.o objs/object.o objs/scene.o objs/ray_tracer.o

default: $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) $(LIB) -o $(TARGET)
	
objs/main.o: main.cpp
	$(CPP) -c $(CPPFLAGS) $(INCLUDE) main.cpp -o objs/main.o

objs/image.o: Image.cpp Image.h
	$(CPP) -c $(CPPFLAGS) $(INCLUDE) Image.cpp -o objs/image.o

objs/object.o: Object.cpp Object.h
	$(CPP) -c $(CPPFLAGS) $(INCLUDE) Object.cpp -o objs/object.o

objs/ray_tracer.o: RayTracer.cpp RayTracer.h
	$(CPP) -c $(CPPFLAGS) $(INCLUDE) RayTracer.cpp -o objs/ray_tracer.o

objs/scene.o: Scene.cpp Scene.h
	$(CPP) -c $(CPPFLAGS) $(INCLUDE) Scene.cpp -o objs/scene.o

clean:
	rm -f $(OBJS) $(TARGET)
