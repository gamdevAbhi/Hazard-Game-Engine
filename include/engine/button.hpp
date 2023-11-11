#ifndef ENGINE_BUTTON_HPP
#define ENGINE_BUTTON_HPP

#include <glm/glm.hpp>
#include <engine/actor.hpp>
#include <engine/recttransform.hpp>
#include <engine/component.hpp>
#include <engine/quadtree.hpp>

namespace Engine
{
    class Button : public Component
    {
    public:
        enum HoverStatus {HOVER_NONE, HOVER_ENTER, HOVER_STAY, HOVER_EXIT};
        enum ClickStatus {BUTTON_NONE, BUTTON_CLICK, BUTTON_HOLD, BUTTON_RELEASE};
        void setSize(float left, float right, float up, float down);
        void setOrder(int order);
        void setClickableStatus(bool status);
        void setTransparentStatus(bool status);
        glm::vec2 getWidth();
        glm::vec2 getHeight();
        int getOrder();
        bool getClickableStatus();
        bool getTransparentStatus();
        HoverStatus getHoverStatus();
        ClickStatus getClickStatus();
    protected:
        inline static QuadTree* root = nullptr;
        Node* node;
        float left;
        float right;
        float up;
        float down;
        int order;
        bool isClickable;
        bool isTransparent;
        bool isClicked;
        bool isHover;
        HoverStatus hoverStatus = HOVER_NONE;
        ClickStatus clickStatus = BUTTON_NONE;
        RectTransform* rect;
        void start();
        void onTransformChanged();
        void onDestroy();
        void clicked();
        void hovered();
        void updateStatus();
        std::vector<glm::vec3> getVertices();
        std::vector<glm::vec3> getAxis();
        glm::vec2 getProjection(glm::vec3 axis);
    friend class ButtonManager;
    };
}

#endif