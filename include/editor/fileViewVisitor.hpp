#ifndef _RPGPP_FILEVIEWVISITOR_H
#define _RPGPP_FILEVIEWVISITOR_H

#include "fileSystemService.hpp"
#include "fileViews/fileView.hpp"
#include <array>
#include <functional>
#include <memory>

class FileViewVisitor {
  private:
	std::array<std::function<std::unique_ptr<FileView>()>, FILETYPE_MAX> funcs;

  public:
	FileViewVisitor();
	std::unique_ptr<FileView> visit(EngineFileType fileType);
	static std::unique_ptr<FileView> emptyView();
	static std::unique_ptr<FileView> tilesetView();
};

#endif