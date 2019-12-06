#include "Ball.h"
#include "Game.h"
#include "Util.h"
#include "GLM/gtx/rotate_vector.hpp"
#include "PlayScene.h"
#include "GLM/gtx/string_cast.hpp"
#include <math.h>
#include "CircleCollider.h"
#include <algorithm>



Ball::Ball() :
	m_currentDirection(0.0f), m_fRadius(0.0f), m_fBounciness(-0.7f), m_fVelocityX(0.0f), m_fVelocityY(0.0f), m_fAccelX(0.0f), m_fAccelY(0.0f),
	m_bIdle(true), m_fDxDy(glm::vec2(0.0f, 0.0f)), m_fGravity(9.81f)
{
	TheTextureManager::Instance()->load("../Assets/textures/ball.png",
		"ball", TheGame::Instance()->getRenderer());

	glm::vec2 size = TheTextureManager::Instance()->getTextureSize("ball");
	setWidth(size.x);
	setHeight(size.y);
	m_fRadius = std::max(size.x * 0.5f, size.y * 0.5f);
	setPosition(glm::vec2(400.0f, 300.0f));
	setVelocity(glm::vec2(m_fVelocityX, m_fVelocityY));
	setAcceleration(glm::vec2(m_fAccelX, m_fAccelY));
	setIsColliding(false);
	setType(GameObjectType::BALL);
	setState(State::IDLE);
	// set the collider shape
	//setCollisionShape(CIRCLE);
}


Ball::~Ball()
{
}

void Ball::draw()
{
	int xComponent = getPosition().x;
	int yComponent = getPosition().y;

	TheTextureManager::Instance()->draw("ball", xComponent, yComponent,
		TheGame::Instance()->getRenderer(), m_currentDirection, 255, true);
}

void Ball::update()
{
	if (!m_bIdle) {
		m_fAccelY = m_fGravity;
		m_fVelocityX += ((m_fAccelX / 2.0f) * (1.0f / 60.0f) * (1.0f / 60.0f));	// 1/2*ax*t*t
		m_fVelocityY += ((m_fAccelY / 2.0f) * (1.0f / 60.0f) * (1.0f / 60.0f)); // 1/2*ay*t*t
		checkWallCollision();
		//calculateVelocity();
		setPosition(glm::vec2(getPosition().x + (m_fVelocityX * 100.0f * m_PPM), getPosition().y + (m_fVelocityY * 100.0f * m_PPM)));
	}

	std::cout << "VelX: " << m_fVelocityX << " VelY: " << m_fVelocityY << std::endl;
}

void Ball::clean()
{
	
}

void Ball::m_reset()
{
	setIsColliding(false);
	int halfWidth = getWidth() * 0.5;
	int xComponent = rand() % (640 - getWidth()) + halfWidth + 1;
	int yComponent = -getHeight();
	setPosition(glm::vec2(xComponent, yComponent));
	m_fAccelX = m_fAccelY = 0.0f;
	m_fVelocityX = 0.0f;
	m_fVelocityY = m_fGravity;
}

void Ball::checkWallCollision() {
	if (getPosition().y > ((float)Config::SCREEN_HEIGHT - (float)m_fRadius)) {
		m_fVelocityY *= m_fBounciness;
		setPosition(glm::vec2(getPosition().x, (float)Config::SCREEN_HEIGHT - (float)m_fRadius));
		TheSoundManager::Instance()->playSound("bounce", 0);
	}
	if (getPosition().y < m_fRadius) {
		m_fVelocityY *= m_fBounciness;
		setPosition(glm::vec2(getPosition().x, m_fRadius));
		TheSoundManager::Instance()->playSound("bounce", 0);
	}
	if (getPosition().x < m_fRadius) {
		m_fVelocityX *= m_fBounciness;
		setPosition(glm::vec2(m_fRadius, getPosition().y));
		TheSoundManager::Instance()->playSound("bounce", 0);
	}
	if (getPosition().x > ((float)(Config::SCREEN_WIDTH) - (float)m_fRadius)) {
		m_fVelocityX *= m_fBounciness;
		setPosition(glm::vec2((float)Config::SCREEN_WIDTH - (float)m_fRadius, getPosition().y));
		TheSoundManager::Instance()->playSound("bounce", 0);
	}

}