#ifndef RPGPP_TILESETTEXTUREVIEWER
#define RPGPP_TILESETTEXTUREVIEWER
#include "tileset.hpp"
#include "worldView.hpp"
#include "TGUI/RenderStates.hpp"
#include "TGUI/Backend/Renderer/BackendRenderTarget.hpp"


class TileSetTextureViewer: public WorldView {
    public:
        TileSet* tileSet{};
        void drawCanvas() override;


        static TileSetTextureViewer::Ptr create(TileSet* tileSet) {
            auto tileSetTextureViewer = std::make_shared<TileSetTextureViewer>();
            tileSetTextureViewer->tileSet = tileSet;
            return tileSetTextureViewer;
        }
};


#endif //RPGPP_TILESETTEXTUREVIEWER