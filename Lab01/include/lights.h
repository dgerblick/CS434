#ifndef __LIGHTS_H__
#define __LIGHTS_H__

#include <GL/glew.h>
#include <glm/glm.hpp>

class LightC {
public:
    LightC();
    void SetPos(glm::vec4 par) { lPos = par; };
    void SetLa(glm::vec3 par) { la = par; };
    void SetLs(glm::vec3 par) { ls = par; };
    void SetLd(glm::vec3 par) { ld = par; };
    void SetLaToShader(GLuint uniform) { laParameter = uniform; };
    void SetLdToShader(GLuint uniform) { ldParameter = uniform; };
    void SetLsToShader(GLuint uniform) { lsParameter = uniform; };
    void SetLposToShader(GLuint uniform) { lPosParameter = uniform; };
    void SetShaders();

private:
    glm::vec4 lPos;      // position
    glm::vec3 la;        // ambient
    glm::vec3 ld;        // diffuse
    glm::vec3 ls;        // specular
    GLuint laParameter;  // shader uniform variables
    GLuint ldParameter;
    GLuint lsParameter;
    GLuint lPosParameter;
};

#endif  // __LIGHTS_H__