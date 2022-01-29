#ifndef __SHAPES_H__
#define __SHAPES_H__

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace dng {

class Material {
public:
    void SetKa(glm::vec3 amb) { ka = amb; }
    void SetKd(glm::vec3 diff) { kd = diff; }
    void SetKs(glm::vec3 spec) { ks = spec; }
    void SetSh(float sh) { this->sh = sh; }
    virtual void SetKaToShader(GLuint uniform) { kaParameter = uniform; }
    virtual void SetKdToShader(GLuint uniform) { kdParameter = uniform; };
    virtual void SetKsToShader(GLuint uniform) { ksParameter = uniform; };
    virtual void SetShToShader(GLuint uniform) { shParameter = uniform; };

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

// Base class for shapes
class Shapes : public Material {
public:
    virtual void SetModelMatrixParamToShader(GLuint uniform);
    virtual void SetModelViewNMatrixParamToShader(GLuint uniform);
    virtual void SetModel(glm::mat4 tmp);
    virtual void SetModelViewN(glm::mat3 tmp);  // 3x3 matrix!
    virtual void SetColor(GLubyte r, GLubyte b, GLubyte g);
    virtual void Render();

protected:
    GLuint modelParameter;  // shader uniform variables
    GLuint modelViewNParameter;
    glm::mat4 model;       // modeling matrix
    glm::mat3 modelViewN;  // model view normals matrix
    GLubyte color[3];
    std::vector<GLfloat>* a;
    std::vector<GLfloat> vertex;
    std::vector<GLfloat> normal;
    GLuint vboHandles[2];  // vertices and normals
    GLuint vaID;
    GLuint buffer;
    GLuint points;
    GLuint normals;
};

// derived class from Shapes
class Sphere : public Shapes {
public:
    Sphere();
    Sphere(int stacks, int slices, GLfloat r);
    virtual void Render();

private:
    GLuint stacks, slices;
    GLfloat r;
    void Sphere::Generate(int stacks, int slices, GLfloat r);
    void InitArrays();
    void Generate();
};

// derived class from Shapes
class CubeC : public Shapes {
public:
    CubeC();
    virtual void Render();

private:
    void InitArrays();
    void Generate();
};

}  // namespace dng

#endif  // __SHAPES_H__
