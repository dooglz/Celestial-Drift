#include "filesystem.h"
#include "maths.h"
#include <fmod\fmod.h>
#include <fmod\fmod.hpp>
#include <fmod\fmod_common.h>
#include <fmod\fmod_errors.h>
#include <vector>

using namespace std;
class SoundManager {
public:
  SoundManager();
  SoundManager(const glm::vec2 &pos, const glm::vec2 &vel);
  ~SoundManager();
  bool init();
  void Update();
  void SetPosition(const glm::vec2 &pos);
  void SetVelocity(const glm::vec2 &vel);
  void shutdown();
  // need to create a sounditem class
  // vector<SoundItem *> *GetItems();

  void *GetPlatformHandle();
};
