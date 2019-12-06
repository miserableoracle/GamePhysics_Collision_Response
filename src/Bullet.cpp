#include "Bullet.h"
#include "Game.h"
#include "Util.h"
#include "GLM/gtx/rotate_vector.hpp"
#include "PlayScene.h"
#include "GLM/gtx/string_cast.hpp"
#include <math.h>
#include "CircleCollider.h"
#include <algorithm>



Bullet::Bullet()
{
	TheTextureManager::Instance()->load("../Assets/textures/bullet.png",
		"bullet", TheGame::Instance()->getRenderer());

	glm::vec2 size = TheTextureManager::Instance()->getTextureSize("bullet");
	setWidth(size.x);
	setHeight(size.y);
	setPosition(glm::vec2(rand() % Config::SCREEN_WIDTH, -(rand() % 500)));
	setIsColliding(false);
	setType(GameObjectType::BULLET);
	setState(State::IDLE);
}


Bullet::~Bullet()
{
}

void Bullet::draw()
{
	int xComponent = getPosition().x;
	int yComponent = getPosition().y;

	TheTextureManager::Instance()->draw("bullet", xComponent, yComponent,
		TheGame::Instance()->getRenderer(), 0.0f, 255, true);
}

void Bullet::update()
{
	// Bullet moves down with the speed of 10 pixels/frame
	setPosition(glm::vec2(getPosition().x, getPosition().y + 10));
	// If bullet goes beyond screen height, change the position of the bullet
	// to random top location.
	if ((getPosition().y - getHeight()/2) > Config::SCREEN_HEIGHT)
		setPosition(glm::vec2(rand() % Config::SCREEN_WIDTH, -(rand() % 500)));
}

void Bullet::clean()
{

}