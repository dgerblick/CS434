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
    
    Particle();
    Particle(int stacks, int slices);
    void render();
private:
    std::vector<GLfloat> _vertices;
    std::vector<GLfloat> _normals;
    GLuint _vboHandles[2];
    GLuint _vaID;
    GLuint _buffer;
    GLuint _vertCount;
    GLuint _normCount;

    void generate(int stacks, int slices);
    void addVertex(std::vector<GLfloat>& a, const glm::vec3& v);
    void initArrays();
};

}  // namespace dng

#endif  // __PARTICLE_H__
