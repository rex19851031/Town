#include "Map.hpp"

#include "GameCode\GameCommon.hpp"

namespace Henry
{

Cell::Cell() : m_type(AIR) , m_size( Vec2i(22,22) ) , m_occupier(nullptr) , m_feature(nullptr)
{
	GameCommon::s_cellSize = m_size;
	m_canBeSeen = false;
	m_debug = true;
	m_beenSetToVisible = false;
	m_discovered = false;
};


Map::Map(void)
{
	m_mapSize = Vec2i(64,32);
	int capacity = m_mapSize.x * m_mapSize.y;
	m_cells.reserve(capacity);
	for(int index = 0; index < capacity; index++)
	{
		Cell cell;
		m_cells.push_back(cell);
	}
}


Map::~Map(void)
{
}


void Map::ProcessOneStep()
{
	for(int x = 0; x < m_mapSize.x; x++)
	{
		for(int y = 0; y < m_mapSize.y; y++)
		{
			int numOfWallsNear = GetNumOfTypeNearBy(x,y,1,WALL) + GetNumOfTypeNearBy(x,y,2,WALL);

			if(numOfWallsNear > 15 || numOfWallsNear < 5 )
				m_cells[y * m_mapSize.x + x].m_nextType = WALL;
			else
				m_cells[y * m_mapSize.x + x].m_nextType = AIR;

			numOfWallsNear = 0;
		}
	}

	ChangeToNextType();
}


CellType Map::GetCellType(int x,int y)
{
	if( x < 0 || x >= m_mapSize.x ||
		y < 0 || y >= m_mapSize.y )
		return WALL;

	int index = y * m_mapSize.x + x;

	return m_cells[index].m_type;
}


int Map::GetNumOfTypeNearBy(int x,int y,int radius,CellType type)
{
	int numOfTypes = 0;
	for(int sx = x - radius; sx <= x + radius; sx++)
	{
		for(int sy = y - radius; sy <= y + radius; sy++)
		{
			if( (sx != x && sy != y) && GetCellType(sx,sy) == type )
				numOfTypes++;
		}
	}
	return numOfTypes;
}


void Map::SetCellType(int x,int y,CellType type)
{
	if( x < 0 || x >= m_mapSize.x ||
		y < 0 || y >= m_mapSize.y )
		return;

	int index = y * m_mapSize.x + x;
	m_cells[index].m_type = type;
}


void Map::SetCellType(int index,CellType type)
{
	if( index >= (int)m_cells.size())
		return;

	m_cells[index].m_type = type;
}


void Map::SetNextCellType(int x,int y,CellType type)
{
	m_cells[y * m_mapSize.x + x].m_nextType =type;
}


void Map::ChangeToNextType()
{
	for(size_t index=0; index < m_cells.size(); index++)
	{
		m_cells[index].ChangeToNextType();
	}
}


int Map::GetNumOfTypeInOrthogonal(int x,int y,int radius,CellType type)
{
	int numOfTypes = 0;
	for(int sx = x - radius; sx <= x + radius; sx++)
	{
		if( (sx != x) && GetCellType(sx,y) == type )
			numOfTypes++;
	}
	
	for(int sy = y - radius; sy <= y + radius; sy++)
	{
		if( (sy != y) && GetCellType(x,sy) == type )
			numOfTypes++;
	}

	return numOfTypes;
}


Vec2i Map::GetDirectionOfTypeInOrthogonal(int x,int y,int radius,CellType type)
{
	Vec2i direction(0,0);

	for(int sx = x - radius; sx <= x + radius; sx++)
	{
		if( (sx != x) && GetCellType(sx,y) == type )
			return direction = Vec2i(sx-x,0);
	}

	for(int sy = y - radius; sy <= y + radius; sy++)
	{
		if( (sy != y) && GetCellType(x,sy) == type )
			return  Vec2i(0,sy-y);
	}

	return direction;
}


void Map::SetAgent(int x,int y,Agent* agent)
{
	m_cells[y * m_mapSize.x + x].m_occupier = agent;
}


void Map::SetAgent(int index,Agent* agent)
{
	m_cells[index].m_occupier = agent;
}


Agent* Map::GetAgent(int x,int y)
{
	if( x < 0 || x >= m_mapSize.x ||
		y < 0 || y >= m_mapSize.y )
		return nullptr;

	return m_cells[y * m_mapSize.x + x].m_occupier;
}


int Map::GetMapSize()
{
	return m_mapSize.x * m_mapSize.y;
}


void Map::SetVisibility(int x,int y,bool canBeSeen)
{
	if( x < 0 || x >= m_mapSize.x ||
		y < 0 || y >= m_mapSize.y )
		return;

	int index = y * m_mapSize.x + x;
	if(!m_cells[index].m_beenSetToVisible || m_cells[index].m_type == WALL)
	{
		m_cells[index].m_canBeSeen = canBeSeen;		

		if(canBeSeen)
		{
			m_cells[index].m_beenSetToVisible = true;
			m_cells[index].m_discovered = true;
		}
	}
}


void Map::SetDebugMode(bool value)
{
	for(int index=0; index<GetMapSize(); index++)
		m_cells[index].m_debug = value;
}


void Map::ResetBeenSetToVisibleFlag()
{
	for(int index=0; index<GetMapSize(); index++)
		m_cells[index].m_beenSetToVisible = false;
}


void Map::ResetDiscoveredFlag()
{
	for(int index=0; index<GetMapSize(); index++)
		m_cells[index].m_discovered = false;
}


void Map::ResetAllToDark()
{
	for(int index=0; index<GetMapSize(); index++)
		m_cells[index].m_canBeSeen = false;
}


bool Map::GetVisibility(int x,int y)
{
	int index = y * m_mapSize.x + x;
	return m_cells[index].m_canBeSeen;
}


bool Map::GetVisibility(Vec2i location)
{
	int index = location.y * m_mapSize.x + location.x;
	return m_cells[index].m_canBeSeen;
}


void Map::SetVisibility(int index,bool canBeSeen)
{
	if(!m_cells[index].m_beenSetToVisible || m_cells[index].m_type == WALL)
	{
		m_cells[index].m_canBeSeen = canBeSeen;		

		if(canBeSeen)
		{
			m_cells[index].m_beenSetToVisible = true;
			m_cells[index].m_discovered = true;
		}
	}
}


void Map::SetItem(int x,int y,Item* entity)
{
	int index = y * m_mapSize.x + x;
	m_cells[index].m_entitys.push_back(entity);
}


void Map::SetItem(int index,Item* entity)
{
	m_cells[index].m_entitys.push_back(entity);
}


void Map::GetItemsOn(int x,int y,std::vector<Item*>& entityList)
{
	int index = y * m_mapSize.x + x;
	std::vector<Item*>::iterator iter = m_cells[index].m_entitys.begin();
	while(iter != m_cells[index].m_entitys.end())
	{
		Item* item = *iter;
		if(!item->m_beenPicked)
			entityList.push_back(item);
		iter = m_cells[index].m_entitys.erase(iter);
	}
}


void Map::GetItemsOn(Vec2i location , std::vector<Item*>& entityList)
{
	int index = location.y * m_mapSize.x + location.x;
	std::vector<Item*>::iterator iter = m_cells[index].m_entitys.begin();
	while(iter != m_cells[index].m_entitys.end())
	{
		Item* item = *iter;
		if(!item->m_beenPicked)
			entityList.push_back(item);
		iter = m_cells[index].m_entitys.erase(iter);
	}
}


void Map::RevealItems(Vec2i location)
{
	int index = location.y * m_mapSize.x + location.x;
	std::vector<Item*>::iterator iter = m_cells[index].m_entitys.begin();
	while(iter != m_cells[index].m_entitys.end())
	{
		Item* item = *iter;
		if(!item->m_beenPicked)
			GameCommon::s_UI->PostMessageToLog("Found '%s' In This Area.",RGBA(),item->m_name.c_str());
		iter++;
	}
}


Feature* Map::GetFeature(Vec2i location)
{
	if( location.x < 0 || location.x >= m_mapSize.x ||
		location.y < 0 || location.y >= m_mapSize.y )
		return nullptr;

	int index = location.y * m_mapSize.x + location.x;
	return m_cells[index].m_feature;
}


void Map::SetFeature(Vec2i location , Feature* feature)
{
	int index = location.y * m_mapSize.x + location.x;
	m_cells[index].m_feature = feature;
}


void Map::SetFeature(int index , Feature* feature)
{
	m_cells[index].m_feature = feature;
}


bool Map::GetDiscoveredFlag(int x,int y)
{
	int index = y * m_mapSize.x + x;
	return m_cells[index].m_discovered;
}


void Map::SetDiscovered(int index,bool discovered)
{
	m_cells[index].m_discovered = discovered;
}


void Map::RemoveFeature(int x,int y)
{
	if( x < 0 || x >= m_mapSize.x ||
		y < 0 || y >= m_mapSize.y )
		return;

	int index = y * m_mapSize.x + x;
	if(m_cells[index].m_feature)
	{
		m_cells[index].m_feature->m_isBroke = true;
		m_cells[index].m_feature = nullptr;
	}
}


};