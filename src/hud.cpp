#include "hud.h"
#include "resource_manager.h"
#include "texture.h"
#include "player.h"
#include <string>

void HUD::loadTextures()
{
	ResourceManager::LoadTexture("textures/hp_3.png", true, "hp_3");
	ResourceManager::LoadTexture("textures/hp_2.png", true, "hp_2");
	ResourceManager::LoadTexture("textures/hp_1.png", true, "hp_1");
	ResourceManager::LoadTexture("textures/hp_0.png", true, "hp_0");

	ResourceManager::LoadTexture("textures/dash_5.png", true, "dash_5");
	ResourceManager::LoadTexture("textures/dash_4.png", true, "dash_4");
	ResourceManager::LoadTexture("textures/dash_3.png", true, "dash_3");
	ResourceManager::LoadTexture("textures/dash_2.png", true, "dash_2");
	ResourceManager::LoadTexture("textures/dash_1.png", true, "dash_1");
	ResourceManager::LoadTexture("textures/dash_0.png", true, "dash_0");
}

HUD::HUD(Game &game, SpriteRenderer &renderer) : game(game), renderer(renderer)
{

	padding = game.Width / 30.0f;
	hp_size = glm::vec2(game.Width / 14.0f, game.Width / 26.3f);
	dash_size = glm::vec2(game.Width / 14.0f, game.Width / 26.3f);

	//calculate positions for the two HUD elements
	float largerHeight = (hp_size.y > dash_size.y) ? hp_size.y : dash_size.y;
	hp_pos.y = dash_pos.y = game.Height - padding - (scale * largerHeight);
	hp_pos.x = padding;
	dash_pos.x = padding + (scale * hp_size.x) + padding;
}

void HUD::draw()
{
	Player *player = game.player;
	std::string hp_str = "hp_" + std::to_string(player->getHP());
	Texture2D hp_txt = ResourceManager::GetTexture(hp_str.c_str());

	int dashLevel = (int)(game.elapsedTime - player->getLastDashTime());
	if (dashLevel > 5)
		dashLevel = 5;
	std::string dash_str = "dash_" + std::to_string(dashLevel);
	Texture2D dash_txt = ResourceManager::GetTexture(dash_str.c_str());

	//draw hp meter
	renderer.DrawSprite(hp_txt, hp_pos, scale * hp_size);

	//draw dash meter
	renderer.DrawSprite(dash_txt, dash_pos, scale * dash_size);
}