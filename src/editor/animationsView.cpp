#include "animationsView.hpp"
#include <raygui.h>
#include "actor.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "editorInterfaceService.hpp"
#include <string>

AnimationsView::AnimationsView() {}

AnimationsView::AnimationsView(Rectangle rect)
{
	this->actorView = nullptr;
	this->rect = rect;
	this->currentAnim = 0;
	this->animPlaying = false;
	this->animFrames = std::vector<Vector2>{};
	this->animDropdownEditMode = false;
	this->animNames = std::vector<std::string>{};

	animNames.push_back("Down Idle");
	animNames.push_back("Down");
	animNames.push_back("Up Idle");
	animNames.push_back("Up");
	animNames.push_back("Left Idle");
	animNames.push_back("Left");
	animNames.push_back("Right Idle");
	animNames.push_back("Right");
}

void AnimationsView::update()
{
	FileSystemService& fs = Editor::getFileSystem();
	Actor* actor = fs.getActor();

	if (actor != nullptr)
	{
		animFrames = actor->getAnimationRaw(static_cast<Direction>(currentAnim));
	}
}

void AnimationsView::draw()
{
	GuiPanel(rect, "Animations View");

	Rectangle baseFrameRect = Rectangle {
		rect.x + 4, rect.y + 58, 48, 48
	};

	FileSystemService& fs = Editor::getFileSystem();
	Actor* actor = fs.getActor();

	if (actor != nullptr) {
		Texture texture = actor->getTileSet().getTexture();
		Vector2 tileSize = actor->getTileSet().getTileSize();
		Vector2 origin = Vector2 { 0, 0 };
		float rotation = 0.0f;

		int frameIndex = 0;
		for (Vector2 frameVec : animFrames) {
			Rectangle source = Rectangle {
				tileSize.x * frameVec.x, tileSize.y * frameVec.y,
				tileSize.x, tileSize.y
			};
			DrawTexturePro(texture, source, baseFrameRect, origin, rotation, WHITE);

			DrawRectangleLinesEx(baseFrameRect, 1.0f, GRAY);

			if (CheckCollisionPointRec(GetMousePosition(), baseFrameRect)) {
				DrawRectangleLinesEx(baseFrameRect, 1.0f, GetColor(GuiGetStyle(BUTTON, BORDER_COLOR_FOCUSED)));
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					if (actorView != nullptr) {
						actorView->setFrame(frameIndex);
					}
				}
			}

			if (actorView != nullptr) {
				if (actorView->getFrame() == frameIndex) {
					DrawRectangleLinesEx(baseFrameRect, 1.0f, GetColor(GuiGetStyle(BUTTON, BORDER_COLOR_PRESSED)));
				}
			}

			baseFrameRect.x += baseFrameRect.width;
			frameIndex++;
		}
	}

	int playIcon = ICON_PLAYER_PLAY;
	if (animPlaying) {
		playIcon = ICON_PLAYER_PAUSE;
	}

	if (EditorGuiDropdown(Rectangle { rect.x + 4, rect.y + 28, 125, 24 }, &animNames, &currentAnim, &animDropdownEditMode)) {
		animPlaying = false;
		if (actorView != nullptr) {
			actorView->setAnimPlaying(false);
			actorView->setFrame(0);
		}
	}

	if (GuiButton(Rectangle { rect.x + 125 + 8, rect.y + 28, 24, 24 }, GuiIconText(playIcon, NULL))) {
		animPlaying = !animPlaying;
	}

	if (GuiButton(Rectangle { rect.x + 149 + 12, rect.y + 28, 24, 24 }, GuiIconText(ICON_PLAYER_STOP, NULL))) {
		animPlaying = false;
		if (actorView != nullptr) {
			actorView->setAnimPlaying(false);
			actorView->setFrame(0);
		}
	}

	if (GuiButton(Rectangle { rect.x + 173 + 16, rect.y + 28, 24, 24 }, GuiIconText(ICON_FILE_ADD, NULL))) {
		animPlaying = false;
		
		if (actor != nullptr) {
			actor->addAnimationFrame(static_cast<Direction>(currentAnim), Vector2 { 0, 0 });
		}
	}

	if (GuiButton(Rectangle { rect.x + 197 + 20, rect.y + 28, 24, 24 }, GuiIconText(ICON_FILE_DELETE, NULL))) {
		animPlaying = false;
		
		if (actor != nullptr) {
			actor->removeAnimationFrame(static_cast<Direction>(currentAnim), actorView->getFrame());
			actorView->updateData();
		}
	}

	if (GuiButton(Rectangle { rect.x + 221 + 24, rect.y + 28, 24, 24 }, GuiIconText(224, NULL))) {
		animPlaying = false;
		
		if (actor != nullptr) {
			EditorInterfaceService& ui = Editor::getUi();
			WindowContainer& windows = ui.getWindowContainer();

			TileSetDialogWindow& dialog = windows.openTileSetDialog();
			dialog.setTileSet(&actor->getTileSet());
			dialog.setSelectedTile(animFrames.at(actorView->getFrame()));
			dialog.setCallback([this, actor](Vector2 atlas) 
				{ actor->setAnimationFrame(static_cast<Direction>(currentAnim), actorView->getFrame(), atlas); }
			);
		}
	}
}

void AnimationsView::setActorView(ActorView *actorView)
{
	if (actorView != nullptr) {
		printf("%s \n", "actorView");
	}
	else {
		printf("no actorView \n");
	}
	this->actorView = actorView;
}

int AnimationsView::getCurrentAnim()
{
	return currentAnim;
}

bool AnimationsView::getAnimPlaying()
{
	return animPlaying;
}