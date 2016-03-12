#include "NecromancerLairMapGenerator.hpp"

#include "Engine\Core\HenryFunctions.hpp"

namespace Henry
{

MapGeneratorRegistration NecromancerLairMapGenerator::s_necromancerLairRegistration("Necromancer Lair",&NecromancerLairMapGenerator::CreateGenerator);


MapGenerator* NecromancerLairMapGenerator::CreateGenerator(const std::string& name)
{
	return ( new NecromancerLairMapGenerator(name) );
}


NecromancerLairMapGenerator::NecromancerLairMapGenerator(const std::string& name) : m_name(name)
{
	m_halfOfMaximumSizeOfARoom = Vec2i(6,6);
	m_maximumOfLength = 10;
	m_overlapLimit = 0;
	m_steps = 100;
	m_riverNum = 0;
}


NecromancerLairMapGenerator::~NecromancerLairMapGenerator()
{

}


void NecromancerLairMapGenerator::GenerateMap(Map* theMap)
{
	std::vector<Room> roomListEmpty;
	std::vector<AABB2> hallwayListEmpty;
	m_roomList = roomListEmpty;
	m_hallwayList = hallwayListEmpty;

	m_riverNum = 0;

	for(size_t index = 0; index < theMap->m_cells.size(); index++)
	{
		theMap->SetCellType(index,WALL);
	}

	int rightPosition = (int)(theMap->m_mapSize.x * (random(0,2) * 0.1f + 0.8f));
	Vec2i rightEdge( rightPosition , (int)(theMap->m_mapSize.y * getRandomPercent()));
	Vec2i necromancerRoomSize(random(5,8),random(8,12));
	CreateRoom(theMap,rightEdge,necromancerRoomSize);

	int leftPosition = (int)(theMap->m_mapSize.x * (random(0,2) * 0.1f));
	Vec2i left( leftPosition , (int)(theMap->m_mapSize.y * getRandomPercent()));
	Vec2i entranceRoomSize(random(4,6),random(4,6));
	CreateRoom(theMap,left,entranceRoomSize);
}


void NecromancerLairMapGenerator::ProcessOneStep(Map* theMap)
{
	if(m_riverNum <= 3)
	{
		Vec2i source = Vec2i(random(10,45),theMap->m_mapSize.y);
		Vec2i destination = Vec2i(random(10,45),0);
		CreateRiver(theMap,source,destination);
		m_riverNum++;
	}
}


bool NecromancerLairMapGenerator::CreateRoom(Map* theMap,Vec2i location,Vec2i halfSize)
{
	Vec2f aabbMin = Vec2f( (float)(location.x - halfSize.x), (float)(location.y - halfSize.y));
	Vec2f aabbMax = Vec2f( (float)(location.x + halfSize.x), (float)(location.y + halfSize.y));
	AABB2 newRoom(aabbMin,aabbMax);

	for(size_t index = 0; index < m_roomList.size(); index++)
	{
		Room currentRoom = m_roomList[index];
		if(currentRoom.m_aabb.isOverlap(&newRoom))
			return false;
	}
	
	m_roomList.push_back(Room(newRoom,0));

	for(int x = location.x - halfSize.x; x < location.x + halfSize.x; x++)
	{
		for(int y = location.y - halfSize.y; y < location.y + halfSize.y; y++)
		{
			theMap->SetCellType(x,y,AIR);
		}
	}

	return true;
}


bool NecromancerLairMapGenerator::CreateHallway(Map* theMap,Vec2i location,Vec2i direction,int length)
{
	Vec2i startLocation = location;
	int numOfConjectionArea = 0;
	for(int index=0; index < length; index++)
	{
		if(theMap->GetNumOfTypeInOrthogonal(startLocation.x,startLocation.y,1,AIR) > 0)
			numOfConjectionArea++;

		startLocation += direction;
	}

	if(numOfConjectionArea > 1)
		return false;

	startLocation = location;
	for(int index=0; index < length; index++)
	{
		theMap->SetCellType(startLocation.x,startLocation.y,AIR);
		startLocation += direction;
	}

	return true;
}


bool NecromancerLairMapGenerator::CreateRiver(Map* theMap,Vec2i source , Vec2i destination)
{
	Vec2i sourceToDestination = destination - source;
	Vec2f step = Vec2f(sourceToDestination.x * 0.001f , sourceToDestination.y * 0.001f);
	Vec2f currentPosition = Vec2f((float)source.x,(float)source.y);
	while(currentPosition.x > destination.x && currentPosition.y > destination.y)
	{
		Vec2i cellCoords = Vec2i((int)currentPosition.x,(int)currentPosition.y);
		theMap->SetCellType(cellCoords.x,cellCoords.y,LAVA);
		currentPosition += step;
	}
	return true;
}


};