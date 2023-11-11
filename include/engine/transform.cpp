#include <engine/transform.hpp>

// get the local up direction
glm::vec3 Engine::Transform::getLocalUp()
{
    glm::mat4 world_matrix = getMatrix();
    glm::vec3 local_up;

    local_up.x = world_matrix[1][0];
    local_up.y = world_matrix[1][1];
    local_up.z = world_matrix[1][2];

    return glm::normalize(local_up);
}

// get the local right direction
glm::vec3 Engine::Transform::getLocalRight()
{

    glm::mat4 world_matrix = getMatrix();
    glm::vec3 local_right;

    local_right.x = world_matrix[0][0];
    local_right.y = world_matrix[0][1];
    local_right.z = world_matrix[0][2];

    return glm::normalize(local_right);
}

// get the local forward direction
glm::vec3 Engine::Transform::getLocalForward()
{
    glm::mat4 world_matrix = getMatrix();
    glm::vec3 local_forward;

    local_forward.x = world_matrix[2][0];
    local_forward.y = world_matrix[2][1];
    local_forward.z = world_matrix[2][2];

    return glm::normalize(local_forward);
}

// get the world up direction
glm::vec3 Engine::Transform::getWorldUp()
{
    return glm::vec3(0.0f, 1.0f, 0.0f);
}

// get the world right direction
glm::vec3 Engine::Transform::getWorldRight()
{
    return glm::vec3(1.0f, 0.0f, 0.0f);
}

// get the world forward direction
glm::vec3 Engine::Transform::getWorldForward()
{
    return glm::vec3(0.0f, 0.0f, -1.0f);
}

// get the world position respective to the local transform
glm::vec3 Engine::Transform::getWorldPosAt(glm::vec3 localOffset)
{
    glm::vec3 position = worldPosition;
    
    position += localOffset.x * getLocalRight();
    position += localOffset.y * getLocalUp();

    return position; 
}

// get the local position
glm::vec3 Engine::Transform::getLocalPosition()
{
    return localPosition;
}

// get the local rotation
glm::vec3 Engine::Transform::getLocalRotation()
{
    return localRotation;
}

// get the local scale
glm::vec3 Engine::Transform::getLocalScale()
{
    return localScale;
}

// get the world position
glm::vec3 Engine::Transform::getWorldPosition()
{
    return worldPosition;
}

// get the world rotation
glm::vec3 Engine::Transform::getWorldRotation()
{
    return worldRotation;
}

// get the world scale
glm::vec3 Engine::Transform::getWorldScale()
{
    return worldScale;
}

// get the world matrix
glm::mat4 Engine::Transform::getMatrix()
{
    return worldMatrix;
}

// get childs size
int Engine::Transform::getChildsSize()
{
    return childs.size();
}

// get the child
Engine::Transform* Engine::Transform::getChild(int index)
{
    if(childs.size() <= index) Handler::debug("child index is out of bound", "");
    return childs[index];
}

// get the parent
Engine::Transform* Engine::Transform::getParent()
{
    return parent;
}

// set the local position
void Engine::Transform::setLocalPosition(glm::vec3 position)
{
    localPosition = position;
    updateWorldMatrix();
}

// set the local rotation
void Engine::Transform::setLocalRotation(glm::vec3 rotation)
{
    localRotation = fixRotation(rotation);
    updateWorldMatrix();
}

// set the local scale
void Engine::Transform::setLocalScale(glm::vec3 scale)
{
    localScale = scale;
    updateWorldMatrix();
}

// set the world position
void Engine::Transform::setWorldPosition(glm::vec3 position)
{
    worldPosition = position;
    updateLocalMatrix();
}

// set the world rotation
void Engine::Transform::setWorldRotation(glm::vec3 rotation)
{
    worldRotation = fixRotation(rotation);
    updateLocalMatrix();
}

// set the world scale
void Engine::Transform::setWorldScale(glm::vec3 scale)
{
    worldScale = scale;
    updateLocalMatrix();
}

// set the parent
void Engine::Transform::setParent(Transform* transform)
{
    if(transform == this) 
    {
        Handler::debug("can't make parent of own transform", "transform");
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

    updateLocalMatrix();
}

// start function
void Engine::Transform::start()
{
    localPosition = glm::vec3(0.0f);
    localRotation = glm::vec3(0.0f);
    localScale = glm::vec3(1.0f);

    updateWorldMatrix();
}

// set destroy
void Engine::Transform::setDestroy()
{
    Handler::error("can't destroy transform", "transform");
}

// update the world matrix
void Engine::Transform::updateWorldMatrix()
{
    glm::mat4 translate(1.0f);
    glm::mat4 rotation(1.0f);
    glm::mat4 scale(1.0f);

    translate = glm::translate(translate, localPosition);
    rotation = glm::mat4_cast(glm::quat(localRotation));
    scale = glm::scale(scale, localScale);

    localMatrix = translate * rotation * scale;
    worldMatrix = (parent != nullptr)? parent->getMatrix() * localMatrix : localMatrix;

    worldPosition = calculatePosition(worldMatrix);
    worldRotation = calculateRotation(worldMatrix);
    worldScale = calculateScale(worldMatrix);

    for(int i = 0; i < childs.size(); i++) childs[i]->updateWorldMatrix();

    for(int i = 0; i < refSiblings->size(); i++) (*refSiblings)[i]->onTransformChanged();
}

// update the local matrix
void Engine::Transform::updateLocalMatrix()
{
    glm::mat4 translate(1.0f);
    glm::mat4 rotation(1.0f);
    glm::mat4 scale(1.0f);

    translate = glm::translate(translate, worldPosition);
    rotation = glm::mat4_cast(glm::quat(worldRotation));
    scale = glm::scale(scale, worldScale);

    worldMatrix = translate * rotation * scale;
    localMatrix = (parent != nullptr)? glm::inverse(parent->getMatrix()) * worldMatrix : worldMatrix;

    localPosition = calculatePosition(localMatrix);
    localRotation = calculateRotation(localMatrix);
    localScale = calculateScale(localMatrix);

    for(int i = 0; i < childs.size(); i++) childs[i]->updateWorldMatrix();

    for(int i = 0; i < refSiblings->size(); i++) (*refSiblings)[i]->onTransformChanged();
}

// add child
void Engine::Transform::addChild(Transform* transform)
{
    childs.push_back(transform);
}

// remove child
void Engine::Transform::removeChild(Transform* transform)
{
    for(int i = 0; i < childs.size(); i++)
    {
        if(childs[i] != transform) continue;
        childs.erase(childs.begin() + i);
        break;
    }
}

// calculate the position from the matrix
glm::vec3 Engine::Transform::calculatePosition(glm::mat4 matrix)
{
    glm::vec3 position;
    
    position.x = matrix[3][0];
    position.y = matrix[3][1];
    position.z = matrix[3][2];
    
    return position;
}

// calculate the rotation from the matrix
glm::vec3 Engine::Transform::calculateRotation(glm::mat4 matrix)
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
glm::vec3 Engine::Transform::calculateScale(glm::mat4 matrix)
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

// fix the rotation to -360 to 360
glm::vec3 Engine::Transform::fixRotation(glm::vec3 rotation)
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