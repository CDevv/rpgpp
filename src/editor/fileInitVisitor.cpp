#include "fileInitVisitor.hpp"
#include "fileSystemService.hpp"
#include "newFileDialog.hpp"

FileInitVisitor::FileInitVisitor() {
	funcs[static_cast<int>(EngineFileType::FILE_TILESET)] = empty;
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