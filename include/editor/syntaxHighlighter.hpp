#ifndef RPGPP_SYNTAX_HIGHLIGHTER
#define RPGPP_SYNTAX_HIGHLIGHTER
#include <TGUI/Color.hpp>
#include <TGUI/Text.hpp>
#include <TGUI/TextStyle.hpp>
#include <TGUI/Vector2.hpp>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>

namespace EditorHighlighting {

struct HighlighterStruct {
  public:
	std::string type;
	uint32_t start;
	uint32_t end;
	bool hasChild;
};

struct TextStyling {
  public:
	tgui::Color color;
	tgui::TextStyle textStyle;
};

struct TextPiece {
	tgui::Text text;
	tgui::Vector2f pos;
	std::size_t nodeStart;
	std::size_t nodeEnd;
};

} // namespace EditorHighlighting

#endif /* RPGPP_SYNTAX_HIGHLIGHTER */
