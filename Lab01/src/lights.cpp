#include <lights.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

LightC::LightC() {
    lPos = glm::vec4(0, 0, 0, 1);
    la = glm::vec3(0, 0, 0);
    ls = glm::vec3(1, 1, 1);
    ld = glm::vec3(0.7, 0.7, 0.7);
}

void LightC::SetShaders() {
    glUniform4fv(lPosParameter, 1, glm::value_ptr(lPos));
    glUniform3fv(laParameter, 1, glm::value_ptr(la));
    glUniform3fv(ldParameter, 1, glm::value_ptr(ld));
    glUniform3fv(lsParameter, 1, glm::value_ptr(ls));
}
