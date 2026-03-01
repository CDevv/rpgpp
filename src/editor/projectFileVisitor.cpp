#include "projectFileVisitor.hpp"

#include "actor.hpp"
#include "dialogue.hpp"
#include "fileViews/actorFileView.hpp"
#include "fileViews/codeFileView.hpp"
#include "fileViews/dialogueFileView.hpp"
#include "fileViews/emptyView.hpp"
#include "fileViews/fileView.hpp"
#include "fileViews/propFileView.hpp"
#include "fileViews/roomFileView.hpp"
#include "fileViews/tilesetFileView.hpp"
#include "projectFile.hpp"
#include "room.hpp"
#include "saveables/fontWrapper.hpp"
#include "saveables/imageWrapper.hpp"
#include "saveables/soundWrapper.hpp"
#include "scriptFile.h"
#include "services/fileSystemService.hpp"
#include "tileset.hpp"
#include "variant.hpp"
#include <memory>
#include <utility>

ProjectFileVisitor::ProjectFileVisitor() {
	funcs[static_cast<int>(EngineFileType::FILE_TILESET)] = tilesetView;
	funcs[static_cast<int>(EngineFileType::FILE_MAP)] = roomView;
	funcs[static_cast<int>(EngineFileType::FILE_SCRIPT)] = codeView;
	funcs[static_cast<int>(EngineFileType::FILE_DIALOGUE)] = dialogueView;
	funcs[static_cast<int>(EngineFileType::FILE_EMPTY)] = emptyView;
	funcs[static_cast<int>(EngineFileType::FILE_ACTOR)] = actorView;
	funcs[static_cast<int>(EngineFileType::FILE_PROP)] = propView;
	funcs[static_cast<int>(EngineFileType::FILE_IMAGE)] = imageView;
	funcs[static_cast<int>(EngineFileType::FILE_FONT)] = fontView;
	funcs[static_cast<int>(EngineFileType::FILE_SOUND)] = soundView;
	funcs[static_cast<int>(EngineFileType::FILE_MUSIC)] = musicView;
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

std::unique_ptr<ProjectFile>
ProjectFileVisitor::codeView(const std::string &path) {
	std::unique_ptr<FileView> view = std::make_unique<CodeFileView>();
	std::unique_ptr<VariantWrapper> variant =
		std::make_unique<Variant<ScriptFile>>(new ScriptFile(path));
	return std::make_unique<ProjectFile>(std::move(view), std::move(variant),
										 EngineFileType::FILE_SCRIPT);
}

std::unique_ptr<ProjectFile>
ProjectFileVisitor::dialogueView(const std::string &path) {
	std::unique_ptr<FileView> view = std::make_unique<DialogueFileView>();
	std::unique_ptr<VariantWrapper> variant =
		std::make_unique<Variant<Dialogue>>(new Dialogue(path));
	return std::make_unique<ProjectFile>(std::move(view), std::move(variant),
										 EngineFileType::FILE_DIALOGUE);
}

std::unique_ptr<ProjectFile>
ProjectFileVisitor::actorView(const std::string &path) {
	std::unique_ptr<FileView> view = std::make_unique<ActorFileView>();
	std::unique_ptr<VariantWrapper> variant =
		std::make_unique<Variant<Actor>>(new Actor(path));
	return std::make_unique<ProjectFile>(std::move(view), std::move(variant),
										 EngineFileType::FILE_ACTOR);
}

std::unique_ptr<ProjectFile>
ProjectFileVisitor::propView(const std::string &path) {
	std::unique_ptr<FileView> view = std::make_unique<PropFileView>();
	std::unique_ptr<VariantWrapper> variant =
		std::make_unique<Variant<Prop>>(new Prop(path));
	return std::make_unique<ProjectFile>(std::move(view), std::move(variant),
										 EngineFileType::FILE_PROP);
}

std::unique_ptr<ProjectFile>
ProjectFileVisitor::imageView(const std::string &path) {
	std::unique_ptr<EmptyFileView> view = std::make_unique<EmptyFileView>();
	std::unique_ptr<VariantWrapper> variant =
		std::make_unique<Variant<ImageWrapper>>(new ImageWrapper(path));
	return std::make_unique<ProjectFile>(std::move(view), std::move(variant),
										 EngineFileType::FILE_IMAGE, false);
}

std::unique_ptr<ProjectFile>
ProjectFileVisitor::fontView(const std::string &path) {
	std::unique_ptr<EmptyFileView> view = std::make_unique<EmptyFileView>();
	std::unique_ptr<VariantWrapper> variant =
		std::make_unique<Variant<FontWrapper>>(new FontWrapper(path));
	return std::make_unique<ProjectFile>(std::move(view), std::move(variant),
										 EngineFileType::FILE_FONT, false);
}

std::unique_ptr<ProjectFile>
ProjectFileVisitor::soundView(const std::string &path) {
	std::unique_ptr<EmptyFileView> view = std::make_unique<EmptyFileView>();
	std::unique_ptr<VariantWrapper> variant =
		std::make_unique<Variant<SoundWrapper>>(new SoundWrapper(path));
	return std::make_unique<ProjectFile>(std::move(view), std::move(variant),
										 EngineFileType::FILE_SOUND, false);
}

std::unique_ptr<ProjectFile>
ProjectFileVisitor::musicView(const std::string &path) {
	std::unique_ptr<EmptyFileView> view = std::make_unique<EmptyFileView>();
	std::unique_ptr<VariantWrapper> variant =
		std::make_unique<Variant<SoundWrapper>>(new SoundWrapper(path));
	return std::make_unique<ProjectFile>(std::move(view), std::move(variant),
										 EngineFileType::FILE_MUSIC, false);
}
