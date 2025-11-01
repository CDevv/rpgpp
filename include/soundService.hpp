#ifndef _RPGPP_SOUNDSERVICE_H
#define _RPGPP_SOUNDSERVICE_H

#include <string>
#include <raylib.h>

class SoundService {
    Music music;
    bool musicLoaded;
    std::string lastId;
public:
    SoundService();
    bool loadMusic(std::string id);
    void playMusic();
    void update();
    void unload();
};

#endif
