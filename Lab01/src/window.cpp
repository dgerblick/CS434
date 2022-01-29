#include <window.h>
#include <shaders.h>
#include <lights.h>
#include <shapes.h>
#include <sphere.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace dng {

bool needRedisplay = false;
// Shapes* sphere;
GLuint shaderProgram;
GLfloat ftime = 0.f;
glm::mat4 view = glm::mat4(1.0);
glm::mat4 proj = glm::perspective(80.0f, 1.0f, 0.01f, 1000.f);
shaders::Params params;
Light light;
std::vector<std::unique_ptr<Shapes>> shapes;
GLint wWindow = 800;
GLint hWindow = 800;
float sh = 1;

// called when a window is reshaped
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glEnable(GL_DEPTH_TEST);
    // remember the settings for the camera
    wWindow = w;
    hWindow = h;
}

// void arm(glm::mat4 m) {
//    // let's use instancing
//    m = glm::translate(m, glm::vec3(0, 0.5, 0.0));
//    m = glm::scale(m, glm::vec3(1.0f, 1.0f, 1.0f));
//    sphere->setModel(m);
//    // now the normals
//    glm::mat3 modelViewN = glm::mat3(view * m);
//    modelViewN = glm::transpose(glm::inverse(modelViewN));
//    sphere->setModelViewN(modelViewN);
//    sphere->render();
//
//    m = glm::translate(m, glm::vec3(0.0, 0.5, 0.0));
//    m = glm::rotate(m, -20.0f * ftime, glm::vec3(0.0, 0.0, 1.0));
//    m = glm::translate(m, glm::vec3(0.0, 1.5, 0.0));
//    sphere->setModel(glm::scale(m, glm::vec3(0.5f, 1.0f, 0.5f)));
//
//    modelViewN = glm::mat3(view * m);
//    modelViewN = glm::transpose(glm::inverse(modelViewN));
//    sphere->setModelViewN(modelViewN);
//    sphere->render();
//}

// the main rendering function
void renderObjects() {
    const int range = 3;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0, 0, 0);
    glPointSize(2);
    glLineWidth(1);
    // set the projection and view once for the scene
    glUniformMatrix4fv(params.projParameter, 1, GL_FALSE, glm::value_ptr(proj));
    // view=glm::lookAt(glm::vec3(25*sin(ftime/40.f),5.f,15*cos(ftime/40.f)),//eye
    //			     glm::vec3(0,0,0),  //destination
    //			     glm::vec3(0,1,0)); //up
    view = glm::lookAt(glm::vec3(10.f, 5.f, 10.f),  // eye
                       glm::vec3(0, 0, 0),          // destination
                       glm::vec3(0, 1, 0));         // up

    glUniformMatrix4fv(params.viewParameter, 1, GL_FALSE, glm::value_ptr(view));
    // set the light
    static glm::vec4 pos;
    pos.x = 20 * sin(ftime / 12);
    pos.y = -10;
    pos.z = 20 * cos(ftime / 12);
    pos.w = 1;
    light.setPos(pos);
    light.setShaders();
    for (auto& shape : shapes)
        shape->render();
    // for (int i = -range; i < range; i++) {
    //    for (int j = -range; j < range; j++) {
    //        glm::mat4 m = glm::translate(glm::mat4(1.0), glm::vec3(4 * i, 0, 4 * j));
    //        arm(m);
    //    }
    //}
}

void idle(void) {
    glClearColor(0.1, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ftime += 0.05;
    glUseProgram(shaderProgram);
    renderObjects();
    glutSwapBuffers();
}

void display(void) {
}

// keyboard callback
void kbd(unsigned char a, int x, int y) {
    switch (a) {
        case 27:
            exit(0);
            break;
        case '+':
        case '=':
            sh += 1;
            for (auto& shape : shapes)
                shape->setSh(sh);
            break;
        case '_':
        case '-':
            sh -= 1;
            if (sh < 1)
                sh = 1;
            for (auto& shape : shapes)
                shape->setSh(sh);
            break;
    }
    std::cout << "shineness=" << sh << '\n';
    glutPostRedisplay();
}

// special keyboard callback
void specKbdPress(int a, int x, int y) {
    switch (a) {
        case GLUT_KEY_LEFT:
            break;
        case GLUT_KEY_RIGHT:
            break;
        case GLUT_KEY_DOWN:
            break;
        case GLUT_KEY_UP:
            break;
    }
    glutPostRedisplay();
}

// called when a special key is released
void specKbdRelease(int a, int x, int y) {
    switch (a) {
        case GLUT_KEY_LEFT:
            break;
        case GLUT_KEY_RIGHT:
            break;
        case GLUT_KEY_DOWN:
            break;
        case GLUT_KEY_UP:
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
}

void initShapes(shaders::Params* params) {
    const int range = 3;
    const int maxS = 5;
    const int minS = 50;

    shapes.clear();
    shapes.reserve(4 * range * range);
    for (int i = -range; i < range; i++) {
        for (int j = -range; j < range; j++) {
            int stacks = (maxS - minS) * (range + i) / (2 * range - 1) + minS;
            int slices = (maxS - minS) * (range + j) / (2 * range - 1) + minS;
            double r = (range + i) / (2.0 * range - 1.0);
            double g = (range + j) / (2.0 * range - 1.0);
            double b = 1.0;
            auto& sphere = shapes.emplace_back(std::make_unique<Sphere>(stacks, slices, 1));
            sphere->setKa(glm::vec3(0.1, 0.1, 0.1));
            sphere->setKs(glm::vec3(0, 0, 1));
            sphere->setKd(glm::vec3(r, g, b));
            sphere->setSh(sh);
            sphere->setModel(glm::translate(glm::mat4(1.0), glm::vec3(4 * i, 0, 4 * j)));
            sphere->setModelMatrixParamToShader(params->modelParameter);
            sphere->setModelViewNMatrixParamToShader(params->modelViewNParameter);
            sphere->setKaToShader(params->kaParameter);
            sphere->setKdToShader(params->kdParameter);
            sphere->setKsToShader(params->ksParameter);
            sphere->setShToShader(params->shParameter);
        }
    }
}

}  // namespace dng
