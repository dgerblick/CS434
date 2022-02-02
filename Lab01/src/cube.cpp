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

    addVertex(vertex, A);
    addVertex(vertex, B);
    addVertex(vertex, C);
    addVertex(vertex, A);
    addVertex(vertex, C);
    addVertex(vertex, D);
    for (int i = 0; i < 6; i++)
        addVertex(normal, glm::vec3(0.0f, 0.0f, 1.0f));
    addVertex(vertex, A);
    addVertex(vertex, E);
    addVertex(vertex, F);
    addVertex(vertex, A);
    addVertex(vertex, F);
    addVertex(vertex, B);
    for (int i = 0; i < 6; i++)
        addVertex(normal, glm::vec3(0.0f, -1.0f, 0.0f));
    addVertex(vertex, B);
    addVertex(vertex, F);
    addVertex(vertex, G);
    addVertex(vertex, B);
    addVertex(vertex, G);
    addVertex(vertex, C);
    for (int i = 0; i < 6; i++)
        addVertex(normal, glm::vec3(1.0f, 0.0f, 0.0f));
    addVertex(vertex, C);
    addVertex(vertex, G);
    addVertex(vertex, H);
    addVertex(vertex, C);
    addVertex(vertex, H);
    addVertex(vertex, D);
    for (int i = 0; i < 6; i++)
        addVertex(normal, glm::vec3(0.0f, 1.0f, 0.0f));
    addVertex(vertex, D);
    addVertex(vertex, H);
    addVertex(vertex, E);
    addVertex(vertex, D);
    addVertex(vertex, E);
    addVertex(vertex, A);
    for (int i = 0; i < 6; i++)
        addVertex(normal, glm::vec3(-1.0f, 0.0f, 0.0f));
    addVertex(vertex, E);
    addVertex(vertex, F);
    addVertex(vertex, G);
    addVertex(vertex, E);
    addVertex(vertex, G);
    addVertex(vertex, H);
    for (int i = 0; i < 6; i++)
        addVertex(normal, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Cube::render() {
    glBindVertexArray(vaID);
    //glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glEnableVertexAttribArray(0);
    glUniformMatrix4fv(modelParameter, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 3 * points);
}

}  // namespace dng
