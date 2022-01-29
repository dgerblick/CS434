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

void Sphere::render() {
    glBindVertexArray(vaID);
    //	glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //	glEnableVertexAttribArray(0);
    // material properties
    glUniform3fv(kaParameter, 1, glm::value_ptr(ka));
    glUniform3fv(kdParameter, 1, glm::value_ptr(kd));
    glUniform3fv(ksParameter, 1, glm::value_ptr(ks));
    glUniform1fv(shParameter, 1, &sh);
    // model matrix
    glUniformMatrix4fv(modelParameter, 1, GL_FALSE, glm::value_ptr(model));
    // model for normals
    glUniformMatrix3fv(modelViewNParameter, 1, GL_FALSE, glm::value_ptr(modelViewN));
    glDrawArrays(GL_TRIANGLES, 0, 3 * points);
}

void Sphere::generate(int stacks, int slices, GLfloat r) {
    glm::vec3 v;

    GLfloat deltaTheta = 2 * M_PI / (GLfloat) slices;
    GLfloat deltaPhi = M_PI / (GLfloat) stacks;

    for (GLint i = 0; i < stacks; i++) {
        GLfloat phi = i * deltaPhi;
        for (GLint j = 0; j < slices; j++) {
            GLfloat theta = j * deltaTheta;
            // the first triangle
            v = glm::vec3(r * cos(theta) * sin(phi), r * sin(theta) * sin(phi), r * cos(phi));
            addVertex(&vertex, &v);  // add the vertex
            glm::normalize(v);       // normalize it
            addVertex(&normal, &v);  // and add the normal vector
            v = glm::vec3(r * cos(theta + deltaTheta) * sin(phi),
                          r * sin(theta + deltaTheta) * sin(phi), r * cos(phi));
            addVertex(&vertex, &v);  // add the vertex
            glm::normalize(v);       // normalize it
            addVertex(&normal, &v);  // and add the normal vector
            v = glm::vec3(r * cos(theta) * sin(phi + deltaPhi),
                          r * sin(theta) * sin(phi + deltaPhi), r * cos(phi + deltaPhi));
            addVertex(&vertex, &v);  // add the vertex
            glm::normalize(v);       // normalize it
            addVertex(&normal, &v);  // and add the normal vector
            // the second triangle
            v = glm::vec3(r * cos(theta + deltaTheta) * sin(phi),
                          r * sin(theta + deltaTheta) * sin(phi), r * cos(phi));
            addVertex(&vertex, &v);  // add the vertex
            glm::normalize(v);       // normalize it
            addVertex(&normal, &v);  // and add the normal vector
            v = glm::vec3(r * cos(theta) * sin(phi + deltaPhi),
                          r * sin(theta) * sin(phi + deltaPhi), r * cos(phi + deltaPhi));
            addVertex(&vertex, &v);  // add the vertex
            glm::normalize(v);       // normalize it
            addVertex(&normal, &v);  // and add the normal vector
            v = glm::vec3(r * cos(theta + deltaTheta) * sin(phi + deltaPhi),
                          r * sin(theta + deltaTheta) * sin(phi + deltaPhi),
                          r * cos(phi + deltaPhi));
            addVertex(&vertex, &v);  // add the vertex
            glm::normalize(v);       // normalize it
            addVertex(&normal, &v);  // and add the normal vector
        }
    }
}

void Sphere::initArrays() {
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

Sphere::Sphere() {
    generate(55, 55, 1.f);
    initArrays();
}

Sphere::Sphere(int stacks, int slices, GLfloat r) {
    this->stacks = stacks;
    this->slices = slices;
    this->r = r;
    generate(stacks, slices, r);
    initArrays();
}

}  // namespace dng
