#ifdef SOUND
#include "SoundManager.h"
#include "maths.h"

FMOD::System *Soundsystem = NULL;
bool init() {
  FMOD_RESULT result;
  result = FMOD::System_Create(&Soundsystem); // Create the main system object.
  if (result != FMOD_OK) {
    printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
    exit(-1);
  }

  result = Soundsystem->init(512, FMOD_INIT_NORMAL, 0); // Initialize FMOD.
  if (result != FMOD_OK) {
    printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
    exit(-1);
  }

  return true;
}

void Update() {
  FMOD_RESULT result;

  result = Soundsystem->update();
  if (result != FMOD_OK) {
    printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
    exit(-1);
  }
}

void SetPosition(const glm::vec2 &pos) {}

void SetVelocity(const glm::vec2 &vel) {}

void shutdown() {
  Soundsystem->close();
  Soundsystem->release();
}
#endif
