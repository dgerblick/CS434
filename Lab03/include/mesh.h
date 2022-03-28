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
    struct Triangle {
        glm::vec3 v0;
        glm::vec3 v1;
        glm::vec3 v2;
    };
    std::vector<Triangle> tris;

    Mesh(std::string filename);
    ~Mesh();
    void render();
private:
    GLuint _vaID;
    GLuint _vboHandles[2];
};

}  // namespace dng

#endif  // __MESH_H__
