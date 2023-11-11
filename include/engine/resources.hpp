#ifndef ENGINE_RESOURCES_H
#define ENGINE_RESOURCES_H

#include <engine/handler.hpp>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>

namespace Engine
{
    // geting resources in the game resouces folder
    class Resources final
    {
    public:
        static bool is_file_exist(std::string file_path); 
        static std::string get_current_dir();
        static std::string get_file_content(std::string file_path);
    };
}

#endif