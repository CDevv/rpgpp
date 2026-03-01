#ifndef RPGPP_FRAMEEDITOR_H
#define RPGPP_FRAMEEDITOR_H

#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/CheckBox.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "TGUI/Widgets/ToggleButton.hpp"
#include "views/actorView.hpp"
#include <memory>

class ActorFileView;
class FrameEditor : public tgui::Panel {

  private:
	ActorFileView *fileView;
	ActorView::Ptr actorView;

	tgui::ComboBox::Ptr directionChooser;
	tgui::ToggleButton::Ptr playPauseButton;
	tgui::CheckBox::Ptr walkAnimation;

  public:
	FrameEditor(ActorFileView *fileView, const char *typeName = "FileChooser",
				bool initRenderer = true);

	typedef std::shared_ptr<FrameEditor> Ptr;
	static FrameEditor::Ptr create(ActorFileView *fileView);

	void init();
	void onFrameChange(int currentFrame);
	void changeFrameState(int index);
	void changeFrameState();

  protected:
	tgui::Widget::Ptr clone() const override;
};

#endif
