#ifndef __BULLET_H__
#define __BULLET_H__

#include <obj_model.h>
#include <shapes.h>
#include <list>
#include <memory>
#include <glm/glm.hpp>

namespace dng {

class Bullets : public ObjModel {
public:
    Bullets();
    virtual void render(std::vector<std::unique_ptr<Shapes>>& shapes);
    virtual void update(float deltaT);
};

}  // namespace dng

#endif  // __BULLET_H__
