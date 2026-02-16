#ifndef _RPGPP_TOOLBOX2_H
#define _RPGPP_TOOLBOX2_H
#include "TGUI/Backend/Renderer/BackendRenderTarget.hpp"
#include "TGUI/Widgets/BitmapButton.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/GrowHorizontalLayout.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "components/tooltip.hpp"
#include "editor.hpp"
#include <string>

template <typename IdType>
struct ToolboxItemIdentifier {
    std::string group;
    IdType id;
};

template <typename IdType>
struct ToolboxItem : ToolboxItemIdentifier<IdType> {
    std::string text{};
    std::string iconResourcePath{};
};

template <typename Type>
class Toolbox : public tgui::ScrollablePanel {
private:
    std::vector<ToolboxItem<Type>> toolboxItems;
    float spaceBetweenItems = 4;
    float itemPadding = 0;

    tgui::GrowHorizontalLayout::Ptr container;
    std::map<std::string, set<Type>> toolGroup;
    std::map<std::string, Type> selectToolInGroup;

    void resetToolSelection(std::string groupToReset);

    // void readjustContentSize();
public:
    Toolbox();
    ~Toolbox();

    typedef std::shared_ptr<Toolbox> Ptr;
    typedef std::shared_ptr<const Toolbox> ConstPtr;

    static Toolbox::Ptr create();
    static Toolbox::Ptr copy(Toolbox::ConstPtr other);

    void setSpaceBetweenItems(float space);
    float getSpaceBetweenItems() const;
    void setItemPadding(float padding);
    float getItemPadding() const;
    void addTool(const ToolboxItem<Type> &item, int idx = -1);
    void addWidget(tgui::Widget::Ptr widget, int idx = -1);
    void removeItemById(const Type &id);

    tgui::SignalTyped<const ToolboxItem<Type> &> onItemClicked = {"OnItemClicked"};
protected:
	Widget::Ptr clone() const override;
};

template<typename T>
void Toolbox<T>::setSpaceBetweenItems(float space) {
    spaceBetweenItems = space;
}

template<typename T>
float Toolbox<T>::getSpaceBetweenItems() const {
    return spaceBetweenItems;
}

template<typename T>
void Toolbox<T>::setItemPadding(float padding) {
    itemPadding = padding;
}

template<typename T>
float Toolbox<T>::getItemPadding() const {
    return itemPadding;
}

template<typename T>
Toolbox<T>::Toolbox() : tgui::ScrollablePanel() {
    getRenderer()->setPadding({2, 2, 2, 2});
    getRenderer()->setBorders({0, 0, 0, 0});
    this->container = tgui::GrowHorizontalLayout::create();
    this->container->getRenderer()->setPadding({0, 0, 0, 0});
    this->container->getRenderer()->setSpaceBetweenWidgets(spaceBetweenItems);
    this->container->setPosition(0, 0);
    this->container->setSize({"100%", "100%"});
    this->add(this->container);
}

template<typename T>
Toolbox<T>::~Toolbox() {
    this->container->removeAllWidgets();
}

template<typename T>
void Toolbox<T>::resetToolSelection(std::string groupToReset) {
    auto defaultBtn = tgui::Button::create();
    for (const auto& widgets : this->container->getWidgets()) {
        if (auto btn = std::dynamic_pointer_cast<tgui::BitmapButton>(widgets)) {
            ToolboxItemIdentifier<T> identifier = btn->getUserData<ToolboxItemIdentifier<T>>();
            if (groupToReset == identifier.group) {
                btn->getRenderer()->setBackgroundColor(defaultBtn->getRenderer()->getBackgroundColor());
            }
        }
    }
}

template<typename T>
void Toolbox<T>::addTool(const ToolboxItem<T> &item, int idx) {
    tgui::Texture texture(
        Editor::instance->getFs().getResourcePath(item.iconResourcePath));

    set<T> toolsInGroup{};
    if (toolGroup.find(item.group) != toolGroup.end()) {
        toolsInGroup = toolGroup.at(item.group);
    }

    if (toolsInGroup.find(item.id) != toolsInGroup.end()) {
        throw std::runtime_error("Tool already exists in group");
    }

    auto btn = tgui::BitmapButton::create();
    btn->setImageScaling(0.8);
    btn->setImage(texture);
    btn->setWidth(std::max(getSize().y, btn->getSize().x));
    ToolboxItemIdentifier<T> identifier;
    identifier.group = item.group;
    identifier.id = item.id;
    btn->setUserData(identifier);

    auto tooltip = Tooltip::create(item.text);
    btn->setToolTip(tooltip);

    btn->onClick([this, btn, item, toolsInGroup]() {
        if (selectToolInGroup.find(item.group) != selectToolInGroup.end()) {
            if (selectToolInGroup.at(item.group) == item.id) {
                return;
            }
        }
        selectToolInGroup[item.group] = item.id;
        onItemClicked.emit(this, item);
        resetToolSelection(item.group);
        btn->getRenderer()->setBackgroundColor(btn->getRenderer()->getBackgroundColorDown());
    });

    this->container->insert(idx, btn);
    toolGroup[item.group].insert(item.id);
};

template<typename T>
void Toolbox<T>::addWidget(tgui::Widget::Ptr widget, int idx) {
    this->container->insert(idx, widget);
}

template<typename T>
void Toolbox<T>::removeItemById(const T &id) {
    for (const auto& widget : this->container->getWidgets()) {
        ToolboxItemIdentifier<T> identifier = widget->getUserData<ToolboxItemIdentifier<T>>();
        if (identifier.id == id) {
            this->container->remove(widget);
            toolGroup[identifier.group].erase(id);
            break;
        }
    }
    // readjustContentSize();
}

template<typename T>
typename Toolbox<T>::Ptr Toolbox<T>::create() {
	return std::make_shared<Toolbox<T>>();
}

template<typename T>
typename Toolbox<T>::Ptr Toolbox<T>::copy(Toolbox<T>::ConstPtr widget) {
	if (widget)
		return std::static_pointer_cast<Toolbox>(widget->clone());
	else
		return nullptr;
}

template<typename T>
tgui::Widget::Ptr Toolbox<T>::clone() const {
	return std::make_shared<Toolbox>(*this);
}

#endif
