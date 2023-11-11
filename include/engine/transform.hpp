#ifndef ENGINE_TRANSFORM_HPP
#define ENGINE_TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <engine/component.hpp>
#include <engine/handler.hpp>

namespace Engine
{
    // transform class
    class Transform final : public Component
    {
    public:
        glm::vec3 getLocalUp();
        glm::vec3 getLocalRight();
        glm::vec3 getLocalForward();
        static glm::vec3 getWorldUp();
        static glm::vec3 getWorldRight();
        static glm::vec3 getWorldForward();
        glm::vec3 getWorldPosAt(glm::vec3 localOffset);
        glm::vec3 getLocalPosition();
        glm::vec3 getLocalRotation();
        glm::vec3 getLocalScale();
        glm::vec3 getWorldPosition();
        glm::vec3 getWorldRotation();
        glm::vec3 getWorldScale();
        glm::mat4 getMatrix();
        int getChildsSize();
        Transform* getChild(int index);
        Transform* getParent();
        void setLocalPosition(glm::vec3 position);
        void setLocalRotation(glm::vec3 rotation);
        void setLocalScale(glm::vec3 scale);
        void setWorldPosition(glm::vec3 position);
        void setWorldRotation(glm::vec3 rotation);
        void setWorldScale(glm::vec3 scale);
        void setParent(Transform* transform);
    protected:
        glm::vec3 localPosition;
        glm::vec3 localRotation;
        glm::vec3 localScale;
        glm::vec3 worldPosition;
        glm::vec3 worldRotation;
        glm::vec3 worldScale;
        glm::mat4 localMatrix;
        glm::mat4 worldMatrix;
        std::vector<Component*>* refSiblings;
        std::vector<Transform*> childs;
        Transform* parent = nullptr;
        void start();
        void setDestroy();
        void updateWorldMatrix();
        void updateLocalMatrix();
        void addChild(Transform* transform);
        void removeChild(Transform* transform);
        static glm::vec3 calculatePosition(glm::mat4 matrix);
        static glm::vec3 calculateRotation(glm::mat4 matrix);
        static glm::vec3 calculateScale(glm::mat4 matrix);
        static glm::vec3 fixRotation(glm::vec3 rotation);
    friend class Actor;
    };
}

#endif