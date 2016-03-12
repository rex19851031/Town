#pragma once

#ifndef PRISONMAPGENERATOR_HPP
#define PRISONMAPGENERATOR_HPP

#include "MapGenerator.hpp"
#include "MapGeneratorRegistration.hpp"

namespace Henry
{

struct Room
{
	Room(Vec2i bottomLeft) : bottomLeftPoint(bottomLeft) , numOfRoomConnected(0) {};
	Vec2i bottomLeftPoint;
	int numOfRoomConnected;
};


struct PreDefineRoom
{
	std::string name;
	Cell* cell;
	int amount;
	Vec2i roomSize;
};


class PrisonMapGenerator : public MapGenerator
{
public:
	PrisonMapGenerator(const std::string& name);
	~PrisonMapGenerator(void);
	void GenerateMap(Map* theMap);
	void ProcessOneStep(Map* theMap);

	static MapGenerator* CreateGenerator(const std::string& name);

protected:
	static MapGeneratorRegistration s_prisonRegistration;

private:
	void LoadPreDefineRoomToList();
	std::string m_name;
	std::vector<Room> m_roomList;
	std::vector<PreDefineRoom> m_preDefineRoom;
	Vec2i m_roomSize;
};

};

#endif