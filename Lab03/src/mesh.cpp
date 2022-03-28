#include <mesh.h>
#include <fstream>

namespace dng {

Mesh::Mesh(std::string filename) {
    std::ifstream ifs(filename, std::ios::in | std::ios::binary);
    char header[80];
    ifs.read(header, sizeof(header));
    ifs.read((char*) (&_numTris), sizeof(uint32_t));
    std::vector<GLfloat> glVerts;
    std::vector<GLfloat> glNorms;
    glVerts.reserve(9 * _numTris);
    glNorms.reserve(9 * _numTris);
    _segments.reserve(3 * _numTris);
    for (int i = 0; i < _numTris; i++) {
        glm::vec3 normal;
        uint16_t attrib;
        glm::vec3 v[3];

        ifs.read((char*) (&normal), sizeof(glm::vec3));
        for (int j = 0; j < 3; j++)
            ifs.read((char*) (v + j), sizeof(glm::vec3));
        ifs.read((char*) (&attrib), sizeof(attrib));

        for (int j = 0; j < 3; j++) {
            glVerts.push_back(v[j].x);
            glVerts.push_back(v[j].y);
            glVerts.push_back(v[j].z);
            glNorms.push_back(normal.x);
            glNorms.push_back(normal.y);
            glNorms.push_back(normal.z);
        }

        for (int j = 0; j < 3; j++) {
            glm::vec3 v0 = v[j];
            glm::vec3 v1 = v[(j + 1) % 3];
            Segment s;
            s.midpoint = (v0 + v1) / 2.0f;
            s.normal = glm::normalize(glm::cross(v0 - v1, normal));
            s.length = glm::distance(v0, v1);
            _segments.push_back(s);
        }
    }

    // get the vertex array handle and bind it
    glGenVertexArrays(1, &_vaID);
    glBindVertexArray(_vaID);

    // the vertex array will have two vbos, vertices and normals
    glGenBuffers(2, _vboHandles);
    GLuint verticesID = _vboHandles[0];
    GLuint normalsID = _vboHandles[1];

    // send vertices
    glBindBuffer(GL_ARRAY_BUFFER, verticesID);
    glBufferData(GL_ARRAY_BUFFER, glVerts.size() * sizeof(GLfloat), glVerts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint) 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // send normals
    glBindBuffer(GL_ARRAY_BUFFER, normalsID);
    glBufferData(GL_ARRAY_BUFFER, glNorms.size() * sizeof(GLfloat), glNorms.data(), GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint) 1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &_vaID);
    glDeleteBuffers(2, _vboHandles);
}

void Mesh::render() {
    glBindVertexArray(_vaID);
    glDrawArrays(GL_TRIANGLES, 0, 3 * _numTris);
    glBindVertexArray(0);
}

}  // namespace dng
