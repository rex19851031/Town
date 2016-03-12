#include "FactionSystem.hpp"

#include "Engine\Parsing\TinyXML\TinyXMLHelper.hpp"

#include "GameCode\Agent.hpp"

#include <string>

namespace Henry
{

std::map<std::string,std::vector<FactionRelationship>> FactionSystem::s_defaultRelationshipMap;


FactionSystem::FactionSystem(Agent* owner) : m_agent(owner)
{
	std::string faction = owner->m_faction;
	std::vector<FactionRelationship> fr(s_defaultRelationshipMap[faction]);
	m_factionRelationships = fr;
}


FactionSystem::~FactionSystem(void)
{
}


void FactionSystem::LoadDefaultFaction(const char* filePath)
{
	TinyXMLHelper xmlHelper;
	xmlHelper.LoadFile(filePath);

	for(TiXmlElement* elem = xmlHelper.m_root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		std::string elemName = elem->Value();
		if(elemName == "Faction")
		{
			std::string name = elem->Attribute("name");
			std::string ally = elem->Attribute("ally");
			std::string enemy = elem->Attribute("enemy");

			FactionRelationship ally_fr;
			ally_fr.name = ally;
			ally_fr.value = 2;

			FactionRelationship enemy_fr;
			enemy_fr.name = enemy;
			enemy_fr.value = -2;

			std::vector<FactionRelationship> relationShips;
			if(ally_fr.name != "")
				relationShips.push_back(ally_fr);
			
			if(enemy_fr.name != "")
				relationShips.push_back(enemy_fr);

			s_defaultRelationshipMap[name] = relationShips; 
		}
	}
}


void FactionSystem::AdjustRelationship(const std::string& faction , int value)
{
	bool found = false;
	std::vector<FactionRelationship>::iterator iter = m_factionRelationships.begin();
	while(iter != m_factionRelationships.end())
	{
		FactionRelationship fr = *iter;
		if(fr.name == faction)
		{
			fr.value += value;
			found = true;
			break;
		}

		iter++;
	}

	if(!found)
	{
		FactionRelationship fr;
		fr.name = faction;
		fr.value = value;
		m_factionRelationships.push_back(fr);
	}
}


int FactionSystem::GetRelationshipValue(const std::string& faction)
{
	std::vector<FactionRelationship>::iterator iter = m_factionRelationships.begin();
	while(iter != m_factionRelationships.end())
	{
		FactionRelationship fr = *iter;
		if(fr.name == faction)
			return fr.value;

		iter++;
	}

	return 0;
}


void FactionSystem::SaveToXML(TiXmlElement* element)
{
	if(m_factionRelationships.size() != 0)
	{
		TiXmlElement* sub_element = new TiXmlElement("Factions");
		element->LinkEndChild(sub_element);

		for(size_t index = 0; index < m_factionRelationships.size(); ++index)
		{
			TiXmlElement* faction = new TiXmlElement("Faction");
			faction->SetAttribute( "name" , m_factionRelationships[index].name.c_str() );
			faction->SetAttribute( "value" , m_factionRelationships[index].value );
			sub_element->LinkEndChild(faction);
		}
	}
}


void FactionSystem::SetRelationShip(const std::string& faction , int value)
{
	std::vector<FactionRelationship>::iterator iter = m_factionRelationships.begin();
	while(iter != m_factionRelationships.end())
	{
		FactionRelationship fr = *iter;
		if(fr.name == faction)
		{
			fr.value = value;
			break;
		}

		iter++;
	}
}


};