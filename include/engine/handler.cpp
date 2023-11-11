#include <engine/handler.hpp>

// debug message and sender name in powershell
void Engine::Handler::debug(std::string message, std::string senderName)
{
    std::cout << "Debug : " << std::endl;
    std::cout << "Sender : " << senderName << std::endl;
    std::cout << "Message :" << std::endl;
    std::cout << message << std::endl;
    std::cout << std::endl;
}

// debug message and sender name in powershell and close the game
void Engine::Handler::error(std::string message, std::string senderName)
{
    std::cout << "Debug : " << std::endl;
    std::cout << "Sender : " << senderName << std::endl;
    std::cout << "Message :" << std::endl;
    std::cout << message << std::endl;
    std::cout << std::endl;

    gameWindow->makeWindowClose();
}