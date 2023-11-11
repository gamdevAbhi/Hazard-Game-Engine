#include <engine/ui.hpp>
#include <engine/recttransform.hpp>

// get the UI resolution
glm::vec2 Engine::UI::getResolution()
{
    return glm::vec2(width, height);
}

// get ortho resolution
glm::mat4 Engine::UI::getOrtho()
{
    return orthoMatrix;
}

// get position respective x and y
glm::vec3 Engine::UI::getScreenPosition(glm::vec2 anchor)
{
    float screenX = (width / 2.f) * (1.f + anchor.x);
    float screenY = (height / 2.f) * (1.f + anchor.y);

    return glm::vec3(screenX, screenY, 0.f);
}

// get the anchor respective to the screen position
glm::vec2 Engine::UI::getAnchor(glm::vec2 screenPos)
{   
    float x = ((screenPos.x * 2.f) / width) - 1.f;
    float y = ((screenPos.y * 2.f) / height) - 1.f;

    return glm::vec2(x, y);
}

// get size respective x and y
glm::vec3 Engine::UI::getAnchorSize(glm::vec2 size)
{
    float sizeX = (width / 2.f) * size.x;
    float sizeY = (height / 2.f) * size.y;

    return glm::vec3(sizeX, sizeY, 0.f);
}

// set the UI resolution
void Engine::UI::setResolution(int width, int height)
{
    UI::width = width;
    UI::height = height;

    updateOrthoMatrix();
    updateUIContents();
}

// update the ortho matrix
void Engine::UI::updateOrthoMatrix()
{
    glm::mat4 ortho = glm::ortho(0.f, (float)width, 0.f, (float)height, 0.1f, 100.f);

    glm::mat4 translate(1.f);
    glm::mat4 rotation(1.f);
    glm::mat4 scale(1.f);
    glm::mat4 view(1.f);

    translate = glm::translate(translate, glm::vec3(0.f, 0.f, 10.f));
    rotation = glm::mat4_cast(glm::quat(glm::vec3(0.f)));
    scale = glm::scale(scale, glm::vec3(1.f));

    view = translate * rotation * scale;

    orthoMatrix = ortho * glm::inverse(view);
}

// update the UI contents
void Engine::UI::updateUIContents()
{
    for(int i = 0; i < roots.size(); i++)
    {
        roots[i]->updateRectProperties();
    }
}

// return true if root exist in roots
bool Engine::UI::containRoot(RectTransform* root)
{
    for(int i = 0; i < roots.size(); i++) if(roots[i] == root) return true;
    return false;
}

// add root in list
void Engine::UI::addRoot(RectTransform* root)
{
    roots.push_back(root);
}

// remove root from the list
void Engine::UI::removeRoot(RectTransform* root)
{
    for(int i = 0; i < roots.size(); i++)
    {
        if(roots[i] != root) continue;
        roots.erase(roots.begin() + i);
        break;
    }
}