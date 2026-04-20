#ifndef _RPGPP_EDITOR_LISTHELPER_H
#define _RPGPP_EDITOR_LISTHELPER_H

#include <sstream>
#include <string>
#include <vector>

template <typename T>
std::string VecToString(const std::vector<T> &vec) {
	std::stringstream stream;
	int i = 0;
	for (auto &item : vec) {
		stream << item;
		if (i != (vec.size() - 1)) {
			stream << ";";
		}
		i++;
	}
	return stream.str();
}

#endif