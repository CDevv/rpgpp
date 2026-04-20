#include "actions/startPointAction.hpp"

#include "actions/mapAction.hpp"

StartPointAction::StartPointAction(MapActionData data) : MapAction(data) {}

void StartPointAction::execute() { data.room->setStartTile(data.worldTile); }

void StartPointAction::undo() { data.room->setStartTile(data.prevTile); }