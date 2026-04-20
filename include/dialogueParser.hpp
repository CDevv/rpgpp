#ifndef _RPGPP_EDITOR_DIALOGUEPARSER_H
#define _RPGPP_EDITOR_DIALOGUEPARSER_H

#include <dialogueBalloon.hpp>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
using json = nlohmann::json;

std::vector<std::string> getColorTypes();
std::map<std::string, Color> &getColors();
std::vector<DialogueTextSection> parseDialogueContent(const std::string &t);

#endif