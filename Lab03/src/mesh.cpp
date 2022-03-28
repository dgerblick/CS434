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
        Triangle t;
        uint16_t attrib;
        ifs.read((char*) (&normal), sizeof(glm::vec3));
        ifs.read((char*) (&t.v0), sizeof(glm::vec3));
        ifs.read((char*) (&t.v1), sizeof(glm::vec3));
        ifs.read((char*) (&t.v2), sizeof(glm::vec3));
        ifs.read((char*) (&attrib), sizeof(attrib));
        tris.push_back(t);

        glVerts.push_back(t.v0.x);
        glVerts.push_back(t.v0.y);
        glVerts.push_back(t.v0.z);
        glVerts.push_back(t.v1.x);
        glVerts.push_back(t.v1.y);
        glVerts.push_back(t.v1.z);
        glVerts.push_back(t.v2.x);
        glVerts.push_back(t.v2.y);
        glVerts.push_back(t.v2.z);
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
}

Mesh::~Mesh() {
}

void Mesh::render() {
    glBindVertexArray(_vaID);
    glDrawArrays(GL_TRIANGLES, 0, 3 * tris.size());
    glBindVertexArray(0);
}

}  // namespace dng
