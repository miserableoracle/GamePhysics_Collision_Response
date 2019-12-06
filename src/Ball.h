#pragma once
#ifndef __Ball__
#define __Ball__

#include <vector>

#include "DisplayObject.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Tile.h"
#include "Scene.h"

class Ball :
	public DisplayObject
{
public:
	Ball();
	~Ball();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	// Getters
	float getVelX() { return m_fVelocityX; }
	float getVelY() { return m_fVelocityY; }
	float getAccelX() { return m_fAccelX; }
	float getAccelY() { return m_fAccelY; }
	void setAccelX(float p_fAccelX) { m_fAccelX = p_fAccelX; }
	void setAccelY(float p_fAccelY) { m_fAccelY = p_fAccelY; }
	void setVelX(float p_fVelX) { m_fVelocityX = p_fVelX; }
	void setVelY(float p_fVelY) { m_fVelocityY = p_fVelY; }
	bool isIdle() { return m_bIdle; }
	void setIdle(bool p_fIdle) { m_bIdle = p_fIdle; }
	void setGrav(float p_fGrav) { m_fGravity = p_fGrav; }
	void setPPM(float p_ppm) { m_PPM = p_ppm; }
	float getRadius() { return m_fRadius; }

private:
	void m_reset();
	double m_currentDirection;
	bool m_bIdle;
	float m_fGravity;
	float m_fAirRes;
	float m_fFric;
	
	// Ball related properties
	float m_fRadius, m_fBounciness,  m_fVelocityX, m_fVelocityY, m_fAccelX, m_fAccelY;
	float m_PPM;
	glm::vec2 m_fDxDy;
	// Ball related functions
	void checkWallCollision();
	//void ball_updateVel();
};

typedef SoundManager TheSoundManager;
#endif /* defined (__Ball__) */

