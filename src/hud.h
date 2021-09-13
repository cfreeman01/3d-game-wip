#pragma once
#include "game.h"
#include "spriteRenderer.h"

/*heads up display to show the HP and DASH meters in the bottom left corner of the window*/
class HUD
{
private:
	Game &game;
	SpriteRenderer &renderer;

	glm::vec2 hp_size;
	glm::vec2 dash_size;
	glm::vec2 hp_pos, dash_pos;
	float scale = 2.0f;
	float padding;

public:
	HUD(Game &game, SpriteRenderer &renderer);
	static void loadTextures();
	void draw();
};