#pragma once
struct GLFWwindow;

class PC_Video {
public:
  static bool Init();
  static bool Resize();
  static bool Swap();
  static bool Shutdown();
  static unsigned int FB_SIZE_X;
  static unsigned int FB_SIZE_Y;
  //
  static GLFWwindow *GetWindow();

protected:
  static GLFWwindow *window_;
  static void Resize(GLFWwindow *window, int width, int height);

private:
  PC_Video() = delete;
  ~PC_Video() = delete;
  // delete copy/move constructors & assign operators
  PC_Video(PC_Video const &) = delete;
  PC_Video(PC_Video &&) = delete;
  PC_Video &operator=(PC_Video const &) = delete;
  PC_Video &operator=(PC_Video &&) = delete;
};
