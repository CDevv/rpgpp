#include "dialogueParser.hpp"

#include <cstdio>
#include <nlohmann/json.hpp>
#include <pugixml.hpp>
#include <string>
#include <vector>

#include "dialogueBalloon.hpp"
#include "raylib.h"
using json = nlohmann::json;

std::map<std::string, Color> textColors = {{"lightgray", LIGHTGRAY}, {"gray", GRAY},	 {"darkgray", DARKGRAY},
										   {"gold", GOLD},			 {"yellow", YELLOW}, {"orange", ORANGE},
										   {"pink", PINK},			 {"red", RED},		 {"maroon", MAROON},
										   {"green", GREEN},		 {"lime", LIME},	 {"darkgreen", DARKGREEN},
										   {"skyblue", SKYBLUE},	 {"blue", BLUE},	 {"darkblue", DARKBLUE},
										   {"purple", PURPLE},		 {"violet", VIOLET}, {"darkpurple", DARKPURPLE},
										   {"beige", BEIGE},		 {"brown", BROWN},	 {"darkbrown", DARKBROWN},
										   {"magenta", MAGENTA},	 {"white", WHITE},	 {"black", BLACK}};

DialogueTextSection parseSection(pugi::xml_node node, DialogueTextSection base) {
	base.key = node.name();
	base.text = node.text().as_string();

	printf("%s : %s \n", base.key.c_str(), base.text.c_str());

	if (textColors.count(base.key) > 0) {
		base.textColor = textColors[base.key];
	}

	if (base.key == "textSize") {
		base.textSize = node.attribute("size").as_int(16);
	}

	if (base.key == "font") {
		base.font = node.attribute("font").as_string("LanaPixel");
	}

	if (base.key == "delay") {
		base.delay = node.attribute("delay").as_float(1.0f);
	}

	return base;
}

std::vector<DialogueTextSection> parseDialogueContent(const std::string &t) {
	std::vector<DialogueTextSection> v;

	pugi::xml_document xmlDoc;
	auto result = xmlDoc.load_string(TextFormat("<text>%s</text>", t.c_str()));
	if (result && !xmlDoc.child("text").empty()) {
		for (auto item : xmlDoc.child("text").children()) {
			DialogueTextSection textSection;
			textSection = parseSection(item, textSection);

			if (!item.children().empty()) {
				for (auto &subitem : item.children()) {
					DialogueTextSection subTextSection;
					subTextSection = parseSection(subitem, textSection);
					v.push_back(subTextSection);
				}
			} else {
				v.push_back(textSection);
			}
		}
	}

	return v;
}
