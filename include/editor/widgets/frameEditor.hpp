#ifndef RPGPP_FRAMEEDITOR_H
#define RPGPP_FRAMEEDITOR_H

#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "TGUI/Widgets/ToggleButton.hpp"
#include "fileViews/actorFileView.hpp"
#include "views/actorView.hpp"
#include <memory>


class FrameEditor : public tgui::Panel {

	private:
		ActorFileView* fileView;
		ActorView::Ptr actorView;

		tgui::ToggleButton::Ptr playPauseButton;

	public:
		FrameEditor(ActorFileView* fileView, const char *typeName = "FileChooser", bool initRenderer = true);

		typedef std::shared_ptr<FrameEditor> Ptr;
		static FrameEditor::Ptr create(ActorFileView* fileView);

		void init();
		void onFrameChange();

	protected:
		tgui::Widget::Ptr clone() const override;
};


#endif
