#include <windmill.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <random>

namespace dng {

Windmill::Windmill(float angle, float speed, int numBlades)
    : base("models/windmillbase.obj"),
      axle("models/axle.obj"),
      blade("models/blade.obj"),
      bladesVisible(numBlades, true),
      angle(angle),
      speed(speed),
      numBlades(numBlades) {
    baseOffset = glm::mat4(1.0);
    axleOffset = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 1.65, 0.0));
    float angleOffset = 360.0f / numBlades;
    bladesOffset.reserve(numBlades);
    for (int i = 0; i < numBlades; i++) {
        bladesOffset.push_back(glm::rotate(glm::translate(glm::mat4(1.0), glm::vec3(0.5, 0.0, 0.0)),
                                           angleOffset * i, glm::vec3(1.0, 0.0, 0.0)));
    }
}

void Windmill::render() {
    base.render();

    glm::mat4 spinOffset = model * glm::rotate(axleOffset, angle, glm::vec3(1.0, 0.0, 0.0));
    axle.setModel(spinOffset);
    axle.render();

    for (int i = 0; i < numBlades; i++) {
        if (bladesVisible[i]) {
            blade.setModel(spinOffset * bladesOffset[i]);
            blade.render();
        }
    }
}

void Windmill::update(float deltaT) {
    angle -= speed * deltaT;
}

void Windmill::setKa(glm::vec3 amb) {
    base.setKa(amb);
    axle.setKa(amb);
    blade.setKa(amb);
    ka = amb;
}

void Windmill::setKd(glm::vec3 diff) {
    base.setKd(diff);
    axle.setKd(diff);
    blade.setKd(diff);
    kd = diff;
}

void Windmill::setKs(glm::vec3 spec) {
    base.setKs(spec);
    axle.setKs(spec);
    blade.setKs(spec);
    ks = spec;
}

void Windmill::setSh(float sh) {
    base.setSh(sh);
    axle.setSh(sh);
    blade.setSh(sh);
    this->sh = sh;
}

void Windmill::setKaToShader(GLuint uniform) {
    base.setKaToShader(uniform);
    axle.setKaToShader(uniform);
    blade.setKaToShader(uniform);
    kaParameter = uniform;
}

void Windmill::setKdToShader(GLuint uniform) {
    base.setKdToShader(uniform);
    axle.setKdToShader(uniform);
    blade.setKdToShader(uniform);
    kdParameter = uniform;
}
void Windmill::setKsToShader(GLuint uniform) {
    base.setKsToShader(uniform);
    axle.setKsToShader(uniform);
    blade.setKsToShader(uniform);
    ksParameter = uniform;
}

void Windmill::setShToShader(GLuint uniform) {
    base.setShToShader(uniform);
    axle.setShToShader(uniform);
    blade.setShToShader(uniform);
    shParameter = uniform;
}

void Windmill::setModelMatrixParamToShader(GLuint uniform) {
    base.setModelMatrixParamToShader(uniform);
    axle.setModelMatrixParamToShader(uniform);
    blade.setModelMatrixParamToShader(uniform);
    modelParameter = uniform;
}

void Windmill::setModelViewNMatrixParamToShader(GLuint uniform) {
    base.setModelViewNMatrixParamToShader(uniform);
    axle.setModelViewNMatrixParamToShader(uniform);
    blade.setModelViewNMatrixParamToShader(uniform);
    modelViewNParameter = uniform;
}

void Windmill::setModel(glm::mat4 tmp) {
    base.setModel(tmp * baseOffset);
    model = tmp;
}

void Windmill::setModelViewN(glm::mat3 tmp) {
    base.setModelViewN(tmp);
    axle.setModelViewN(tmp);
    blade.setModelViewN(tmp);
    modelViewN = tmp;
}

void Windmill::setColor(GLubyte r, GLubyte b, GLubyte g) {
    base.setColor(r, g, b);
    axle.setColor(r, g, b);
    blade.setColor(r, g, b);
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

}  // namespace dng
