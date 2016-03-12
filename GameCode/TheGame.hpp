#pragma once

#ifndef THEGAME_HPP
#define THEGAME_HPP

#include <stdio.h>
#include <Windows.h>
#include <Xinput.h>					// include the Xinput API
#pragma comment( lib, "xinput" )	// Link in the xinput.lib static library

#include "Engine\Core\Camera3D.hpp"
#include "Engine\Renderer\Material.hpp"
#include "Engine\Math\GeneralStruct.hpp"
#include "Engine\Input\InputSystem.hpp"
#include "Engine\Parsing\TinyXML\TinyXMLHelper.hpp"

#include "GameCode\Generators\MapGenerator.hpp"
#include "GameCode\Generators\MapGeneratorRegistration.hpp"
#include "GameCode\World.hpp"
#include "GameCode\Player.hpp"
#include "GameCode\NPC\NPC.hpp"
#include "GameCode\Features\Feature.hpp"

namespace Henry
{

enum GameState{STARTING_UP = 0,LOADING,PLAYING,SHOWING_MAIN_MENU,QUITTING,SHOWING_QUEST_MENU,SHUTTING_DOWN,GENERATING,SHOWING_LOAD_MENU,SHOWING_GAMEOVER_MENU};


class TheGame
{
public:
	TheGame(void);
	~TheGame(void);
	void Input();
	void Update( float deltaSeconds );
	void Render();
	GameState GetGameState();
	void SetGameState(GameState state);
	
	bool m_isQuitting;

private:
	void Initialize();
	void LoadDataFiles();
	void SetUpPerspectiveProjection();
	void ApplyCameraTransform( const Camera3D& camera );
	void DrawFrameBuffer( FBO& toDraw, Material* fboMaterial );
	void RenderAxes();
	void UpdateCameraFromMouseAndKeyboard( Camera3D& camera, float deltaSeconds );
	void PollKeyStates();
	void ToggleCursorDisplay(bool show);
	void LoadMap(const char* filePath);
	void SaveGameToXml(const char* filepath);
	void LoadGameFromXml();
	void CleanAllData();

	void UpdatePlaying();
	void UpdateQuestMenu();

	void DrawMainMenu();
	void DrawQuestMenu();
	void DrawGenerating();
	void DrawLoadMenu();
	void DrawPlaying();
	void DrawGameOver();

	void InputForMainMenu();
	void InputForQuestMenu();
	void InputForLoadMenu();
	void InputForPlaying();
	void InputForGenerating();
	void InputForGameOver();

	void UpdateFeatureList();
	void UpdateItemList();
	void UpdateNPCList(bool updateMove = true);
	Vec2f GetMouseMovementSinceLastChecked();

	Material* m_material;
	Camera3D m_camera;
	FBO m_fbo;
	Vec2f m_centerCursorPos;
	double m_timeOfLastUpdate;
	World* m_world;
	GameState m_gameState;
	BitmapFont* m_font;
	std::vector<std::string> m_generatorNames;
	std::vector<std::string> m_mapFileList;
	bool m_autoGenerateMap;
	bool m_update;
	bool m_debug;
	bool m_save;
	bool m_hasSavedFile;
	Player* m_player;
	TinyXMLHelper m_xmlHelper;
	std::vector<NPC*> m_npcs;
	std::vector<Item*> m_items;
	std::vector<Feature*> m_features;
	double m_startTime;
	double m_endTime;
};

};

#endif