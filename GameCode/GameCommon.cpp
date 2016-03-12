#include "GameCode\GameCommon.hpp"

#include "Engine\Core\HenryFunctions.hpp"

#include <algorithm>

namespace Henry
{

World* GameCommon::s_world;

BitmapFont* GameCommon::s_drawingFont;

Vec2i GameCommon::s_cellSize;

Vec2i GameCommon::s_borderOffset;

Vec2i GameCommon::s_windowSize;

CombatSystem GameCommon::s_combatSystem;

UI* GameCommon::s_UI;

InputSystem* GameCommon::s_input;

std::vector<NPC*>* GameCommon::s_npcs;

std::vector<Item*>* GameCommon::s_items;

std::vector<Feature*>* GameCommon::s_features;

std::vector<NPC*>* GameCommon::s_newNPCs;


void GameCommon::PoorMansFOV(Agent* agent,int viewRadius,bool setMapVisibility,int accuracy)
{
	float checkStride = 1.0f / (float)accuracy;
	Map* theMap = agent->m_map;
	std::vector<Agent*> empty;
	agent->m_agentsInSight = empty;
	
	if(setMapVisibility)
		theMap->ResetAllToDark();

	for(float degree=0; degree<360; degree += 0.5f)
	{
		float radians = degree2radians(degree);
		Vec2f ray_direction = Vec2f(sin(radians),cos(radians));
		Vec2f startPoint = Vec2f( (float)agent->m_position.x , (float)agent->m_position.y);
		Vec2f destination = Vec2f(agent->m_position.x + ray_direction.x * viewRadius,agent->m_position.y + ray_direction.y * viewRadius);
		Vec2f startToDest = destination - startPoint;
		Vec2f stride = startToDest * checkStride;
		Vec2f currentCheckPoint = startPoint;
		bool visibility = true;
		for(int step=0; step < accuracy; step++)
		{
			Vec2i currentCheckPoint_i = Vec2i((int)currentCheckPoint.x,(int)currentCheckPoint.y);
			
			if(setMapVisibility)
			{
				theMap->SetVisibility(currentCheckPoint_i.x,currentCheckPoint_i.y,visibility);
			}

			bool featureBlock = false;
			if(theMap->GetFeature(currentCheckPoint_i))
			{
				if(theMap->GetFeature(currentCheckPoint_i)->m_isWalkable)
					featureBlock = true;
			}
			else
				featureBlock = true;

			CellType type = theMap->GetCellType(currentCheckPoint_i.x,currentCheckPoint_i.y);
			if( (type == AIR || type == WATER || type == LAVA || type == ICE) && featureBlock)
			{
				Agent* otherAgent = theMap->GetAgent(currentCheckPoint_i.x,currentCheckPoint_i.y);
				
				if(visibility && otherAgent && std::find(agent->m_agentsInSight.begin(), agent->m_agentsInSight.end(), otherAgent) == agent->m_agentsInSight.end())
					agent->m_agentsInSight.push_back(otherAgent);
			}
			else
			{
				visibility = false;
			}

			currentCheckPoint += stride;
		}
	}

	if(setMapVisibility)
		theMap->ResetBeenSetToVisibleFlag();
}

};