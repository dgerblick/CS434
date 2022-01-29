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

void Cube::initArrays() {
    glGenVertexArrays(1, &vaID);
    glBindVertexArray(vaID);
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    points = vertex.size();
    glBufferData(GL_ARRAY_BUFFER, points * sizeof(GLfloat), &vertex[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint) 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    vertex.clear();  // no need for the vertex data, it is on the GPU now
}

Cube::Cube() {
    generate();
    initArrays();
}

void Cube::generate() {
    const glm::vec3 A = glm::vec3(-0.5f, -0.5f, -0.5f);
    const glm::vec3 B = glm::vec3(+0.5f, -0.5f, -0.5f);
    const glm::vec3 C = glm::vec3(+0.5f, +0.5f, -0.5f);
    const glm::vec3 D = glm::vec3(-0.5f, +0.5f, -0.5f);
    const glm::vec3 E = glm::vec3(-0.5f, -0.5f, +0.5f);
    const glm::vec3 F = glm::vec3(+0.5f, -0.5f, +0.5f);
    const glm::vec3 G = glm::vec3(+0.5f, +0.5f, +0.5f);
    const glm::vec3 H = glm::vec3(-0.5f, +0.5f, +0.5f);
    addVertex(&vertex, &A);
    addVertex(&vertex, &B);
    addVertex(&vertex, &C);
    addVertex(&vertex, &A);
    addVertex(&vertex, &C);
    addVertex(&vertex, &D);
    addVertex(&vertex, &A);
    addVertex(&vertex, &E);
    addVertex(&vertex, &F);
    addVertex(&vertex, &A);
    addVertex(&vertex, &F);
    addVertex(&vertex, &B);
    addVertex(&vertex, &B);
    addVertex(&vertex, &F);
    addVertex(&vertex, &G);
    addVertex(&vertex, &B);
    addVertex(&vertex, &G);
    addVertex(&vertex, &C);
    addVertex(&vertex, &C);
    addVertex(&vertex, &G);
    addVertex(&vertex, &H);
    addVertex(&vertex, &C);
    addVertex(&vertex, &H);
    addVertex(&vertex, &D);
    addVertex(&vertex, &D);
    addVertex(&vertex, &H);
    addVertex(&vertex, &E);
    addVertex(&vertex, &D);
    addVertex(&vertex, &E);
    addVertex(&vertex, &A);
    addVertex(&vertex, &E);
    addVertex(&vertex, &F);
    addVertex(&vertex, &G);
    addVertex(&vertex, &E);
    addVertex(&vertex, &G);
    addVertex(&vertex, &H);
}

void Cube::render() {
    glBindVertexArray(vaID);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glEnableVertexAttribArray(0);
    glUniformMatrix4fv(modelParameter, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 3 * points);
}

}  // namespace dng
