#ifndef _RPGPP_INTERFACESERVICE_H
#define _RPGPP_INTERFACESERVICE_H

#include <raylib.h>

/** The InterfaceService acts for the User Interface (UI). */
class InterfaceService {
private:
    /** The loaded font that will be used for the User Interface. */
    Font font;
    bool fpsVisible;
public:
    /** Empty constructor. */
    InterfaceService();
    ~InterfaceService();
    /** Update routine. */
    void update();
    /** Draw routine. */
    void draw();
    /** Unload routine. */
    void unload();
};

#endif
