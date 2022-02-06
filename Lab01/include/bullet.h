#ifndef __BULLET_H__
#define __BULLET_H__

#include <obj_model.h>
#include <shapes.h>
#include <list>
#include <memory>
#include <glm/glm.hpp>

namespace dng {

class Bullet : public ObjModel {
public:
    Bullet(glm::vec3 vec, float timeToLive = 10.0f);
    virtual void update(float deltaT);
private:
    glm::vec3 vec;
    float timeToLive;
};

}  // namespace dng

#endif  // __BULLET_H__
