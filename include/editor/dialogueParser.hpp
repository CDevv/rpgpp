#ifndef _RPGPP_EDITOR_DIALOGUEPARSER_H
#define _RPGPP_EDITOR_DIALOGUEPARSER_H

#include <string>
#include <dialogueBalloon.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

Dialogue parseDialogueText(std::string text);
json dialogueDumpJson(Dialogue diag);

#endif
