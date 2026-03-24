
#ifndef RPGPP_RECENTPROJECTSERVICE_H
#define RPGPP_RECENTPROJECTSERVICE_H

#include <filesystem>
#include <queue>
#include <string>
#define RPGPP_RECENT_FILE ".rpgpp_recent_project"

class RecentProjectService {
  private:
	int limit = 10;
	std::deque<std::string> recentProjects;
	std::filesystem::path path;
	void save();

  public:
	RecentProjectService();
	void enqueue(const std::string &projectPath);
	const std::deque<std::string> &getRecentProjects() const;
};

#endif // RPGPP_RECENTPROJECTSERVICE_H
