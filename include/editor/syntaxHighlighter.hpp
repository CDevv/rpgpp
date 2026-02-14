#ifndef RPGPP_SYNTAX_HIGHLIGHTER
#define RPGPP_SYNTAX_HIGHLIGHTER
#include <TGUI/Color.hpp>
#include <TGUI/TextStyle.hpp>
#include <cstdint>
#include <map>
#include <string>

namespace EditorHighlighting {
struct HighlighterStruct {

  public:
	const char *type;
	uint32_t start;
	uint32_t end;
};

struct TextStyling {
  public:
	tgui::Color color;
	tgui::TextStyle textStyle;
};
} // namespace EditorHighlighting

#endif /* RPGPP_SYNTAX_HIGHLIGHTER */
