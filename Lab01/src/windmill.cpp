#include <windmill.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <random>

namespace dng {

Windmill::Windmill(float angle, float speed, int numBlades)
    : bladesVisible(numBlades, true), angle(angle), speed(speed), numBlades(numBlades) {
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
    base->setKa(ka);
    axle->setKa(ka);
    blade->setKa(ka);

    base->setKd(kd);
    axle->setKd(kd);
    blade->setKd(kd);

    base->setKs(ks);
    axle->setKs(ks);
    blade->setKs(ks);

    base->setSh(sh);
    axle->setSh(sh);
    blade->setSh(sh);

    base->setModelViewN(modelViewN);
    axle->setModelViewN(modelViewN);
    blade->setModelViewN(modelViewN);

    base->setKaToShader(kaParameter);
    axle->setKaToShader(kaParameter);
    blade->setKaToShader(kaParameter);

    base->setKdToShader(kdParameter);
    axle->setKdToShader(kdParameter);
    blade->setKdToShader(kdParameter);

    base->setKsToShader(ksParameter);
    axle->setKsToShader(ksParameter);
    blade->setKsToShader(ksParameter);

    base->setShToShader(shParameter);
    axle->setShToShader(shParameter);
    blade->setShToShader(shParameter);

    base->setModelMatrixParamToShader(modelParameter);
    axle->setModelMatrixParamToShader(modelParameter);
    blade->setModelMatrixParamToShader(modelParameter);

    base->setModelViewNMatrixParamToShader(modelViewNParameter);
    axle->setModelViewNMatrixParamToShader(modelViewNParameter);
    blade->setModelViewNMatrixParamToShader(modelViewNParameter);

    base->setColor(color[0], color[1], color[2]);
    axle->setColor(color[0], color[1], color[2]);
    blade->setColor(color[0], color[1], color[2]);

    base->setModel(model);
    base->render();

    glm::mat4 spinOffset = model * glm::rotate(axleOffset, angle, glm::vec3(1.0, 0.0, 0.0));
    axle->setModel(spinOffset);
    axle->render();

    for (int i = 0; i < numBlades; i++) {
        if (bladesVisible[i]) {
            blade->setModel(spinOffset * bladesOffset[i]);
            blade->render();
        }
    }
}

void Windmill::update(float deltaT) {
    angle -= speed * deltaT;
}

void Windmill::init() {
    base.emplace("models/windmillbase.obj");
    axle.emplace("models/axle.obj");
    blade.emplace("models/blade.obj");
}

std::optional<ObjModel> Windmill::base;
std::optional<ObjModel> Windmill::axle;
std::optional<ObjModel> Windmill::blade;

}  // namespace dng
