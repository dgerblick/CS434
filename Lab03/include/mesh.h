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
    bool intersect(Particle p, float deltaT);

private:
    std::vector<Triangle> _tris;
    uint32_t _numTris;
    GLuint _vaID;
    GLuint _vboHandles[2];

    static bool insideTri(Triangle t, glm::vec2 p);
    static float side(glm::vec2 p, glm::vec2 v0, glm::vec2 v1);
};

}  // namespace dng

#endif  // __MESH_H__
