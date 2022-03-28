#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <material.h>
#include <shaders.h>
#include <vector>

namespace dng {

struct Particle {
public:
    Material material;
    shaders::Params params;
    glm::vec2 position;
    glm::vec2 velocity;
    float mass;
    float radius;
    
    void render();
    static void generate(int stacks, int slices);
private:
    static std::vector<GLfloat> _vertices;
    static std::vector<GLfloat> _normals;
    static GLuint _vboHandles[2];
    static GLuint _vaID;
    static GLuint _buffer;
    static GLuint _vertCount;
    static GLuint _normCount;

    static void addVertex(std::vector<GLfloat>& a, const glm::vec3& v);
    static void initArrays();
};

}  // namespace dng

#endif  // __PARTICLE_H__
