#ifndef _RPGPP_SOUNDSERVICE_H
#define _RPGPP_SOUNDSERVICE_H

#include <raylib.h>
#include <string>

class SoundService {
	Music music;
	bool musicLoaded;
	std::string lastId;

  public:
	SoundService();
	bool loadMusic(const std::string &id);
	void playMusic() const;
	void playSound(const std::string &id) const;
	void update() const;
	void unload() const;
};

#endif
