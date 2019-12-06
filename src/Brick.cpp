#include "Brick.h"
#include "Game.h"
#include "Util.h"
#include "GLM/gtx/rotate_vector.hpp"
#include "PlayScene.h"
#include "GLM/gtx/string_cast.hpp"
#include <math.h>
#include "CircleCollider.h"
#include <algorithm>

Brick::Brick()
{
	TheTextureManager::Instance()->load("../Assets/textures/brick.png",
		"brick", TheGame::Instance()->getRenderer());

	glm::vec2 size = TheTextureManager::Instance()->getTextureSize("brick");
	setWidth(size.x);
	setHeight(size.y);
	setPosition(glm::vec2(50.0f, 50.0f));
	setIsColliding(false);
	setType(GameObjectType::BRICK);
}


Brick::~Brick()
{
}

void Brick::draw()
{
	int xComponent = getPosition().x;
	int yComponent = getPosition().y;

	TheTextureManager::Instance()->draw("brick", xComponent, yComponent,
		TheGame::Instance()->getRenderer(), 0.0f, 255, true);
}

void Brick::update()
{
	
}

void Brick::clean()
{

}
