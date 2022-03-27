#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace dng {

struct Material {
    void setKa(glm::vec3 amb) { ka = amb; }
    void setKd(glm::vec3 diff) { kd = diff; }
    void setKs(glm::vec3 spec) { ks = spec; }
    void setSh(float sh) { this->sh = sh; }

    glm::vec3 ka;        // ambient
    glm::vec3 kd;        // diffuse
    glm::vec3 ks;        // specular
    float sh;            // shineness
};

}  // namespace dng

#endif  //__MATERIAL_H__
