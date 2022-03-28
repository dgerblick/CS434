#include <particle.h>
#include <string>
#include <vector>
#include <cmath>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shaders.h>

namespace dng {

void Particle::render() {
    // glm::mat4 mv(1.0);
    // glm::mat4 mv =
    //     glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(radius)), glm::vec3(position,
    //     0.0f));
    glm::mat4 mv =
        glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(position, -1.0f)), glm::vec3(radius));
    glBindVertexArray(_vaID);
    // material properties
    glUniform3fv(params.kaParameter, 1, glm::value_ptr(material.ka));
    glUniform3fv(params.kdParameter, 1, glm::value_ptr(material.kd));
    glUniform3fv(params.ksParameter, 1, glm::value_ptr(material.ks));
    glUniform1fv(params.shParameter, 1, &material.sh);
    // model matrix
    glUniformMatrix4fv(params.modelParameter, 1, GL_FALSE, glm::value_ptr(mv));
    // model for normals
    glUniformMatrix3fv(params.modelViewNParameter, 1, GL_FALSE, glm::value_ptr(mv));
    glDrawArrays(GL_TRIANGLES, 0, 3 * _vertCount);
}

void Particle::generate(int stacks, int slices) {
    glm::vec3 v;

    GLfloat deltaTheta = 2 * M_PI / (GLfloat) slices;
    GLfloat deltaPhi = M_PI / (GLfloat) stacks;

    for (GLint i = 0; i < stacks; i++) {
        GLfloat phi = i * deltaPhi;
        for (GLint j = 0; j < slices; j++) {
            GLfloat theta = j * deltaTheta;
            // the first triangle
            v = glm::vec3(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi));
            addVertex(_vertices, v);  // add the vertex
            glm::normalize(v);        // normalize it
            addVertex(_normals, v);   // and add the normal vector
            v = glm::vec3(cos(theta + deltaTheta) * sin(phi), sin(theta + deltaTheta) * sin(phi),
                          cos(phi));
            addVertex(_vertices, v);  // add the vertex
            glm::normalize(v);        // normalize it
            addVertex(_normals, v);   // and add the normal vector
            v = glm::vec3(cos(theta) * sin(phi + deltaPhi), sin(theta) * sin(phi + deltaPhi),
                          cos(phi + deltaPhi));
            addVertex(_vertices, v);  // add the vertex
            glm::normalize(v);        // normalize it
            addVertex(_normals, v);   // and add the normal vector
            // the second triangle
            v = glm::vec3(cos(theta + deltaTheta) * sin(phi), sin(theta + deltaTheta) * sin(phi),
                          cos(phi));
            addVertex(_vertices, v);  // add the vertex
            glm::normalize(v);        // normalize it
            addVertex(_normals, v);   // and add the normal vector
            v = glm::vec3(cos(theta) * sin(phi + deltaPhi), sin(theta) * sin(phi + deltaPhi),
                          cos(phi + deltaPhi));
            addVertex(_vertices, v);  // add the vertex
            glm::normalize(v);        // normalize it
            addVertex(_normals, v);   // and add the normal vector
            v = glm::vec3(cos(theta + deltaTheta) * sin(phi + deltaPhi),
                          sin(theta + deltaTheta) * sin(phi + deltaPhi), cos(phi + deltaPhi));
            addVertex(_vertices, v);  // add the vertex
            glm::normalize(v);        // normalize it
            addVertex(_normals, v);   // and add the normal vector
        }
    }

    initArrays();
}

void Particle::addVertex(std::vector<GLfloat>& a, const glm::vec3& v) {
    a.push_back(v.x);
    a.push_back(v.y);
    a.push_back(v.z);
}

void Particle::initArrays() {
    _vertCount = _vertices.size();
    _normCount = _normals.size();

    // get the vertex array handle and bind it
    glGenVertexArrays(1, &_vaID);
    glBindVertexArray(_vaID);

    // the vertex array will have two vbos, vertices and normals
    glGenBuffers(2, _vboHandles);
    GLuint verticesID = _vboHandles[0];
    GLuint normalsID = _vboHandles[1];

    // send vertices
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glBufferData(GL_ARRAY_BUFFER, _vertCount * sizeof(GLfloat), _vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint) 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    _vertices.clear();  // no need for the vertex data, it is on the GPU now

    // send normals
    glBindBuffer(GL_ARRAY_BUFFER, normalsID);
    glBufferData(GL_ARRAY_BUFFER, _normCount * sizeof(GLfloat), _normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint) 1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    _normals.clear();  // no need for the normal data, it is on the GPU now
}

std::vector<GLfloat> Particle::_vertices;
std::vector<GLfloat> Particle::_normals;
GLuint Particle::_vboHandles[2];
GLuint Particle::_vaID;
GLuint Particle::_buffer;
GLuint Particle::_vertCount;
GLuint Particle::_normCount;

}  // namespace dng
