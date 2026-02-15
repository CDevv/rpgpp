#ifndef _RPGPP_FILEINITVISITOR_H
#define _RPGPP_FILEINITVISITOR_H

#include "fileSystemService.hpp"
#include "widgets/newFileDialog.hpp"
#include "projectFile.hpp"
#include <array>
#include <functional>
#include <memory>

class FileInitVisitor {
  private:
	std::array<std::function<void(NewFileDialog::Ptr)>, FILETYPE_MAX> funcs;

  public:
	FileInitVisitor();
	void visit(EngineFileType fileType, NewFileDialog::Ptr dialog);
	bool funcIsEmpty(EngineFileType fileType);
	static void empty(NewFileDialog::Ptr dialog);
	static void tileset(NewFileDialog::Ptr dialog);
	static void room(NewFileDialog::Ptr dialog);
};

#endif
