#include "pc_video.h"
#include "GL/glew.h"
#include "pc_renderer.h"
#include "common.h"
#include <GLFW/glfw3.h>

unsigned int PC_Video::FB_SIZE_X = DEFAULT_RESOLUTION_X;
unsigned int PC_Video::FB_SIZE_Y = DEFAULT_RESOLUTION_Y;

GLFWwindow *PC_Video::window_;

void PC_Video::SplitviewPort(const unsigned int total, const unsigned int active) {
  if (total <= 1) {
    glViewport(0, 0, FB_SIZE_X, FB_SIZE_Y);
    return;
  }
  if (total == 2) {
    glViewport(0, active * (FB_SIZE_Y / 2), FB_SIZE_X, (FB_SIZE_Y / 2));
  }
  return;
}

void PC_Video::Resize(GLFWwindow *window, int width, int height) {
  LOG(logINFO) << "window Resized to " << width << " " << height;
  FB_SIZE_X = width;
  FB_SIZE_Y = height;
  glViewport(0, 0, width, height);
  PC_Renderer::ResetProjectionMatrix();
  return;
}

bool PC_Video::Init() {

  /* Initialize the library */
  if (!glfwInit())
    return false;

  glfwWindowHint(GLFW_RESIZABLE, 0);
  glfwWindowHint(GLFW_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

#ifdef __APPLE__
  /* We need to explicitly ask for a 3.2 context on OS X */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  // glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

  /* Create a windowed mode window and its OpenGL context */
  window_ = glfwCreateWindow(DEFAULT_RESOLUTION, GAME_NAME.c_str(), NULL, NULL);

  FB_SIZE_X = DEFAULT_RESOLUTION_X;
  FB_SIZE_Y = DEFAULT_RESOLUTION_Y;
  if (!window_) {
    return false;
  }
#if defined(_DEBUG)
// glfwSetWindowPos(window_, 5400, 100);
#endif
  /* Make the window's context current */
  glfwMakeContextCurrent(window_);
  glfwSwapInterval(0);
  glewInit();

  std::cout << "GL_VERSION " << glGetString(GL_VERSION) << std::endl;

  glfwSetFramebufferSizeCallback(window_, &Resize);
  int w, h;
  glfwGetFramebufferSize(window_, &w, &h);
  Resize(nullptr, w, h);
  return true;
}
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
