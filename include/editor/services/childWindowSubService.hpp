#ifndef RPGPP_CHILDWINDOWSERVICE_H
#define RPGPP_CHILDWINDOWSERVICE_H
#include "childWindows/popupWindow.hpp"
#include <map>
#include <string>

class ChildWindowSubService {
  private:
	std::map<std::string, std::unique_ptr<PopupWindow>> childWindows = {};

  public:
	ChildWindowSubService();
	void createWindows();
	void openWindow(const std::string &windowName);
	void resetAndOpen(const std::string windowName);
};

#endif /* RPGPP_CHILDWINDOWSERVICE_H */
