#pragma once

class PC_Sound {
public:
  static bool Init();
  static bool Shutdown();
  static void Update(double delta);
  static void bump();
protected:
private:
  PC_Sound() = delete;
  ~PC_Sound() = delete;
  // delete copy/move constructors & assign operators
  PC_Sound(PC_Sound const &) = delete;
  PC_Sound(PC_Sound &&) = delete;
  PC_Sound &operator=(PC_Sound const &) = delete;
  PC_Sound &operator=(PC_Sound &&) = delete;
};
