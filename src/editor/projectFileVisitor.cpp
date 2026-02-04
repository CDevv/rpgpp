#include "projectFileVisitor.hpp"
#include "fileSystemService.hpp"
#include "fileView.hpp"
#include "projectFile.hpp"
#include "room.hpp"
#include "roomFileView.hpp"
#include "tileset.hpp"
#include "tilesetFileView.hpp"
#include "variant.hpp"
#include <memory>
#include <utility>

ProjectFileVisitor::ProjectFileVisitor() {
	funcs[static_cast<int>(EngineFileType::FILE_TILESET)] = tilesetView;
	funcs[static_cast<int>(EngineFileType::FILE_MAP)] = roomView;
}

std::unique_ptr<ProjectFile>
ProjectFileVisitor::visit(EngineFileType fileType, const std::string &path) {
	if (funcs.at(static_cast<int>(fileType)) == nullptr) {
		return emptyView(path);
	}
	return funcs[static_cast<int>(fileType)](path);
}

std::unique_ptr<ProjectFile>
ProjectFileVisitor::emptyView(const std::string &path) {
	return std::make_unique<ProjectFile>();
}

std::unique_ptr<ProjectFile>
ProjectFileVisitor::tilesetView(const std::string &path) {
	std::unique_ptr<FileView> view = std::make_unique<TileSetFileView>();
	std::unique_ptr<VariantWrapper> variant =
		std::make_unique<Variant<TileSet>>(new TileSet(path));
	return std::make_unique<ProjectFile>(std::move(view), std::move(variant),
										 EngineFileType::FILE_TILESET);
}

std::unique_ptr<ProjectFile>
ProjectFileVisitor::roomView(const std::string &path) {
	std::unique_ptr<FileView> view = std::make_unique<RoomFileView>();
	std::unique_ptr<VariantWrapper> variant =
		std::make_unique<Variant<Room>>(new Room(path));
	return std::make_unique<ProjectFile>(std::move(view), std::move(variant),
										 EngineFileType::FILE_MAP);
}