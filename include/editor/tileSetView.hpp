#ifndef RPGPP_TILESETTEXTUREVIEWER
#define RPGPP_TILESETTEXTUREVIEWER

#include "TGUI/Signal.hpp"
#include "gamedata.hpp"
#include "tileset.hpp"
#include "worldView.hpp"
#include <memory>

class TileSetView : public WorldView {
  private:
	IVector getTileAtMouse();
	IVector selectedTile;

  public:
	typedef std::shared_ptr<TileSetView> Ptr;

	tgui::SignalTyped<IVector> onTileSelected = {"TileSelected"};

	TileSet *tileSet{nullptr};
	void drawCanvas() override;
	void drawOverlay() override;

	static TileSetView::Ptr create(TileSet *tileSet);
	static TileSetView::Ptr create();
	void setTileSet(TileSet *newTileSet);
	TileSet *getTileSet();

	IVector getSelectedTile();
	void setSelectedTile(IVector newTile);

	bool leftMousePressed(tgui::Vector2f pos) override;
};

#endif // RPGPP_TILESETTEXTUREVIEWER