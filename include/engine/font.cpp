#include <engine/font.hpp>

Engine::Font::Font(std::string relative_font_path)
{
    if(Resources::is_file_exist(Resources::get_current_dir() + relative_font_path) == false)
    {
        Handler::debug("font path not exist", "Font");
        return;
    }

    font_path = Resources::get_current_dir() + relative_font_path;
}

// destroy font
void Engine::Font::destroy()
{
    for(auto map : character_map)
    {
        for(auto character : map.second)
        {
            character.second.sprite->destroy();
        }
    }

    character_map.clear();
}

// get font character
std::map<char, Engine::Character> Engine::Font::getCharacter(int fontSize)
{
    if(character_map.find(fontSize) == character_map.end()
    && addCharacter(fontSize) == false) return std::map<char, Engine::Character>();
    else return character_map[fontSize];
}

// add font character
bool Engine::Font::addCharacter(int fontSize)
{
    std::map<char, Character> newCharacters;
    FT_Library fLibrary;
    FT_Face face;

    if(FT_Init_FreeType(&fLibrary))
    {
        Handler::debug("font can't be instantiate", "Font");
        return false;
    }

    if(FT_New_Face(fLibrary, font_path.c_str(), 0, &face))
    {
        Handler::debug("font can't create new face", "Font");
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    FT_UInt index;
    char c = FT_Get_First_Char(face, &index);

    while(index)
    {
        FT_Load_Char(face, c, FT_LOAD_RENDER);

        Character character = 
        {
            new Sprite(face), glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };

        newCharacters.insert(std::pair<char, Character>(c, character));

        c = FT_Get_Next_Char(face, c, &index);
    }

    FT_Done_Face(face);
    FT_Done_FreeType(fLibrary);

    character_map.insert(std::pair<int, std::map<char, Character>>(fontSize, newCharacters));

    return true;
}