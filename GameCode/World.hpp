#pragma once

#ifndef WORLD_HPP 
#define WORLD_HPP

#include <map>
#include "GameCode\Map.hpp"
#include "GameCode\Generators\MapGenerator.hpp"

namespace Henry
{

class World
{
public:
	World(void);
	~World(void);
	void Render();
	void Update();
	void ProcessOneStep();
	void GenerateMap();
	void SetGenerator(MapGenerator* generator);
	void AutoGenerate();
	void InitializeMap();
	void SaveToXML(TiXmlElement* element);
	bool IsThisCellSolid(Vec2f location);
	int GetNumOfExploredCells();
	Map* GetMap(int mapIndex = 0);

	int m_steps;
private:
	Vec2f m_playerPosition;
	MapGenerator* m_generator;
	std::map<int,Map> m_maps;
};

};

#endif