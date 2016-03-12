#pragma once

#ifndef FACTIONSYSTEM_HPP
#define FACTIONSYSTEM_HPP

#include "Engine\Parsing\TinyXML\tinyxml.h"

#include <vector>
#include <map>


namespace Henry
{

struct FactionRelationship
{
	std::string name;
	int value;
};

class Agent;

class FactionSystem
{
public:
	FactionSystem(Agent* owner);
	~FactionSystem(void);
	static void LoadDefaultFaction(const char* filePath);
	void AdjustRelationship(const std::string& faction , int value);
	void SetRelationShip(const std::string& faction , int value);
	void SaveToXML(TiXmlElement* element);
	int GetRelationshipValue(const std::string& faction);
private:
	std::vector<FactionRelationship> m_factionRelationships;
	static std::map<std::string,std::vector<FactionRelationship>> s_defaultRelationshipMap;
	Agent* m_agent;
};

};

#endif