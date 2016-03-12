#pragma once

#ifndef MAPGENERATOR_HPP
#define MAPGENERATOR_HPP

#include <string>
#include "GameCode\Map.hpp"

namespace Henry
{

class MapGenerator;
typedef std::map<std::string,MapGenerator*> mapGeneratorMap;

class MapGenerator
{
public:
	MapGenerator();
	MapGenerator(std::string name);
	~MapGenerator();
	virtual void GenerateMap(Map* theMap);
	virtual void ProcessOneStep(Map* theMap);
	static MapGenerator* CreateAllGenerators(const std::string& name);
	static void GetNameOfGenerators(std::vector<std::string>* names);
	static MapGenerator* GetGenerator(const std::string& name);

	int m_steps;

protected:
	static mapGeneratorMap s_generators;

private:
	std::string m_name;
};

};

#endif