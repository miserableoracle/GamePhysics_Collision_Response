#pragma once
#ifndef __SCENE__
#define __SCENE__

#include <iostream>
#include <vector>
#include <string>

#include "GameObject.h"

enum SceneState
{
	NO_SCENE = -1,
	START_SCENE,
	BULLET_RAIN_SCENE,
	PLAY_SCENE,
	END_SCENE,
	NUM_OF_SCENES
};

class Scene : public GameObject
{
public:
	friend class DisplayObject;
	Scene();
	virtual ~Scene();

	// Inherited via GameObject
	virtual void draw() override = 0;
	virtual void update() override = 0;
	virtual void clean() override = 0;
	virtual void handleEvents() = 0;
	virtual void start() = 0;

	void addChild(DisplayObject* child);
	void removeAllChildren();
	int numberOfChildren();

private:
	std::vector<DisplayObject*> m_displayList;
};

#endif /* defined (__SCENE__) */
