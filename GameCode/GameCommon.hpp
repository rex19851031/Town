#pragma once

#ifndef GAMECOMMON_HPP
#define GAMECOMMON_HPP

#include "GameCode\World.hpp"
#include "GameCode\CombatSystem.hpp"
#include "GameCode\UI.hpp"
#include "GameCode\NPC\NPC.hpp"
#include "GameCode\Item\Item.hpp"

#include "Engine\Renderer\BitmapFont.hpp"

#include <vector>

namespace Henry
{

class GameCommon
{
public:
	static World* s_world;

	static BitmapFont* s_drawingFont;

	static Vec2i s_cellSize;

	static Vec2i s_borderOffset;

	static Vec2i s_windowSize;

	static CombatSystem s_combatSystem;

	static UI* s_UI;

	static InputSystem* s_input;

	static std::vector<NPC*>* s_npcs;
	
	static std::vector<Feature*>* s_features;

	static std::vector<Item*>* s_items;

	static std::vector<NPC*>* s_newNPCs;
	
	static void PoorMansFOV(Agent* agent,int viewRadius,bool setMapVisibility,int accuracy);
};

};

#endif

