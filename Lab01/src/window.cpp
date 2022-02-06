#include <window.h>
#include <shaders.h>
#include <lights.h>
#include <cube.h>
#include <windmill.h>
#include <bullet.h>
#include <algorithm>
#include <iostream>
#include <memory>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace dng {

bool needRedisplay = false;
GLuint shaderProgram;
GLfloat ftime = 0.f;
glm::vec3 cameraPos(0.0f, 1.0f, 0.0f);
glm::vec2 cameraRot(0.0f);
glm::vec3 cameraPosIn(0.0f);
glm::vec2 cameraRotIn(0.0f);
glm::mat4 proj;
shaders::Params params;
Light light;
GLint wWindow = 800;
GLint hWindow = 800;
float sh = 1;
int lastTime = 0;

float rand(float min = 0.0f, float max = 1.0f) {
    return min + static_cast<float>(::rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

// called when a window is reshaped
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glEnable(GL_DEPTH_TEST);
    proj = glm::perspective(80.0f, (float) w / h, 0.01f, 1000.f);
    // remember the settings for the camera
    wWindow = w;
    hWindow = h;
}

// the main rendering function
void renderObjects(float deltaT) {
    const int range = 3;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0, 0, 0);
    glPointSize(2);
    glLineWidth(1);
    // set the projection and view once for the scene
    glUniformMatrix4fv(params.projParameter, 1, GL_FALSE, glm::value_ptr(proj));
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + lookVec(), UP);
    glUniformMatrix4fv(params.viewParameter, 1, GL_FALSE, glm::value_ptr(view));
    // set the light
    static glm::vec4 pos;
    pos.x = 20 * sin(ftime / 12);
    pos.y = 30;
    pos.z = 20 * cos(ftime / 12);
    pos.w = 1;
    light.setPos(pos);
    light.setShaders();
    Shapes::step(deltaT);
}

void idle() {
    glClearColor(0.1, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update time var
    int elapsed = glutGet(GLUT_ELAPSED_TIME);
    float deltaT = (elapsed - lastTime) / 1000.0f;
    lastTime = elapsed;
    ftime += deltaT;

    // Rotate Camera
    cameraRot += deltaT * LOOK_KEY_RATE * cameraRotIn;
    if (cameraRot.y > 2 * M_PI)
        cameraRot.y -= 2 * M_PI;
    if (cameraRot.y < 0)
        cameraRot.y += 2 * M_PI;
    if (cameraRot.x < LOOK_LIMIT - M_PI_2)
        cameraRot.x = LOOK_LIMIT - M_PI_2;
    if (cameraRot.x > LOOK_LIMIT + M_PI_2)
        cameraRot.x = LOOK_LIMIT + M_PI_2;

    // Move Camera
    if (cameraPosIn.x != 0 || cameraPosIn.y != 0) {
        glm::vec3 right = glm::normalize(glm::cross(lookVec(), UP));
        glm::vec3 forward = glm::cross(right, UP);
        glm::vec3 direction = glm::normalize(cameraPosIn);
        cameraPos += deltaT * MOVE_KEY_RATE * (direction.x * forward + direction.y * right);
    }

    glUseProgram(shaderProgram);
    renderObjects(deltaT);
    glutSwapBuffers();
}

void display() {
}

// keyboard callback
void kbd(unsigned char a, int x, int y) {
    switch (a) {
    case 27:
        exit(0);
        break;
    case 'w':
        cameraPosIn.x -= 1.0f;
        break;
    case 'a':
        cameraPosIn.y -= 1.0f;
        break;
    case 's':
        cameraPosIn.x += 1.0f;
        break;
    case 'd':
        cameraPosIn.y += 1.0f;
        break;
    case ' ':
        spawnBullet();
        break;
    case '.':
        std::cout << "cameraPos:\n"
                  << "  x: " << cameraPos.x << '\n'
                  << "  y: " << cameraPos.y << '\n'
                  << "  z: " << cameraPos.z << '\n'
                  << "cameraRot:\n"
                  << "  x: " << cameraRot.x << '\n'
                  << "  y: " << cameraRot.y << '\n'
                  << "lookVec:\n"
                  << "  x: " << lookVec().x << '\n'
                  << "  y: " << lookVec().y << '\n'
                  << "  z: " << lookVec().z << '\n';
        break;
    }
    glutPostRedisplay();
}

// keyboard up callback
void kbdRelease(unsigned char a, int x, int y) {
    switch (a) {
    case 'w':
        cameraPosIn.x += 1.0f;
        break;
    case 'a':
        cameraPosIn.y += 1.0f;
        break;
    case 's':
        cameraPosIn.x -= 1.0f;
        break;
    case 'd':
        cameraPosIn.y -= 1.0f;
        break;
    }
    glutPostRedisplay();
}

// special keyboard callback
void specKbdPress(int a, int x, int y) {
    switch (a) {
    case GLUT_KEY_LEFT:
        cameraRotIn.y += 1.0f;
        break;
    case GLUT_KEY_RIGHT:
        cameraRotIn.y -= 1.0f;
        break;
    case GLUT_KEY_DOWN:
        cameraRotIn.x -= 1.0f;
        break;
    case GLUT_KEY_UP:
        cameraRotIn.x += 1.0f;
        break;
    }
    glutPostRedisplay();
}

// called when a special key is released
void specKbdRelease(int a, int x, int y) {
    switch (a) {
    case GLUT_KEY_LEFT:
        cameraRotIn.y -= 1.0f;
        break;
    case GLUT_KEY_RIGHT:
        cameraRotIn.y += 1.0f;
        break;
    case GLUT_KEY_DOWN:
        cameraRotIn.x += 1.0f;
        break;
    case GLUT_KEY_UP:
        cameraRotIn.x -= 1.0f;
        break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    std::cout << "Location is [" << x << "'" << y << "]\n";
}

void initializeProgram(GLuint* program) {
    std::vector<GLuint> shaderList;

    // load and compile shaders
    shaderList.push_back(
        shaders::createShader(GL_VERTEX_SHADER, shaders::loadShader("shaders/phong.vert")));
    shaderList.push_back(
        shaders::createShader(GL_FRAGMENT_SHADER, shaders::loadShader("shaders/phong.frag")));

    // create the shader program and attach the shaders to it
    *program = shaders::createProgram(shaderList);

    // delete shaders (they are on the GPU now)
    std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);

    params.modelParameter = glGetUniformLocation(*program, "model");
    params.modelViewNParameter = glGetUniformLocation(*program, "modelViewN");
    params.viewParameter = glGetUniformLocation(*program, "view");
    params.projParameter = glGetUniformLocation(*program, "proj");
    // now the material properties
    params.kaParameter = glGetUniformLocation(*program, "mat.ka");
    params.kdParameter = glGetUniformLocation(*program, "mat.kd");
    params.ksParameter = glGetUniformLocation(*program, "mat.ks");
    params.shParameter = glGetUniformLocation(*program, "mat.sh");
    // now the light properties
    light.setLaToShader(glGetUniformLocation(*program, "light.la"));
    light.setLdToShader(glGetUniformLocation(*program, "light.ld"));
    light.setLsToShader(glGetUniformLocation(*program, "light.ls"));
    light.setLposToShader(glGetUniformLocation(*program, "light.lPos"));

    // Set up camera
    proj = glm::perspective(80.0f, (float) wWindow / hWindow, 0.01f, 1000.f);
}

void initShapes(shaders::Params* params) {
    const int numWindmills = 250;
    ::srand(::time(nullptr));

    Shapes::listClear();

    auto& bullet = Shapes::listAdd<Bullet>(glm::vec3(0.0), 0.0);
    auto& ground = Shapes::listAdd<Cube>();
    ground.setKa(glm::vec3(0.1, 0.1, 0.1));
    ground.setKs(glm::vec3(1, 1, 1));
    ground.setKd(glm::vec3(0.7, 0.7, 0.7));
    ground.setSh(100);
    ground.setModel(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(150.0f, 1.0f, 150.0f)),
                                   glm::vec3(0.0f, -1.0f, 0.0f)));
    ground.setModelMatrixParamToShader(params->modelParameter);
    ground.setModelViewNMatrixParamToShader(params->modelViewNParameter);
    ground.setKaToShader(params->kaParameter);
    ground.setKdToShader(params->kdParameter);
    ground.setKsToShader(params->ksParameter);
    ground.setShToShader(params->shParameter);

    std::vector<int> numBlades({ 3, 3, 4, 4, 4, 4, 4, 6, 6, 8, 12 });
    for (int i = 0; i < numWindmills; i++) {
        auto& windmill = Shapes::listAdd<Windmill>(rand(0.0f, 360.0f), rand(60.0f, 360.0f),
                                                   numBlades[i % numBlades.size()]);
        windmill.setKa(glm::vec3(0.1, 0.1, 0.1));
        windmill.setKs(glm::vec3(rand(), rand(), rand()));
        windmill.setKd(glm::vec3(rand(), rand(), rand()));
        windmill.setSh(rand(0.0f, 100.0f));
        windmill.setModel(
            glm::scale(glm::translate(glm::rotate(glm::mat4(1.0), rand(0.0f, 360.0f),
                                                  glm::vec3(0.0f, 1.0f, 0.0f)),
                                      glm::vec3(rand(-50.0f, 50.0f), -0.25f, rand(-50.0f, 50.0f))),
                       glm::vec3(0.75f)));
        windmill.setModelMatrixParamToShader(params->modelParameter);
        windmill.setModelViewNMatrixParamToShader(params->modelViewNParameter);
        windmill.setKaToShader(params->kaParameter);
        windmill.setKdToShader(params->kdParameter);
        windmill.setKsToShader(params->ksParameter);
        windmill.setShToShader(params->shParameter);
    }
}

glm::vec3 lookVec() {
    return glm::vec3(std::cos(cameraRot.x) * std::sin(cameraRot.y), std::sin(cameraRot.x),
                     std::cos(cameraRot.x) * std::cos(cameraRot.y));
}

void spawnBullet() {
    glm::vec3 flatVec = lookVec();
    flatVec.y = 0;
    flatVec = glm::normalize(flatVec);

    auto& bullet = Shapes::listAdd<Bullet>(glm::vec3(0.0, 0.0, -100.0), 1.0);
    bullet.setKa(glm::vec3(0.2, 0.2, 0.2));
    bullet.setKs(glm::vec3(1, 1, 1));
    bullet.setKd(glm::vec3(0.7, 0.7, 0.7));
    bullet.setSh(100);
    bullet.setModel(glm::rotate(
        glm::scale(
            glm::translate(glm::translate(glm::mat4(1.0), glm::vec3(0.0, -0.5, 0.0)), cameraPos),
            glm::vec3(0.2, 0.2, 0.2)),
        glm::degrees(cameraRot.y) - 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    bullet.setModelMatrixParamToShader(params.modelParameter);
    bullet.setModelViewNMatrixParamToShader(params.modelViewNParameter);
    bullet.setKaToShader(params.kaParameter);
    bullet.setKdToShader(params.kdParameter);
    bullet.setKsToShader(params.ksParameter);
    bullet.setShToShader(params.shParameter);
}

}  // namespace dng
