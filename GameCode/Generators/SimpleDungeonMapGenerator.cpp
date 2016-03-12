#include "SimpleDungeonMapGenerator.hpp"

#include "Engine\Core\HenryFunctions.hpp"

namespace Henry
{

MapGeneratorRegistration SimpleDungeonMapGenerator::s_simpleDungeonRegistration("Simple Dungeon",&SimpleDungeonMapGenerator::CreateGenerator);


MapGenerator* SimpleDungeonMapGenerator::CreateGenerator(const std::string& name)
{
	return ( new SimpleDungeonMapGenerator(name) );
}


SimpleDungeonMapGenerator::SimpleDungeonMapGenerator(const std::string& name) : m_name(name)
{
	m_halfOfMaximumSizeOfARoom = Vec2i(6,6);
	m_maximumOfLength = 10;
	m_overlapLimit = 0;
	m_steps = 100;
}


SimpleDungeonMapGenerator::~SimpleDungeonMapGenerator()
{

}


void SimpleDungeonMapGenerator::GenerateMap(Map* theMap)
{
	std::vector<Room> roomListEmpty;
	std::vector<AABB2> hallwayListEmpty;
	m_roomList = roomListEmpty;
	m_hallwayList = hallwayListEmpty;

	for(size_t index = 0; index < theMap->m_cells.size(); index++)
	{
		theMap->SetCellType(index,WALL);
	}

	Vec2i center(theMap->m_mapSize.x >> 1,theMap->m_mapSize.y >> 1);
	CreateRoom(theMap,center,Vec2i(3,3));
}


void SimpleDungeonMapGenerator::ProcessOneStep(Map* theMap)
{
	enum type{ room , hallway };
	Vec2i location;
	int locateTime = 0;
	int numOfAirInOrthogonal = 0;

	while(locateTime < 10000)
	{
		location = Vec2i((int)(getRandomPercent() * theMap->m_mapSize.x) , (int)(getRandomPercent() * theMap->m_mapSize.y));
		numOfAirInOrthogonal = theMap->GetNumOfTypeInOrthogonal(location.x,location.y,1,AIR);
		
		if(numOfAirInOrthogonal == 1)
		{
			type rollType = getRandomPercent() < 0.5 ? room : hallway;
			Vec2i oppositeDirection = theMap->GetDirectionOfTypeInOrthogonal(location.x,location.y,1,AIR) * -1;
			bool generated = false;

			if( rollType == room)
			{
				Vec2i roomSize = Vec2i((int)(m_halfOfMaximumSizeOfARoom.x * getRandomPercent() + 2),(int)(m_halfOfMaximumSizeOfARoom.y * getRandomPercent() + 2));
				generated = CreateRoom(theMap,location,roomSize);
			}
			
			if(rollType == hallway)
			{
				int length = (int)(m_maximumOfLength * getRandomPercent() + 3);
				generated = CreateHallway(theMap,location,oppositeDirection,length);
			}
			
			if(generated)
				break;
		}

		locateTime++;
	}
}


bool SimpleDungeonMapGenerator::CreateRoom(Map* theMap,Vec2i location,Vec2i halfSize)
{
	Vec2f aabbMin = Vec2f( (float)(location.x - halfSize.x), (float)(location.y - halfSize.y));
	Vec2f aabbMax = Vec2f( (float)(location.x + halfSize.x), (float)(location.y + halfSize.y));
	AABB2 newRoom(aabbMin,aabbMax);

	for(size_t index = 0; index < m_roomList.size(); index++)
	{
		Room currentRoom = m_roomList[index];
		if(currentRoom.m_aabb.isOverlap(&newRoom))// && currentRoom.m_overlapTime > m_overlapLimit)
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


bool SimpleDungeonMapGenerator::CreateHallway(Map* theMap,Vec2i location,Vec2i direction,int length)
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

};