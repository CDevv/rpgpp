#include "animationsView.hpp"
#include <string>
#include <raygui.h>
#include <IconsFontAwesome6.h>
#include "actor.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "editorInterfaceService.hpp"
#include "tileSetDialogWindow.hpp"
#include "windowContainer.hpp"

bool rlImGuiImageButtonRect(const char* str_id, const Texture* image, int destWidth, int destHeight, Rectangle sourceRect)
{
    if (!image)
        return false;

    //if (GlobalContext)
        //ImGui::SetCurrentContext(GlobalContext);

    ImVec2 uv0;
    ImVec2 uv1;

    if (sourceRect.width < 0)
    {
        uv0.x = -sourceRect.x / image->width;
        uv1.x = (uv0.x - float(fabs(sourceRect.width) / image->width));
    }
    else
    {
        uv0.x = sourceRect.x / image->width;
        uv1.x = uv0.x + float(sourceRect.width / image->width);
    }

    if (sourceRect.height < 0)
    {
        uv0.y = -sourceRect.y / image->height;
        uv1.y = (uv0.y - fabsf(sourceRect.height) / image->height);
    }
    else
    {
        uv0.y = sourceRect.y / image->height;
        uv1.y = uv0.y + sourceRect.height / image->height;
    }

    return ImGui::ImageButton(str_id, (ImTextureID)image->id, ImVec2(float(destWidth), float(destHeight)), uv0, uv1);
}

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

void AnimationsView::setRect(Rectangle rect)
{
	this->rect = rect;
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
	FileSystemService& fs = Editor::getFileSystem();
	Actor* actor = fs.getActor();

	EditorInterfaceService& ui = Editor::getUi();
	WindowContainer& windows = ui.getWindowContainer();

	ImVec2 cursorScreen = ImVec2(0, 0);

	ImGui::SetNextWindowPos(ImVec2 { rect.x, rect.y });
	ImGui::SetNextWindowSize(ImVec2 { rect.width, rect.height });
	if (ImGui::Begin("AnimationsView", nullptr,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {

		if (ImGui::BeginChild("top_bar", ImVec2(-1, 32))) {
			ImGui::BeginGroup();
			ImGui::PushItemWidth(240);
			ImGui::Combo("Animation", &currentAnim, "Down Idle\0Down\0Up Idle\0Up\0Left Idle\0Left\0Right Idle\0Right\0");
			ImGui::EndGroup();

			ImGui::SameLine();
			ImGui::BeginGroup();

			if (ImGui::Button(ICON_FA_PLAY " Play")) {
				animPlaying = !animPlaying;
			}
			ImGui::SameLine();

			if (ImGui::Button(ICON_FA_STOP " Stop")) {
				animPlaying = false;
				if (actorView != nullptr) {
					actorView->setAnimPlaying(false);
					actorView->setFrame(0);
				}
			}
			ImGui::SameLine();

			if (ImGui::Button(ICON_FA_FILE_PEN " Add")) {
				animPlaying = false;

				if (actor != nullptr) {
					actor->addAnimationFrame(static_cast<Direction>(currentAnim), Vector2 { 0, 0 });
				}
			}
			ImGui::SameLine();

			if (ImGui::Button(ICON_FA_FILE_PEN " Remove")) {
				animPlaying = false;

				if (actor != nullptr) {
					actor->removeAnimationFrame(static_cast<Direction>(currentAnim), actorView->getFrame());
					actorView->updateData();
				}
			}
			ImGui::SameLine();

			if (ImGui::Button(ICON_FA_FILE_PEN " Edit")) {
                TileSetDialogWindow& tileSetDialog = windows.openTileSetDialog();
                tileSetDialog.setTileSet(&actor->getTileSet());
                tileSetDialog.setSelectedTile(animFrames.at(actorView->getFrame()));
                tileSetDialog.setCallback([this, actor](Vector2 atlas)
    				{ actor->setAnimationFrame(static_cast<Direction>(currentAnim), actorView->getFrame(), atlas); }
    			);
                //tileSetDialog.setTile(actor->getTileSet().getTile(actorView->getFrame()));
			}
			ImGui::SameLine();

			ImGui::EndGroup();

			windows.drawTileSetDialog();

			ImGui::EndChild();
		}

		if (actor != nullptr) {
			Texture texture = actor->getTileSet().getTexture();
			Vector2 tileSize = actor->getTileSet().getTileSize();
			Vector2 origin = Vector2 { 0, 0 };
			float rotation = 0.0f;

			ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 { 0, 0 });
			if (ImGui::BeginChild("frames_view", ImVec2(-1, 64), ImGuiChildFlags_Borders)) {

				int frameIndex = 0;
				for (Vector2 frameVec : animFrames) {
					Rectangle source = Rectangle {
						tileSize.x * frameVec.x, tileSize.y * frameVec.y,
						tileSize.x, tileSize.y
					};

					if (actorView->getFrame() != frameIndex) {
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 38, 38, 38, 0 });
            			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(53,53,53, 255));
					}

					if (rlImGuiImageButtonRect(TextFormat("frame-%i", frameIndex), &texture, 48, 48, source)) {
						ImGui::PopStyleColor(2);

						if (actorView != nullptr) {
							actorView->setFrame(frameIndex);
						}
					}
					ImGui::SameLine(0.0f, 0.0f);

					if (actorView->getFrame() != frameIndex) ImGui::PopStyleColor(2);

					frameIndex++;
				}

				ImGui::EndChild();
			}
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
		}

		ImGui::End();
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
