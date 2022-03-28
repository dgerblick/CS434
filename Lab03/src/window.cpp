#include <window.h>

#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/color_space.hpp>

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

#include <particle.h>
#include <shaders.h>
#include <light.h>
#include <mesh.h>

namespace dng::window {

std::vector<Particle> particles;
std::list<Mesh> meshes;
Light light;
shaders::Params params;
GLuint shaderProgram;

float rand(float min = 0.0f, float max = 1.0f) {
    return min + static_cast<float>(::rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

glm::vec2 forceField(Particle p) {
    glm::vec2 drag = glm::normalize(p.velocity) * -glm::dot(p.velocity, p.velocity);
    // n-body
    // glm::vec2 force(0.0f);
    // for (int i = 0; i < particles.size(); i++) {
    //     float dist = glm::distance(particles[i].position, p.position);
    //     if (dist > 0.01) {
    //         float magnitude = 0.0001 * particles[i].mass * p.mass / (dist * dist);
    //         force += glm::normalize(particles[i].position - p.position) * magnitude;
    //     }
    // }
    // return force + drag;

    // Gravity + Drag
    return glm::vec2(0.0f, -9.8f) + drag;

    // Orbit Center
    // float distSqr = glm::dot(p.position, p.position);
    // if (distSqr < 0.001f)
    //     return glm::vec2(0.0f);
    // return (0.1f * -glm::normalize(p.position) / distSqr) + drag;
}

void initializeProgram() {
    std::vector<GLuint> shaderList;

    // load and compile shaders
    shaderList.push_back(
        shaders::createShader(GL_VERTEX_SHADER, shaders::loadShader("shaders/phong.vert")));
    shaderList.push_back(
        shaders::createShader(GL_FRAGMENT_SHADER, shaders::loadShader("shaders/phong.frag")));

    // create the shader program and attach the shaders to it
    shaderProgram = shaders::createProgram(shaderList);

    // delete shaders (they are on the GPU now)
    std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);

    params.modelParameter = glGetUniformLocation(shaderProgram, "model");
    params.modelViewNParameter = glGetUniformLocation(shaderProgram, "modelViewN");
    params.viewParameter = glGetUniformLocation(shaderProgram, "view");
    params.projParameter = glGetUniformLocation(shaderProgram, "proj");
    // now the material properties
    params.kaParameter = glGetUniformLocation(shaderProgram, "mat.ka");
    params.kdParameter = glGetUniformLocation(shaderProgram, "mat.kd");
    params.ksParameter = glGetUniformLocation(shaderProgram, "mat.ks");
    params.shParameter = glGetUniformLocation(shaderProgram, "mat.sh");
    // now the light properties
    light.setLaToShader(glGetUniformLocation(shaderProgram, "light.la"));
    light.setLdToShader(glGetUniformLocation(shaderProgram, "light.ld"));
    light.setLsToShader(glGetUniformLocation(shaderProgram, "light.ls"));
    light.setLposToShader(glGetUniformLocation(shaderProgram, "light.lPos"));
}

void init() {
    const int numParticles = 1000;
    ::srand(::time(nullptr));

    light.setPos(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    light.setLa(glm::vec3(1.0f));
    light.setLd(glm::vec3(0.0f));
    light.setLs(glm::vec3(0.0f));

    initializeProgram();

    // Load Meshes
    meshes.emplace_back("meshes/dots.stl");

    particles.reserve(numParticles);
    Particle::generate(20, 100);
    for (int i = 0; i < numParticles; i++) {
        Particle p;
        glm::vec3 color = glm::rgbColor(glm::vec3(rand(0.0f, 360.0f), 1.0f, 1.0f));
        p.params = params;
        // p.material.ka = 0.1f * color;
        // p.material.kd = 0.9f * color;
        p.material.ka = color;
        p.material.kd = glm::vec3(0.0f);
        p.material.ks = glm::vec3(0.0f);
        p.material.sh = 128.0f;
        p.position = glm::vec2(rand(-1.0f, 1.0f), rand(-1.0f, 1.0f));
        p.velocity = glm::vec2(rand(-0.1f, 0.1f), rand(-0.1f, 0.1f));
        p.mass = rand(0.1f, 10.0f);
        p.radius = 0.01;
        particles.push_back(p);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_POLYGON);
    {
        glVertex3f(-1.0f, -1.0f, -10.0f);
        glVertex3f(1.0f, -1.0f, -10.0f);
        glVertex3f(1.0f, 1.0f, -10.0f);
        glVertex3f(-1.0f, 1.0f, -10.0f);
    }
    glEnd();

    glUseProgram(shaderProgram);
    {
        glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
        glUniformMatrix4fv(params.projParameter, 1, GL_FALSE, glm::value_ptr(proj));
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(params.viewParameter, 1, GL_FALSE, glm::value_ptr(view));

        light.setShaders();
        for (auto& particle : particles) {
            particle.render();
        }
    }
    glUseProgram(0);

    for (auto& mesh : meshes) {
        mesh.render();
    }

    glFlush();
    glutSwapBuffers();
}

void idle() {
    // glutPostRedisplay();
}

void timer(int t) {
    std::vector<glm::vec2> posBuffer(particles.size());
    std::vector<glm::vec2> velBuffer(particles.size());

#pragma omp parallel for
    for (int i = 0; i < particles.size(); i++) {
        posBuffer[i] = particles[i].position + DELTA_T * particles[i].velocity;
        velBuffer[i] =
            particles[i].velocity + DELTA_T * forceField(particles[i]) / particles[i].mass;
    }
#pragma omp parallel for
    for (int i = 0; i < particles.size(); i++) {
        particles[i].position = posBuffer[i];
        particles[i].velocity = velBuffer[i];

        if (particles[i].position.x > 1.0f) {
            particles[i].velocity.x = -particles[i].velocity.x;
            particles[i].position.x = 1.0f - (particles[i].position.x - 1.0f);
        } else if (particles[i].position.x < -1.0f) {
            particles[i].velocity.x = -particles[i].velocity.x;
            particles[i].position.x = -1.0f - (particles[i].position.x + 1.0f);
        }
        if (particles[i].position.y > 1.0f) {
            particles[i].velocity.y = -particles[i].velocity.y;
            particles[i].position.y = 1.0f - (particles[i].position.y - 1.0f);
        } else if (particles[i].position.y < -1.0f) {
            particles[i].velocity.y = -particles[i].velocity.y;
            particles[i].position.y = -1.0f - (particles[i].position.y + 1.0f);
        }
    }
    glutPostRedisplay();
    resetTimer();
}

void resetTimer() {
    glutTimerFunc(DELTA_T_MS, timer, 0);
}

}  // namespace dng::window
