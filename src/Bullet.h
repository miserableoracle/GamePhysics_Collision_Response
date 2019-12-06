#pragma once
#ifndef __Bullet__
#define __Bullet__

#include <vector>

#include "DisplayObject.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Tile.h"
#include "Scene.h"

class Bullet :
	public DisplayObject
{
public:
	Bullet();
	~Bullet();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

};

typedef SoundManager TheSoundManager;
#endif /* defined (__Ball__) */

