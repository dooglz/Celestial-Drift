#include "GL/glew.h"
#include "common.h"
#include "pc_video.h"
#include <GLFW/glfw3.h>

unsigned int PC_Video::FB_SIZE_X = DEFAULT_RESOLUTION_X;
unsigned int PC_Video::FB_SIZE_Y = DEFAULT_RESOLUTION_Y;

GLFWwindow *PC_Video::window_;

void PC_Video::Resize(GLFWwindow* window, int width, int height) { 
	LOG(logINFO) << "window Resized to " << width << " " << height;
	FB_SIZE_X = width;
	FB_SIZE_Y = height;
	glViewport(0, 0, width, height);
	return ; 
}

bool PC_Video::Init() {

  /* Initialize the library */
  if (!glfwInit())
    return false;

  glfwWindowHint(GLFW_RESIZABLE, 0);
  glfwWindowHint(GLFW_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  /* Create a windowed mode window and its OpenGL context */
  window_ = glfwCreateWindow(DEFAULT_RESOLUTION, GAME_NAME.c_str(), NULL, NULL);
  FB_SIZE_X = DEFAULT_RESOLUTION_X;
  FB_SIZE_Y = DEFAULT_RESOLUTION_Y;
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

  glfwSetFramebufferSizeCallback(window_, &Resize);

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
