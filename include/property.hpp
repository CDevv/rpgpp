#ifndef _RPGPP_PROPERTY_H
#define _RPGPP_PROPERTY_H

#include <string>

class IProperty {
	virtual const std::string &getName() const;
};

template <typename Owner, typename T>
class Property : public IProperty {
	std::string name;
	T Owner::*member;

public:
	Property(const std::string &name, T Owner::*member) {
		this->name = name;
		this->member = member;
	}

	const std::string &getName() const override { return name; }
};

#endif