#ifndef _RPGPP_INTERFACESERVICE_H
#define _RPGPP_INTERFACESERVICE_H

#include <raylib.h>
#include "dialogueBalloon.hpp"

/** The InterfaceService acts for the User Interface (UI). */
class InterfaceService {
private:
    /** The loaded font that will be used for the User Interface. */
    Font font;
    bool fpsVisible;
    /** Texture for UI components. */
    Texture uiTexture;
    /** Component for in-game dialogue. */
    DialogueBalloon dialogue;
public:
    /** Empty constructor. */
    InterfaceService();
    ~InterfaceService();
    /** Get the font used for the game's UI. */
    Font getFont();
    /** Get the texture, used for UI nine-patch components. */
    Texture getTexture();
    /** Open the dialogue with a Dialogue structure */
    void showDialogue(Dialogue dialogue);
    /** Update routine. */
    void update();
    /** Draw routine. */
    void draw();
    /** Unload routine. */
    void unload();
};

#endif
