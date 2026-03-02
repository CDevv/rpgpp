#ifndef _RPGPP_VIEWS_IMAGEVIEW_H
#define _RPGPP_VIEWS_IMAGEVIEW_H

#include "raylib.h"
#include "saveables/imageWrapper.hpp"
#include "views/worldView.hpp"
#include <memory>

class ImageView : public WorldView {
  public:
	typedef std::shared_ptr<ImageView> Ptr;

	ImageView();
	~ImageView();

	ImageWrapper *image{};
	Texture texture{};

	static Ptr create();

	void setImage(ImageWrapper *image);
	ImageWrapper *getImage();

	void drawCanvas() override;
	void drawOverlay() override;
};

#endif