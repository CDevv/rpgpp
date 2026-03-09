#ifndef _RPGPP_ACTION_H
#define _RPGPP_ACTION_H

class Action {
  public:
	bool executeOnAdd = true;
	Action() = default;
	virtual void execute() {};
	virtual void undo() {};
};

#endif