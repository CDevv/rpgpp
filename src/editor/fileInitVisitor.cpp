#include "fileInitVisitor.hpp"
#include "TGUI/Widget.hpp"
#include "editor.hpp"
#include "services/fileSystemService.hpp"
#include "gamedata.hpp"
#include "newFileDialog.hpp"
#include "screens/projectScreen.hpp"
#include "raylib.h"
#include "room.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <utility>

FileInitVisitor::FileInitVisitor() {
	funcs[static_cast<int>(EngineFileType::FILE_TILESET)] = tileset;
	funcs[static_cast<int>(EngineFileType::FILE_MAP)] = room;
}

bool FileInitVisitor::funcIsEmpty(EngineFileType fileType) {
	return funcs[static_cast<int>(fileType)] == nullptr;
}

void FileInitVisitor::visit(EngineFileType fileType,
							NewFileDialog::Ptr dialog) {
	if (funcIsEmpty(fileType)) {
		empty(dialog);
	} else {
		funcs[static_cast<int>(fileType)](dialog);
	}
}

void FileInitVisitor::empty(NewFileDialog::Ptr dialog) {
	dialog->confirmButton->onPress([dialog] {
		printf("%s \n", dialog->titleField->getText().toStdString().c_str());
		printf("%s \n",
			   dialog->fileField->getChosenPath().toStdString().c_str());
		dialog->window->close();
	});
}

void FileInitVisitor::tileset(NewFileDialog::Ptr dialog) {
	dialog->fileLabel->setText("Texture");
	dialog->setPathFilters({{"Images", {"*.png", "*.jpg"}}});
	dialog->confirmButton->onPress([dialog] {
		std::string title = dialog->titleField->getText().toStdString();
		std::string filePath = dialog->fileField->getChosenPath().toStdString();
		if (!title.empty() && !filePath.empty()) {
			dialog->window->close();

			std::unique_ptr<TileSet> tileSet =
				std::make_unique<TileSet>(filePath, 16);
			nlohmann::json fileJson = tileSet->dumpJson();
			std::string newFilePath =
				TextFormat("tilesets/%s.tiles", title.c_str());
			SaveFileText(newFilePath.c_str(), fileJson.dump(4).c_str());

			auto ptr = aurora::downcast<screens::ProjectScreen *>(
				Editor::instance->getGui().currentScreen.get());
			ptr->addFileView(EngineFileType::FILE_TILESET, newFilePath);
			ptr->addResourceButtons(EngineFileType::FILE_TILESET);
		}
	});
}

void FileInitVisitor::room(NewFileDialog::Ptr dialog) {
	dialog->fileLabel->setText("TileSet");
	dialog->setPathFilters({{"RPG++ TileSet", {"*.rtiles"}}});
	dialog->confirmButton->onPress([dialog] {
		std::string title = dialog->titleField->getText().toStdString();
		std::string filePath = dialog->fileField->getChosenPath().toStdString();
		if (!title.empty() && !filePath.empty()) {
			std::unique_ptr<TileSet> tileSet =
				std::make_unique<TileSet>(filePath);
			std::unique_ptr<TileMap> tileMap = std::make_unique<TileMap>(
				std::move(tileSet), 20, 20, _RPGPP_TILESIZE,
				_RPGPP_TILESIZE * RPGPP_DRAW_MULTIPLIER);
			std::unique_ptr<Room> room =
				std::make_unique<Room>(std::move(tileMap));

			std::string newFilePath =
				TextFormat("maps/%s.tiles", title.c_str());
			nlohmann::json fileJson = room->dumpJson();
			SaveFileText(newFilePath.c_str(), fileJson.dump(4).c_str());

			auto ptr = aurora::downcast<screens::ProjectScreen *>(
				Editor::instance->getGui().currentScreen.get());
			ptr->addFileView(EngineFileType::FILE_TILESET, newFilePath);
			ptr->addResourceButtons(EngineFileType::FILE_TILESET);

			dialog->window->close();
		}
	});
}
