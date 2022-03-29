#ifndef __MESH_H__
#define __MESH_H__

#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <particle.h>

namespace dng {

class Mesh {
public:
    struct Triangle {
        glm::vec2 v0;
        glm::vec2 v1;
        glm::vec2 v2;
    };

    Mesh(std::string filename);
    ~Mesh();
    void render();
    std::pair<glm::vec2, glm::vec2> intersect(Particle p, glm::vec2 nextPos, glm::vec2 nextVel);
    void isClearInside(std::vector<Particle>& particles);
    bool isInMesh(Particle p);

private:
    std::vector<Triangle> _tris;
    uint32_t _numTris;
    GLuint _vaID;
    GLuint _vboHandles[2];

    static bool lineSegmentIntersect(glm::vec2 v0, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3,
                                     glm::vec2& out, float& dist);
    static bool triangleIntersect(glm::vec2 v0, glm::vec2 v1, Triangle tri, glm::vec2& out,
                                  float& dist, glm::vec2& normal);
    static bool insideTri(Triangle t, glm::vec2 p);
    static float side(glm::vec2 p, glm::vec2 v0, glm::vec2 v1);
};

}  // namespace dng

#endif  // __MESH_H__
