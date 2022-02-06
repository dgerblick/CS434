#include <bullet.h>
#include <glm/gtc/matrix_transform.hpp>

namespace dng {

Bullet::Bullet(glm::vec3 vec, float timeToLive)
    : ObjModel("models/bullet.obj"), vec(vec), timeToLive(timeToLive) {
}

void Bullet::update(float deltaT) {
    model = glm::translate(model, deltaT * vec);
    timeToLive -= deltaT;
    if (timeToLive < 0)
        deleteShape();
}

}  // namespace dng
