#pragma once
#ifndef __START_SCENE__
#define __START_SCENE__

#include "Scene.h"
#include "Label.h"
#include "Ball.h"
#include "Brick.h"
#include "ship.h"
#include "MoveState.h"
#include "Planet.h"
#include "Mine.h"
#include "CollisionManager.h"
#include "SoundManager.h"

class StartScene : public Scene
{
public:
	StartScene();
	~StartScene();

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
	Ball* m_pBall;
	Brick* m_pBrick;
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