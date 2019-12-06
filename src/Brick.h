#pragma once
#ifndef __Brick__
#define __Brick__

#include <vector>

#include "DisplayObject.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Tile.h"
#include "Scene.h"

class Brick :
	public DisplayObject
{
public:
	Brick();
	~Brick();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;
};

#endif /* defined (__Brick__) */

