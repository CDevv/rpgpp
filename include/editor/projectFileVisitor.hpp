#ifndef _RPGPP_PROJECTFILEVISITOR_H
#define _RPGPP_PROJECTFILEVISITOR_H

#include "fileSystemService.hpp"
#include "projectFile.hpp"
#include <array>
#include <functional>
#include <memory>

class ProjectFileVisitor {
  private:
	std::array<std::function<std::unique_ptr<ProjectFile>(const std::string &)>,
			   FILETYPE_MAX>
		funcs;

  public:
	ProjectFileVisitor();
	std::unique_ptr<ProjectFile> visit(EngineFileType fileType,
									   const std::string &path);
	static std::unique_ptr<ProjectFile> emptyView(const std::string &path);
	static std::unique_ptr<ProjectFile> tilesetView(const std::string &path);
};

#endif