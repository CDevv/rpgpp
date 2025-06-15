#ifndef _RPGPP_INTERFACESERVICE_H
#define _RPGPP_INTERFACESERVICE_H

#include <raylib.h>

class InterfaceService {
private:
    Font font;
    bool fpsVisible;
public:
    InterfaceService();
    ~InterfaceService();
    void update();
    void draw();
    void unload();
};

#endif
