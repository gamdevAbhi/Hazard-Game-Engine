#include <engine/component.hpp>

// get the actor
Engine::Actor* Engine::Component::getActor()
{
    return this->actor;
}

// compoenent will be destroyed in next update loop
void Engine::Component::setDestroy()
{
    shouldDestroy = true;
}

Engine::Component::~Component() {}

void Engine::Component::start() {}
void Engine::Component::update() {}
void Engine::Component::lateUpdate() {}
void Engine::Component::fixedUpdate() {}
void Engine::Component::onTransformChanged() {}
void Engine::Component::onCollisionEnter(Collider* collider) {}
void Engine::Component::onCollisionStay(Collider* collider) {}
void Engine::Component::onCollisionExit(Collider* collider) {}
void Engine::Component::onTriggerEnter(Collider* collider) {}
void Engine::Component::onTriggerStay(Collider* collider) {}
void Engine::Component::onTriggerExit(Collider* collider) {}
void Engine::Component::onDestroy() {}