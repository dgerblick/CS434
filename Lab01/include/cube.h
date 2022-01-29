#ifndef __CUBE_H__
#define __CUBE_H__

#include <shapes.h>

namespace dng {

// derived class from Shapes
class Cube : public Shapes {
public:
    Cube();
    virtual void render();

private:
    void initArrays();
    void generate();
};

}  // namespace dng

#endif  // __CUBE_H__
