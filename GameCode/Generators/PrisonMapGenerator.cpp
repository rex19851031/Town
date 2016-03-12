#include "PrisonMapGenerator.hpp"

#include "Engine\Core\HenryFunctions.hpp"

#include "GameCode\Item\ItemFactory.hpp"
#include "GameCode\Features\FeatureFactory.hpp"
#include "GameCode\NPC\NPCFactory.hpp"
#include "GameCode\GameCommon.hpp"

#include <windows.h>

namespace Henry
{

MapGeneratorRegistration PrisonMapGenerator::s_prisonRegistration("Prison",&PrisonMapGenerator::CreateGenerator);


MapGenerator* PrisonMapGenerator::CreateGenerator(const std::string& name)
{
	return ( new PrisonMapGenerator(name) );
}


PrisonMapGenerator::PrisonMapGenerator(const std::string& name) : m_name(name)
{
	m_roomSize = Vec2i(6,5);
	m_steps = 150;
}


PrisonMapGenerator::~PrisonMapGenerator(void)
{
}


void PrisonMapGenerator::GenerateMap(Map* theMap)
{
	std::vector<Room> empty;
	m_roomList = empty;
	std::vector<PreDefineRoom> emptyPreDefineRoom;
	m_preDefineRoom = emptyPreDefineRoom;

	LoadPreDefineRoomToList();

	for(size_t index = 0; index < theMap->m_cells.size(); index++)
	{
		theMap->SetCellType(index,WALL);
	}
	
	int yOffset = 0;
	int type = getRandomPercent() < 0.5f ? 0 : 1;
	
	for(int x=1; x < theMap->m_mapSize.x; x += m_roomSize.x+1)
	{
		if(type == 0)
			yOffset = -x%2+1;
		else
			yOffset = x%2;

		for(int y= 1 + yOffset; y < theMap->m_mapSize.y; y += m_roomSize.y+1)
		{
			if(x+m_roomSize.x > theMap->m_mapSize.x || y + m_roomSize.y > theMap->m_mapSize.y)
				break;
			
			m_roomList.push_back(Room(Vec2i(x,y)));

			for(int rx=0; rx < m_roomSize.x; rx++)
			{
				for(int ry=0; ry < m_roomSize.y; ry++)
				{
					theMap->SetCellType(x+rx,y+ry,AIR);
				} 
			}
		}
	}
}


void PrisonMapGenerator::ProcessOneStep(Map* theMap)
{
	Vec2i location;
	int loopingTimes = 0;
	int numOfAirInOrthogonal = 0;
	while(loopingTimes < 10000)
	{
		location = Vec2i((int)(theMap->m_mapSize.x * getRandomPercent()),(int)(theMap->m_mapSize.y * getRandomPercent()));
		numOfAirInOrthogonal = theMap->GetNumOfTypeInOrthogonal(location.x,location.y,1,AIR);
		
		if(numOfAirInOrthogonal == 2 && theMap->GetCellType(location.x,location.y) != AIR )
		{
			int leastConnectNumOfRooms = m_roomList.size() + 20;
			int connectNumberOfNextConnectRoom[2];
			int indexOfConnectedRoom[2];
			int roomIndex = 0;
			
			for(size_t index = 0; index < m_roomList.size(); index++)
			{
				Vec2i min = m_roomList[index].bottomLeftPoint;
				Vec2i max = m_roomList[index].bottomLeftPoint + m_roomSize - 1;

				if( ((location.x + 1 == min.x || location.x - 1 == max.x) && location.y >= min.y && location.y <= max.y) ||
					(location.x >= min.x && location.x <= max.x && (location.y + 1 == min.y || location.y - 1 == max.y)) )
				{
					connectNumberOfNextConnectRoom[roomIndex] = m_roomList[index].numOfRoomConnected;
					indexOfConnectedRoom[roomIndex] = index;
					roomIndex++;
				}

				if(m_roomList[index].numOfRoomConnected < leastConnectNumOfRooms)
					leastConnectNumOfRooms = m_roomList[index].numOfRoomConnected;
			}

			if((leastConnectNumOfRooms == connectNumberOfNextConnectRoom[0] || leastConnectNumOfRooms == connectNumberOfNextConnectRoom[1]) && roomIndex == 2)
			{
				theMap->SetCellType(location.x,location.y,AIR);

				FeatureFactorySet::iterator iter = FeatureFactory::s_featureFactorySet.begin();
				FeatureFactory* factory = FeatureFactory::FindFactory("WoodDoor");
				if(factory)
				{
					Feature* feature = factory->SpawnFeature(theMap,location);
					if(feature)
						GameCommon::s_features->push_back(feature);
				}

				m_roomList[indexOfConnectedRoom[0]].numOfRoomConnected++;
				m_roomList[indexOfConnectedRoom[1]].numOfRoomConnected++;

				PreDefineRoom pdr;
				bool generateRoom = false;
				for(size_t index = 0; index < m_preDefineRoom.size(); index++)
				{
					if(m_preDefineRoom[index].amount > 0)
					{
						pdr = m_preDefineRoom[index];
						m_preDefineRoom[index].amount--;
						generateRoom = true;
						break;
					}
				}
				
				if(generateRoom)
				{
					bool firstAir = true;

					Vec2i startPoint = m_roomList[indexOfConnectedRoom[0]].bottomLeftPoint;
					int pdrIndex = 0;
					for(int y=0; y < pdr.roomSize.y; ++y)
					{
						for(int x=pdr.roomSize.x-1; x >=0 ; --x)
						{
							if(pdr.cell[pdrIndex].m_type == AIR && firstAir)
							{
								if(pdr.name == "weaponRoom")
								{
									ItemFactory* factory = ItemFactory::FindFactory("MagicSword");
									if(factory)
									{
										Item* item = factory->SpawnItem(theMap , Vec2i(startPoint.x + x , startPoint.y + y) );
										if(item)
											GameCommon::s_items->push_back(item);
									}
								}

								if(pdr.name == "bossRoom")
								{
									NPCFactory* factory = NPCFactory::FindFactory("Warden");
									if(factory)
									{
										NPC* npc = factory->SpawnNPC(theMap , Vec2i(startPoint.x + x , startPoint.y + y) );
										if(npc)
											GameCommon::s_npcs->push_back(npc);
									}
								}

								if(pdr.name == "wandRoom")
								{
									ItemFactory* factory = ItemFactory::FindFactory("IceWand");
									if(factory)
									{
										Item* item = factory->SpawnItem(theMap , Vec2i(startPoint.x + x , startPoint.y + y) );
										if(item)
											GameCommon::s_items->push_back(item);
									}
								}

								if(pdr.name == "fountainRoom")
								{
									FeatureFactory* factory = FeatureFactory::FindFactory("HealingFountain");
									if(factory)
									{
										Feature* feature = factory->SpawnFeature(theMap , Vec2i(startPoint.x + x , startPoint.y + y) );
										if(feature)
											GameCommon::s_features->push_back(feature);
									}
								}

								firstAir = false;
							}

							theMap->SetCellType(startPoint.x + x , startPoint.y + y , pdr.cell[pdrIndex++].m_type);
							if(pdr.name != "fountainRoom")
								theMap->RemoveFeature(startPoint.x + x , startPoint.y + y );
						} 
					}
				}

				break;
			}
		}

		loopingTimes++;
	}
}


void PrisonMapGenerator::LoadPreDefineRoomToList()
{
	char* filePath = ".\\Data\\Maps\\prison_preDefinedRooms.xml";
	TiXmlDocument* m_metaDoc = new	TiXmlDocument(filePath);
	bool loadSuccessfully = m_metaDoc->LoadFile();
	if(!loadSuccessfully)
	{
		std::string errorInfo(filePath);
		errorInfo = "Failed to load file : " + errorInfo;
		MessageBoxA( NULL , errorInfo.c_str(), "Failed to loading files", MB_ICONERROR | MB_OK );
		exit(0);
	}

	if(m_metaDoc->ErrorId() > 0)
	{
		std::string errorInfo(filePath);
		errorInfo = "Error , can't analysis meta-data file : " + errorInfo + "\n\n";
		MessageBoxA( NULL , (LPCSTR)errorInfo.c_str() , "BMP Fonts Metadata Analysis Failed!", MB_ICONERROR | MB_OK );
	}

	TiXmlElement* root = m_metaDoc->RootElement();
	if(!root)
	{
		std::string errorInfo(filePath);
		errorInfo = "Error , can't find root element of meta-data file : " + errorInfo + "\n\n";
		MessageBoxA( NULL , (LPCSTR)errorInfo.c_str() , "BMP Fonts Metadata Analysis Failed!", MB_ICONERROR | MB_OK );
	}

	for(TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		std::string elemName = elem->Value();
		if(elemName == "map")
		{
			std::string text = elem->GetText();
			std::string name = elem->Attribute("name");
			int amount = std::atoi(elem->Attribute("rarity"));
			Vec2i roomSize;
			sscanf_s(elem->Attribute("size"),"%d,%d",&roomSize.x,&roomSize.y);
			
			DebuggerPrintf("Room Size X : %d , Room Size Y : %d",roomSize.x,roomSize.y);

			PreDefineRoom pdr;
			pdr.amount = amount;
			pdr.roomSize = roomSize;
			pdr.cell = new Cell[roomSize.x * roomSize.y];
			pdr.name = name;

			int mapIndex = 0;
			for(int index = text.size()-1; index >= 0; index--)
			{
				if(text[index] != ' ')
				{
					if(text[index] == '0')
						pdr.cell[mapIndex].m_type = AIR;
					if(text[index] == '1')
						pdr.cell[mapIndex].m_type = WALL;
					if(text[index] == 'w')
						pdr.cell[mapIndex].m_type = WOOD; 
					if(text[index] == 'c')
						pdr.cell[mapIndex].m_type = CHAIR; 
					if(text[index] == 'p')
						pdr.cell[mapIndex].m_type = PILLAR; 
					if(text[index] == 't')
						pdr.cell[mapIndex].m_type = TREASURE;
					if(text[index] == '~')
						pdr.cell[mapIndex].m_type = WATER;
					if(text[index] == 'l')
						pdr.cell[mapIndex].m_type = LAVA; 
					mapIndex++;
				}
			}

			m_preDefineRoom.push_back(pdr);
		}
	}
}

};