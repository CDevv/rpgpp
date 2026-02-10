#ifndef _RPGPP_EDITOR_DIALOGUEPARSER_H
#define _RPGPP_EDITOR_DIALOGUEPARSER_H

#include <dialogueBalloon.hpp>
#include <nlohmann/json.hpp>
#include <string>
using json = nlohmann::json;

Dialogue parseDialogueText(std::string text);
json dialogueDumpJson(const Dialogue &diag);

#endif