#pragma once

#ifndef CELLULARAUTOMATAMAPGENERATOR_HPP
#define CELLULARAUTOMATAMAPGENERATOR_HPP

#include <string>

#include "MapGeneratorRegistration.hpp"
#include "MapGenerator.hpp"

namespace Henry
{

class CellularAutomataMapGenerator : public MapGenerator
{
public:
	CellularAutomataMapGenerator(const std::string& name);
	~CellularAutomataMapGenerator();
	void GenerateMap(Map* theMap);
	void ProcessOneStep(Map* theMap);
	static MapGenerator* CreateGenerator(const std::string& name);

protected:
	static MapGeneratorRegistration s_cellularAutomataRegistration;

private:
	std::string m_name;
};

};

#endif