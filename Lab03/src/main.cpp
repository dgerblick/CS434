#include <window.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(dng::window::WIDTH, dng::window::HEIGHT);
    glutInitWindowPosition(500, 100);
    glutCreateWindow("CS 434 - Project 3");
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }
    glutDisplayFunc(dng::window::display);
    dng::window::init();
    glutMainLoop();
    return 0;
}
