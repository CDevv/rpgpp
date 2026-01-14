#include "propInitWindow.hpp"

#include "editor.hpp"
#include "fileSystemService.hpp"
#include "windowPopup.hpp"

PropInitWindow::PropInitWindow() : rect(), title{}, hasSetImageSource(false)
{
}

PropInitWindow::PropInitWindow(Rectangle rect)
: WindowPopup("New Prop..", rect)
{
    this->rect = rect;

    strcpy(title, "");
    imageSourcePath = "";
    hasSetImageSource = false;
}

void PropInitWindow::closeWindow()
{
    WindowPopup::closeWindow();
}

void PropInitWindow::openWindow()
{
    WindowPopup::openWindow();
}

void PropInitWindow::draw()
{
    FileSystemService& fs = Editor::getFileSystem();
    EditorInterfaceService& ui = Editor::getUi();

    ImGui::SetNextWindowSize(ImVec2 { rect.width, -1 });
    if (ImGui::BeginPopupModal("New Prop..")) {
		ImGui::InputText("Title", title, 256);

		std::string textureSource = imageSourcePath;
		textureSource.push_back('\0');
		ImGui::InputText("Texture", textureSource.data(), textureSource.size());

		if (ImGui::Button("Change Texture", ImVec2(-1, 0))) {
            FS_Result fsResult = fs.openImage();
            if (fsResult.result == NFD_OKAY) {
                imageSourcePath = fsResult.path;
                hasSetImageSource = true;
            }
		}

		if (ImGui::Button("Submit")) {
			std::string titleText = title;

			bool validated = !titleText.empty() && hasSetImageSource;

			if (validated) {
			    Prop prop(Rectangle { 0, 0, 16, 16 }, Vector2 { 0, 0 });
				prop.setTextureFromPath(imageSourcePath);
    			nlohmann::json propJson = prop.dumpJson();
    			std::string jsonString = propJson.dump(4);

    			std::string filePath = std::string("props/").append(titleText).append(".rprop");
    			SaveFileText(filePath.c_str(), const_cast<char*>(jsonString.data()));

    			fs.getProject()->reloadPaths();
    			fs.openProjectFile(filePath);
    			ui.setInitial();

    			closeWindow();
			}
		}
		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			closeWindow();
		}

	    ImGui::EndPopup();
	}
}
