#ifndef _RPGPP_STARTPOINTACTION_H
#define _RPGPP_STARTPOINTACTION_H

#include "actions/mapAction.hpp"
class StartPointAction : public MapAction {
public:
	StartPointAction(MapActionData data);
	void execute() override;
	void undo() override;
};

#endif