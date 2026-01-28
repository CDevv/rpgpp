#ifndef RPGPP_TILESETTEXTUREVIEWER
#define RPGPP_TILESETTEXTUREVIEWER

#include "tileset.hpp"
#include "worldView.hpp"
#include <memory>

class TileSetView : public WorldView {
  public:
	typedef std::shared_ptr<TileSetView> Ptr;

	TileSet *tileSet{nullptr};
	void drawCanvas() override;
	void drawOverlay() override;

	static TileSetView::Ptr create(TileSet *tileSet);
	static TileSetView::Ptr create();
	void setTileSet(TileSet *newTileSet);
	TileSet *getTileSet();
};

#endif // RPGPP_TILESETTEXTUREVIEWER