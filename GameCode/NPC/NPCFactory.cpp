#include "NPCFactory.hpp"

#include "Engine\Parsing\TinyXML\TinyXMLHelper.hpp"
#include "Engine\Core\HenryFunctions.hpp"

#include "GameCode\GameCommon.hpp"

#include <string>

namespace Henry
{

NPCFactorySet NPCFactory::s_npcFactorySet;


NPCFactory::NPCFactory()
{

}


NPCFactory::~NPCFactory()
{
	if(m_npcTemplate)
	{
		delete m_npcTemplate;
		m_npcTemplate = nullptr;
	}

	deleteVectorOfPointer(m_behaviors);
}


void NPCFactory::LoadAndParseAllFilesUnder(const char* path)
{
	std::vector<std::string> allFiles;
	FileSystem files;
	std::string fullPath(path);
	
	fullPath.append("\\*.xml");
	files.EnumerateFileInFolder(fullPath.c_str(),allFiles);
	for(size_t index=0; index < allFiles.size(); index++)
	{
		std::string xmlPath(path);
		xmlPath.append(allFiles[index]);
		TinyXMLHelper xmlHelper;
		xmlHelper.LoadFile(xmlPath.c_str());
		
		if(!xmlHelper.m_root)
			continue;

		for(TiXmlElement* elem = xmlHelper.m_root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			std::string elemName = elem->Value();
			if(elemName == "NPC")
			{
				std::string name(elem->Attribute("name"));
				std::string glyph(elem->Attribute("glyph"));
				std::string color(elem->Attribute("color"));
				std::string maxHP(elem->Attribute("maxHP"));
				std::string faction(elem->Attribute("faction"));
				std::string random(elem->Attribute("randomGenerate"));

				NPC* npc = new NPC(name , nullptr); //GameCommon::s_world->GetMap());
				npc->m_glyph = glyph[0];
				sscanf_s(color.c_str(),"%d,%d,%d",&npc->m_color.r,&npc->m_color.g,&npc->m_color.b);
				npc->m_color.a = 255;
				npc->m_maxHP = (float)atoi(maxHP.c_str());
				npc->m_currentHP = npc->m_maxHP;
				npc->m_faction = faction;
				npc->m_random = (random == "true") ? true : false;

				NPCFactory* factory = new NPCFactory(); //pass root to create NPCFactory() instead of get the value here
				factory->m_npcTemplate = npc;
				s_npcFactorySet.insert(factory);

				for(TiXmlElement* sub_elem = elem->FirstChildElement(); sub_elem != NULL; sub_elem = sub_elem->NextSiblingElement())
				{
					std::string sub_elemName = sub_elem->Value();
					if(sub_elemName == "AIBehavior")
					{
						AIBehavior* behavior = AIBehavior::CreateAndGetBehavior(sub_elem->Attribute("name"),sub_elem);
						if(behavior)
						{
							factory->m_behaviors.push_back(behavior->Clone());
							delete behavior;
							behavior = nullptr;
						}
					}
				}
			}
		}
	}
}


NPC* NPCFactory::SpawnNPC(Map* theMap , const Vec2i location)
{
	NPC* npc = nullptr;
	if(theMap->GetCellType(location.x,location.y) == AIR && theMap->GetAgent(location.x,location.y) == nullptr)
	{
		npc = new NPC(*m_npcTemplate);
		npc->m_factionRelationship = new FactionSystem(npc);
		npc->m_map = theMap;
		npc->MoveTo(location);

		for(size_t index=0; index < m_behaviors.size(); index++)
			npc->AddBehavior(m_behaviors[index]->Clone());
	}
	return npc;
}


NPCFactory* NPCFactory::FindFactory(const std::string& name)
{
	NPCFactory* target_factory = nullptr;
	NPCFactorySet::iterator iter = s_npcFactorySet.begin();
	while(iter != s_npcFactorySet.end())
	{
		NPCFactory* factory = *iter;
		if(factory->m_npcTemplate->m_name == name)
		{
			target_factory = factory;
			break;
		}
		iter++;
	}

	return target_factory;
}


};