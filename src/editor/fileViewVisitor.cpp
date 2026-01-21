#include "fileViewVisitor.hpp"
#include "fileSystemService.hpp"
#include "fileViews/emptyView.hpp"
#include "fileViews/fileView.hpp"
#include "fileViews/tilesetFileView.hpp"
#include <functional>
#include <memory>
#include <utility>

FileViewVisitor::FileViewVisitor() {
	funcs[static_cast<int>(EngineFileType::FILE_TILESET)] = tilesetView;
}

std::unique_ptr<FileView> FileViewVisitor::tilesetView() {
	return std::make_unique<TileSetFileView>();
}

std::unique_ptr<FileView> FileViewVisitor::visit(EngineFileType fileType) {
	int idx = static_cast<int>(fileType);
	if (funcs.at(idx) == nullptr) {
		return std::make_unique<EmptyFileView>();
	} else {
		return std::move(funcs[idx]());
	}
}