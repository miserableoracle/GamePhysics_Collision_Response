// Core Libraries
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <string>

#include <math.h>
#include <Windows.h>

//#include <map> // for std::map
//#include <memory> // for std::shared_ptr
//#include <fstream>

// 3rd Party Libraries
#include <GLEW\glew.h>
//#include <GLUT/freeglut.h>
//#include <IL/il.h> // for ilInit()
//#include "IL\ilu.h"
#include <glm\vec3.hpp>
#include <glm\gtx\color_space.hpp>


#include "TextureManager.h"
#include "Game.h"

const int FPS = 60;
const int DELAY_TIME = 2500.0f / FPS;

int main(int argc, char * args[])
{
	Uint32 frameStart, frameTime;
	UINT32 frames = 0;


	// show console
	AllocConsole();
	freopen("CON", "w", stdout);


	TheGame::Instance()->init("Hello World", 100, 100, 800, 600, false);

	while (TheGame::Instance()->running())
	{
		frameStart = SDL_GetTicks();

		TheGame::Instance()->handleEvents();
		TheGame::Instance()->update();
		TheGame::Instance()->render();

		frameTime = SDL_GetTicks() - frameStart;
		if (frameTime< DELAY_TIME)
		{
			SDL_Delay((int)(DELAY_TIME - frameTime));
		}

		frames++;
		TheGame::Instance()->setFrames(frames);

	}

	TheGame::Instance()->clean();
	return 0;
}

