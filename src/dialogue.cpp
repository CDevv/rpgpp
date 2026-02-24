#include "dialogue.hpp"
#include "dialogueBalloon.hpp"
#include "dialogueParser.hpp"
#include "raylib.h"
#include <nlohmann/json_fwd.hpp>
#include <string>

Dialogue::Dialogue(const std::string &filePath) {
	DialogueBin result;

	json jsonObj = json::parse(LoadFileText(filePath.c_str()));
	std::vector<std::vector<std::string>> diagVector = jsonObj.at("diag");
	for (auto lineVec : diagVector) {
		if (lineVec.size() == 4) {
			DialogueLine line;
			line.characterName = lineVec.at(0);
			line.text = lineVec.at(1);
			line.hasPortrait = std::stoi(lineVec.at(2));
			line.imageId = lineVec.at(3);

			line.sections = parseDialogueContent(line.text);

			result.lines.push_back(line);
		}
	}

	dialogueBin = result;
}

nlohmann::json Dialogue::dumpJson() {
	std::vector<std::vector<std::string>> diagVec;
	for (DialogueLine line : dialogueBin.lines) {
		std::vector<std::string> lineVec;
		lineVec.push_back(line.characterName);
		lineVec.push_back(line.text);
		lineVec.push_back((line.hasPortrait ? "1" : "0"));
		lineVec.push_back(line.imageId);

		diagVec.push_back(lineVec);
	}

	json result = {{"diag", diagVec}};

	return result;
}

DialogueBin &Dialogue::getData() { return dialogueBin; }
