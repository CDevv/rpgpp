#include "widgets/fileChooser.hpp"
#include "TGUI/String.hpp"
#include "TGUI/SubwidgetContainer.hpp"
#include "TGUI/Widgets/BitmapButton.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/FileDialog.hpp"
#include "editor.hpp"

FileChooser::FileChooser(const char *typeName, bool initRenderer)
	: tgui::SubwidgetContainer(typeName, initRenderer) {

	TranslationService &tService = Editor::instance->getTranslations();
	chosenPathLabel = tgui::EditBox::create();
	chosenPathLabel->setText(tService.getKey("widget.filechooser.select_a_file"));
	chosenPathLabel->setReadOnly(true);
	chosenPathLabel->setEnabled(false);
	iconButton = tgui::BitmapButton::create();
	auto iconTexture = tgui::Texture(
		Editor::instance->getFs().getResourcePath("open_folder.png"));
	iconButton->setImageScaling(0.95);
	iconButton->setImage(iconTexture);

	iconButton->onClick([this] {
		auto fileDialog = tgui::FileDialog::create();
		if (Editor::instance->project != nullptr) {
			fileDialog->setPath(Editor::instance->getProject()->getBasePath());
		} else {
			fileDialog->setPath(Editor::instance->getFs().getEditorBaseDir());
		}
		fileDialog->setFileTypeFilters(pathFilters);
		fileDialog->setSelectingDirectory(selectingDirectory);
		fileDialog->onFileSelect([this](const tgui::String &path) {
			chosenPathLabel->setText(
				std::string(GetFileName(path.toStdString().c_str())));
			chosenPath = path;
		});

		Editor::instance->getGui().gui->add(fileDialog);
	});

	m_container->add(chosenPathLabel);
	m_container->add(iconButton);

	updateSize();
}

FileChooser::Ptr FileChooser::create() {
	return std::make_shared<FileChooser>();
}

FileChooser::Ptr FileChooser::copy(FileChooser::ConstPtr widget) {
	if (widget)
		return std::static_pointer_cast<FileChooser>(widget->clone());
	else
		return nullptr;
}

tgui::Widget::Ptr FileChooser::clone() const {
	return std::make_shared<FileChooser>(*this);
}

void FileChooser::setSize(const tgui::Layout2d &size) {
	tgui::SubwidgetContainer::setSize(size);
	updateSize();
}

void FileChooser::updateSize() {
    int h = getSize().y;
    int btnSize = h;
	chosenPathLabel->setSize({getSize().x - btnSize - PADDING, h});
	iconButton->setSize({btnSize, btnSize});
	iconButton->setPosition({getSize().x - btnSize, 0});
}

tgui::String &FileChooser::getChosenPath() { return chosenPath; }

void FileChooser::setSelectingDirectory(bool selectingDirectory) {
	this->selectingDirectory = selectingDirectory;
}
