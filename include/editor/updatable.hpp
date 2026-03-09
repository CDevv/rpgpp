#ifndef _RPGPP_UPDATABLE_H
#define _RPGPP_UPDATABLE_H

class IUpdatable {
  public:
	virtual ~IUpdatable() = default;

	virtual void update() = 0;
};

#endif