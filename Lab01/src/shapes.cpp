#include <shapes.h>
#include <sphere.h>
#include <cube.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace dng {

// help procedure that send values from glm::vec3 to a STL vector of float
// used for creating VBOs
void Shapes::addVertex(std::vector<GLfloat>* a, const glm::vec3* v) {
    a->push_back(v->x);
    a->push_back(v->y);
    a->push_back(v->z);
}

void Shapes::render() {
    std::cout << "Base class cannot render\n";
}

void Shapes::setModel(glm::mat4 tmp) {
    model = tmp;
}

void Shapes::setModelViewN(glm::mat3 tmp) {
    modelViewN = tmp;
}

void Shapes::setModelMatrixParamToShader(GLuint uniform) {
    modelParameter = uniform;
}

void Shapes::setModelViewNMatrixParamToShader(GLuint uniform) {
    modelViewNParameter = uniform;
}

void Shapes::setColor(GLubyte r, GLubyte b, GLubyte g) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

}  // namespace dng
