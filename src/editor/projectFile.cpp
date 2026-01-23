#include "projectFile.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "emptyView.hpp"
#include <memory>
#include <utility>

ProjectFile::ProjectFile() { view = std::make_unique<EmptyFileView>(); }

ProjectFile::ProjectFile(std::unique_ptr<FileView> view,
						 std::unique_ptr<VariantWrapper> variant) {
	this->view = std::move(view);
	this->variant = std::move(variant);
}

void ProjectFile::initUi(tgui::Group::Ptr group) {
	if (variant.get() == nullptr) {
		view->init(group, nullptr);
	} else {
		view->init(group, variant.get());
	}
}