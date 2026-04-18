#ifndef _RPGPP_EDITOR_DIALOGUEPARSER_H
#define _RPGPP_EDITOR_DIALOGUEPARSER_H

#include <dialogueBalloon.hpp>
#include <nlohmann/json.hpp>
#include <string>
using json = nlohmann::json;



std::vector<DialogueTextSection> parseDialogueContent(const std::string &t);

#endif