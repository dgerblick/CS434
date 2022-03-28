#ifndef __MESH_H__
#define __MESH_H__

#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace dng {

class Mesh {
public:
    Mesh(std::string filename);
    ~Mesh();
    void render();

private:
    struct Segment {
        glm::vec3 midpoint;
        glm::vec3 normal;
        float length;
    };
    std::vector<Segment> _segments;

    uint32_t _numTris;
    GLuint _vaID;
    GLuint _vboHandles[2];
};

}  // namespace dng

#endif  // __MESH_H__
