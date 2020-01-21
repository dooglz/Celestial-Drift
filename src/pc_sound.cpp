#ifdef SOUND
#include "pc_sound.h"
#include "common.h"
#include "filesystem.h"
#include "fmod/fmod.hpp"

static FMOD::System *fmsystem;
static FMOD::Sound *sound1, *sound2;
static FMOD::Channel *channel = 0;

bool PC_Sound::Init() {
  FMOD_RESULT result;
  unsigned int version;
  void *extradriverdata = 0;

  // Common_Init(&extradriverdata);

  /*
  Create a System object and initialize
  */
  result = FMOD::System_Create(&fmsystem);
  result = fmsystem->getVersion(&version);

  if (version < FMOD_VERSION) {
    // Common_Fatal("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
    return false;
  }

  result = fmsystem->init(32, FMOD_INIT_NORMAL, extradriverdata);

  std::string aa = fileIO::SearchDirsPath("bgm.ogg");
  result = fmsystem->createSound(aa.c_str(), FMOD_DEFAULT, 0, &sound1);
  result = sound1->setMode(FMOD_LOOP_NORMAL);
  result = fmsystem->playSound(sound1, 0, false, &channel);
  aa = fileIO::SearchDirsPath("bump.ogg");
  result = fmsystem->createSound(aa.c_str(), FMOD_DEFAULT, 0, &sound2);
  result = sound2->setMode(FMOD_LOOP_OFF);
  // result = system->playSound(sound2, 0, false, &channel);
  return true;
}

void PC_Sound::bump() { fmsystem->playSound(sound2, 0, false, &channel); }

void PC_Sound::Update(double delta) {}

bool PC_Sound::Shutdown() { return false; }
#endif
