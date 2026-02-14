#include "widgets/fileTab.hpp"
#include "TGUI/Color.hpp"
#include "TGUI/Loading/Theme.hpp"
#include "TGUI/Sprite.hpp"
#include "TGUI/Text.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Tabs.hpp"
#include "editor.hpp"
#include "fileTabRenderer.hpp"
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <memory>

using namespace tgui;

FileTab::FileTab(const char *typeName, bool initRenderer)
	: tgui::Tabs(typeName, false) {
	m_distanceToSideCached = (std::round(
		Text::getLineHeight(m_fontCached, getGlobalTextSize()) * 0.4f));
	if (initRenderer) {
		m_renderer = aurora::makeCopied<FileTabRenderer>();
		setRenderer(tgui::Theme::getDefault()->getRendererNoThrow("Tabs"));

		setTextSize(getGlobalTextSize());
		setTabHeight(
			std::round(Text::getLineHeight(m_fontCached, m_textSizeCached) *
					   1.25f) +
			m_bordersCached.getTopPlusBottom());
	}
}

FileTab::Ptr FileTab::create() { return std::make_shared<FileTab>(); }

FileTab::Ptr FileTab::copy(FileTab::ConstPtr widget) {
	if (widget)
		return std::static_pointer_cast<FileTab>(widget->clone());
	else
		return nullptr;
}

bool FileTab::isMouseOnWidget(tgui::Vector2f pos) const {
	return tgui::FloatRect{getPosition().x, getPosition().y, getSize().x,
						   getSize().y}
		.contains(pos);
}

bool FileTab::leftMousePressed(Vector2f pos) {
	pos -= getPosition();

	float tabStart = 0.0f;
	float tabEnd = m_bordersCached.getLeft() / 2.f;
	for (std::size_t i = 0; i < m_tabs.size(); ++i) {
		if (!m_tabs[i].visible)
			continue;

		// Append the width of the tab
		tabEnd += (m_bordersCached.getLeft() / 2.f) + m_tabs[i].width +
				 (m_bordersCached.getRight() / 2.0f);

		// If the mouse went down on this tab then select it
		if (pos.x >= tabStart &&
		    pos.x < tabEnd - CLOSE_BUTTON_SIZE - MARGIN_LR) {
			select(i);
			break;
		} else if (pos.x >= tabStart + (m_tabs[i].width - MARGIN_LR - CLOSE_BUTTON_SIZE) &&
				   pos.x < tabEnd - MARGIN_LR) {
			int prevSelected = m_selectedTab;
			tgui::String prevId = m_tabs[i].id;
			remove(i);
			onTabClose.emit(this, prevId);
			if (prevSelected == i && m_tabs.size() > 0) {
			    select(std::min(i, m_tabs.size() - 1));
			}
			break;
		}

		tabStart += m_tabs[i].width;
	}

	return false;
}

bool FileTab::select(std::size_t index) {
	// Don't select a tab that is already selected
	if (m_selectedTab == static_cast<int>(index))
		return true;

	// If the index is too high or if the tab is invisible or disabled then we
	// can't select it
	if ((index >= m_tabs.size()) || !m_enabled || !m_tabs[index].visible ||
		!m_tabs[index].enabled) {
		deselect();
		return false;
	}

	// Select the tab
	m_selectedTab = static_cast<int>(index);
	m_tabs[index].text.setColor(m_selectedTextColorCached);
	updateTextColors();

	// Send the callback
	onTabSelect.emit(this, m_tabs[index].id);
	return true;
}

size_t FileTab::addFileTab(const std::string &path, const std::string &fileName) {
    size_t tabIdxToInsert = m_selectedTab;
    if (tabIdxToInsert == -1) {
        tabIdxToInsert = 0;
    }

    for (size_t i = 0; i < m_tabs.size(); ++i) {
        if (m_tabs[i].id == path) {
            select(i);
            return -1;
        }
    }

    tabIdxToInsert += 1;
    tabIdxToInsert = std::min(tabIdxToInsert, m_tabs.size());

    insert(tabIdxToInsert, fileName, true);
    m_tabs[tabIdxToInsert].id = path;
    return tabIdxToInsert;
}

void FileTab::draw(tgui::BackendRenderTarget &target,
				   tgui::RenderStates states) const {
	// tgui::Tabs::draw(target, states);
	auto closeImagePath =
		Editor::instance->getFs().getResourcePath("close.png");
	tgui::Texture closeTexture(closeImagePath);
	Sprite close(closeTexture);

	const float borderWidth = (m_bordersCached.getLeftPlusRight()) / 2.f;
	const bool roundedCorners =
		(m_roundedBorderRadiusCached > 0) && !m_spriteTab.isSet();
	if (!roundedCorners) {
		// Draw the borders around the tabs
		if (m_bordersCached != Borders{0}) {
			target.drawBorders(states, m_bordersCached, getSize(),
							   tgui::Color::applyOpacity(m_borderColorCached,
														 m_opacityCached));
			states.transform.translate(m_bordersCached.getOffset());
		}
	} else
		states.transform.translate({borderWidth, 0});

	const float usableHeight = getSize().y - m_bordersCached.getTopPlusBottom();
	for (std::size_t i = 0; i < m_tabs.size(); ++i) {
		if (!m_tabs[i].visible)
			continue;

		RenderStates textStates = states;
		if (roundedCorners)
			textStates.transform.translate({0, m_bordersCached.getTop()});

		// change cursor when hovering
		if (m_hoveringTab != -1 && !isHovering) {
            m_parentGui->setOverrideMouseCursor(tgui::Cursor::Type::Hand);
            isHovering = true;
		} else if (m_hoveringTab == -1 && isHovering) {
		    m_parentGui->restoreOverrideMouseCursor();
            isHovering = false;
		}

		tgui::Color backgroundColor;
		if ((!m_enabled || !m_tabs[i].enabled) &&
			m_backgroundColorDisabledCached.isSet())
			backgroundColor = m_backgroundColorDisabledCached;
		else if (m_selectedTab == static_cast<int>(i)) {
			if ((m_hoveringTab == static_cast<int>(i)) &&
				m_selectedBackgroundColorHoverCached.isSet())
				backgroundColor = m_selectedBackgroundColorHoverCached;
			else
				backgroundColor = m_selectedBackgroundColorCached;
		} else if ((m_hoveringTab == static_cast<int>(i)) &&
				   m_backgroundColorHoverCached.isSet())
			backgroundColor = m_backgroundColorHoverCached;
		else
			backgroundColor = m_backgroundColorCached;

		// Draw the background of the tab
		const Sprite *spriteTab = nullptr;
		if ((!m_enabled || !m_tabs[i].enabled) && m_spriteDisabledTab.isSet())
			spriteTab = &m_spriteDisabledTab;
		else if ((m_selectedTab == static_cast<int>(i)) &&
				 m_spriteSelectedTab.isSet()) {
			if ((m_hoveringTab == static_cast<int>(i)) &&
				m_spriteSelectedTabHover.isSet())
				spriteTab = &m_spriteSelectedTabHover;
			else
				spriteTab = &m_spriteSelectedTab;
		} else if ((m_hoveringTab == static_cast<int>(i)) &&
				   m_spriteTabHover.isSet())
			spriteTab = &m_spriteTabHover;
		else if (m_spriteTab.isSet())
			spriteTab = &m_spriteTab;

		if (roundedCorners) {
			states.transform.translate({-borderWidth, 0});
			target.drawRoundedRectangle(
				states, {m_tabs[i].width + (2 * borderWidth), getSize().y},
				tgui::Color::applyOpacity(backgroundColor, m_opacityCached),
				m_roundedBorderRadiusCached, m_bordersCached,
				tgui::Color::applyOpacity(m_borderColorCached,
										  m_opacityCached));
			states.transform.translate({m_tabs[i].width + 2 * borderWidth, 0});
		} else {
			if (spriteTab) {
				Sprite spriteTabCopy = *spriteTab;
				spriteTabCopy.setSize({m_tabs[i].width, usableHeight});
				target.drawSprite(states, spriteTabCopy);

			} else // No texture was loaded
				target.drawFilledRect(states, {m_tabs[i].width, usableHeight},
									  tgui::Color::applyOpacity(
										  backgroundColor, m_opacityCached));

			// draw close button
			auto tabState = states;
			tabState.transform.translate(
				{m_tabs[i].width - CLOSE_BUTTON_SIZE - MARGIN_LR, (usableHeight - CLOSE_BUTTON_SIZE) / 2.f});
			close.setSize({CLOSE_BUTTON_SIZE, CLOSE_BUTTON_SIZE});
			target.drawSprite(tabState, close);

			// Draw the borders between the tabs
			states.transform.translate({m_tabs[i].width, 0});
			if ((borderWidth != 0) && (i < m_tabs.size() - 1)) {
				target.drawFilledRect(
					states, {borderWidth, usableHeight},
					tgui::Color::applyOpacity(m_borderColorCached,
											  m_opacityCached));
				states.transform.translate({borderWidth, 0});
			}
		}

		// Highlight the borders of the selected and hovered tab when requested
		if (m_bordersCached != Borders{0}) {
			tgui::Color highlightColor;
			if ((m_hoveringTab == static_cast<int>(i)) &&
				m_borderColorHoverCached.isSet())
				highlightColor = m_borderColorHoverCached;
			if (m_selectedTab == static_cast<int>(i)) {
				if ((m_hoveringTab == static_cast<int>(i)) &&
					m_selectedBorderColorHoverCached.isSet())
					highlightColor = m_selectedBorderColorHoverCached;
				else if (m_selectedBorderColorCached.isSet())
					highlightColor = m_selectedBorderColorCached;
			}

			if (highlightColor.isSet()) {
				float leftBorderWidth = borderWidth;
				float rightBorderWidth = borderWidth;
				if (i == 0)
					leftBorderWidth = m_bordersCached.getLeft();
				if (i == m_tabs.size() - 1)
					rightBorderWidth = m_bordersCached.getRight();

				if ((m_selectedTab >= 0) && (m_hoveringTab >= 0) &&
					(m_borderColorHoverCached.isSet() &&
					 (m_selectedBorderColorCached.isSet() ||
					  m_selectedBorderColorHoverCached.isSet()))) {
					if ((m_selectedTab == static_cast<int>(i - 1)) ||
						(m_hoveringTab == static_cast<int>(i - 1)))
						leftBorderWidth /= 2;
					else if ((m_selectedTab == static_cast<int>(i + 1)) ||
							 (m_hoveringTab == static_cast<int>(i + 1)))
						rightBorderWidth /= 2;
				}

				RenderStates highlightStates = states;
				if (i < m_tabs.size() - 1)
					highlightStates.transform.translate({-borderWidth, 0});
				highlightStates.transform.translate(
					{-m_tabs[i].width - leftBorderWidth,
					 -m_bordersCached.getTop()});
				target.drawBorders(
					highlightStates,
					{leftBorderWidth, m_bordersCached.getTop(),
					 rightBorderWidth, m_bordersCached.getBottom()},
					{m_tabs[i].width + leftBorderWidth + rightBorderWidth,
					 getSize().y},
					highlightColor);
			}
		}

		// Apply clipping if required for the text in this tab
		const float usableWidth =
			m_tabs[i].width - (2 * m_distanceToSideCached);
		const bool clippingRequired =
			(m_tabs[i].text.getSize().x > usableWidth);
		if (clippingRequired)
			target.addClippingLayer(textStates, {{m_distanceToSideCached, 0},
												 {usableWidth, usableHeight}});

		// Draw the text
		textStates.transform.translate(
			{m_distanceToSideCached +
				 ((usableWidth - m_tabs[i].text.getSize().x) / 2.f),
			 ((usableHeight - m_tabs[i].text.getSize().y) / 2.f)});
		target.drawText(textStates, m_tabs[i].text);

		if (clippingRequired)
			target.removeClippingLayer();
	}
}

tgui::Widget::Ptr FileTab::clone() const {
	return std::make_shared<FileTab>(*this);
}

FileTabRenderer *FileTab::getSharedRenderer() {
	return aurora::downcast<FileTabRenderer *>(
		tgui::Widget::getSharedRenderer());
}

const FileTabRenderer *FileTab::getSharedRenderer() const {
	return aurora::downcast<const FileTabRenderer *>(
		tgui::Widget::getSharedRenderer());
}

FileTabRenderer *FileTab::getRenderer() {
	return aurora::downcast<FileTabRenderer *>(tgui::Widget::getRenderer());
}
