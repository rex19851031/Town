#pragma once

#ifndef MAP_HPP
#define MAP_HPP

#include <vector>

#include "Engine\Math\GeneralStruct.hpp"
#include "Engine\Renderer\BitmapFont.hpp"

#include "GameCode\Agent.hpp"
#include "GameCode\Features\Feature.hpp"

namespace Henry
{

enum CellType{ AIR = 0, WALL , WOOD , CHAIR , PILLAR , TREASURE , WATER , LAVA , ICE};

struct Cell
{
	Cell();
	CellType m_type;
	char m_glyph;
	Vec2i m_size;
	CellType m_nextType;
	Agent* m_occupier;
	Feature* m_feature;
	void ChangeToNextType(){ m_type = m_nextType; };
	bool m_canBeSeen;
	bool m_discovered;
	bool m_beenSetToVisible;
	bool m_debug;
	std::vector<Item*> m_entitys;
	RGBA GetColor()
	{
		float tant = 1.0f;
		if(m_discovered && !m_debug && !m_canBeSeen)
			tant = 0.5f;

		if(!m_canBeSeen && !m_discovered && !m_debug)
			return RGBA(0.0f,0.0f,0.0f,1.0f);
		if(m_type == AIR) 
			return RGBA(0.2f,0.2f,0.2f,1.0f) * tant;
		if(m_type == WALL) 
			return RGBA(0.2f,0.5f,0.8f,1.0f) * tant; 
		if(m_type == WOOD )
			return RGBA(0.51f,0.32f,0.21f,1.0f) * tant;
		if(m_type == CHAIR)
			return RGBA(0.28f,0.18f,0.12f,1.0f) * tant;
		if(m_type == PILLAR)
			return RGBA(0.3f,0.3f,0.3f,1.0f) * tant;
		if(m_type == TREASURE)
			return RGBA(1.0f,0.5f,0.25f,1.0f) * tant;
		if(m_type == WATER)
			return RGBA(0.2f,0.2f,0.5f,1.0f) * tant;
		if(m_type == LAVA)
			return RGBA(1.0f,0.1f,0.1f,1.0f) * tant;
		if(m_type == ICE)
			return RGBA(0.8f,0.8f,1.0f,1.0f) * tant;
		return RGBA();
	};
};

class Map
{
public:
	Map(void);
	Map(Vec2f coords) : m_mapCoords(coords){};
	~Map(void);
	void ProcessOneStep();
	void SetCellType(int x,int y,CellType type);
	void SetCellType(int index,CellType type);
	void SetNextCellType(int x,int y,CellType type);
	CellType GetCellType(int x,int y);
	int GetNumOfTypeNearBy(int x,int y,int radius,CellType type);
	int GetNumOfTypeInOrthogonal(int x,int y,int radius,CellType type);
	int GetMapSize();
	Vec2i GetDirectionOfTypeInOrthogonal(int x,int y,int radius,CellType type);
	void ChangeToNextType();
	
	void SetVisibility(int x,int y,bool canBeSeen);
	void SetVisibility(int index,bool canBeSeen);
	void SetDiscovered(int index,bool discovered);
	bool GetVisibility(int x,int y);
	bool GetVisibility(Vec2i location);
	bool GetDiscoveredFlag(int x,int y);
	void ResetDiscoveredFlag();
	void SetDebugMode(bool value);
	void ResetBeenSetToVisibleFlag();
	void ResetAllToDark();

	void SetAgent(int x,int y,Agent* agent);
	void SetAgent(int index,Agent* agent);
	Agent* GetAgent(int x,int y);
	
	void SetItem(int x,int y,Item* entity);
	void SetItem(int index,Item* entity);
	
	void GetItemsOn(int x,int y , std::vector<Item*>& entityList);
	void GetItemsOn(Vec2i location , std::vector<Item*>& entityList);
	void RevealItems(Vec2i location);

	Feature* GetFeature(Vec2i location);
	void SetFeature(Vec2i location , Feature* feature);
	void SetFeature(int index , Feature* feature);
	void RemoveFeature(int x,int y);
	//return reference of the cell
	//pass a vector to have map fill the cell surround location which is not wall.

	std::vector<Cell> m_cells;
	Vec2f m_mapCoords;
	Vec2i m_mapSize;
private:
	//float GetPercentOfSquaresAreOpen?
};

};

#endif
