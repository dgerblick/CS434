#ifndef __WINDOW_H__
#define __WINDOW_H__

namespace dng::window {

const int WIDTH = 800;
const int HEIGHT = 800;
const int MAX_FRAME_TIME = 1000 / 60.0f;

void init();
void display();
void idle();
void timer(int t);
void resetTimer();

}  // namespace dng::window

#endif  // __WINDOW_H__