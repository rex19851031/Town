#include "CellularAutomataMapGenerator.hpp"

#include "Engine\Core\HenryFunctions.hpp"

namespace Henry
{

MapGeneratorRegistration CellularAutomataMapGenerator::s_cellularAutomataRegistration("Cellular Automata",&CellularAutomataMapGenerator::CreateGenerator);


MapGenerator* CellularAutomataMapGenerator::CreateGenerator(const std::string& name)
{
	return ( new CellularAutomataMapGenerator(name) );
}


CellularAutomataMapGenerator::CellularAutomataMapGenerator(const std::string& name) : m_name(name)
{
	m_steps = 10;
}


CellularAutomataMapGenerator::~CellularAutomataMapGenerator()
{

}


void CellularAutomataMapGenerator::GenerateMap(Map* theMap)
{
	for(size_t index = 0; index < theMap->m_cells.size(); index++)
	{
		getRandomPercent() < 0.6f ? theMap->SetCellType(index,AIR) : theMap->SetCellType(index,WALL);
	}
}


void CellularAutomataMapGenerator::ProcessOneStep(Map* theMap)
{
	for(int x = 0; x < theMap->m_mapSize.x; x++)
	{
		for(int y = 0; y < theMap->m_mapSize.y; y++)
		{
			int numOfWallsNear = theMap->GetNumOfTypeNearBy(x,y,1,WALL) + theMap->GetNumOfTypeNearBy(x,y,2,WALL);

			if(numOfWallsNear > 15 || numOfWallsNear < 5 )
				theMap->SetNextCellType(x,y,WALL);
			else
				theMap->SetNextCellType(x,y,AIR);

			numOfWallsNear = 0;
		}
	}

	theMap->ChangeToNextType();
}

};