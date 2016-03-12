#include "TheGame.hpp"
#include "TheApplication.hpp"

#include "Engine\Renderer\OpenGLRenderer.hpp"
#include "Engine\Core\Time.hpp"
#include "Engine\Core\HenryFunctions.hpp"

#include "GameCode\Generators\CellularAutomataMapGenerator.hpp"
#include "GameCode\Generators\MapGeneratorRegistration.hpp"
#include "GameCode\NPC\NPCFactory.hpp"
#include "GameCode\Item\ItemFactory.hpp"
#include "GameCode\Features\FeatureFactory.hpp"
#include "GameCode\GameCommon.hpp"
#include "GameCode\PathFinding.hpp"
#include "GameCode\FactionSystem.hpp"

#include <fstream>
#include <sstream>


namespace Henry
{

extern TheApplication* g_theApplication;


TheGame::TheGame(void)
{
	m_isQuitting = false;
	m_gameState = STARTING_UP;
}


TheGame::~TheGame(void)
{
	std::vector<Item*>::iterator item_iter = m_items.begin();
	while(item_iter != m_items.end())
	{
		Item* item = *item_iter;
		item_iter = m_items.erase(item_iter);
		if(item)
		{
			delete item;
			item = nullptr;
		}
	}	

	std::vector<NPC*>::iterator npc_iter = GameCommon::s_npcs->begin();
	while(npc_iter != GameCommon::s_npcs->end())
	{
		NPC* npc = *npc_iter;
		npc_iter = GameCommon::s_npcs->erase(npc_iter);
		if(npc)
		{
			delete npc;
			npc = nullptr;
		}
	}

	std::vector<Feature*>::iterator feature_iter = GameCommon::s_features->begin();
	while(feature_iter != GameCommon::s_features->end())
	{
		Feature* feature = *feature_iter;
		feature_iter = GameCommon::s_features->erase(feature_iter);
		if(feature)
		{
			delete feature;
			feature = nullptr;
		}
	}

	FeatureFactorySet::iterator featureFactory_iter = FeatureFactory::s_featureFactorySet.begin();
	while(featureFactory_iter != FeatureFactory::s_featureFactorySet.end())
	{
		FeatureFactory* factory = *featureFactory_iter;
		featureFactory_iter = FeatureFactory::s_featureFactorySet.erase(featureFactory_iter);
		if(factory)
		{
			delete factory;
			factory = nullptr;
		}
	}

	ItemFactorySet::iterator itemFactory_iter = ItemFactory::s_itemFactorySet.begin();
	while(itemFactory_iter != ItemFactory::s_itemFactorySet.end())
	{
		ItemFactory* factory = *itemFactory_iter;
		itemFactory_iter = ItemFactory::s_itemFactorySet.erase(itemFactory_iter);
		if(factory)
		{
			delete factory;
			factory = nullptr;
		}
	}

	NPCFactorySet::iterator npcFactory_iter = NPCFactory::s_npcFactorySet.begin();
	while(npcFactory_iter != NPCFactory::s_npcFactorySet.end())
	{
		NPCFactory* factory = *npcFactory_iter;
		npcFactory_iter = NPCFactory::s_npcFactorySet.erase(npcFactory_iter);
		if(factory)
		{
			delete factory;
			factory = nullptr;
		}
	}

	if(m_font)
	{
		delete m_font;
		m_font = nullptr;
	}

	if(m_material)
	{
		delete m_material;
		m_material = nullptr;
	}

	if(m_world)
	{
		delete m_world;
		m_world = nullptr;
	}
}


void TheGame::Input()
{
	switch(m_gameState)
	{
	case SHOWING_MAIN_MENU:
		InputForMainMenu();
		break;
	case SHOWING_QUEST_MENU:
		InputForQuestMenu();
		break;
	case SHOWING_LOAD_MENU:
		InputForLoadMenu();
		break;
	case SHOWING_GAMEOVER_MENU:
		InputForGameOver();
		break;
	case GENERATING:
		InputForGenerating();
		break;
	case PLAYING:
		InputForPlaying();
		break;
	};
}


void TheGame::Render()
{
	SetUpPerspectiveProjection();
	ApplyCameraTransform( m_camera );

	OpenGLRenderer::BindFrameBuffer(m_fbo.ID);
	OpenGLRenderer::ClearScreen(0.0f,0.0f,0.0f);

	switch(m_gameState)
	{
	case SHOWING_MAIN_MENU:
		DrawMainMenu();
		break;
	case SHOWING_QUEST_MENU:
		DrawQuestMenu();
		break;
	case SHOWING_LOAD_MENU:
		DrawLoadMenu();
		break;
	case SHOWING_GAMEOVER_MENU:
		DrawGameOver();
		break;
	case GENERATING:
		DrawGenerating();
		break;
	case PLAYING:
		DrawPlaying();
		break;
	};

	OpenGLRenderer::BindFrameBuffer(0);
	OpenGLRenderer::ClearScreen(0.f, 0.f, 0.f);
	DrawFrameBuffer( m_fbo, m_material );
}


void TheGame::Update( float deltaSeconds )
{
	switch(m_gameState)
	{
	case STARTING_UP:
		Initialize();
		break;
	case LOADING:
		LoadDataFiles();
		break;
	case PLAYING:
		UpdatePlaying();
		break;
	case SHOWING_QUEST_MENU:
		UpdateQuestMenu();
		break;
	case QUITTING:
		m_isQuitting = true;
		break;
	case GENERATING:
		if(m_autoGenerateMap)
		{
			m_debug = false;
			m_world->AutoGenerate();
			m_world->GetMap()->SetDebugMode(m_debug);
			m_world->GetMap()->ResetDiscoveredFlag();
			SetGameState(PLAYING);
			UpdatePlaying();
		}
	case SHUTTING_DOWN:
		break;
	};

	UpdateCameraFromMouseAndKeyboard( m_camera, deltaSeconds );
}


void TheGame::RenderAxes()
{
	OpenGLRenderer::LineWidth(2.0f);
	float axisLength = 2.0f;
	float axisStart = 0.0f;
	Vertex_PosColor vertices[6];
	vertices[0].color = RGBA(1.0f,0.0f,0.0f,1.0f);
	vertices[0].position = Vec3f(axisStart,0.0f,0.0f);
	vertices[1].color = RGBA(1.0f,0.0f,0.0f,1.0f);
	vertices[1].position = Vec3f(axisLength,0.0f,0.0f);

	vertices[2].color = RGBA(0.0f,1.0f,0.0f,1.0f);
	vertices[2].position = Vec3f(0.0f,axisStart,0.0f);
	vertices[3].color = RGBA(0.0f,1.0f,0.0f,1.0f);
	vertices[3].position = Vec3f(0.0f,axisLength,0.0f);

	vertices[4].color = RGBA(0.0f,0.0f,1.0f,1.0f);
	vertices[4].position = Vec3f(0.0f,0.0f,axisStart);
	vertices[5].color = RGBA(0.0f,0.0f,1.0f,1.0f);
	vertices[5].position = Vec3f(0.0f,0.0f,axisLength);

	OpenGLRenderer::DrawVertexWithVertexArray(vertices,6,OpenGLRenderer::SHAPE_LINES);
}


void TheGame::DrawFrameBuffer( FBO& toDraw, Material* fboMaterial )
{
	fboMaterial->Activate();
	OpenGLRenderer::SetOrtho(0.0 , 1600 , 0.0 , 900 , 0 , 1 );
	OpenGLRenderer::SendBasicTwoFBOTextures(toDraw.depthTextureID, toDraw.colorTextureID);
	Vertex_PCT quad[4];
	quad[0].position = Vec3f( 0.0f										, 0.0f										, 0.0f );
	quad[1].position = Vec3f( (float)g_theApplication->m_windowSize.x	, 0.0f										, 0.0f );
	quad[2].position = Vec3f( (float)g_theApplication->m_windowSize.x	, (float)g_theApplication->m_windowSize.y	, 0.0f );
	quad[3].position = Vec3f( 0.0f										, (float)g_theApplication->m_windowSize.y	, 0.0f );

	quad[0].texCoords = Vec2f( 0.0f	, 0.0f );
	quad[1].texCoords = Vec2f( 1.0f	, 0.0f );
	quad[2].texCoords = Vec2f( 1.0f	, 1.0f );
	quad[3].texCoords = Vec2f( 0.0f	, 1.0f );

	OpenGLRenderer::DrawVertexWithVertexArray2D(quad,4,OpenGLRenderer::SHAPE_QUADS,(float)g_theApplication->m_windowSize.x,(float)g_theApplication->m_windowSize.y);
}


void TheGame::SetUpPerspectiveProjection()
{
	float aspect = (16.f / 9.f); // VIRTUAL_SCREEN_WIDTH / VIRTUAL_SCREEN_HEIGHT;
	float fovX = 70.f;
	float fovY = (fovX / aspect);
	float zNear = 0.1f;
	float zFar = 1000.f;

	OpenGLRenderer::LoadIdentity();
	OpenGLRenderer::Perspective( fovY, aspect, zNear, zFar ); // Note: Absent in OpenGL ES 2.0*/
}


void TheGame::ApplyCameraTransform( const Camera3D& camera )
{
	float matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);

	// Put us in our preferred coordinate system: +X is east (forward), +Y is north, +Z is up
	OpenGLRenderer::Rotatef( -90.f, 1.f, 0.f, 0.f ); // lean "forward" 90 degrees, to put +Z up (was +Y)
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
	OpenGLRenderer::Rotatef( 90.f, 0.f, 0.f, 1.f ); // spin "left" 90 degrees, to put +X forward (was +Y)
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
	// Orient the view per the camera's orientation
	OpenGLRenderer::Rotatef( -camera.m_orientation.rollDegreesAboutX,	1.f, 0.f, 0.f );
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
	OpenGLRenderer::Rotatef( -camera.m_orientation.pitchDegreesAboutY,	0.f, 1.f, 0.f );
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
	OpenGLRenderer::Rotatef( -camera.m_orientation.yawDegreesAboutZ,	0.f, 0.f, 1.f );
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
	// Position the view per the camera's position
	OpenGLRenderer::Translatef( -camera.m_position.x, -camera.m_position.y, -camera.m_position.z );
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
}


Vec2f TheGame::GetMouseMovementSinceLastChecked()
{
	POINT cursorPos;
	GetCursorPos( &cursorPos );

	if(GameCommon::s_input->isFocus)
		SetCursorPos( (int)m_centerCursorPos.x, (int)m_centerCursorPos.y );
	else
		return Vec2f(0.0f,0.0f);

	SetCursorPos( (int)m_centerCursorPos.x, (int)m_centerCursorPos.y );
	Vec2i mouseDeltaInts( (int)(cursorPos.x - m_centerCursorPos.x), (int)(cursorPos.y - m_centerCursorPos.y) );
	Vec2f mouseDeltas( (float) mouseDeltaInts.x, (float) mouseDeltaInts.y );
	return mouseDeltas;
}


void TheGame::UpdateCameraFromMouseAndKeyboard( Camera3D& camera, float deltaSeconds )
{
	// Update camera orientation (yaw and pitch only) from mouse x,y movement
	const float degreesPerMouseDelta = 0.04f;
	float moveSpeed = 1.0f;
	Vec2f mouseDeltas = GetMouseMovementSinceLastChecked();
	camera.m_orientation.yawDegreesAboutZ		-= (degreesPerMouseDelta * mouseDeltas.x);
	camera.m_orientation.pitchDegreesAboutY		+= (degreesPerMouseDelta * mouseDeltas.y);

	// Update camera position based on which (if any) movement keys are down
	float cameraYawRadians = degree2radians(camera.m_orientation.yawDegreesAboutZ);
	float cameraPitchRadians = degree2radians(camera.m_orientation.pitchDegreesAboutY);
	Vec3f cameraForwardVector = Vec3f( cos(cameraYawRadians) * cos(cameraPitchRadians) , sin(cameraYawRadians) * cos(cameraPitchRadians) , -sin(cameraPitchRadians));
	camera.m_forwardVector = cameraForwardVector;

	if(GameCommon::s_input->m_keyState[Inputs::INPUT_SHIFT].isHold)
		moveSpeed = 4.0f;

	Vec3f movementVector( 0.f, 0.f, 0.f );
	if( GameCommon::s_input->m_keyState[ 'W' ].isHold )
		movementVector += camera.m_forwardVector * moveSpeed;
	if( GameCommon::s_input->m_keyState[ 'S' ].isHold )
		movementVector -= camera.m_forwardVector * moveSpeed;
	if( GameCommon::s_input->m_keyState[ 'A' ].isHold )
		movementVector -= camera.m_forwardVector.crossProductWith(Vec3f(0,0,1)) * moveSpeed;
	if( GameCommon::s_input->m_keyState[ 'D' ].isHold )
		movementVector += camera.m_forwardVector.crossProductWith(Vec3f(0,0,1)) * moveSpeed;

	if( GameCommon::s_input->m_keyState[Inputs::INPUT_SPACE].isHold )
		movementVector = Vec3f(0.f, 0.f, 1.f);

	if( GameCommon::s_input->m_keyState[ 'Z' ].isHold )
		movementVector = Vec3f(0.f, 0.f, -1.f);
	//...and so on, for A,S,D moving left, back, right - and for E,C moving up, down.

	camera.m_position += (movementVector * deltaSeconds);
}


void TheGame::ToggleCursorDisplay(bool show)
{
	if(!show)
	{
		while(ShowCursor(false)>=0);
		SetCursorPos( (int)m_centerCursorPos.x, (int)m_centerCursorPos.y );
	}
	else
		ShowCursor(true);
}


GameState TheGame::GetGameState()
{
	return m_gameState;
}


void TheGame::SetGameState(GameState state)
{
	m_gameState = state;
}


void TheGame::Initialize()
{
	m_centerCursorPos = Vec2f(400,300);
	SetCursorPos(400,300);
	ShowCursor(false);
	m_isQuitting = false;
	m_timeOfLastUpdate = GetCurrentTimeSeconds();
	m_world = new World();
	GameCommon::s_world = m_world;
	MapGenerator::CreateAllGenerators("MapGenerator");
	MapGenerator::GetNameOfGenerators(&m_generatorNames);

	ShaderProgram sp(OpenGLRenderer::CreateProgram(),new OpenGLShader( "./Data/Shaders/fragment.glsl" , OpenGLRenderer::FRAGMENT_SHADER),new OpenGLShader( "./Data/Shaders/vertex.glsl" , OpenGLRenderer::VERTEX_SHADER));
	m_material = new Material(sp);

	OpenGLRenderer::InitializeFrameBuffer(m_fbo);
	
	m_autoGenerateMap = false;
	m_update = false;
	m_debug = false;
	m_save = false;
	m_hasSavedFile = std::ifstream("./Data/Saved.xml") ? true : false;

	GameEntity::s_nextGameEntityID = 1;
	m_player = nullptr;

	GameCommon::s_windowSize = g_theApplication->m_windowSize;
	GameCommon::s_input = g_theApplication->m_input;
	GameCommon::s_UI = new UI();
	GameCommon::s_npcs = &m_npcs;
	GameCommon::s_features = &m_features;
	GameCommon::s_items = &m_items;
	GameCommon::s_newNPCs = new std::vector<NPC*>();

	SetGameState(LOADING);
}


void TheGame::LoadDataFiles()
{
	m_font = new BitmapFont("./Data/BMPFonts/arial_normal.fnt");
	SetGameState(SHOWING_MAIN_MENU);

	NPCFactory::LoadAndParseAllFilesUnder("./Data/NPCs/");
	ItemFactory::LoadAndParseAllFilesUnder("./Data/Items/");
	FeatureFactory::LoadAndParseAllFilesUnder("./Data/Features/");

	FactionSystem::LoadDefaultFaction("./Data/Factions.xml");

	GameCommon::s_drawingFont = m_font;
}


void TheGame::DrawMainMenu()
{
	std::string mainMenu("Main Menu");
	std::string newGame("New Game");
	std::string quitGame("Quit Game");
	float yOffset = 0;
	if(m_hasSavedFile)
	{
		m_font->Draw( "Continue"	, Vec2f(700.0f,300.0f)	, 64.0f	, RGBA(1.0f,1.0f,1.0f,1.0f) , g_theApplication->m_windowSize );
		m_font->Draw( "C"			, Vec2f(700.0f,300.0f)	, 64.0f	, RGBA(1.0f,0.0f,0.0f,1.0f) , g_theApplication->m_windowSize );
		yOffset = 100;
	}

	m_font->Draw( "Main Menu"	, Vec2f(600.0f,600.0f)				, 100.0f	, RGBA()					, g_theApplication->m_windowSize );

	m_font->Draw( "New Game"	, Vec2f(700.0f,400.0f)				, 64.0f		, RGBA()					, g_theApplication->m_windowSize );
	m_font->Draw( "Quit Game"	, Vec2f(700.0f,300.0f - yOffset)	, 64.0f		, RGBA()					, g_theApplication->m_windowSize );

	m_font->Draw( "N"			, Vec2f(700.0f,400.0f)				, 64.0f		, RGBA(1.0f,0.0f,0.0f,1.0f)	, g_theApplication->m_windowSize );
	m_font->Draw( "Q"			, Vec2f(700.0f,300.0f - yOffset)	, 64.0f		, RGBA(1.0f,0.0f,0.0f,1.0f)	, g_theApplication->m_windowSize );
}


void TheGame::DrawQuestMenu()
{
	m_font->Draw("Quest Menu",Vec2f(550.0f,700.0f),100,RGBA(),g_theApplication->m_windowSize);

	for(size_t index = 0; index <= m_generatorNames.size(); index++)
	{
		char cIndex[3] = {(char)(index + '1'),'.','\0'};
		
		if(index < m_generatorNames.size())
		{
			char* name = (char*)m_generatorNames[index].c_str();

			m_font->Draw(cIndex,Vec2f(500.0f,500.0f - 100 * index),64.0f,RGBA(1.0f,0.0f,0.0f,1.0f),g_theApplication->m_windowSize);
			m_font->Draw(name,Vec2f(600.0f,500.0f - 100 * index),64.0f,RGBA(),g_theApplication->m_windowSize);
		}
		else
		{
			m_font->Draw(cIndex,Vec2f(500.0f,500.0f - 100 * index),64.0f,RGBA(1.0f,0.0f,0.0f,1.0f),g_theApplication->m_windowSize);
			m_font->Draw("Load From Data",Vec2f(600.0f,500.0f - 100 * index),64.0f,RGBA(),g_theApplication->m_windowSize);
		}
	}

	if(m_autoGenerateMap)
		m_font->Draw("Auto Generate",Vec2f(1250.0f,850.0f),48,RGBA(1.0f,0.5f,1.0f,1.0f),g_theApplication->m_windowSize);
	else
		m_font->Draw("Manual Generate",Vec2f(1250.0f,850.0f),48,RGBA(1.0f,0.5f,1.0f,1.0f),g_theApplication->m_windowSize);
}


void TheGame::DrawGenerating()
{
	OpenGLRenderer::BindWhiteTexture();
	OpenGLRenderer::SetOrtho(0.0 , (float)g_theApplication->m_windowSize.x , 0.0 , (float)g_theApplication->m_windowSize.y , 0 , 1 );
	m_material->Activate();
	m_world->Render();

	m_font->Draw("Re-Generate",Vec2f(200.0f,820.0f),64,RGBA(),g_theApplication->m_windowSize);
	m_font->Draw("R",Vec2f(200.0f,820.0f),64,RGBA(1.0f,0.0f,0.0f,1.0f),g_theApplication->m_windowSize);

	char num[10];
	itoa(m_world->m_steps,num,10);
	m_font->Draw("Steps:",Vec2f(1000.0f,820.0f),64,RGBA(),g_theApplication->m_windowSize);
	m_font->Draw(num,Vec2f(1200.0f,820.0f),64,RGBA(1.0f,0.0f,0.0f,1.0f),g_theApplication->m_windowSize);

	m_font->Draw("Space for Next Step",Vec2f(200.0f,20.0f),64,RGBA(),g_theApplication->m_windowSize);
	m_font->Draw("Esc to Quit",Vec2f(1000.0f,20.0f),64,RGBA(),g_theApplication->m_windowSize);

	m_font->Draw("Space",Vec2f(200.0f,20.0f),64,RGBA(1.0f,0.0f,0.0f,1.0f),g_theApplication->m_windowSize);
	m_font->Draw("Esc",Vec2f(1000.0f,20.0f),64,RGBA(1.0f,0.0f,0.0f,1.0f),g_theApplication->m_windowSize);
}


void TheGame::DrawPlaying()
{
	OpenGLRenderer::BindWhiteTexture();
	OpenGLRenderer::SetOrtho(0.0 , (float)g_theApplication->m_windowSize.x , 0.0 , (float)g_theApplication->m_windowSize.y , 0 , 1 );
	m_material->Activate();
	m_world->Render();
	
	for(size_t index = 0; index < m_npcs.size(); index++)
	{
		if(m_npcs[index]->m_map->GetVisibility(m_npcs[index]->m_position) || m_debug)
			m_npcs[index]->Render();
	}

	for(size_t index = 0; index < m_items.size(); index++)
	{
		if( (m_items[index]->m_map->GetVisibility(m_items[index]->m_position) || m_debug) && !m_items[index]->m_beenPicked )
			m_items[index]->Render();
	}

	for(size_t index = 0; index < m_features.size(); index++)
	{
		if(m_features[index]->m_map->GetVisibility(m_features[index]->m_position) || m_debug)
			m_features[index]->Render();
	}

	m_player->Render();
	GameCommon::s_UI->Draw();
}


void TheGame::DrawLoadMenu()
{
	m_xmlHelper.LoadFile("./Data/MapDirectory.xml");

	m_font->Draw("Loading Menu",Vec2f(550.0f,700.0f),100,RGBA(),g_theApplication->m_windowSize);
	Vec2f position(600,500);
	int index = 0;

	for(TiXmlElement* elem = m_xmlHelper.m_root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		std::string elemName = elem->Value();
		if(elemName == "map")
		{
			m_mapFileList.push_back(elem->Attribute("file"));

			char cIndex[3] = {(char)(index + '1'),'.','\0'};
			m_font->Draw(cIndex,position - Vec2f(100,0),64,RGBA(1.0f,0.0f,0.0f,1.0f),g_theApplication->m_windowSize);
			m_font->Draw(elem->Attribute("file"),position,64,RGBA(),g_theApplication->m_windowSize);

			index++;
			position -= Vec2f(0,100);
		}
	}
}

void TheGame::InputForMainMenu()
{
	if(GameCommon::s_input->PressedOnce(Inputs::INPUT_ESCAPE))
		SetGameState(QUITTING);

	if(GameCommon::s_input->PressedOnce('Q') )
		SetGameState(QUITTING);

	if(GameCommon::s_input->PressedOnce('N') )
		SetGameState(SHOWING_QUEST_MENU);

	if(GameCommon::s_input->PressedOnce('C') && m_hasSavedFile )
	{
		LoadGameFromXml();
	}
}


void TheGame::InputForQuestMenu()
{
	unsigned char asKey = 0;
	bool selectGenerator = false;

	if(GameCommon::s_input->m_chars.size() != 0)
		asKey = GameCommon::s_input->m_chars[0];
	
	if(asKey > '0' && asKey <= '0' + m_generatorNames.size() )
	{
		int selectedIndex = (asKey - '1');
		m_world->SetGenerator(MapGenerator::GetGenerator(m_generatorNames[selectedIndex]));
		m_world->GenerateMap();
		selectGenerator = true;
	}

	if(asKey == '1' + m_generatorNames.size())
		SetGameState(SHOWING_LOAD_MENU);
		
	if(asKey == Inputs::INPUT_ESCAPE)
		SetGameState(SHOWING_MAIN_MENU);

	if(asKey == 'A')
		m_autoGenerateMap = !m_autoGenerateMap;

	if(selectGenerator)
		SetGameState(GENERATING);
}


void TheGame::InputForPlaying()
{
	if(GameCommon::s_input->PressedOnce(Inputs::INPUT_ESCAPE))
	{
		for(int index = 0; index < m_world->GetMap()->GetMapSize(); index++)
			m_world->GetMap()->SetAgent(index,nullptr);

		m_world->GetMap()->SetDebugMode(true);
		std::vector<NPC*> empty;
		m_npcs = empty;

		SetGameState(SHOWING_QUEST_MENU);
	}

	if(GameCommon::s_input->PressedOnce(Inputs::INPUT_F6) && !m_save)
	{
		SaveGameToXml("./Data/Saved.xml");
		GameCommon::s_UI->PostMessageToLog("You Are Not Supposed To See This Line!");
		GameCommon::s_UI->PostMessageToLog("Game Saved! Good Bye!!");
		GameCommon::s_UI->Update();
		m_save = true;
	}

	if(GameCommon::s_input->PressedOnce(Inputs::INPUT_TAB) || GameCommon::s_input->PressedOnce('M'))
	{
		m_debug = !m_debug;
		m_world->GetMap()->SetDebugMode(m_debug);
	}
	
	if(!m_save)
	{
		m_player->Input();
		m_update = m_player->m_doAnything;
	}

	if(GameCommon::s_input->PressedOnce(Inputs::INPUT_SPACE) && m_save)
	{
		GameCommon::s_UI->Update();
		SetGameState(SHOWING_MAIN_MENU);
		m_save = false;
	}
}


void TheGame::InputForGenerating()
{
	if(GameCommon::s_input->PressedOnce(Inputs::INPUT_SPACE))
		m_world->ProcessOneStep();

	if(GameCommon::s_input->PressedOnce(Inputs::INPUT_RETURN))
	{
		m_debug = false;
		m_world->AutoGenerate();
		m_world->GetMap()->SetDebugMode(m_debug);
		m_world->GetMap()->ResetDiscoveredFlag();
		SetGameState(PLAYING);
	}

	if(GameCommon::s_input->PressedOnce(Inputs::INPUT_ESCAPE))
		SetGameState(SHOWING_QUEST_MENU);

	if(GameCommon::s_input->PressedOnce('R'))
		m_world->GenerateMap();
}


void TheGame::InputForLoadMenu()
{
	unsigned char asKey = 0;
	if(GameCommon::s_input->m_chars.size() != 0)
		asKey = GameCommon::s_input->m_chars[0];
	
	if(asKey > '0' && asKey <= '0' + m_mapFileList.size() )
	{
		int selectedIndex = (asKey - '1');
		LoadMap(m_mapFileList[selectedIndex].c_str());
		SetGameState(PLAYING);
	}

	if(GameCommon::s_input->PressedOnce(Inputs::INPUT_ESCAPE))
		SetGameState(SHOWING_QUEST_MENU);
}


void TheGame::LoadMap(const char* filePath)
{
	m_xmlHelper.LoadFile(filePath);

	std::string text = m_xmlHelper.m_root->GetText();
	int mapSize = m_world->GetMap()->m_mapSize.x * m_world->GetMap()->m_mapSize.y;
	int mapWidth = m_world->GetMap()->m_mapSize.x;
	int mapIndex = mapSize - mapWidth;
	for(size_t index = 0; index < text.size(); index++)
	{
		if(text[index] != ' ')
		{
			if(text[index] == '0')
				m_world->GetMap()->SetCellType(mapIndex,AIR);
			if(text[index] == '1')
				m_world->GetMap()->SetCellType(mapIndex,WALL);

			mapIndex++;
		}
		else
		{
			mapIndex -= 2 * mapWidth;
		}
	}
}


void TheGame::UpdatePlaying()	
{
	if(!m_player)
	{
		UpdateFeatureList();

		m_startTime = GetCurrentTimeSeconds();
		std::string playerName = "newbie";
		m_player = new Player(playerName,m_world->GetMap());
		
		int loopingTime = 0;
		while(loopingTime < 10000)
		{
			Vec2i location;
			location.x = (int)(getRandomPercent() * m_world->GetMap()->m_mapSize.x);
			location.y = (int)(getRandomPercent() * m_world->GetMap()->m_mapSize.y);
			
			if(m_player->CanMoveTo(location))
			{
				m_player->MoveTo(location);
				break;
			}

			loopingTime++;
		}

		loopingTime = 0;
		size_t numOfNPC = NPCFactory::s_npcFactorySet.size();
		int numOfEachType = 1;
		int currentNumOfEachType = 0;
		NPCFactorySet::iterator npc_iter = NPCFactory::s_npcFactorySet.begin();

		while(loopingTime < 10000 && numOfNPC > 0)
		{
			Vec2i location;
			location.x = (int)(getRandomPercent() * m_world->GetMap()->m_mapSize.x);
			location.y = (int)(getRandomPercent() * m_world->GetMap()->m_mapSize.y);

			NPCFactory* factory = *npc_iter;

			NPC* npc = factory->SpawnNPC(m_world->GetMap(),location);
			if(npc != nullptr)
			{
				m_npcs.push_back(npc);
				currentNumOfEachType++;
				if(currentNumOfEachType == numOfEachType)
				{
					numOfNPC--;
					npc_iter++;
					currentNumOfEachType = 0;
				}

				if(!npc->m_random)
				{
					npc->m_isDead = true;
				}
			}

			loopingTime++;
		}

		loopingTime = 0;
		size_t numOfItem = ItemFactory::s_itemFactorySet.size();
		ItemFactorySet::iterator item_iter = ItemFactory::s_itemFactorySet.begin();
		while(loopingTime < 10000 && numOfItem > 0)
		{
			Vec2i location;
			location.x = (int)(getRandomPercent() * m_world->GetMap()->m_mapSize.x);
			location.y = (int)(getRandomPercent() * m_world->GetMap()->m_mapSize.y);

			ItemFactory* factory = *item_iter;
			Item* item = factory->SpawnItem(m_world->GetMap(),location);
			if(item)
			{
				m_items.push_back(item);
				numOfItem--;
				item_iter++;

				if(!item->m_random)
				{
 					item->m_isBroke = true;
				}
			}

			loopingTime++;
		}

		UpdateNPCList(false);
		UpdateItemList();
		m_player->Update();
		GameCommon::s_UI->SetDisplayAgent(m_player);
	}
	
	if(m_update)
	{
		m_player->Update();

		UpdateNPCList();
		UpdateItemList();
		UpdateFeatureList();
		
		GameCommon::s_UI->Update();
	}

	if(m_player->m_isDead)
	{
		m_endTime = GetCurrentTimeSeconds();
		SetGameState(SHOWING_GAMEOVER_MENU);
	}
}


void TheGame::UpdateQuestMenu()
{
	CleanAllData();	
}


void TheGame::DrawGameOver()
{
	m_font->Draw("Game Over",Vec2f(600.0f,700.0f),100,RGBA(),g_theApplication->m_windowSize);

	m_font->Draw("Restart"	,Vec2f(700.0f,600.0f),64,RGBA(),g_theApplication->m_windowSize);
	m_font->Draw("Quit Game",Vec2f(700.0f,500.0f),64,RGBA(),g_theApplication->m_windowSize);

	m_font->Draw("R",Vec2f(700.0f,600.0f),64,RGBA(1.0f,0.0f,0.0f,1.0f),g_theApplication->m_windowSize);
	m_font->Draw("Q",Vec2f(700.0f,500.0f),64,RGBA(1.0f,0.0f,0.0f,1.0f),g_theApplication->m_windowSize);

	std::ostringstream totalSteps;
	std::ostringstream totalTime;
	std::ostringstream avgTime;

	std::ostringstream totalMonsterKilled;
	std::ostringstream totalCellsExplored;
	std::ostringstream totalItemsPickedUp;

	totalSteps << "Total Steps : " << m_player->m_totalTurns;
	totalTime << "Total Time : " << (m_endTime - m_startTime);
	avgTime << "Average Time Per Step : " << (m_endTime - m_startTime)/(float)m_player->m_totalTurns;

	totalMonsterKilled << "Number of Monster Killed : "	 << m_player->m_numOfMonsterKilled;
	totalCellsExplored << "Number Of Cells Explored : "	 << m_world->GetNumOfExploredCells();
	totalItemsPickedUp << "Number Of Items Picked Up : " << m_player->m_numOfItemPicked;

	m_font->Draw( totalSteps.str()				, Vec2f(200.0f,300.0f),40,RGBA(0.8f,0.2f,0.2f,1.0f),g_theApplication->m_windowSize);
	m_font->Draw( totalTime.str()				, Vec2f(200.0f,200.0f),40,RGBA(0.8f,0.2f,0.2f,1.0f),g_theApplication->m_windowSize);
	m_font->Draw( avgTime.str().substr(0,28)    , Vec2f(200.0f,100.0f),40,RGBA(0.8f,0.2f,0.2f,1.0f),g_theApplication->m_windowSize);
	
	m_font->Draw( totalMonsterKilled.str() , Vec2f(900.0f,300.0f),40,RGBA(0.8f,0.2f,0.2f,1.0f),g_theApplication->m_windowSize);
	m_font->Draw( totalCellsExplored.str() , Vec2f(900.0f,200.0f),40,RGBA(0.8f,0.2f,0.2f,1.0f),g_theApplication->m_windowSize);
	m_font->Draw( totalItemsPickedUp.str() , Vec2f(900.0f,100.0f),40,RGBA(0.8f,0.2f,0.2f,1.0f),g_theApplication->m_windowSize);
}


void TheGame::InputForGameOver()
{
	if(GameCommon::s_input->m_keyState[Inputs::INPUT_ESCAPE].isPressedFrame)
		SetGameState(SHOWING_QUEST_MENU);

	if(GameCommon::s_input->m_keyState['Q'].isPressedFrame )
		SetGameState(QUITTING);

	if(GameCommon::s_input->m_keyState['R'].isPressedFrame )
		SetGameState(SHOWING_QUEST_MENU);
}


void TheGame::SaveGameToXml(const char* filepath)
{
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );

	TiXmlElement * element = new TiXmlElement("MapData");
	m_world->SaveToXML(element);
	doc.LinkEndChild(element);

	element = new TiXmlElement("NPCData");
	doc.LinkEndChild(element);
	std::vector<NPC*>::iterator npc_iter = m_npcs.begin();
	while(npc_iter != m_npcs.end())
	{
		NPC* npc = *npc_iter;
		npc->SaveToXML(element);
		npc_iter++;
	}

	element = new TiXmlElement("FeatureData");
	doc.LinkEndChild(element);
	std::vector<Feature*>::iterator feature_iter = m_features.begin();
	while(feature_iter != m_features.end())
	{
		Feature* feature = *feature_iter;
		feature->SaveToXML(element);
		feature_iter++;
	}

	element = new TiXmlElement("ItemData");
	doc.LinkEndChild(element);
	std::vector<Item*>::iterator item_iter = m_items.begin();
	while(item_iter != m_items.end())
	{
		Item* item = *item_iter;
		item->SaveToXML(element);
		item_iter++;
	}

	element = new TiXmlElement("PlayerData");
	doc.LinkEndChild(element);
	m_player->SaveToXML(element);
	
	element = new TiXmlElement("PlayTime");
	int totalTime = (int)(GetCurrentTimeSeconds() - m_startTime);
	element->SetAttribute("Seconds" , totalTime);
	doc.LinkEndChild(element);

	doc.SaveFile( filepath );
}


void TheGame::LoadGameFromXml()
{
	CleanAllData();

	m_debug = false;

	m_world->InitializeMap();
	m_world->GetMap()->SetDebugMode(m_debug);
	m_world->GetMap()->ResetDiscoveredFlag();
	m_world->GetMap()->ResetAllToDark();

	Map* map = m_world->GetMap();
	TinyXMLHelper xmlHelper;
	xmlHelper.LoadFile("./Data/Saved.xml");

	int mapSize = map->m_mapSize.x * map->m_mapSize.y;
	int mapWidth = map->m_mapSize.x;
	int mapIndex = mapSize - mapWidth;
	int visibleIndex = mapSize - mapWidth;
	TiXmlElement* currentElem = xmlHelper.m_root;
	std::string catagoryName = currentElem->Value();
	if(catagoryName == "MapData")
	{
		for(TiXmlElement* elem = currentElem->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			std::string elemName = elem->Value();
			if(elemName == "MapSlice")
			{
				std::string text = elem->GetText();
				for(size_t index=0; index < text.size(); index++)
				{
					std::ostringstream temp;
					temp << text[index];
					CellType type = (CellType)atoi(temp.str().c_str());
					map->SetCellType(mapIndex,type);
					mapIndex++;
				}
				mapIndex -= 2 * mapWidth;
			}

			if(elemName == "Visibility")
			{
				std::string text = elem->GetText();
				for(size_t index=0; index < text.size(); index++)
				{
					bool discovered = text[index] == '1' ? true : false;
					map->SetDiscovered(visibleIndex,discovered);
					visibleIndex++;
				}
				visibleIndex -= 2 * mapWidth;
			}
		}
	}

 	currentElem = currentElem->NextSiblingElement();
	catagoryName = currentElem->Value();
 	if(catagoryName == "NPCData")
 	{
 		for(TiXmlElement* elem = currentElem->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
 			std::string elemName = elem->Value();
 			if(elemName == "NPC")
 			{
 				std::string name = elem->Attribute("name");
 				std::string location = elem->Attribute("location");
				std::string faction = elem->Attribute("faction");
				std::string prev_id = elem->Attribute("id");
 
				NPCFactory* factory = NPCFactory::FindFactory(faction);
 				if(factory)
 				{
 					Vec2i position;
 					sscanf_s(location.c_str(),"%d,%d",&position.x,&position.y);
 					NPC* npc = factory->SpawnNPC(map,position);
					if(npc)
					{
						if(elem->Attribute("health"))
						{
							std::string health = elem->Attribute("health");
	 						npc->m_currentHP = (float)atof(health.c_str());
						}

						npc->m_prev_id = atoi(prev_id.c_str());
 
 						for(TiXmlElement* sub_elem = elem->FirstChildElement(); sub_elem != NULL; sub_elem = sub_elem->NextSiblingElement())
 						{
 							std::string sub_elemName = sub_elem->Value();
 							if(sub_elemName == "Factions")
 							{
 								for(TiXmlElement* sub_sub_elem = sub_elem->FirstChildElement(); sub_sub_elem != NULL; sub_sub_elem = sub_sub_elem->NextSiblingElement())
 								{
 									std::string sub_sub_elemName = sub_sub_elem->Value();
 									if(sub_sub_elemName == "Faction" )
 									{
 										std::string faction_name = sub_sub_elem->Attribute("name");
 										std::string faction_value = sub_sub_elem->Attribute("value");
 										npc->m_factionRelationship->SetRelationShip(faction_name,atoi(faction_value.c_str()));
 									}
 								}
 							}
 						}
 						m_npcs.push_back(npc);
					}
 				}
 			}
 		}
 	}

	currentElem = currentElem->NextSiblingElement();
	catagoryName = currentElem->Value();
	if(catagoryName == "FeatureData")
	{
		for(TiXmlElement* elem = currentElem->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			std::string elemName = elem->Value();
			if(elemName == "Feature")
			{
				std::string name = elem->Attribute("name");
				std::string location = elem->Attribute("location");
				std::string prev_id = elem->Attribute("id");
				std::string health = elem->Attribute("health");
				std::string status = elem->Attribute("status");

				FeatureFactory* factory = FeatureFactory::FindFactory(name);
				if(factory)
				{
					Vec2i position;
					sscanf_s(location.c_str(),"%d,%d",&position.x,&position.y);

					Feature* feature= factory->SpawnFeature(map,position);
					feature->m_prev_id = atoi(prev_id.c_str());
					feature->m_isActivate = status == "On" ? true : false;
					feature->m_isWalkable = status == "On" ? true : false;
					feature->m_currentHP = (float)atof(health.c_str());
					m_features.push_back(feature);
				}
			}
		}
	}

	currentElem = currentElem->NextSiblingElement();
	catagoryName = currentElem->Value();
	if(catagoryName == "ItemData")
	{
		for(TiXmlElement* elem = currentElem->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			std::string elemName = elem->Value();
			if(elemName == "Item")
			{
				std::string name = elem->Attribute("name");
				std::string location = elem->Attribute("location");
				std::string prev_id = elem->Attribute("id");

				ItemFactory* factory = ItemFactory::FindFactory(name);
				if(factory)
				{
					Vec2i position;
					sscanf_s(location.c_str(),"%d,%d",&position.x,&position.y);

					Item* item = factory->SpawnItem(map,position);
					item->m_prev_id = atoi(prev_id.c_str());
					m_items.push_back(item);
				}
			}
		}
	}

	currentElem = currentElem->NextSiblingElement();
	catagoryName = currentElem->Value();
	if(catagoryName == "PlayerData")
	{
		std::string totalTurns = currentElem->Attribute("totalTurns");
		std::string monsterKilled = currentElem->Attribute("monsterKilled");
		std::string itemPicked = currentElem->Attribute("itemPicked");
		std::string name = currentElem->Attribute("name");
		std::string id = currentElem->Attribute("id");
		std::string location = currentElem->Attribute("location");
		std::string faction = currentElem->Attribute("faction");
		std::string equipment = currentElem->Attribute("equipment");
		std::string durability = currentElem->Attribute("durability");

		Vec2i position;
		sscanf_s(location.c_str(),"%d,%d",&position.x,&position.y);

		m_player = new Player(name,map);
		m_player->m_prev_id = atoi(id.c_str());
		m_player->m_totalTurns = atoi(totalTurns.c_str());
		m_player->m_numOfMonsterKilled = atoi(monsterKilled.c_str());
		m_player->m_numOfItemPicked = atoi(itemPicked.c_str());
		m_player->MoveTo(position);
		m_player->m_faction = faction;

		if(currentElem->Attribute("health"))
			m_player->m_currentHP = (float)atof( currentElem->Attribute("health") );

		int equip_id[7];
		int equip_durability[7];
		sscanf_s(equipment.c_str(),"%d,%d,%d,%d,%d,%d,%d",&equip_id[0],&equip_id[1],&equip_id[2],&equip_id[3],&equip_id[4],&equip_id[5],&equip_id[6]);
		sscanf_s(durability.c_str(),"%d,%d,%d,%d,%d,%d,%d",&equip_durability[0],&equip_durability[1],&equip_durability[2],&equip_durability[3],&equip_durability[4],&equip_durability[5],&equip_durability[6]);

		for(int index=0; index < NUM_OF_SLOTS; ++index)
		{
			if(equip_id[index] != -1)
			{
				std::vector<Item*>::iterator item_iter = m_items.begin();
				
				while(item_iter != m_items.end())
				{
					Item* currentItem = *item_iter;
					if(currentItem->m_prev_id == equip_id[index])
					{
						m_player->m_equipmentSlot[index] = currentItem;
						currentItem->m_beenPicked = true;
						currentItem->m_durability = equip_durability[index];
						currentItem->m_owner = m_player;
						break;
					}
					item_iter++;
				}
			}
		}


		for(TiXmlElement* elem = currentElem->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			std::string elemName = elem->Value();
			if(elemName == "Potions")
			{
				for(TiXmlElement* sub_elem = elem->FirstChildElement(); sub_elem != NULL; sub_elem = sub_elem->NextSiblingElement())
				{
					std::string sub_elemName = sub_elem->Value();
					if(sub_elemName == "Potion")
					{
						std::string name = sub_elem->Attribute("name");
						std::string id = sub_elem->Attribute("id");
						int potion_id = atoi(id.c_str());


						std::vector<Item*>::iterator item_iter = m_items.begin();
						while(item_iter != m_items.end())
						{
							Item* currentItem = *item_iter;
							if(currentItem->m_prev_id == potion_id)
							{
								m_player->m_potionInventory.push_back(currentItem);
								currentItem->m_beenPicked = true;
								currentItem->m_owner = m_player;
								break;
							}
							item_iter++;
						}
					}
				}
			}
		}

		for(TiXmlElement* elem = currentElem->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			std::string elemName = elem->Value();
			if(elemName == "Wands")
			{
				for(TiXmlElement* sub_elem = elem->FirstChildElement(); sub_elem != NULL; sub_elem = sub_elem->NextSiblingElement())
				{
					std::string sub_elemName = sub_elem->Value();
					if(sub_elemName == "Wand")
					{
						std::string name = sub_elem->Attribute("name");
						std::string id = sub_elem->Attribute("id");
						int potion_id = atoi(id.c_str());


						std::vector<Item*>::iterator item_iter = m_items.begin();
						while(item_iter != m_items.end())
						{
							Item* currentItem = *item_iter;
							if(currentItem->m_prev_id == potion_id)
							{
								m_player->m_wandInventory.push_back(currentItem);
								currentItem->m_beenPicked = true;
								currentItem->m_owner = m_player;
								break;
							}
							item_iter++;
						}
					}
				}
			}
		}
	}

	currentElem = currentElem->NextSiblingElement();
	catagoryName = currentElem->Value();
	if(catagoryName == "PlayTime")
	{
		std::string playTime = currentElem->Attribute("Seconds");
		m_startTime -= atof(playTime.c_str());
	}

	UpdateFeatureList();
	m_player->Update();
	if(!GameCommon::s_UI)
		GameCommon::s_UI = new UI();
	GameCommon::s_UI->SetDisplayAgent(m_player);
	map->ResetBeenSetToVisibleFlag();
	SetGameState(PLAYING);
}


void TheGame::CleanAllData()
{
	if(m_player)
	{
		delete m_player;
		m_player = nullptr;
	}

	std::vector<NPC*> empty_npc;
	std::vector<Item*> empty_item;
	std::vector<Feature*> empty_feature;

	for(size_t index = 0; index < m_npcs.size(); index++)
		delete m_npcs[index];

	for(size_t index = 0; index < m_items.size(); index++)
		delete m_items[index];

	for(size_t index = 0; index < m_features.size(); index++)
		delete m_features[index];

	m_npcs = empty_npc;
	m_items = empty_item;
	m_features = empty_feature;
}


void TheGame::UpdateFeatureList()
{
	std::vector<Feature*>::iterator feature_iter = m_features.begin();
	while(feature_iter != m_features.end())
	{
		Feature* feature = *feature_iter;
		if(feature->m_isBroke)
		{
			feature_iter = m_features.erase(feature_iter);
			delete feature;
		}
		else
		{
			feature->Update();
			feature_iter++;
		}
	}
}


void TheGame::UpdateItemList()
{
	std::vector<Item*>::iterator item_iter = m_items.begin();
	while(item_iter != m_items.end())
	{
		Item* item = *item_iter;
		if(item->m_isBroke)
		{
			item_iter = m_items.erase(item_iter);
			delete item;
		}
		else
			item_iter++;
	}
}


void TheGame::UpdateNPCList(bool updateMove)
{
	std::vector<NPC*>::iterator npc_iter = m_npcs.begin();

	while(npc_iter != m_npcs.end())
	{
		NPC* theNpc = *npc_iter;
		if(theNpc->m_isDead)
		{
			theNpc->Destory();
			npc_iter = m_npcs.erase(npc_iter);
		}
		else
		{
			if(updateMove)
				theNpc->Update();
			npc_iter++;
		}
	}

	if(GameCommon::s_newNPCs->size() != 0)
	{
		std::vector<NPC*>::iterator npc_iter = GameCommon::s_newNPCs->begin();

		while(npc_iter != GameCommon::s_newNPCs->end())
		{
			NPC* theNpc = *npc_iter;
			npc_iter = GameCommon::s_newNPCs->erase(npc_iter);
			m_npcs.push_back(theNpc);
		}
	}
}


};