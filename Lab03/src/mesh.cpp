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
    _tris.reserve(_numTris);
    glVerts.reserve(9 * _numTris);
    glNorms.reserve(9 * _numTris);

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

        Triangle t;
        t.v0 = glm::vec2(v[0].x, v[0].y);
        if (side(t.v0, t.v1, t.v2) < 0.0f) {
            t.v1 = glm::vec2(v[1].x, v[1].y);
            t.v2 = glm::vec2(v[2].x, v[2].y);
        } else {
            t.v1 = glm::vec2(v[2].x, v[2].y);
            t.v2 = glm::vec2(v[1].x, v[1].y);
        }

        _tris.push_back(t);
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

std::pair<glm::vec2, glm::vec2> Mesh::intersect(Particle p, glm::vec2 nextPos, glm::vec2 nextVel) {
    float bestDist = std::numeric_limits<float>().infinity();
    glm::vec2 bestIntersect;
    glm::vec2 bestNormal;

    for (auto& tri : _tris) {
        float dist;
        glm::vec2 intersect;
        glm::vec2 normal;
        if (triangleIntersect(p.position, nextPos, tri, intersect, dist, normal) &&
            dist < bestDist) {
            bestDist = dist;
            bestIntersect = intersect;
            bestNormal = normal;
        }
    }
    if (bestDist != std::numeric_limits<float>().infinity()) {
        glm::vec2 vel = glm::reflect(nextVel, bestNormal);
        glm::vec2 pos = bestIntersect + glm::normalize(vel) * (1 - bestDist);
        return std::make_pair(p.position, vel);
    }
    return std::make_pair(nextPos, nextVel);
}

bool Mesh::lineSegmentIntersect(glm::vec2 v0, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3,
                                glm::vec2& out, float& dist) {
    if (v0 == v1)
        return false;
    float t = (v0.x - v2.x) * (v2.y - v3.y) - (v0.y - v2.y) * (v2.x - v3.x);
    float u = (v0.x - v2.x) * (v0.y - v1.y) - (v0.y - v2.y) * (v0.x - v1.x);
    float d = (v0.x - v1.x) * (v2.y - v3.y) - (v0.y - v1.y) * (v2.x - v3.x);
    if (d != 0.0f && 0.0f <= t && t <= d && 0.0f <= u && u <= d) {
        out = v0 + (v1 - v0) * t / d;
        dist = t / d;
        return true;
    }
    return false;
}

bool Mesh::triangleIntersect(glm::vec2 v0, glm::vec2 v1, Triangle tri, glm::vec2& out, float& dist,
                             glm::vec2& normal) {
    if (v0 == v1)
        return false;
    glm::vec2 currIntersect;
    float currDist;
    glm::vec2 bestIntersect;
    glm::vec2 bestNormal;
    float bestDist = std::numeric_limits<float>().infinity();

    if (lineSegmentIntersect(v0, v1, tri.v0, tri.v1, currIntersect, currDist) &&
        currDist < bestDist) {
        bestDist = currDist;
        bestIntersect = currIntersect;
        bestNormal = glm::vec2(tri.v1.y - tri.v0.y, tri.v0.x - tri.v1.x);
    }

    if (lineSegmentIntersect(v0, v1, tri.v1, tri.v2, currIntersect, currDist) &&
        currDist < bestDist) {
        bestDist = currDist;
        bestIntersect = currIntersect;
        bestNormal = glm::vec2(tri.v2.y - tri.v1.y, tri.v1.x - tri.v2.x);
    }

    if (lineSegmentIntersect(v0, v1, tri.v2, tri.v0, currIntersect, currDist) &&
        currDist < bestDist) {
        bestDist = currDist;
        bestIntersect = currIntersect;
        bestNormal = glm::vec2(tri.v0.y - tri.v2.y, tri.v2.x - tri.v0.x);
    }

    if (bestDist == std::numeric_limits<float>().infinity()) {
        return false;
    } else {
        out = bestIntersect;
        dist = bestDist;
        normal = glm::normalize(bestNormal);
        return true;
    }
}

bool Mesh::insideTri(Triangle t, glm::vec2 p) {
    return side(p, t.v0, t.v1) > 0.0f && side(p, t.v1, t.v2) > 0.0f && side(p, t.v2, t.v0) > 0.0f;
}

float Mesh::side(glm::vec2 p, glm::vec2 v0, glm::vec2 v1) {
    return glm::dot(p - v0, glm::vec2(v1.y - v0.y, v0.x - v1.x));
}

}  // namespace dng
