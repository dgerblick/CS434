#include <window.h>
#include <GL/glew.h>
#include <GL/glut.h>

int main(int argc, char** argv) {
    glutInitDisplayString("stencil>=2 rgb double depth samples");
    glutInit(&argc, argv);
    glutInitWindowSize(dng::wWindow, dng::hWindow);
    glutInitWindowPosition(500, 100);
    glutCreateWindow("Model View Projection GLSL");
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    glutDisplayFunc(dng::display);
    glutIdleFunc(dng::idle);
    glutMouseFunc(dng::mouse);
    glutReshapeFunc(dng::reshape);
    glutKeyboardFunc(dng::kbd);
    glutSpecialUpFunc(dng::specKbdRelease);
    glutSpecialFunc(dng::specKbdPress);
    dng::initializeProgram(&dng::shaderProgram);
    dng::initShapes(&dng::params);
    glutMainLoop();
    return 0;
}
