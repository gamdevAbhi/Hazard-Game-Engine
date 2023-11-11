#ifndef ENGINE_TEXT_HPP
#define ENGINE_TEXT_HPP

#include <engine/vao.hpp>
#include <engine/vbo.hpp>
#include <engine/ebo.hpp>
#include <engine/shader.hpp>
#include <engine/sprite.hpp>
#include <engine/font.hpp>
#include <engine/renderer.hpp>
#include <engine/recttransform.hpp>
#include <engine/quadtree.hpp>

namespace Engine
{
    class Text : public Renderer
    {
    public:
        enum Alignment {RIGHT, MIDDLE, LEFT};
        Font* getFont();
        std::string getText();
        glm::vec4 getTextColor();
        float getScale();
        unsigned int getCharPerLine();
        int getFontSize();
        Alignment getAlignment();
        void setFont(Font* font);
        void setText(std::string text);
        void setTextColor(glm::vec4 color);
        void setScale(float scale);
        void setCharPerLine(unsigned int chars);
        void setAlignment(Alignment alignment);
    protected:
        inline static Font* defaultFont = nullptr;
        inline static QuadTree* root = nullptr;
        inline static Shader* shader = nullptr;
        inline static VAO* vao = nullptr;
        inline static VBO* vbo = nullptr;
        inline static EBO* ebo = nullptr;
        std::vector<vertex> vertices;
        std::vector<glm::mat4> charMatrices;
        std::vector<Sprite*> sprites;
        Node* node;
        Font* font;
        std::string text;
        glm::vec4 textColor;
        float scale;
        unsigned int charPerLine;
        int fontSize;
        Alignment textAlignment;
        float lengthX;
        float lengthY;
        void start();
        void onTransformChanged();
        void updateProperties();
        void nodeUpdate();
        void onDestroy();
        void setFontSize(int fontSize);
        glm::mat4 computeCharMatrix(glm::vec2 offset);
    private:
        static void initialize();
        void draw();
    friend class RendererManager;
    };
}

#endif