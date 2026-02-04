#include "widgets/roomToolbox.hpp"
#include "TGUI/Signal.hpp"
#include "TGUI/Sprite.hpp"
#include "TGUI/Texture.hpp"
#include "TGUI/Vector2.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Tabs.hpp"
#include "editor.hpp"

RoomToolbox::RoomToolbox(const char *typeName, bool initRenderer)
	: tgui::Tabs(typeName, false) {

	if (initRenderer) {
		m_renderer = aurora::makeCopied<RoomToolboxRenderer>();
		setRenderer(tgui::Theme::getDefault()->getRendererNoThrow("Tabs"));
	}
}

RoomToolbox::Ptr RoomToolbox::create() {
	return std::make_shared<RoomToolbox>();
}

RoomToolbox::Ptr RoomToolbox::copy(RoomToolbox::ConstPtr widget) {
	if (widget)
		return std::static_pointer_cast<RoomToolbox>(widget->clone());
	else
		return nullptr;
}

tgui::Widget::Ptr RoomToolbox::clone() const {
	return std::make_shared<RoomToolbox>(*this);
}

RoomToolboxRenderer *RoomToolbox::getSharedRenderer() {
	return aurora::downcast<RoomToolboxRenderer *>(Widget::getSharedRenderer());
}

const RoomToolboxRenderer *RoomToolbox::getSharedRenderer() const {
	return aurora::downcast<const RoomToolboxRenderer *>(
		Widget::getSharedRenderer());
}

RoomToolboxRenderer *RoomToolbox::getRenderer() {
	return aurora::downcast<RoomToolboxRenderer *>(tgui::Widget::getRenderer());
}

void RoomToolbox::rendererChanged(const tgui::String &property) {
	tgui::Tabs::rendererChanged(property);
}

tgui::Signal &RoomToolbox::getSignal(tgui::String signalName) {
	if (signalName == onToolPressed.getName()) {
		return onToolPressed;
	} else {
		return tgui::Tabs::getSignal(signalName);
	}
}

bool RoomToolbox::isMouseOnWidget(tgui::Vector2f pos) const {
	return tgui::FloatRect{getPosition().x, getPosition().y, getSize().x,
						   getSize().y}
		.contains(pos);
}

void RoomToolbox::draw(tgui::BackendRenderTarget &target,
					   tgui::RenderStates states) const {
	auto &selectedTexture = getSharedRenderer()->getTextureSelectedTab();
	tgui::Sprite selectedSprite(selectedTexture);

	auto &normalTexture = getSharedRenderer()->getTextureTab();
	tgui::Sprite normalSprite(normalTexture);

	tgui::Vector2f buttonSize = {getSize().y, getSize().y};
	selectedSprite.setSize(buttonSize);
	normalSprite.setSize(buttonSize);

	int idx = 0;
	for (auto tool : tools) {
		if (idx == selectedTool) {
			target.drawSprite(states, selectedSprite);
		} else {
			target.drawSprite(states, normalSprite);
		}

		tgui::Texture &toolTexture = tool.texture;
		tgui::Sprite toolSprite(toolTexture);
		target.drawSprite(states, toolSprite);

		states.transform.translate({buttonSize.x + 4, 0.0f});
		idx++;
	}
}

bool RoomToolbox::leftMousePressed(tgui::Vector2f pos) {
	tgui::Vector2f buttonSize = {getSize().y + 4, getSize().y};

	int idx = 0;
	for (auto tool : tools) {
		if (pos.x > (idx * buttonSize.x) &&
			pos.x < ((idx + 1) * buttonSize.x)) {
			selectedTool = idx;
			onToolPressed.emit(this, tool.type);
		}
		idx++;
	}

	return true;
}

void RoomToolbox::addTool(const std::string &resourcePath, RoomTool tool) {
	tgui::Texture texture(
		Editor::instance->getFs().getResourcePath(resourcePath));
	tools.push_back({tool, texture});
}
