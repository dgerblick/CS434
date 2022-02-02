#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace dng {

class Material {
public:
    virtual void setKa(glm::vec3 amb) { ka = amb; }
    virtual void setKd(glm::vec3 diff) { kd = diff; }
    virtual void setKs(glm::vec3 spec) { ks = spec; }
    virtual void setSh(float sh) { this->sh = sh; }
    virtual void setKaToShader(GLuint uniform) { kaParameter = uniform; }
    virtual void setKdToShader(GLuint uniform) { kdParameter = uniform; };
    virtual void setKsToShader(GLuint uniform) { ksParameter = uniform; };
    virtual void setShToShader(GLuint uniform) { shParameter = uniform; };

protected:
    glm::vec3 ka;        // ambient
    glm::vec3 kd;        // diffuse
    glm::vec3 ks;        // specular
    float sh;            // shineness
    GLuint kaParameter;  // shader uniform variables
    GLuint kdParameter;
    GLuint ksParameter;
    GLuint shParameter;
};

}  // namespace dng

#endif  //__MATERIAL_H__
