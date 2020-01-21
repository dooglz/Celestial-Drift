#include "menu.h"
#include "font.h"
#include "gamelogic.h"
#include <iostream>
#if defined(_PLATFORM_X64) || defined(WIN32)
#include "pc_menu.h"
#include "pc_video.h"
#elif defined(_PLATFORM_PS4)
#include "ps4_menu.h"
#else

#endif

Menu *Menu::activeMenu = nullptr;
vector<MenuItem *> pastitems;
extern bool RUNGAME;
bool helpmenu = false;

Menu::Menu() {
  position_ = glm::vec2(0, 0);
  maxWidth_ = 0;
  platformHandle_ = Platform_Menu::Prep();
}

Menu::Menu(const glm::vec2 &pos) {
  position_ = pos;
  maxWidth_ = 0;
  platformHandle_ = Platform_Menu::Prep();
}

vector<MenuItem *> *Menu::GetItems() { return &items_; }
void Menu::Move(const MenuDirection &dir) {

  int8_t direction = 0;
  if (dir == MENU_DOWN) {
    direction = 1;
  } else if (dir == MENU_UP) {
    direction = -1;
  } else if (dir == MENU_RIGHT) {
    for (auto &i : items_) {
      if (i->selected && i->func != nullptr) {
        i->func({});
      }
    }
  } else {
    return;
  }

  vector<MenuItem *> si;
  for (auto &i : items_) {
    if (i->selectable) {
      si.push_back(i);
      i->selected = false;
    }
  }
  static int selected = 0;
  if (si.size() == 0) {
    selected = 0;
  } else {
    selected += direction;
    if (selected < 0) {
      selected = (uint32_t)si.size() - 1;
    } else if (selected > si.size() - 1) {
      selected = 0;
    }
    si[selected]->selected = true;
  }
}

Menu::~Menu() {}
void *Menu::GetPlatformHandle() { return platformHandle_; };

MenuItem::MenuItem() {
  text = "";
  selected = false;
  selectable = false;
}

MenuItem::MenuItem(const string &str, const bool &sble, std::function<bool(const std::vector<std::string> &param)> cb) {
  text = str;
  selected = false;
  selectable = sble;
  func = cb;
}

MenuItem::~MenuItem() {}

void Menu::Update() {
  const auto s = items_.size();
  if (s == 0) {
    return;
  }
  positions_.clear();
  colours_.clear();
  totalHeight_ = (float)(s * (MENU_BUTTON_HEIGHT + MENU_BUTTON_HEIGHT_SPACE));
  maxWidth_ = 0;
  for (size_t i = 0; i < items_.size(); i++) {
    maxWidth_ = (items_[i]->text.size() > maxWidth_ ? items_[i]->text.size() : maxWidth_);
    positions_.push_back(position_ + glm::vec2(0, i * (MENU_BUTTON_HEIGHT + MENU_BUTTON_HEIGHT_SPACE)));
    if (items_[i]->selectable) {
      if (items_[i]->selected) {
        colours_.push_back({0.651f, 0.976f, 0.0f, 0.7f});
        positions_[i].x += 20.0f;
      } else {
        colours_.push_back({1.0f, 1.0f, 0.0f, 0.7f});
      }
    } else {
      colours_.push_back({1.00f, 0.827f, 0.0f, 0.7f});
    }
  }

  maxWidth_ *= 30.0f;
  verts_[0] = {0, 0};
  verts_[1] = {maxWidth_, 0};
  verts_[2] = {0, MENU_BUTTON_HEIGHT};
  verts_[3] = {maxWidth_, MENU_BUTTON_HEIGHT};
  verts_[4] = {maxWidth_ + 30.0f, MENU_BUTTON_HEIGHT};
  Platform_Menu::Update(platformHandle_, verts_, positions_, colours_);
};

void Menu::Render() {
  Platform_Menu::Render(platformHandle_, items_.size());
  for (size_t i = 0; i < items_.size(); i++) {
    const glm::vec2 p = positions_[i] + glm::vec2(20, 25);
    items_[i]->Render(p, {maxWidth_, MENU_BUTTON_HEIGHT});
  }
}

void MenuItem::Render(const glm::vec2 &pos, const glm::vec2 &size) {
  // Platform_Menu::Render(cplatformHandle_);
  Font::Draw(25, text.c_str(), pos, {0.0f, 0, 0, 1.0f});
}

void Menu::SetPosition(const glm::vec2 &pos) { position_ = pos; }
