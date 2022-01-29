#include <algorithm>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shaders.h>
#include <lights.h>
#include <shapes.h>
#include <sphere.h>

bool needRedisplay = false;
dng::Shapes* sphere;

// shader program ID
GLuint shaderProgram;
GLfloat ftime = 0.f;
glm::mat4 view = glm::mat4(1.0);
glm::mat4 proj = glm::perspective(80.0f,           // fovy
                                  1.0f,            // aspect
                                  0.01f, 1000.f);  // near, far
class ShaderParams {
public:
    GLint modelParameter;       // modeling matrix
    GLint modelViewNParameter;  // modeliview for normals
    GLint viewParameter;        // viewing matrix
    GLint projParameter;        // projection matrix
    // material
    GLint kaParameter;  // ambient material
    GLint kdParameter;  // diffuse material
    GLint ksParameter;  // specular material
    GLint shParameter;  // shinenness material
} params;

dng::Light light;

// the main window size
GLint wWindow = 800;
GLint hWindow = 800;

float sh = 1;

/*********************************
Some OpenGL-related functions
**********************************/

// called when a window is reshaped
void Reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glEnable(GL_DEPTH_TEST);
    // remember the settings for the camera
    wWindow = w;
    hWindow = h;
}

void Arm(glm::mat4 m) {
    // let's use instancing
    m = glm::translate(m, glm::vec3(0, 0.5, 0.0));
    m = glm::scale(m, glm::vec3(1.0f, 1.0f, 1.0f));
    sphere->setModel(m);
    // now the normals
    glm::mat3 modelViewN = glm::mat3(view * m);
    modelViewN = glm::transpose(glm::inverse(modelViewN));
    sphere->setModelViewN(modelViewN);
    sphere->render();

    m = glm::translate(m, glm::vec3(0.0, 0.5, 0.0));
    m = glm::rotate(m, -20.0f * ftime, glm::vec3(0.0, 0.0, 1.0));
    m = glm::translate(m, glm::vec3(0.0, 1.5, 0.0));
    sphere->setModel(glm::scale(m, glm::vec3(0.5f, 1.0f, 0.5f)));

    modelViewN = glm::mat3(view * m);
    modelViewN = glm::transpose(glm::inverse(modelViewN));
    sphere->setModelViewN(modelViewN);
    sphere->render();
}

// the main rendering function
void RenderObjects() {
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
    for (int i = -range; i < range; i++) {
        for (int j = -range; j < range; j++) {
            glm::mat4 m = glm::translate(glm::mat4(1.0), glm::vec3(4 * i, 0, 4 * j));
            Arm(m);
        }
    }
}

void Idle(void) {
    glClearColor(0.1, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ftime += 0.05;
    glUseProgram(shaderProgram);
    RenderObjects();
    glutSwapBuffers();
}

void Display(void) {
}

// keyboard callback
void Kbd(unsigned char a, int x, int y) {
    switch (a) {
        case 27:
            exit(0);
            break;
        case 'r':
        case 'R': {
            sphere->setKd(glm::vec3(1, 0, 0));
            break;
        }
        case 'g':
        case 'G': {
            sphere->setKd(glm::vec3(0, 1, 0));
            break;
        }
        case 'b':
        case 'B': {
            sphere->setKd(glm::vec3(0, 0, 1));
            break;
        }
        case 'w':
        case 'W': {
            sphere->setKd(glm::vec3(0.7, 0.7, 0.7));
            break;
        }
        case '+': {
            sphere->setSh(sh += 1);
            break;
        }
        case '-': {
            sphere->setSh(sh -= 1);
            if (sh < 1)
                sh = 1;
            break;
        }
    }
    std::cout << "shineness=" << sh << '\n';
    glutPostRedisplay();
}

// special keyboard callback
void SpecKbdPress(int a, int x, int y) {
    switch (a) {
        case GLUT_KEY_LEFT: {
            break;
        }
        case GLUT_KEY_RIGHT: {
            break;
        }
        case GLUT_KEY_DOWN: {
            break;
        }
        case GLUT_KEY_UP: {
            break;
        }
    }
    glutPostRedisplay();
}

// called when a special key is released
void SpecKbdRelease(int a, int x, int y) {
    switch (a) {
        case GLUT_KEY_LEFT: {
            break;
        }
        case GLUT_KEY_RIGHT: {
            break;
        }
        case GLUT_KEY_DOWN: {
            break;
        }
        case GLUT_KEY_UP: {
            break;
        }
    }
    glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y) {
    std::cout << "Location is "
              << "[" << x << "'" << y << "]\n";
}

void InitializeProgram(GLuint* program) {
    std::vector<GLuint> shaders;

    // load and compile shaders
    shaders.push_back(dng::CreateShader(GL_VERTEX_SHADER, dng::LoadShader("shaders/phong.vert")));
    shaders.push_back(dng::CreateShader(GL_FRAGMENT_SHADER, dng::LoadShader("shaders/phong.frag")));

    // create the shader program and attach the shaders to it
    *program = dng::CreateProgram(shaders);

    // delete shaders (they are on the GPU now)
    std::for_each(shaders.begin(), shaders.end(), glDeleteShader);

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

void InitShapes(ShaderParams* params) {
    // create one unit sphere in the origin
    sphere = new dng::Sphere(50, 50, 1);
    sphere->setKa(glm::vec3(0.1, 0.1, 0.1));
    sphere->setKs(glm::vec3(0, 0, 1));
    sphere->setKd(glm::vec3(0.7, 0.7, 0.7));
    sphere->setSh(200);
    sphere->setModel(glm::mat4(1.0));
    sphere->setModelMatrixParamToShader(params->modelParameter);
    sphere->setModelViewNMatrixParamToShader(params->modelViewNParameter);
    sphere->setKaToShader(params->kaParameter);
    sphere->setKdToShader(params->kdParameter);
    sphere->setKsToShader(params->ksParameter);
    sphere->setShToShader(params->shParameter);
}

int main(int argc, char** argv) {
    glutInitDisplayString("stencil>=2 rgb double depth samples");
    glutInit(&argc, argv);
    glutInitWindowSize(wWindow, hWindow);
    glutInitWindowPosition(500, 100);
    glutCreateWindow("Model View Projection GLSL");
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glutMouseFunc(Mouse);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Kbd);              //+ and -
    glutSpecialUpFunc(SpecKbdRelease);  // smooth motion
    glutSpecialFunc(SpecKbdPress);
    InitializeProgram(&shaderProgram);
    InitShapes(&params);
    glutMainLoop();
    return 0;
}
