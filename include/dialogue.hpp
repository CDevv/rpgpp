#ifndef _RPGPP_DIALOGUE
#define _RPGPP_DIALOGUE

#include "dialogueBalloon.hpp"
#include "saveable.hpp"
#include <nlohmann/json_fwd.hpp>
class Dialogue : public ISaveable {
  private:
	DialogueBin dialogueBin;

  public:
	Dialogue() = default;
	Dialogue(const std::string &filePath);
	DialogueLine addNewLine();
	nlohmann::json dumpJson() override;
	DialogueBin &getData();
};

#endif