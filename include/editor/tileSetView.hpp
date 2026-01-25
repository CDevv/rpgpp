#ifndef RPGPP_TILESETTEXTUREVIEWER
#define RPGPP_TILESETTEXTUREVIEWER

#include "tileset.hpp"
#include "worldView.hpp"

class TileSetView : public WorldView {
  public:
	TileSet *tileSet{};
	void drawCanvas() override;
	void drawOverlay() override;

	static TileSetView::Ptr create(TileSet *tileSet);
	void setTileSet(TileSet *newTileSet);
};

#endif // RPGPP_TILESETTEXTUREVIEWER