#include "projectMenuBox.hpp"
#include <IconsKenney.h>
#include "actor.hpp"
#include "dialogueBalloon.hpp"
#include "dialogueParser.hpp"
#include "gamedata.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "editorInterfaceService.hpp"
#include "projectFile.hpp"
#include "room.hpp"
#include "tileset.hpp"

ProjectMenuBox::ProjectMenuBox() {}

ProjectMenuBox::ProjectMenuBox(Rectangle rect)
{
	this->rect = rect;
}

void ProjectMenuBox::setRect(Rectangle rect)
{
	this->rect = rect;
}

void ProjectMenuBox::draw()
{
	FileSystemService& fs = Editor::getFileSystem();
	EditorInterfaceService& ui = Editor::getUi();

	float buttonSize = (rect.height - (4 * 2));
	ImVec2 buttonVec = ImVec2(buttonSize, buttonSize);

	ImGui::SetNextWindowPos(ImVec2 { rect.x, rect.y });
	ImGui::SetNextWindowSize(ImVec2 { rect.width, rect.height });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
	if (ImGui::Begin("ProjectMenuBox", nullptr,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)) {

		//Open Project
		if (ImGui::Button(ICON_KI_UPLOAD, buttonVec)) {
			fs.promptOpenProject();
			ui.setInitial();
		}
		ImGui::SameLine(0, 4);

		if (!fs.fileIsOpen()) ImGui::BeginDisabled();

		//Save File
		if (ImGui::Button(ICON_KI_SAVE, buttonVec)) {
			if (fs.fileIsOpen()) {
				if (fs.getType() == FILE_TILESET) {
	                TileSet *tileSet = fs.getCurrentFile()->getData<TileSet>();

	                std::string jsonString = tileSet->dumpJson().dump(4);

	                char *text = const_cast<char*>(jsonString.data());
	                SaveFileText(fs.getOpenedFilePath().c_str(), text);
	            } else if (fs.getType() == FILE_ROOM) {
	                std::string mapJsonString = fs.getCurrentFile()->getData<Room>()->dumpJson().dump(4);

	                char *text = const_cast<char*>(mapJsonString.data());
	                SaveFileText(fs.getOpenedFilePath().c_str(), text);
	            } else if (fs.getType() == FILE_ACTOR) {
	            	std::string actorJsonString = fs.getCurrentFile()->getData<Actor>()->dumpJson().dump(4);

	            	char *text = const_cast<char*>(actorJsonString.data());
	                SaveFileText(fs.getOpenedFilePath().c_str(), text);
	            } else if (fs.getType() == FILE_DIALOGUE) {
					std::string dialogJsonString = dialogueDumpJson(*fs.getCurrentFile()->getData<Dialogue>()).dump(4);

					char *text = const_cast<char*>(dialogJsonString.data());
                    SaveFileText(fs.getOpenedFilePath().c_str(), text);
				}
			}
		}
		ImGui::SameLine(0, 4);

		if (!fs.fileIsOpen()) ImGui::EndDisabled();

		if (!fs.projectIsOpen()) ImGui::BeginDisabled();

		//Play Project
		if (ImGui::Button(ICON_KI_FLAG, buttonVec)) {
			fs.getProject()->runProject();
		}
		ImGui::SameLine(0, 4);

		//Build Project
		if (ImGui::Button(ICON_KI_WRENCH, buttonVec)) {
			fs.getProject()->compileProject();
		}
		ImGui::SameLine(0, 4);

		//Export Binary
		if (ImGui::Button(ICON_KI_BIN, buttonVec)) {
			std::string binFile = std::string(fs.getProject()->getProjectBasePath());
        	binFile.append("/game.bin");
    		serializeDataToFile(binFile, fs.getProject()->generateStruct());
		}
		ImGui::SameLine(0, 4);

		if (!fs.projectIsOpen()) ImGui::EndDisabled();

		ImGui::End();
	}
	ImGui::PopStyleVar();
}
