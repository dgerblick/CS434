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

void Shapes::render() {
    std::cout << "Base class cannot render\n";
}

void Shapes::update(float deltaT) {
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

void Shapes::initArrays() {
    points = vertex.size();
    normals = normal.size();

    // get the vertex array handle and bind it
    glGenVertexArrays(1, &vaID);
    glBindVertexArray(vaID);

    // the vertex array will have two vbos, vertices and normals
    glGenBuffers(2, vboHandles);
    GLuint verticesID = vboHandles[0];
    GLuint normalsID = vboHandles[1];

    // send vertices
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glBufferData(GL_ARRAY_BUFFER, points * sizeof(GLfloat), &vertex[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint) 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    vertex.clear();  // no need for the vertex data, it is on the GPU now

    // send normals
    glBindBuffer(GL_ARRAY_BUFFER, normalsID);
    glBufferData(GL_ARRAY_BUFFER, normals * sizeof(GLfloat), &normal[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint) 1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    normal.clear();  // no need for the normal data, it is on the GPU now
}

void Shapes::addVertex(std::vector<GLfloat>& a, const glm::vec3& v) {
    a.push_back(v.x);
    a.push_back(v.y);
    a.push_back(v.z);
}

void Shapes::deleteShape() {
    shouldDelete = true;
}

void Shapes::listClear() {
    shapes.clear();
}

void Shapes::step(float deltaT) {
    std::vector<std::pair<List&, List::iterator>> deleteList;
    for (auto& [_, list] : shapes)
        for (auto& shape : list)
            shape->update(deltaT);
    for (auto& [_, list] : shapes)
        for (auto& shape : list)
            if (shape->shouldDelete)
                deleteList.emplace_back(list, shape->it);
    for (auto& [l, it] : deleteList)
        l.erase(it);
    for (auto& [_, list] : shapes)
        for (auto& shape : list)
            shape->render();
}

std::map<std::type_index, Shapes::List> Shapes::shapes;

}  // namespace dng
