#include <mesh.h>
#include <fstream>

namespace dng {

Mesh::Mesh(std::string filename) {
    std::ifstream ifs(filename, std::ios::in | std::ios::binary);
    char header[80];
    ifs.read(header, sizeof(header));
    uint32_t numTris;
    ifs.read((char*) (&numTris), sizeof(uint32_t));

    std::vector<GLfloat> glVerts;
    std::vector<GLfloat> glNorms;
    glVerts.reserve(9 * numTris);
    glNorms.reserve(9 * numTris);

    tris.clear();
    tris.reserve(numTris);
    for (int i = 0; i < numTris; i++) {
        glm::vec3 normal;
        uint16_t attrib;
        glm::vec3 v0;
        glm::vec3 v1;
        glm::vec3 v2;

        ifs.read((char*) (&normal), sizeof(glm::vec3));
        ifs.read((char*) (&v0), sizeof(glm::vec3));
        ifs.read((char*) (&v1), sizeof(glm::vec3));
        ifs.read((char*) (&v2), sizeof(glm::vec3));
        ifs.read((char*) (&attrib), sizeof(attrib));

        Triangle t;
        t.v0.x = v0.x;
        t.v0.y = v0.y;
        t.v1.x = v1.x;
        t.v1.y = v1.y;
        t.v2.x = v2.x;
        t.v2.y = v2.y;
        tris.push_back(t);

        glVerts.push_back(v0.x);
        glVerts.push_back(v0.y);
        glVerts.push_back(v0.z);
        glVerts.push_back(v1.x);
        glVerts.push_back(v1.y);
        glVerts.push_back(v1.z);
        glVerts.push_back(v2.x);
        glVerts.push_back(v2.y);
        glVerts.push_back(v2.z);
        glNorms.push_back(normal.x);
        glNorms.push_back(normal.y);
        glNorms.push_back(normal.z);
        glNorms.push_back(normal.x);
        glNorms.push_back(normal.y);
        glNorms.push_back(normal.z);
        glNorms.push_back(normal.x);
        glNorms.push_back(normal.y);
        glNorms.push_back(normal.z);
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
    glDrawArrays(GL_TRIANGLES, 0, 3 * tris.size());
    glBindVertexArray(0);
}

}  // namespace dng
