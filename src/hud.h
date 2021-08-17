#pragma once
#include "game.h"
#include "spriteRenderer.h"

/*heads up display to show the HP and DASH meters in the bottom left corner of the window*/
class HUD {
private:
	Game& game;
	SpriteRenderer& renderer;
public:
	const glm::vec2 hp_size   = glm::vec2(52.0f, 28.0f);
	const glm::vec2 dash_size = glm::vec2(52.0f, 28.0f);
	glm::vec2 hp_pos, dash_pos;
	const float scale   = 2.0f;
	const float padding = 30.0f;

	HUD(Game& game, SpriteRenderer& renderer);
	static void loadTextures();
	void draw();
};