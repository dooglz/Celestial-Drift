#ifndef _FMODORBIS_H
#define _FMODORBIS_H

#include <user_service.h>

/*
[ENUM] 
[
    [DESCRIPTION]
    Cores available for mapping threads onto.

    [REMARKS]

    [SEE_ALSO]
    FMOD_ORBIS_THREADAFFINITY
]
*/
typedef enum
{
    FMOD_THREAD_DEFAULT,    /* Use default thread assignment. */
    FMOD_THREAD_CORE0 = 1 << 0,
    FMOD_THREAD_CORE1 = 1 << 1,
    FMOD_THREAD_CORE2 = 1 << 2,      /* Default for all threads. */
    FMOD_THREAD_CORE3 = 1 << 3,
    FMOD_THREAD_CORE4 = 1 << 4,
    FMOD_THREAD_CORE5 = 1 << 5,
    FMOD_THREAD_CORE6 = 1 << 6,
    FMOD_THREAD_MAX         /* Maximum number of thread options supported. */
} FMOD_THREAD;


/*
[STRUCTURE] 
[
    [DESCRIPTION]   
    Mapping of cores to threads.

    [REMARKS]

    [SEE_ALSO]
    FMOD_THREAD
    FMOD_Orbis_SetThreadAffinity
]
*/
typedef struct FMOD_ORBIS_THREADAFFINITY
{
    unsigned int mixer;          /* Software mixer thread. */
    unsigned int feeder;         /* Audio hardware feeder thread. */
    unsigned int stream;         /* Stream thread. */
    unsigned int nonblocking;    /* Asynchronous sound loading thread. */
    unsigned int file;           /* File thread. */
    unsigned int geometry;       /* Geometry processing thread. */
    unsigned int recording;      /* Audio input feeder thread. */
    unsigned int studioUpdate;   /* Studio update thread. */
    unsigned int studioLoad;     /* Studio loading thread. */
} FMOD_ORBIS_THREADAFFINITY;


/*
[API]
[
    [DESCRIPTION]
    Control which core particular FMOD threads are created on.

    [PARAMETERS]
    'affinity'    Pointer to a structure that describes the affinity for each thread.

    [REMARKS]
    Call before System::init or affinity values will not apply.

    [SEE_ALSO]
    FMOD_ORBIS_THREADAFFINITY
]
*/
extern "C" FMOD_RESULT F_API FMOD_Orbis_SetThreadAffinity(FMOD_ORBIS_THREADAFFINITY *affinity);

/*
[ENUM] 
[
    [DESCRIPTION]
    Port types avaliable for routing audio.

    [REMARKS]

    [SEE_ALSO]
    System::AttachChannelGroupToPort
    FMOD_PORT_TYPE
]
*/
typedef enum
{
    FMOD_PS4_PORT_TYPE_MUSIC,           /* Background music, pass FMOD_PORT_INDEX_NONE as port index */
    FMOD_PS4_PORT_TYPE_VOICE,           /* Voice chat, pass SceUserServiceUserId of desired user as port index */
    FMOD_PS4_PORT_TYPE_PERSONAL,        /* Personal audio device, pass SceUserServiceUserId of desired user as port index */
    FMOD_PS4_PORT_TYPE_CONTROLLER,      /* Controller speaker, pass SceUserServiceUserId of desired user as port index */
    FMOD_PS4_PORT_TYPE_COPYRIGHT_MUSIC, /* Copyright background music, pass FMOD_PORT_INDEX_NONE as port index. You cannot have both copyright and non-copyright music open simultaneously */
    FMOD_PS4_PORT_TYPE_MAX              /* Maximum number of port types supported. */
} FMOD_PS4_PORT_TYPES;

/*
[API]
[
    [DESCRIPTION]
    Retrieve the output volume of the pad speaker as set by the user in the system software.

    [PARAMETERS]
    'userID'  SceUserServiceUserId that identifies a logged in user.
    'volume'  Output parameter to receive the volume

    [REMARKS]
    There must be at least one channel group attached to the users pad speaker for this function to work.

    [SEE_ALSO]
    System::AttachChannelGroupToPort
]
*/
extern "C" FMOD_RESULT F_API FMOD_Orbis_GetPadVolume(FMOD_SYSTEM* system, SceUserServiceUserId userID, float* volume);

#endif /* _FMODORBIS_H */
