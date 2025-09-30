#include "projectMenuBox.hpp"
#include <raygui.h>
#include <cstddef>
#include <IconsFontAwesome6.h>
#include "gamedata.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "editorInterfaceService.hpp"

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
	/*
	FileSystemService& fs = Editor::getFileSystem();
	EditorInterfaceService& ui = Editor::getUi();

	DrawRectangleLinesEx(rect, 1.0f, GRAY);

	Rectangle baseButtonRect = Rectangle
	{
		rect.x + 4, rect.y + 4,
		rect.height - 8, rect.height - 8
	};

	if (GuiButton(baseButtonRect, GuiIconText(ICON_FOLDER_OPEN, NULL))) {
		fs.promptOpenProject();
		ui.setInitial();
	}

	Rectangle secondButtonRect = baseButtonRect;
	secondButtonRect.x += baseButtonRect.width + 4;

	if (fs.fileIsOpen()) {
		if (GuiButton(secondButtonRect, GuiIconText(ICON_FILE_SAVE, NULL))) {
			if (fs.fileIsOpen()) {
				if (fs.getType() == FILE_TILESET) {
	                TileSet *tileSet = fs.getTileSet();

	                std::string jsonString = tileSet->dumpJson().dump(4);

	                char *text = const_cast<char*>(jsonString.data());
	                SaveFileText(fs.getOpenedFilePath().c_str(), text);
	            } else if (fs.getType() == FILE_ROOM) {
	                std::string mapJsonString = fs.getRoom()->dumpJson().dump(4);

	                char *text = const_cast<char*>(mapJsonString.data());
	                SaveFileText(fs.getOpenedFilePath().c_str(), text);
	            } else if (fs.getType() == FILE_ACTOR) {
	            	std::string actorJsonString = fs.getActor()->dumpJson().dump(4);

	            	char *text = const_cast<char*>(actorJsonString.data());
	                SaveFileText(fs.getOpenedFilePath().c_str(), text);
	            }
			}
		}
	} else {
		GuiDummyRec(secondButtonRect, GuiIconText(ICON_FILE_SAVE, NULL));
	}
	
	Rectangle runButtonRect = secondButtonRect;
	runButtonRect.x += baseButtonRect.width + 4;
	
	Rectangle buildButtonRect = runButtonRect;
	buildButtonRect.x += baseButtonRect.width + 4;

	Rectangle binOpenRect = buildButtonRect;
	binOpenRect.x += baseButtonRect.width + 4;

	Rectangle binSaveRect = binOpenRect;
	binSaveRect.x += baseButtonRect.width + 4;

	if (fs.projectIsOpen()) {
    	if (GuiButton(runButtonRect, GuiIconText(ICON_PLAYER_PLAY, NULL))) {
			fs.getProject()->runProject();
		}
		if (GuiButton(buildButtonRect, GuiIconText(220, NULL))) {
        	fs.getProject()->compileProject();
    	}
    	if (GuiButton(binSaveRect, GuiIconText(222, NULL))) {
    		std::string binFile = std::string(fs.getProject()->getProjectBasePath());
        	binFile.append("/game.bin");
    		serializeDataToFile(binFile, fs.getProject()->generateStruct());
    	}
	} else {
		GuiDummyRec(runButtonRect, GuiIconText(ICON_PLAYER_PLAY, NULL));
		GuiDummyRec(buildButtonRect, GuiIconText(220, NULL));
		GuiDummyRec(binSaveRect, GuiIconText(222, NULL));
	}

	if (GuiButton(binOpenRect, GuiIconText(221, NULL))) {
		FS_Result fsResult = fs.openGameData();
        GameData data = deserializeFile(fsResult.path);
        ProjectBinaryViewWindow& window = ui.getWindowContainer().openProjectBinaryView();
        window.setData(data);
	}
	*/

	float buttonSize = (rect.height - (4 * 2));
	ImVec2 buttonVec = ImVec2(buttonSize, buttonSize);

	ImGui::SetNextWindowPos(ImVec2 { rect.x, rect.y });
	ImGui::SetNextWindowSize(ImVec2 { rect.width, rect.height });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
	if (ImGui::Begin("ProjectMenuBox", nullptr,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)) {

		//Open Project
		if (ImGui::Button(ICON_FA_FOLDER_OPEN, buttonVec)) {
			fs.promptOpenProject();
			ui.setInitial();
		}
		ImGui::SameLine(0, 4);

		if (!fs.fileIsOpen()) ImGui::BeginDisabled();

		//Save File
		if (ImGui::Button(ICON_FA_FILE_PEN, buttonVec)) {
			if (fs.fileIsOpen()) {
				if (fs.getType() == FILE_TILESET) {
	                TileSet *tileSet = fs.getTileSet();

	                std::string jsonString = tileSet->dumpJson().dump(4);

	                char *text = const_cast<char*>(jsonString.data());
	                SaveFileText(fs.getOpenedFilePath().c_str(), text);
	            } else if (fs.getType() == FILE_ROOM) {
	                std::string mapJsonString = fs.getRoom()->dumpJson().dump(4);

	                char *text = const_cast<char*>(mapJsonString.data());
	                SaveFileText(fs.getOpenedFilePath().c_str(), text);
	            } else if (fs.getType() == FILE_ACTOR) {
	            	std::string actorJsonString = fs.getActor()->dumpJson().dump(4);

	            	char *text = const_cast<char*>(actorJsonString.data());
	                SaveFileText(fs.getOpenedFilePath().c_str(), text);
	            }
			}
		}
		ImGui::SameLine(0, 4);

		if (!fs.fileIsOpen()) ImGui::EndDisabled();

		if (!fs.projectIsOpen()) ImGui::BeginDisabled();

		//Play Project
		if (ImGui::Button(ICON_FA_PLAY, buttonVec)) {
			fs.getProject()->runProject();
		}
		ImGui::SameLine(0, 4);

		//Build Project
		if (ImGui::Button(ICON_FA_HAMMER, buttonVec)) {
			fs.getProject()->compileProject();
		}
		ImGui::SameLine(0, 4);

		//Export Binary
		if (ImGui::Button(ICON_FA_FILE_LINES, buttonVec)) {
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