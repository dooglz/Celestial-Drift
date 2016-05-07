#include "GL/glew.h"
#include "common.h"
#include "pc_video.h"
#include <GLFW/glfw3.h>

GLFWwindow *PC_Video::window_;

bool PC_Video::Init() {

  /* Initialize the library */
  if (!glfwInit())
    return false;

  glfwWindowHint(GLFW_RESIZABLE, 0);
  glfwWindowHint(GLFW_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_VERSION_MINOR, 3);

  /* Create a windowed mode window and its OpenGL context */
  window_ = glfwCreateWindow(DEFAULT_RESOLUTION, "Hello World", NULL, NULL);
  if (!window_) {
    return false;
  }
#if defined(_DEBUG)
 //glfwSetWindowPos(window_, 5400, 100);
#endif
  /* Make the window's context current */
  glfwMakeContextCurrent(window_);
  glfwSwapInterval(0);
  glewInit();

  return true;
}
bool PC_Video::Resize() { return false; }

bool PC_Video::Shutdown() {
  glfwTerminate();
  glfwDestroyWindow(window_);
  glfwTerminate();
  return true;
}

bool PC_Video::Swap() {
  glfwSwapBuffers(window_);
  return true;
}

GLFWwindow *PC_Video::GetWindow() { return window_; }
