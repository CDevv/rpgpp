#include "dialogueParser.hpp"

#include <cstdio>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "dialogueBalloon.hpp"
using json = nlohmann::json;

std::vector<DialogueTextSection> parseDialogueContent(const std::string &t) {
	std::string tagName = "";
	std::string tagContent = "";
	std::string tagClose = "";
	bool parseTag = false;
	bool parseTagContent = true;
	bool parseTagEnd = false;
	bool parseTagClose = false;

	std::vector<DialogueTextSection> v;
	int idx = 0;
	for (auto c : t) {
		if (c == '<' && !parseTag) {
			if (tagName == "") {
				v.push_back({"", tagContent});
				tagContent = "";
			}

			parseTagContent = false;
			parseTag = true;
		} else if (c == '>') {
			parseTagEnd = true;
			parseTag = false;

			if (parseTagClose) {
				parseTagClose = false;

				if (tagName == tagClose) {
					v.push_back({tagName, tagContent});

					tagName = "";
					tagContent = "";
					tagClose = "";
					parseTag = false;
					parseTagContent = true;
					parseTagEnd = false;
					parseTagClose = false;
				}
			}
		}

		if (c == '/') {
			parseTag = false;
			parseTagClose = true;
		}

		if (parseTag && c != '<') {
			tagName.push_back(c);
		} else if (parseTagClose && c != '/') {
			tagClose.push_back(c);
		}

		if (parseTagEnd) {
			parseTagContent = true;
			parseTagEnd = false;
		}

		if (parseTagContent && c != '>') {
			tagContent.push_back(c);
		}

		idx++;

		if (idx == (t.length())) {
			v.push_back({"", tagContent});
			tagContent = "";
		}
	}

	for (auto item : v) {
		printf("%s : %s \n", item.key.c_str(), item.text.c_str());
	}

	return v;
}

Dialogue parseDialogueText(std::string text) {
	Dialogue result;

	json jsonObj = json::parse(text);
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

	return result;
}

json dialogueDumpJson(const Dialogue &diag) {
	std::vector<std::vector<std::string>> diagVec;
	for (DialogueLine line : diag.lines) {
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