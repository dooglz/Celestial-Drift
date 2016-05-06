#pragma once

class PC_Platform {
public:
  static bool Init();
  static bool Shutdown();

protected:
private:
  PC_Platform() = delete;
  ~PC_Platform() = delete;
  // delete copy/move constructors & assign operators
  PC_Platform(PC_Platform const &) = delete;
  PC_Platform(PC_Platform &&) = delete;
  PC_Platform &operator=(PC_Platform const &) = delete;
  PC_Platform &operator=(PC_Platform &&) = delete;
};
