#include "CommandParser.h"
#include "common.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

#define MENU_BUTTON_HEIGHT 30
#define MENU_BUTTON_HEIGHT_SPACE 10
#define MENU_BUTTON_FONT 20

using namespace std;

enum MenuDirection { MENU_UP, MENU_DOWN, MENU_LEFT, MENU_RIGHT };

struct MenuItem;

class Menu {
public:
  Menu();
  Menu(const glm::vec2 &pos);
  ~Menu();
  void Update();
  void Render();
  void Move(const MenuDirection &dir);
  void SetPosition(const glm::vec2 &pos);
  vector<MenuItem *> *GetItems();
  void *GetPlatformHandle();
  static Menu *activeMenu;

private:
  float maxWidth_;
  float totalHeight_;
  vector<MenuItem *> items_;
  glm::vec2 position_;
  glm::vec2 verts_[5];
  vector<glm::vec2> positions_;
  vector<glm::vec4> colours_;
  void *platformHandle_;
};

struct MenuItem {
public:
  MenuItem();
  MenuItem(const string &str, const bool &selectable, std::function<bool(const std::vector<std::string> &param)> cb);
  ~MenuItem();
  void Render(const glm::vec2 &pos, const glm::vec2 &size);
  Menu *parent;
  bool selectable;
  bool selected;
  string text;
  std::function<bool(const std::vector<std::string> &param)> func;
};
