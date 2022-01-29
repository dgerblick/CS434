#ifndef __SHAPES_H__
#define __SHAPES_H__

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <material.h>

namespace dng {

// Base class for shapes
class Shapes : public Material {
public:
    virtual void setModelMatrixParamToShader(GLuint uniform);
    virtual void setModelViewNMatrixParamToShader(GLuint uniform);
    virtual void setModel(glm::mat4 tmp);
    virtual void setModelViewN(glm::mat3 tmp);  // 3x3 matrix!
    virtual void setColor(GLubyte r, GLubyte b, GLubyte g);
    virtual void render();
    static void addVertex(std::vector<GLfloat>* a, const glm::vec3* v);

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

}  // namespace dng

#endif  // __SHAPES_H__
