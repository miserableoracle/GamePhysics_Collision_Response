#include "StartScene.h"
#include "Game.h"
#include <ctime>
#include "GLM/gtx/string_cast.hpp"
#include <algorithm>
#include "TileComparators.h"
#include <iomanip>

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
{
}

void StartScene::draw()
{
	m_pBall->draw();
	m_pBrick->draw();
	// ImGui Rendering section - DO NOT MOVE OR DELETE
	if (m_displayUI)
	{
		ImGui::Render();
		ImGuiSDL::Render(ImGui::GetDrawData());
		SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), 255, 255, 255, 255);
		if (m_debugMode) {
			//Util::DrawCircle(m_pShip->getPosition(), std::max(m_pShip->getWidth() * 0.5, m_pShip->getHeight() * 0.5));
			Util::DrawCircle(m_pBall->getPosition(), std::max(m_pBall->getWidth() * 0.5f, m_pBall->getHeight() * 0.5f));
			Util::DrawRect(glm::vec2(m_pBrick->getPosition().x - (m_pBrick->getWidth() / 2.0f), m_pBrick->getPosition().y - (m_pBrick->getHeight() / 2.0f)), m_pBrick->getWidth(), m_pBrick->getHeight());
			//Util::DrawCircle(m_pMine->getPosition(), std::max(m_pMine->getWidth() * 0.5, m_pMine->getHeight() * 0.5));
		}
	}
}

void StartScene::update()
{
	m_move();
	m_pBrick->update();
	if(sceneBegin)
		m_pBall->update();

	//CollisionManager::squaredRadiusCheck(m_pShip, m_pPlanet);
	//CollisionManager::squaredRadiusCheck(m_pShip, m_pMine);

	//CollisionManager::AABBCheck(m_pShip, m_pPlanet);
	//CollisionManager::AABBCheck(m_pShip, m_pMine);

	if (sceneBegin && CollisionManager::circleAABBCheck(m_pBall, m_pBrick)) {
		// Detect which side
		if (m_pBall->getPosition().y <= m_pBrick->getPosition().y + (m_pBrick->getHeight() / 2)) {
			// Hit from top the brick
			if(m_pBall->getVelY() < 0.0f)
				// Velocity of the ball in same direction
				m_pBall->setVelY((m_pBall->getVelY() - 0.02));
			else
				m_pBall->setVelY(-(m_pBall->getVelY() + 0.02));
		}
		if (m_pBall->getPosition().y >= m_pBrick->getPosition().y - (m_pBrick->getHeight() / 2)) {
			// Hit from bottom the brick
			if(m_pBall->getVelY() < 0.0f)
				m_pBall->setVelY(-(m_pBall->getVelY() - 0.02));
			else
				m_pBall->setVelY((m_pBall->getVelY() + 0.02));
		}

		if (m_pBall->getPosition().x < m_pBrick->getPosition().x  - (m_pBrick->getWidth() / 2)) {
			// Hit from left side of the brick
			if(m_pBall->getVelX() < 0.0f)
				m_pBall->setVelX((m_pBall->getVelX() - 0.02));
			else
				m_pBall->setVelX(-(m_pBall->getVelX() + 0.02));
		}
		
		if (m_pBall->getPosition().x > m_pBrick->getPosition().x + (m_pBrick->getWidth() / 2)) {
			// Hit from right side of the brick
			if (m_pBall->getVelX() < 0.0f)
				m_pBall->setVelX(-(m_pBall->getVelX() - 0.02));
			else
				m_pBall->setVelX((m_pBall->getVelX() + 0.02));
		}
	}
	//CollisionManager::circleAABBCheck(m_pShip, m_pMine);
	
	
	if (m_displayUI)
	{
		m_updateUI();
	}
}

void StartScene::clean()
{
	/*delete m_pStartLabel;
	delete m_pInstructionsLabel;*/

	delete m_pBall;
	delete m_pBrick;
	removeAllChildren();
}

void StartScene::handleEvents()
{
	ImGuiIO& io = ImGui::GetIO();
	int wheel = 0;

	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			TheGame::Instance()->quit();
			break;
		case SDL_MOUSEMOTION:
			m_mousePosition.x = event.motion.x;
			m_mousePosition.y = event.motion.y;
			m_pBrick->setPosition(m_mousePosition);
			break;
		case SDL_MOUSEWHEEL:
			wheel = event.wheel.y;
			break;
		case SDL_TEXTINPUT:
			io.AddInputCharactersUTF8(event.text.text);
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				TheGame::Instance()->quit();
				break;
			case SDLK_BACKQUOTE:
				m_displayUI = (m_displayUI) ? false : true;
				break;
			
			/***************************************************************/
			case SDLK_w:
				m_moveState = MOVE_UP;
				break;
			case SDLK_s:
				m_moveState = MOVE_DOWN;
				break;
			case SDLK_a:
				m_moveState = MOVE_LEFT;
				break;
			case SDLK_d:
				m_moveState = MOVE_RIGHT;
				break;
			}
			{
				int key = event.key.keysym.scancode;
				IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
				io.KeysDown[key] = (event.type == SDL_KEYDOWN);
				io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
				io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
				io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
				io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				m_moveState = MOVE_IDLE;
				break;
			case SDLK_s:
				m_moveState = MOVE_IDLE;
				break;
			case SDLK_a:
				m_moveState = MOVE_IDLE;
				break;
			case SDLK_d:
				m_moveState = MOVE_IDLE;
				break;
			}
			{
				int key = event.key.keysym.scancode;
				IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
				io.KeysDown[key] = (event.type == SDL_KEYDOWN);
				io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
				io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
				io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
				io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
			}
			break;
		default:
			break;
		}
	}

	io.DeltaTime = 1.0f / 60.0f;
	int mouseX, mouseY;
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
	io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	io.MouseWheel = static_cast<float>(wheel);

	io.DisplaySize.x = 1280;
	io.DisplaySize.y = 720;

	m_ImGuiKeyMap();
	m_ImGuiSetStyle();
}

void StartScene::start()
{
	TheSoundManager::Instance()->load(
		"../Assets/audio/yay.ogg", 
		"yay", SOUND_SFX);
	TheSoundManager::Instance()->load(
		"../Assets/audio/thunder.ogg", 
		"thunder", SOUND_SFX);
	TheSoundManager::Instance()->load(
		"../Assets/audio/bounce.wav",
		"bounce", SOUND_SFX
	);

	m_position = glm::vec2(400.0f, 400.0f);
	m_pBall = new Ball();
	m_pBall->setPosition(m_position);
	//m_pBall->setGrav(m_gravity);
	m_pBall->setPPM(m_PPM);
	addChild(m_pBall);

	m_pBrick = new Brick();
	m_pBrick->setPosition(glm::vec2(50.0f, 50.0f));
	addChild(m_pBrick);

	m_moveState = MOVE_IDLE;
	m_speedFactor = glm::vec2(2.0f, 2.0f);
}

void StartScene::m_ImGuiKeyMap()
{
	ImGuiIO& io = ImGui::GetIO();

	// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
	io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
	io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
	io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
	io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
	io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
	io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
	io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
	io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
	io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;

	io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
	io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
	io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
	io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
	io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
	io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;
}

void StartScene::m_ImGuiSetStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 0.8f;
	style.FrameRounding = 3.0f;
	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void StartScene::m_updateUI()
{
	// Prepare Window Frame
	ImGui::NewFrame();
	//ImGui::ShowDemoWindow(); // use for debug purposes

	std::string windowString = "Physics Settings ";

	ImGui::Begin(&windowString[0], NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar);

	// set window to top left corner
	ImGui::SetWindowPos(ImVec2(0, 0), true);

	/*************************************************************************************************/
	/* MENU                                                                                          */
	/*************************************************************************************************/

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::Separator();
			ImGui::MenuItem("Exit", NULL, &m_exitApp);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::Separator();
			ImGui::MenuItem("About", NULL, &m_displayAbout);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (m_exitApp)
	{
		TheGame::Instance()->quit();
	}

	if (m_displayAbout)
	{
		ImGui::Begin("About Physics...", &m_displayAbout, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Separator();
		ImGui::Text("Authors:");
		ImGui::Text("Tom Tsiliopoulos ");
		ImGui::Text("Editors:");
		ImGui::Text("Hiren Pandya, Nigel Mortley, Maxwell Bojarzin");
		ImGui::End();
	}

	/*************************************************************************************************/
	if (ImGui::Button("Play"))
	{
		sceneBegin = true;
		m_pBall->setGrav(m_gravity);
		m_pBall->setIdle(false);
	}

	ImGui::SameLine();

	if (ImGui::Button("Reset"))
	{
		sceneBegin = false;
		m_pBall->setVelX(0.0f);
		m_pBall->setVelY(0.0f);
		m_pBall->setPosition(glm::vec2(400.0f, 300.0f));
		m_gravity = 9.81f;
		m_pBall->setGrav(m_gravity);
	}
	
	ImGui::Separator();
	if (ImGui::Button("Next Scene"))
	{
		Game::Instance()->changeSceneState(SceneState::BULLET_RAIN_SCENE);
	}

	ImGui::Separator();
	if (ImGui::Button("Toggle Debug Mode")) {
		m_debugMode = (m_debugMode) ? false : true;
	}

	//ImGui::SameLine();

	//if (ImGui::Button("Respawn Mines"))
	//{
	//	/*m_respawnMines();
	//	m_moveAlongPath = false;*/
	//}

	//ImGui::SameLine();

	//if (ImGui::Button("Toggle Grid"))
	//{
	//	/*m_debugMode = (m_debugMode) ? false : true;*/
	//}

	//ImGui::SameLine();

	//if (ImGui::Button("Reset All"))
	//{
	//	/*m_resetAll();*/
	//}

	//ImGui::PushItemWidth(80);
	///*if (ImGui::SliderFloat("Manhattan Factor", m_grid[0][0]->getManhanttanFactor(), 0.1f, 10.0f, "%.1f"))
	//{
	//	float newFactor = *m_grid[0][0]->getManhanttanFactor();

	//	for (size_t row = 0; row < m_rowSize; row++)
	//	{
	//		for (size_t col = 0; col < m_colSize; col++)
	//		{
	//			m_grid[col][row]->setManhanttanFactor(newFactor);
	//		}
	//	}
	//}*/

	//ImGui::SameLine();

	///*if (ImGui::SliderFloat("Euclidean Factor", m_grid[0][0]->getEuclideanFactor(), 0.1f, 10.0f, "%.1f"))
	//{
	//	float newFactor = *m_grid[0][0]->getEuclideanFactor();

	//	for (size_t row = 0; row < m_rowSize; row++)
	//	{
	//		for (size_t col = 0; col < m_colSize; col++)
	//		{
	//			m_grid[col][row]->setEuclideanFactor(newFactor);
	//		}
	//	}
	//}*/

	//ImGui::SameLine();

	///*if (ImGui::SliderFloat("Mine Factor", m_grid[0][0]->getMineFactor(), 0.1f, 10.0f, "%.1f"))
	//{
	//	float newFactor = *m_grid[0][0]->getMineFactor();

	//	for (size_t row = 0; row < m_rowSize; row++)
	//	{
	//		for (size_t col = 0; col < m_colSize; col++)
	//		{
	//			m_grid[col][row]->setMineFactor(newFactor);
	//		}
	//	}
	//}*/
	//ImGui::PopItemWidth();

	//if (ImGui::CollapsingHeader("Ship Locations"))
	//{
	//	/*ImGui::PushItemWidth(80);
	//	int count = 0;

	//	std::string shipText;
	//	shipText = "Ship " + count;
	//	shipText += " Position: ";
	//	ImGui::Text(shipText.c_str());
	//	ImGui::SameLine();
	//	glm::vec2 pos = m_ship.getPosition();
	//	ImGui::InputFloat2("", &pos[0], 0, ImGuiInputTextFlags_ReadOnly);
	//	count++;
	//	ImGui::PopItemWidth();*/
	//}

	//if (ImGui::CollapsingHeader("Planet Location"))
	//{
	//	/*ImGui::PushItemWidth(80);
	//	std::string planetText;
	//	planetText = "Planet Position: ";
	//	ImGui::Text(planetText.c_str());
	//	ImGui::SameLine();
	//	glm::vec2 pos = m_planet.getPosition();
	//	ImGui::InputFloat2("", &pos[0], 0, ImGuiInputTextFlags_ReadOnly);
	//	ImGui::PopItemWidth();*/
	//}

	//if (ImGui::CollapsingHeader("Mine Locations"))
	//{
	//	/*ImGui::PushItemWidth(80);
	//	int count = 0;
	//	for (Mine mine : m_pMines)
	//	{
	//		std::string mineText;
	//		mineText = "Mine " + count;
	//		mineText += " Position: ";
	//		ImGui::Text(mineText.c_str());
	//		ImGui::SameLine();
	//		glm::vec2 pos = mine.getPosition();
	//		ImGui::InputFloat2("", &pos[0], 0, ImGuiInputTextFlags_ReadOnly);
	//		count++;
	//	}
	//	ImGui::PopItemWidth();*/
	//}

	//ImGui::Separator();

	//ImGui::PushItemWidth(80);
	////glm::vec2 targetPosition = getTargetPosition();
	///*if (ImGui::SliderFloat("Target X Position", &targetPosition.x, 0.0f, 800.0f, "%.0f"))
	//{
	//	setTargetPosition(targetPosition);
	//}*/
	//ImGui::SameLine();
	/*if (ImGui::SliderFloat("Target Y Position", &targetPosition.y, 0.0f, 600.0f, "%.0f"))
	{
		setTargetPosition(targetPosition);
	}*/
	//ImGui::PopItemWidth();

	//ImGui::Separator();

	//if (ImGui::Button("Find Path"))
	//{
	//	//std::cout << "**** NEW PATH ****" << std::endl;

	//	/*findShortestPath();
	//	m_moveAlongPath = false;*/
	//}

	//if (ImGui::CollapsingHeader("Open Tiles"))
	//{
	//	/*ImGui::PushItemWidth(80);
	//	int count = 0;
	//	for (Tile* tile : m_pOpen)
	//	{
	//		std::string tileText;
	//		tileText = "Tile ";
	//		tileText += std::to_string(count);
	//		tileText += " Position: ";
	//		ImGui::Text(tileText.c_str());
	//		ImGui::SameLine();
	//		glm::vec2 pos = tile->getPosition();
	//		ImGui::InputFloat2("", &pos[0], 0, ImGuiInputTextFlags_ReadOnly);
	//		count++;
	//	}
	//	ImGui::PopItemWidth();*/
	//}

	//if (ImGui::CollapsingHeader("Closed Tiles"))
	//{
	//	/*ImGui::PushItemWidth(80);
	//	int count = 0;
	//	for (Tile* tile : m_pClosed)
	//	{
	//		std::string tileText;
	//		tileText = "Tile ";
	//		tileText += std::to_string(count);
	//		tileText += " Position: ";
	//		ImGui::Text(tileText.c_str());
	//		ImGui::SameLine();
	//		glm::vec2 pos = tile->getPosition();
	//		ImGui::InputFloat2("", &pos[0], 0, ImGuiInputTextFlags_ReadOnly);
	//		count++;
	//	}
	//	ImGui::PopItemWidth();*/
	//}

	/*if (!m_pOpen.empty())
	{
		if (ImGui::Button("Move Ship Along Path"))
		{
			m_moveAlongPath = true;
			m_pathLength = 0;
		}
	}*/

	// Main Window End
	ImGui::End();
}

void StartScene::m_move()
{
	//m_acceleration = glm::vec2(0.0f, 0.5 * m_gravity * m_PPM);
	if (sceneBegin) {

		/*if (m_moveState == MOVE_UP)
		{
			m_velocity = glm::vec2(m_velocity.x, -1.0f);
		}

		if (m_moveState == MOVE_DOWN)
		{
			m_velocity = glm::vec2(m_velocity.x, 1.0f);
		}

		if (m_moveState == MOVE_LEFT)
		{
			m_velocity = glm::vec2(-1.0f, m_velocity.y);
		}

		if (m_moveState == MOVE_RIGHT)
		{
			m_velocity = glm::vec2(1.0f, m_velocity.y);
		}

		if (m_moveState == MOVE_IDLE)
		{
			m_velocity = glm::vec2(0.0f, 0.0f);
		}*/

		// Detect collision with walls
			// BounceY | BounceX
		// Detect collision with brick
			// Direction and angle 
		//m_pBall->setVelX(2.0f);
		//m_pBall->setAccelY(m_gravity);
		//m_velocity = glm::vec2(0.0f, m_gravity);
		//m_position = m_pBall->getPosition() + m_velocity; // +m_acceleration;
		//m_pBall->setPosition(m_position);
	}
}
