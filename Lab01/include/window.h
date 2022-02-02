#ifndef __SCENE_H__
#define __SCENE_H__

#include <shaders.h>
#include <lights.h>
#include <shapes.h>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace dng {

extern GLuint shaderProgram;
extern shaders::Params params;
extern Light light;
extern std::vector<std::unique_ptr<Shapes>> shapes;
extern GLint wWindow;
extern GLint hWindow;

void reshape(int w, int h);
void renderObjects();
void idle();
void display();
void kbd(unsigned char a, int x, int y);
void kbdRelease(unsigned char a, int x, int y);
void specKbdPress(int a, int x, int y);
void specKbdRelease(int a, int x, int y);
void mouse(int button, int state, int x, int y);
void initializeProgram(GLuint* program);
void initShapes(shaders::Params* params);
glm::vec3 lookVec();

const float LOOK_KEY_RATE = M_PI;
const float MOVE_KEY_RATE = 10.0f;
const float LOOK_LIMIT = 0.01f;
const glm::vec3 UP(0.0f, 1.0f, 0.0f);

}  // namespace dng

#endif  // __SCENE_H__