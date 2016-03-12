#pragma once

#ifndef MAPGENERATORREGISTRATION_HPP 
#define MAPGENERATORREGISTRATION_HPP

#include <vector>
#include <map>

#include "MapGenerator.hpp"

namespace Henry
{

class MapGenerator;
class MapGeneratorRegistration;
typedef std::map<std::string,MapGeneratorRegistration*> MapRegistrationMap;
typedef MapGenerator* (RegistrationFunc)(const std::string& name);

class MapGeneratorRegistration
{
public:
	MapGeneratorRegistration(const std::string& name,RegistrationFunc* registrationFunc);
	~MapGeneratorRegistration(void);
	static MapRegistrationMap* GetMapRegistrationMap();
	MapGenerator* CreateThisGenerator();
protected:
	std::string m_name;
	RegistrationFunc* m_registrationFunc;
	static MapRegistrationMap* s_mapRegistrationMap;
};

};

#endif