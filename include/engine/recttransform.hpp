#ifndef ENGINE_RECT_TRANSFORM_HPP
#define ENGINE_RECT_TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <engine/ui.hpp>
#include <engine/component.hpp>
#include <engine/handler.hpp>

namespace Engine
{
    class RectTransform : public Component
    {
    public:
        glm::vec3 getAnchorUp();
        glm::vec3 getAnchorRight();
        glm::vec3 getAnchorForward();
        static glm::vec3 getRectUp();
        static glm::vec3 getRectRight();
        static glm::vec3 getRectForward();
        glm::vec2 getAnchorAt(glm::vec2 screenPos);
        glm::vec3 getScreenPosAt(glm::vec2 anchor);
        glm::vec3 getScreenPosAt(glm::vec3 localScreenOffset);
        glm::vec2 getAnchor();
        glm::vec3 getAnchorRotation();
        glm::vec2 getAnchorSize();
        glm::vec3 getRectPosition();
        glm::vec3 getRectRotation();
        glm::vec3 getRectScale();
        int getChildsSize();
        RectTransform* getChild(int index);
        RectTransform* getParent();
        glm::mat4 getRectMatrix();
        void setAnchor(glm::vec2 anchor);
        void setAnchorRotation(glm::vec3 rotation);
        void setAnchorSize(glm::vec2 size);
        void setParent(RectTransform* transform);
    protected:
        glm::vec2 anchor;
        glm::vec3 anchorRotation;
        glm::vec2 anchorSize;
        glm::vec3 rectPosition;
        glm::vec3 rectRotation;
        glm::vec3 rectScale;
        glm::mat4 anchorMatrix;
        glm::mat4 rectMatrix;
        std::vector<Component*>* refSiblings;
        std::vector<RectTransform*> childs;
        RectTransform* parent = nullptr;
        void start();
        void setDestroy();
        void onDestroy();
        void updateRectProperties();
        void addChild(RectTransform* transform);
        void removeChild(RectTransform* transform);
        static glm::vec3 fixRotation(glm::vec3 rotation);
        static glm::vec3 calculatePosition(glm::mat4 matrix);
        static glm::vec3 calculateRotation(glm::mat4 matrix);
        static glm::vec3 calculateScale(glm::mat4 matrix);
    friend class Actor;
    friend class UI;
    };
}

#endif