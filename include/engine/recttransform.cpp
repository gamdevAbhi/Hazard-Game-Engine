#include <engine/recttransform.hpp>

// get the anchor up direction
glm::vec3 Engine::RectTransform::getAnchorUp()
{
    glm::vec3 anchor_up;

    anchor_up.x = rectMatrix[1][0];
    anchor_up.y = rectMatrix[1][1];
    anchor_up.z = rectMatrix[1][2];

    return glm::normalize(anchor_up);
}

// get the anchor right direction
glm::vec3 Engine::RectTransform::getAnchorRight()
{
    glm::vec3 anchor_right;

    anchor_right.x = rectMatrix[0][0];
    anchor_right.y = rectMatrix[0][1];
    anchor_right.z = rectMatrix[0][2];

    return glm::normalize(anchor_right);
}

// get the anchor forward direction
glm::vec3 Engine::RectTransform::getAnchorForward()
{
    glm::vec3 anchor_forward;

    anchor_forward.x = rectMatrix[2][0];
    anchor_forward.y = rectMatrix[2][1];
    anchor_forward.z = rectMatrix[2][2];

    return glm::normalize(anchor_forward);
}

// get the screen up direction
glm::vec3 Engine::RectTransform::getRectUp()
{
    return glm::vec3(0.f, 1.f, 0.f);
}

// get the Rect right direction
glm::vec3 Engine::RectTransform::getRectRight()
{
    return glm::vec3(1.f, 0.f, 0.f);
}

// get the Rect forward direction
glm::vec3 Engine::RectTransform::getRectForward()
{
    return glm::vec3(0.f, 0.f, -1.f);
}

// get the anchor at screen position
glm::vec2 Engine::RectTransform::getAnchorAt(glm::vec2 screenPos)
{
    glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(screenPos, 0.f));
    glm::mat4 rotation = glm::mat4_cast(glm::quat(glm::vec3(0.f)));
    glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(1.f));
    
    glm::mat4 screenMatrix = translate * rotation * scale;

    return calculatePosition(glm::inverse(rectMatrix) * screenMatrix);
}

// get the rect position respective to the anchor
glm::vec3 Engine::RectTransform::getScreenPosAt(glm::vec2 anchor)
{
    glm::mat4 newMatrix = glm::translate(glm::mat4(1.f), glm::vec3(anchor, 0.f)) * 
    glm::mat4_cast(glm::quat(glm::vec3(0.f))) * glm::scale(glm::mat4(1.f), glm::vec3(1.f));

    return calculatePosition(rectMatrix * newMatrix);
}

// get the rect position respective to the screen offset
glm::vec3 Engine::RectTransform::getScreenPosAt(glm::vec3 localScreenOffset)
{
    glm::vec3 position = rectPosition;

    position += getAnchorRight() * localScreenOffset.x;
    position += getAnchorUp() * localScreenOffset.y;

    return position;
}

// get the rect position
glm::vec3 Engine::RectTransform::getRectPosition()
{
    return rectPosition;
}

// get the rect rotation
glm::vec3 Engine::RectTransform::getRectRotation()
{
    return rectRotation;
}

// get the rect scale
glm::vec3 Engine::RectTransform::getRectScale()
{
    return rectScale;
}

// get the anchor
glm::vec2 Engine::RectTransform::getAnchor()
{
    return anchor;
}

// get the anchor rotation
glm::vec3 Engine::RectTransform::getAnchorRotation()
{
    return anchorRotation;
}

// get the anchor size
glm::vec2 Engine::RectTransform::getAnchorSize()
{
    return anchorSize;
}

// get childs size
int Engine::RectTransform::getChildsSize()
{
    return childs.size();
}

// get the child
Engine::RectTransform* Engine::RectTransform::getChild(int index)
{
    if(childs.size() <= index) Handler::debug("child index is out of bound", "");
    return childs[index];
}

// get the parent
Engine::RectTransform* Engine::RectTransform::getParent()
{
    return parent;
}

// get the rect matrix
glm::mat4 Engine::RectTransform::getRectMatrix()
{
    return rectMatrix;
}

// set the anchor position
void Engine::RectTransform::setAnchor(glm::vec2 anchor)
{
    this->anchor = anchor;
    updateRectProperties();
}

// set the anchor rotation
void Engine::RectTransform::setAnchorRotation(glm::vec3 rotation)
{
    anchorRotation = fixRotation(rotation);
    updateRectProperties();
}

// set the anchor scale
void Engine::RectTransform::setAnchorSize(glm::vec2 size)
{
    anchorSize = size;
    updateRectProperties();
}

// set the parent
void Engine::RectTransform::setParent(RectTransform* transform)
{
    if(transform == this) 
    {
        Handler::debug("can't make parent of own rect transform", "rect transform");
        return;
    }

    for(int i = 0; i < childs.size(); i++)
    {
        if(childs[i] != transform) continue;
        Handler::debug("can't make parent of his own child", "transform");
        return;
    }

    if(parent != nullptr) parent->removeChild(this);
    if(transform != nullptr) transform->addChild(this);
    
    parent = transform;

    updateRectProperties();

    if(parent == nullptr && UI::containRoot(this) == false) UI::addRoot(this);
}

// start function
void Engine::RectTransform::start()
{
    anchor = glm::vec2(0.0f);
    anchorRotation = glm::vec3(0.0f);
    anchorSize = glm::vec3(0.25f);

    updateRectProperties();
}

// set destroy
void Engine::RectTransform::setDestroy()
{
    Handler::error("can't destroy rect transform", "rect transform");
}

// on destroy
void Engine::RectTransform::onDestroy()
{
    if(UI::containRoot(this) == true) UI::removeRoot(this);
}

// update rect properties
void Engine::RectTransform::updateRectProperties()
{
    glm::mat4 translate(1.f);
    glm::mat4 rotation(1.f);
    glm::mat4 scale(1.f);

    translate = glm::translate(translate, (parent == nullptr)? UI::getScreenPosition(anchor) : 
    glm::vec3(anchor, 0.f));
    rotation = glm::mat4_cast(glm::quat(anchorRotation));
    scale = glm::scale(scale, (parent == nullptr)? UI::getAnchorSize(anchorSize) : 
    glm::vec3(anchorSize, 1.f));

    anchorMatrix = translate * rotation * scale;
    rectMatrix = (parent == nullptr)? anchorMatrix : parent->getRectMatrix() * anchorMatrix;

    rectPosition = calculatePosition(rectMatrix);
    rectRotation = calculateRotation(rectMatrix);
    rectScale = calculateScale(rectMatrix);

    for(int i = 0; i < childs.size(); i++) childs[i]->updateRectProperties();
    for(int i = 0; i < refSiblings->size(); i++) (*refSiblings)[i]->onTransformChanged();
}

// add child
void Engine::RectTransform::addChild(RectTransform* transform)
{
    childs.push_back(transform);
}

// remove child
void Engine::RectTransform::removeChild(RectTransform* transform)
{
    for(int i = 0; i < childs.size(); i++)
    {
        if(childs[i] != transform) continue;
        childs.erase(childs.begin() + i);
        break;
    }
}

// fix the rotation to -360 to 360
glm::vec3 Engine::RectTransform::fixRotation(glm::vec3 rotation)
{
    glm::vec3 fixRotation;

    fixRotation.x = (rotation.x - (int)rotation.x) + (int)rotation.x % 360;
    fixRotation.y = (rotation.y - (int)rotation.y) + (int)rotation.y % 360;
    fixRotation.z = (rotation.z - (int)rotation.z) + (int)rotation.z % 360;

    if(fixRotation.x < -360.f) fixRotation.x = std::abs(fixRotation.x) - 360.f;
    if(fixRotation.y < -360.f) fixRotation.y = std::abs(fixRotation.y) - 360.f;
    if(fixRotation.z < -360.f) fixRotation.z = std::abs(fixRotation.z) - 360.f;

    return fixRotation;
}

// calculate the position from the matrix
glm::vec3 Engine::RectTransform::calculatePosition(glm::mat4 matrix)
{
    glm::vec3 position;
    
    position.x = matrix[3][0];
    position.y = matrix[3][1];
    position.z = matrix[3][2];
    
    return position;
}

// calculate the rotation from the matrix
glm::vec3 Engine::RectTransform::calculateRotation(glm::mat4 matrix)
{
    glm::mat3 rotationMatrix;
    glm::vec3 scale = calculateScale(matrix);

    rotationMatrix[0] = matrix[0] / scale.x;
    rotationMatrix[1] = matrix[1] / scale.y;
    rotationMatrix[2] = matrix[2] / scale.z;
    
    float roll = 0;
    float pitch = 0;
    float yaw = 0;

    if(rotationMatrix[0][2] != -1 && rotationMatrix[0][2] != 1)
    {
        pitch = -1 * glm::asin(rotationMatrix[0][2]);
        roll = glm::atan2(rotationMatrix[1][2] / glm::cos(pitch), 
        rotationMatrix[2][2] / glm::cos(pitch));
        yaw = glm::atan2(rotationMatrix[0][1] / glm::cos(pitch), 
        rotationMatrix[0][0] / glm::cos(pitch));
    }
    else
    {
        if(rotationMatrix[0][2] == -1)
        {
            pitch = glm::pi<float>() / 2.0f;
            roll = yaw + glm::atan2(rotationMatrix[1][0], rotationMatrix[2][0]);
        }
        else
        {
            pitch = -glm::pi<float>() / 2.0f;
            roll = -yaw + glm::atan2(-1.0f * rotationMatrix[1][0], -1.0f * rotationMatrix[2][0]);
        }
    }

    glm::vec3 rotation = glm::vec3(roll * 180.0f / glm::pi<float>(), pitch * 180.0f / glm::pi<float>(), 
    yaw * 180.0f / glm::pi<float>());
    
    return rotation;
}

// calculate the scale from the matrix
glm::vec3 Engine::RectTransform::calculateScale(glm::mat4 matrix)
{
    glm::vec3 scale;

    scale.x = glm::sqrt(matrix[0][0] * matrix[0][0] + matrix[0][1] * matrix[0][1] 
    + matrix[0][2] * matrix[0][2]);
    scale.y = glm::sqrt(matrix[1][0] * matrix[1][0] + matrix[1][1] * matrix[1][1] 
    + matrix[1][2] * matrix[1][2]);
    scale.z = glm::sqrt(matrix[2][0] * matrix[2][0] + matrix[2][1] * matrix[2][1] 
    + matrix[2][2] * matrix[2][2]);

    return scale;
}