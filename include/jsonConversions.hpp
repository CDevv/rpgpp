#ifndef _RPGPP_JSONCONVERSIONS_H
#define _RPGPP_JSONCONVERSIONS_H

#include <nlohmann/json.hpp>

#include "raylib.h"

using json = nlohmann::json;

void to_json(json &j, const Rectangle &rect);
void from_json(const json &j, Rectangle &rect);

void to_json(json &j, const Color &color);
void from_json(const json &j, Color &color);

void to_json(json &j, const NPatchInfo &info);
void from_json(const json &j, NPatchInfo &info);

#endif