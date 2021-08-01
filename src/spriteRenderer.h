#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "texture.h"
#include "game.h"

class SpriteRenderer
{
public:
    SpriteRenderer(Shader& shader, Game& game);
    ~SpriteRenderer();

    void DrawSprite(Texture2D& texture, glm::vec2 position,
        glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
        glm::vec3 color = glm::vec3(1.0f));
private:
    Game& game;
    Shader       shader;
    unsigned int quadVAO;

    void initRenderData();
};