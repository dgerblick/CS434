#include <window.h>
#include <particle.h>
#include <shaders.h>
#include <light.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/color_space.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

namespace dng::window {

std::vector<Particle> particles;
Light light;
shaders::Params params;
GLuint shaderProgram;

float rand(float min = 0.0f, float max = 1.0f) {
    return min + static_cast<float>(::rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

glm::vec2 forceField(glm::vec2 pos) {
    float distSqr = glm::dot(pos, pos);
    if (distSqr < 0.0001f)
        return glm::vec2(0.0f);
    return 0.00001f * -glm::normalize(pos) / distSqr;
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

    // Set up camera
    // proj = glm::perspective(80.0f, (float) wWindow / hWindow, 0.01f, 1000.f);
}

void init() {
    const int numParticles = 100;
    ::srand(::time(nullptr));

    light.setPos(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    light.setLa(glm::vec3(1.0f));
    light.setLd(glm::vec3(0.0f));
    light.setLs(glm::vec3(0.0f));

    initializeProgram();

    particles.reserve(numParticles);
    for (int i = 0; i < numParticles; i++) {
        Particle p(20, 100);
        glm::vec3 color = glm::rgbColor(glm::vec3(rand(0.0f, 360.0f), 1.0f, 1.0f));
        p.params = params;
        // p.material.ka = 0.1f * color;
        // p.material.kd = 0.9f * color;
        p.material.ka = color;
        p.material.kd = glm::vec3(0.0f);
        p.material.ks = glm::vec3(0.0f);
        p.material.sh = 128.0f;
        p.position = glm::vec2(rand(-1.0f, 1.0f), rand(-1.0f, 1.0f));
        p.velocity = glm::vec2(0.0f);
        p.mass = 1.0;
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

    glFlush();
    glutSwapBuffers();
}

void idle() {
    // glutPostRedisplay();
}

void timer(int t) {
    for (int i = 0; i < particles.size(); i++) {
        glm::vec2 newPos = particles[i].position + particles[i].velocity;
        glm::vec2 newVel =
            particles[i].velocity + forceField(particles[i].position) / particles[i].mass;

        particles[i].position = newPos;
        particles[i].velocity = newVel;

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
    glutTimerFunc(MAX_FRAME_TIME, timer, 0);
}

}  // namespace dng::window
