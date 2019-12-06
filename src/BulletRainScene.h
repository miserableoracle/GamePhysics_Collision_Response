#pragma once
#ifndef __BULLET_RAIN_SCENE__
#define __BULLET_RAIN_SCENE__

#include "Scene.h"
#include "Label.h"
#include "ship.h"
#include "MoveState.h"
#include "Bullet.h"
#include "Planet.h"
#include "Mine.h"
#include "CollisionManager.h"
#include "SoundManager.h"

class BulletRainScene : public Scene
{
public:
	BulletRainScene();
	~BulletRainScene();

	// Inherited via Scene
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

private:
	/*Label* m_pStartLabel;
	Label* m_pInstructionsLabel;*/

	// Ship Stuff
	
	Planet* m_pPlanet;
	Bullet* m_pBullet[10];
	MoveState m_moveState;
	glm::vec2 m_speedFactor;

	glm::vec2 m_mousePosition;
	bool sceneBegin = false;
	bool m_debugMode = false;
	// ImGui utility functions
	void m_ImGuiKeyMap();
	void m_ImGuiSetStyle();
	void m_updateUI();

	// ImGui menu variables
	bool m_exitApp = false;
	bool m_displayAbout = false;
	bool m_displayUI = true;

	// Physics Variables
	float m_gravity = 9.8f;
	float m_PPM = 10.01f; // pixels per meter
	glm::vec2 m_position = glm::vec2(0.0f, 0.0f);
	glm::vec2 m_velocity = glm::vec2(0.0f, 0.0f);
	glm::vec2 m_acceleration = glm::vec2(0.0f, 0.0f);

	// Physics functions
	void m_move();
};

#endif /* defined (__START_SCENE__) */