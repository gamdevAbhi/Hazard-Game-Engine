#include <engine/gameloop.hpp>


int main()
{
    Engine::GameLoop::initialize("game");
    
    Engine::GameLoop::begin();
    
    return 0;
}