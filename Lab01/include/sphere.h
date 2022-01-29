#ifndef __SPHERE_H__
#define __SPHERE_H__

#include <shapes.h>

namespace dng {

// derived class from Shapes
class Sphere : public Shapes {
public:
    Sphere();
    Sphere(int stacks, int slices, GLfloat r);
    virtual void render();

private:
    GLuint stacks, slices;
    GLfloat r;
    void Sphere::generate(int stacks, int slices, GLfloat r);
    void initArrays();
    void generate();
};

}  // namespace dng

#endif  // __SPHERE_H__
