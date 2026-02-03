#ifndef _RPGPP_VARIANT_H
#define _RPGPP_VARIANT_H

#include "saveable.hpp"
#include <memory>

class VariantWrapper {
  public:
	VariantWrapper() {};
	virtual ~VariantWrapper() {};
	virtual ISaveable *toSaveable() = 0;
};

class SaveableVariant {
  public:
	std::unique_ptr<ISaveable> data;
};

template <typename T> class Variant : public VariantWrapper {
  public:
	std::unique_ptr<T> data;
	Variant<T>(){};
	Variant<T>(T *p) {
		data = std::make_unique<T>();
		data.reset(p);
	};
	void set(T *data) { this->data.reset(data); };
	T *get() { return data.get(); };
	ISaveable *toSaveable() { return dynamic_cast<ISaveable *>(data.get()); }
};

#endif