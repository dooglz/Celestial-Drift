#include <glm/glm.hpp>

class PC_Font {
public:
  static bool Init();
  static bool Shutdown();
  static void Draw(const uint16_t &fontsizes, const char *text, const glm::vec2 &position, const glm::vec4 &color);
  static void Render();
  static void Clear();

protected:
private:
  PC_Font() = delete;
  ~PC_Font() = delete;
  // delete copy/move constructors & assign operators
  PC_Font(PC_Font const &) = delete;
  PC_Font(PC_Font &&) = delete;
  PC_Font &operator=(PC_Font const &) = delete;
  PC_Font &operator=(PC_Font &&) = delete;
};
