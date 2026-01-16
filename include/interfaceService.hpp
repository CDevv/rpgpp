#ifndef _RPGPP_INTERFACESERVICE_H
#define _RPGPP_INTERFACESERVICE_H

#include <map>
#include <raylib.h>
#include <string>
#include <memory>
#include "dialogueBalloon.hpp"
#include "interfaceView.hpp"

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
    /** Available UI Views. */
    std::unique_ptr<std::map<std::string, InterfaceView>> views;
public:
    /** Empty constructor. */
    InterfaceService();
    ~InterfaceService();
    /** Get the font used for the game's UI. */
    Font getFont() const;
    /** Get the texture, used for UI nine-patch components. */
    Texture getTexture() const;
    /** Open the dialogue with a Dialogue structure */
    void showDialogue(const Dialogue &dialogue);
    /** Update routine. */
    void update();
    /** Draw routine. */
    void draw();
    /** Unload routine. */
    void unload() const;
};

#endif
