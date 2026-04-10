#ifndef _RPGPP_SOUNDSERVICE_H
#define _RPGPP_SOUNDSERVICE_H

#include <raylib.h>

#include <string>

//** The SoundService is responsible for playing sounds and managing the current background music */
class SoundService {
	//** The current Raylib Music instance */
	Music music;
	//** Whether the music was loaded or not */
	bool musicLoaded;
	//** The id of the last played music */
	std::string lastId;

public:
	//** Default constructor */
	SoundService();
	//** Load music with the specified id. */
	bool loadMusic(const std::string &id);
	//** Play the loaded music. */
	void playMusic() const;
	//** Play sound with the specified id. */
	void playSound(const std::string &id) const;
	//** Update routine. */
	void update() const;
	//** Unload routine. */
	void unload() const;
};

#endif
