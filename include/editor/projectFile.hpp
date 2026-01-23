#ifndef _RPGPP_PROJECTFILE_H
#define _RPGPP_PROJECTFILE_H

#include "TGUI/Widgets/Group.hpp"
#include "fileView.hpp"
#include "variant.hpp"
#include <memory>

class ProjectFile {
  private:
	std::unique_ptr<FileView> view;
	std::unique_ptr<VariantWrapper> variant;

  public:
	ProjectFile();
	ProjectFile(std::unique_ptr<FileView> view,
				std::unique_ptr<VariantWrapper> variant);
	void initUi(tgui::Group::Ptr group);
};

#endif