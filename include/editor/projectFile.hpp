#ifndef _RPGPP_PROJECTFILE_H
#define _RPGPP_PROJECTFILE_H

#include "TGUI/Widgets/Group.hpp"
#include "services/fileSystemService.hpp"
#include "fileViews/fileView.hpp"
#include "variant.hpp"
#include <memory>

class ProjectFile {
  private:
	std::unique_ptr<FileView> view;
	std::unique_ptr<VariantWrapper> variant;
	EngineFileType fileType;
	std::string filePath;

  public:
	ProjectFile();
	ProjectFile(std::unique_ptr<FileView> view,
				std::unique_ptr<VariantWrapper> variant,
				EngineFileType fileType);
	void setFilePath(const std::string &filePath);
	std::string &getFilePath();
	void initUi(tgui::Group::Ptr group);
	void addWidgets(tgui::Group::Ptr layout);
	void saveFile(const std::string &path);
	FileView &getView();
};

#endif
