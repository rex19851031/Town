#pragma once

#ifndef SIMPLEDUNGEONMAPGENERATOR_HPP
#define SIMPLEDUNGEONMAPGENERATOR_HPP

#include <string>

#include "MapGeneratorRegistration.hpp"
#include "MapGenerator.hpp"

namespace Henry
{

struct Room
{
	Room() : m_overlapTime(0) { };
	Room(AABB2 aabb,int overlapTime) : m_aabb(aabb) , m_overlapTime(overlapTime) { };
	AABB2 m_aabb;
	int m_overlapTime;
};


class NecromancerLairMapGenerator : public MapGenerator
{
public:
	NecromancerLairMapGenerator(const std::string& name);
	~NecromancerLairMapGenerator(void);
	void GenerateMap(Map* theMap);
	void ProcessOneStep(Map* theMap);
	static MapGenerator* CreateGenerator(const std::string& name);

protected:
	static MapGeneratorRegistration s_necromancerLairRegistration;

private:
	bool CreateRoom(Map* theMap,Vec2i location,Vec2i halfSize);
	bool CreateHallway(Map* theMap,Vec2i location,Vec2i direction,int length);
	bool CreateRiver(Map* theMap,Vec2i source , Vec2i destination);

	std::string m_name;
	Vec2i m_halfOfMaximumSizeOfARoom;
	int m_maximumOfLength;
	int m_overlapLimit;
	int m_riverNum;
	std::vector<Room> m_roomList;
	std::vector<AABB2> m_hallwayList;
};

};

#endif