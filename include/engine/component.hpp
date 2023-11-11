#ifndef ENGINE_COMPONENT_HPP
#define ENGINE_COMPONENT_HPP

#include <vector>

namespace Engine
{
    class Actor;
    class Collider;
    
    // base component class
    class Component
    {
    public:
        // get the attached actor
        Actor* getActor();
        virtual void setDestroy();
    protected:
        bool shouldDestroy = false;
        virtual ~Component() = 0;
    private:
        Actor* actor;
        // called when component is add
        virtual void start();
        // called in every frame
        virtual void update();
        // called every specific time interval
        virtual void fixedUpdate();
        // called after update
        virtual void lateUpdate();
        // called when transform is changed
        virtual void onTransformChanged();
        // called when collision detected
        virtual void onCollisionEnter(Collider* collider);
        virtual void onCollisionStay(Collider* collider);
        virtual void onCollisionExit(Collider* collider);
        virtual void onTriggerEnter(Collider* collider);
        virtual void onTriggerStay(Collider* collider);
        virtual void onTriggerExit(Collider* collider);
        // called when component is destroying
        virtual void onDestroy();
    friend class Actor;
    friend class Transform;
    friend class RectTransform;
    friend class Collider;
    friend class GameLoop;
    };
}

#endif