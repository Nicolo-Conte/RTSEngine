#include <iostream>
#include <string>
#include <memory>

#include "Engine.h"
#include "ResourceManager.h"
#include "SaveManager.h"
#include "AudioManager.h"

//Used for test
#include <vector>
#include "EntityManager/Faction.h"
#include "EntityManager/Object.h"
#include "EntityManager/Camera.h"
#include "ResourceManager/TileMap.h"
#include "PlayerControls.h"
#include "GameMatch.h"

#ifdef _WIN64
#ifndef DX12
#include "SFMLRenderer.h"
#endif // !DX12
#elif __ORBIS__
#include "kernel.h"
size_t sceLibcHeapSize = 128 * 1024 * 1024; //Allocate 128 Megabytes
#endif


#ifdef DX12
FILE* stream;
int CALLBACK wWinMain(HINSTANCE a_hInstance, HINSTANCE, PWSTR, int)
#else
int main()
#endif
{
#ifdef DX12
	AllocConsole();

	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	std::unique_ptr engine = std::make_unique<Engine>();
	engine->Initialize(a_hInstance);
#else
	std::unique_ptr engine = std::make_unique<Engine>();
	engine->Initialize();
#endif
#ifdef __ORBIS__
	sceKernelLoadStartModule("/app0/Lib/Debug/libfmodL.prx", 0, 0, 0, NULL, NULL);
#endif
	//Add audio
	engine->GetAudioManager()->AddAudio("RTSBackgroundMusic.wav", AudioType::Music, "BGMusic", true);
	engine->GetAudioManager()->PlayAudio("BGMusic");

	//Create match data & player controls
	std::unique_ptr Gamematch = std::make_unique<GameMatch>();
	Gamematch->InitMatch(1, 2, engine->GetEntityManager(), engine->GetResourceManager());
	std::shared_ptr Playercontrols = std::make_unique<PlayerControls>(Gamematch->GetGameObjectManager(), engine->GetEntityManager(), engine->GetRenderer());
	engine->GetRenderer()->LoadMap(engine->GetResourceManager()->GetTileMaps()[0]);
	Gamematch->GetGameObjectManager()->SetPlayerControls(Playercontrols);
	std::shared_ptr<Object> MapTile = std::make_shared<Object>(true, true, glm::vec2(1, 1), Sprite(1, 1, false, SpriteType::Tile), Animation(), Transform());
	std::shared_ptr<Object> Player = std::make_shared<Object>(true, true, glm::vec2(1, 1), Sprite(1, 1, false, SpriteType::Unit), Animation(), Transform());

	Player->GetTransform().Scale = glm::vec2(1, 1);
	Player->GetTransform().Position = glm::vec2(-100, -100);

	engine->GetEntityManager()->AddObject(Player);

#ifdef _WIN64
#ifdef DX12
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		engine->Update();
		engine->GetInput()->Update(msg);

		float MouseInfoX = engine->GetInput()->GetAxis(PCInput::MouseX, EPS4Input::PS4LeftStickX);
		float MouseInfoY = engine->GetInput()->GetAxis(PCInput::MouseY, EPS4Input::PS4LeftStickY);
		printf("MouseInfo: %f - %f \n", MouseInfoX, MouseInfoY);

	}
#else
	sf::Event event;
	while (engine->GetRenderer()->GetWindow().isOpen())
	{
		//Get input
		while (engine->GetRenderer()->GetWindow().pollEvent(event))
		{
			engine->GetRenderer()->PollEventUI(event);

			if (event.type == sf::Event::Closed)
				engine->GetRenderer()->GetWindow().close();

			if (std::abs(event.mouseWheelScroll.delta) > 0.05f && std::abs(event.mouseWheelScroll.delta) < 50.f) {
				engine->GetEntityManager()->GetCamera()->AdjustZoom(-static_cast<float>(event.mouseWheelScroll.delta) / 10);
			}
		}

		//Input using keyboard for camera movement
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			engine->GetEntityManager()->GetCamera()->SetDirection(glm::vec2(200.0f, 0));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			engine->GetEntityManager()->GetCamera()->SetDirection(glm::vec2(-100.0f, 0));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			engine->GetEntityManager()->GetCamera()->SetDirection(glm::vec2(0, -100.0f));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			engine->GetEntityManager()->GetCamera()->SetDirection(glm::vec2(0, 200.0f));
		}


		Gamematch->GetGameObjectManager()->Update(engine->GetDeltaTime());
		engine->GetRenderer()->UpdateUI();

		std::string mousePos = "X: " + std::to_string(engine->GetEntityManager()->GetCamera()->GetPosition().x) + " Y: " + std::to_string(engine->GetEntityManager()->GetCamera()->GetPosition().y);
		std::string delta = std::to_string(engine->GetDeltaTime());

		ImGui::Begin("Enemy action");
		for (int i = 0; i < Gamematch->GetGameObjectManager()->GetEnemy()->GetAction().size(); i++)
		{
			ImGui::Text("My current action is: %s", Gamematch->GetGameObjectManager()->GetEnemy()->GetAction()[i].c_str());
		}
		
		ImGui::End();

		Playercontrols->Update(engine->GetDeltaTime());
		engine->Update(); //Also renders
	}
#endif
#elif __ORBIS__
	while (true)
	{
		engine->Update();
		engine->GetInput()->Update();

		glm::vec2 LeftStickInfo = glm::vec2(engine->GetInput()->GetAxis(PCInput::MouseX, EPS4Input::PS4LeftStickX), engine->GetInput()->GetAxis(PCInput::MouseY, EPS4Input::PS4LeftStickY));
		printf("LeftStick: %f - %f \n", LeftStickInfo.x, LeftStickInfo.y);

		if (engine->GetInput()->GetButton(PCInput::Space, EPS4Input::PS4Cross))
			printf("aii \n");
	}

#endif
}
