#include <engine/resources.hpp>

// get if file exist or not
bool Engine::Resources::is_file_exist(std::string file_path)
{
    std::ifstream file;

    file.open(file_path);

    bool exist = file.is_open();

    file.close();

    return exist;
}

// get the current executable path
std::string Engine::Resources::get_current_dir()
{
    char buff[MAX_PATH];
    GetModuleFileNameA(NULL, buff, MAX_PATH);
    std::string::size_type pos = std::string(buff).find_last_of("\\/");
    return std::string(buff).substr(0, pos);
}

// get the file contents in string
std::string Engine::Resources::get_file_content(std::string file_path)
{
    std::string content;

    // checking if file exist
    if(is_file_exist(file_path) == false) 
    {
        Handler::error("file path : " + file_path + " not found", "resources");
        return content;
    }

    // opening the file
    std::ifstream file;

    file.open(file_path);

    std::string line;

    // storing file content
    while(file)
    {
        std::getline(file, line);
        content += line + "\n";
        if(file.eof()) break;
    }

    content += "\0";

    file.close();

    return content;
}