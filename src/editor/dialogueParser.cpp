#include "dialogueParser.hpp"
#include "dialogueBalloon.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

Dialogue parseDialogueText(std::string text)
{
    Dialogue result;

    json jsonObj = json::parse(text);
    std::vector<std::vector<std::string>> diagVector = jsonObj.at("diag");
    for (auto lineVec : diagVector) {
        if (lineVec.size() == 2) {
            DialogueLine line;
            line.characterName = lineVec.at(0);
            line.text = lineVec.at(1);
            result.lines.push_back(line);
        }
    }

    return result;
}

json dialogueDumpJson(Dialogue diag)
{
    std::vector<std::vector<std::string>> diagVec;
    for (DialogueLine line : diag.lines) {
        std::vector<std::string> lineVec;
        lineVec.push_back(line.characterName);
        lineVec.push_back(line.text);

        diagVec.push_back(lineVec);
    }

    json result = {
        {"diag", diagVec}
    };

    return result;
}
