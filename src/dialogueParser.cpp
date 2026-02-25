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
