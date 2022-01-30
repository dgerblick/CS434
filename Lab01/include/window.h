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

extern bool needRedisplay;
extern GLuint shaderProgram;
extern GLfloat ftime;
extern glm::mat4 view;
extern glm::mat4 proj;
extern shaders::Params params;
extern Light light;
extern std::vector<std::unique_ptr<Shapes>> shapes;
extern GLint wWindow;
extern GLint hWindow;
extern float sh;

void reshape(int w, int h);
void renderObjects();
void idle();
void display();
void kbd(unsigned char a, int x, int y);
void specKbdPress(int a, int x, int y);
void specKbdRelease(int a, int x, int y);
void mouse(int button, int state, int x, int y);
void initializeProgram(GLuint* program);
void initShapes(shaders::Params* params);

}  // namespace dng

#endif  // __SCENE_H__