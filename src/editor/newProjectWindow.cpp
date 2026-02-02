#include "newProjectWindow.hpp"
#include "TGUI/Loading/Theme.hpp"
#include "TGUI/Renderers/ChildWindowRenderer.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"

/*
NewProjectWindow::NewProjectWindow(const char *typeName, bool initRenderer)
	: tgui::ChildWindow(typeName, false) {

	if (initRenderer) {
		m_renderer = aurora::makeCopied<NewProjectWindowRenderer>();
		setRenderer(
			tgui::Theme::getDefault()->getRendererNoThrow("ChildWindow"));
	}
}

NewProjectWindow::Ptr NewProjectWindow::create() {
	return std::make_shared<NewProjectWindow>();
}

NewProjectWindow::Ptr
NewProjectWindow::copy(NewProjectWindow::ConstPtr widget) {
	if (widget)
		return std::static_pointer_cast<NewProjectWindow>(widget->clone());
	else
		return nullptr;
}

tgui::Widget::Ptr NewProjectWindow::clone() const {
	return std::make_shared<NewProjectWindow>(*this);
}

NewProjectWindowRenderer *NewProjectWindow::getSharedRenderer() {
	return aurora::downcast<NewProjectWindowRenderer *>(
		Widget::getSharedRenderer());
}

const NewProjectWindowRenderer *NewProjectWindow::getSharedRenderer() const {
	return aurora::downcast<const NewProjectWindowRenderer *>(
		Widget::getSharedRenderer());
}

NewProjectWindowRenderer *NewProjectWindow::getRenderer() {
	return aurora::downcast<NewProjectWindowRenderer *>(
		tgui::Widget::getRenderer());
}

void NewProjectWindow::rendererChanged(const tgui::String &property) {
	tgui::ChildWindow::rendererChanged(property);
}

void NewProjectWindow::draw(tgui::BackendRenderTarget &target,
							tgui::RenderStates states) const {
	// Draw the borders
	if (m_bordersCached != tgui::Borders{0}) {
		if (m_focused && m_borderColorFocusedCached.isSet())
			target.drawBorders(
				states, m_bordersCached, getSize(),
				tgui::Color::applyOpacity(m_borderColorFocusedCached,
										  m_opacityCached));
		else
			target.drawBorders(states, m_bordersCached, getSize(),
							   tgui::Color::applyOpacity(m_borderColorCached,
														 m_opacityCached));

		states.transform.translate(m_bordersCached.getOffset());
	}

	// Draw the title bar
	if (m_spriteTitleBar.isSet())
		target.drawSprite(states, m_spriteTitleBar);
	else
		target.drawFilledRect(
			states, {getInnerTitleBarSize().x, m_titleBarHeightCached},
			tgui::Color::applyOpacity(m_titleBarColorCached, m_opacityCached));

	// Draw the text in the title bar (after setting the clipping area)
	{
		float buttonOffsetX = 0;
		for (const auto &button : {m_closeButton.get(), m_maximizeButton.get(),
								   m_minimizeButton.get()}) {
			if (button->isVisible())
				buttonOffsetX +=
					(buttonOffsetX > 0 ? m_paddingBetweenButtonsCached : 0) +
					button->getSize().x;
		}

		if (buttonOffsetX > 0)
			buttonOffsetX += m_distanceToSideCached;

		const float clippingLeft = m_distanceToSideCached;
		const float clippingRight =
			getInnerTitleBarSize().x - m_distanceToSideCached - buttonOffsetX;
		target.addClippingLayer(
			states, {{clippingLeft, 0},
					 {clippingRight - clippingLeft, m_titleBarHeightCached}});
		target.drawText(states, m_titleText);
		target.removeClippingLayer();
	}

	// Draw the buttons
	states.transform.translate(
		{-m_bordersCached.getLeft(), -m_bordersCached.getTop()});
	for (auto &button : {m_closeButton.get(), m_maximizeButton.get(),
						 m_minimizeButton.get()}) {
		if (!button->isVisible())
			continue;

		tgui::RenderStates buttonStates = states;
		buttonStates.transform.translate(button->getPosition());
		button->draw(target, buttonStates);
	}
	states.transform.translate(
		{m_bordersCached.getLeft(),
		 m_bordersCached.getTop() + m_titleBarHeightCached});

	// Draw the border below the title bar
	if (m_borderBelowTitleBarCached > 0) {
		if (m_focused && m_borderColorFocusedCached.isSet())
			target.drawFilledRect(
				states, {getInnerTitleBarSize().x, m_borderBelowTitleBarCached},
				tgui::Color::applyOpacity(m_borderColorFocusedCached,
										  m_opacityCached));
		else
			target.drawFilledRect(
				states, {getInnerTitleBarSize().x, m_borderBelowTitleBarCached},
				tgui::Color::applyOpacity(m_borderColorCached,
										  m_opacityCached));

		states.transform.translate({0, m_borderBelowTitleBarCached});
	}

	// Draw the background
	if (m_spriteBackground.isSet())
		target.drawSprite(states, m_spriteBackground);
	else if (m_backgroundColorCached != tgui::Color::Transparent)
		target.drawFilledRect(states, getInnerSizeWithPadding(),
							  tgui::Color::applyOpacity(m_backgroundColorCached,
														m_opacityCached));

	// Draw the widgets in the child window
	states.transform.translate(
		{m_clientPaddingCached.getLeft(), m_clientPaddingCached.getTop()});
	target.addClippingLayer(states, {{}, {getClientSize()}});
	Container::draw(target, states);
	target.removeClippingLayer();
}
*/