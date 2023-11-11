#include <engine/gameloop.hpp>

// create window and input for the game
void Engine::GameLoop::initialize(std::string title)
{
    GameLoop::title = title;
    GameLoop::width = Window::screen_width;
    GameLoop::height = Window::screen_height;

    window = new Window(title.c_str(), width, height);
    
    Input::window = window;
    Input::initialize();

    UI::setResolution(width, height);

    RendererManager::initialize();
    ColliderManager::initialize();
    ButtonManager::initialize();

    Camera::gameWindow = window;
    Handler::gameWindow = window;

    Actor::createMainCamera();
}

// called after game is ready to execute game loop
void Engine::GameLoop::begin()
{
    double fixedTime = 0.0f;

    while(window->shouldClose() == false)
    {   
        window->clearWindow();
        Input::updateInputs();
        
        if(Window::onFocus)
        {
            Time::frameStart();

            fixedTime += Time::getDeltaTime();

            Actor::cleanDestroyables();

            if(fixedTime >= 0.2f)
            {
                fixedTime = 0.0f;
                Actor::callFixedUpdate();
            }

            Actor::callUpdate();
            Actor::callLateUpdate();
            
            ColliderManager::startDetection();
            ButtonManager::checkInteraction();
        }
        
        RendererManager::drawFrame();

        window->updateWindow();
        
        if(Window::onFocus) Time::frameEnd();
    }

    Actor::eraseAllActors();

    window->close();
}

// end the game loop
void Engine::GameLoop::endGameLoop()
{
    window->makeWindowClose();
}

// get the current window size
glm::ivec2 Engine::GameLoop::getCurrentWindowSize()
{
    return window->getSize();
}