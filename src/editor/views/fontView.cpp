#include "views/fontView.hpp"
#include "raylib.h"
#include "saveables/fontWrapper.hpp"
#include <cstdio>
#include <memory>

FontView::FontView() {}

FontView::Ptr FontView::create() { return std::make_shared<FontView>(); }

void FontView::setFont(FontWrapper *wrapper) { this->font = wrapper; }

FontWrapper *FontView::getFont() { return font; }

void FontView::setBaseFontSize(int newSize) {
	baseFontSize = newSize;
	font->font = LoadFontEx(font->fontSource.c_str(), newSize, nullptr, 256);
}

void FontView::setText(const std::string &text) { this->text = text; }

const std::string &FontView::getText() { return text; }

void FontView::drawCanvas() {
	if (font == nullptr) {
		return;
	}

	ClearBackground(RAYWHITE);
	drawOrigin();
	DrawTextEx(font->font, text.c_str(), {0, 0}, baseFontSize, 1.0f, BLACK);
}

void FontView::drawOverlay() {}
