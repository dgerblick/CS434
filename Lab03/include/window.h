#ifndef __WINDOW_H__
#define __WINDOW_H__

namespace dng::window {

const int WIDTH = 800;
const int HEIGHT = 800;
const float DELTA_T = 1.0f / 60.0f;
const int DELTA_T_MS = (int) (1000 * DELTA_T);

void init();
void display();
void idle();
void timer(int t);
void resetTimer();

}  // namespace dng::window

#endif  // __WINDOW_H__