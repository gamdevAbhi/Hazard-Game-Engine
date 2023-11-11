#include <engine/input.hpp>

// give key status
Engine::Input::KeyStatus Engine::Input::getKeyStatus(int key)
{
    return keyboardInputs[key];
}

// get mouse button status
Engine::Input::KeyStatus Engine::Input::getMouseButtonStatus(int mouseButton)
{
    return mouseInputs[mouseButton];
}

// give mouse position
glm::vec2 Engine::Input::getMousePos()
{
    return mousePos;
}

// initialize
void Engine::Input::initialize()
{
    // adding key value pair for keyboard input and mouse input
    keyboardInputs[GLFW_KEY_SPACE] = KEY_NONE;

    for(int i = GLFW_KEY_0; i <= GLFW_KEY_9; i++)   keyboardInputs[i] = KEY_NONE;
    for(int i = GLFW_KEY_A; i <= GLFW_KEY_Z; i++)   keyboardInputs[i] = KEY_NONE;

    keyboardInputs[GLFW_KEY_GRAVE_ACCENT] = KEY_NONE;
    keyboardInputs[GLFW_KEY_ESCAPE] = KEY_NONE;
    keyboardInputs[GLFW_KEY_ENTER] = KEY_NONE;
    keyboardInputs[GLFW_KEY_TAB] = KEY_NONE;
    keyboardInputs[GLFW_KEY_RIGHT] = KEY_NONE;
    keyboardInputs[GLFW_KEY_LEFT] = KEY_NONE;
    keyboardInputs[GLFW_KEY_DOWN] = KEY_NONE;
    keyboardInputs[GLFW_KEY_UP] = KEY_NONE;
    keyboardInputs[GLFW_KEY_LEFT_SHIFT] = KEY_NONE;
    keyboardInputs[GLFW_KEY_LEFT_CONTROL] = KEY_NONE;
    keyboardInputs[GLFW_KEY_LEFT_ALT] = KEY_NONE;
    keyboardInputs[GLFW_KEY_RIGHT_SHIFT] = KEY_NONE;
    keyboardInputs[GLFW_KEY_RIGHT_CONTROL] = KEY_NONE;
    keyboardInputs[GLFW_KEY_RIGHT_ALT] = KEY_NONE;

    mouseInputs[GLFW_MOUSE_BUTTON_LEFT] = KEY_NONE;
    mouseInputs[GLFW_MOUSE_BUTTON_RIGHT] = KEY_NONE;

    updateInputs();
}

// update inputs
void Engine::Input::updateInputs()
{
    // update key inputs
    for(std::pair<int, KeyStatus> pair : keyboardInputs)
    {
        KeyStatus action = pair.second;
        int newAction = glfwGetKey(window->glfwWindow, pair.first);

        if(action == KEY_PRESS && newAction == GLFW_PRESS
        || action == KEY_HOLD && newAction == GLFW_PRESS) keyboardInputs[pair.first] = KEY_HOLD;
        else if(newAction == GLFW_PRESS) keyboardInputs[pair.first] = KEY_PRESS;
        else if(action != KEY_RELEASE && action != KEY_NONE && newAction == GLFW_RELEASE) keyboardInputs[pair.first] = KEY_RELEASE;
        else keyboardInputs[pair.first] = KEY_NONE;
    }

    // update mouse button
    for(std::pair<int, KeyStatus> pair : mouseInputs)
    {
        KeyStatus action = pair.second;
        int newAction = glfwGetMouseButton(window->glfwWindow, pair.first);

        if(action == KEY_PRESS && newAction == GLFW_PRESS
        || action == KEY_HOLD && newAction == GLFW_PRESS) mouseInputs[pair.first] = KEY_HOLD;
        else if(newAction == GLFW_PRESS) mouseInputs[pair.first] = KEY_PRESS;
        else if(action != KEY_RELEASE && action != KEY_NONE && newAction == GLFW_RELEASE) mouseInputs[pair.first] = KEY_RELEASE;
        else mouseInputs[pair.first] = KEY_NONE; 
    }

    // update mouse position
    double x, y;
    glfwGetCursorPos(window->glfwWindow, &x, &y);
    y = window->getSize().y - y;
    mousePos = glm::vec2(x, y);
}