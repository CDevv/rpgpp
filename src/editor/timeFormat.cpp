#include "timeFormat.hpp"
#include <cmath>
#include <raylib.h>
#include <string>

std::string formatTime(float duration) {
	std::string result = "";

	int secs = ceil(duration);
	int mins = secs / 60;
	int remainingSecs = secs - (mins * 60);

	std::string minsText = TextFormat("%i", mins);
	if (mins < 10) {
		minsText.insert(0, "0");
	}

	std::string secsText = TextFormat("%i", remainingSecs);
	if (remainingSecs < 10) {
		secsText.insert(0, "0");
	}

	result = result.append(minsText).append(":").append(secsText);

	return result;
}