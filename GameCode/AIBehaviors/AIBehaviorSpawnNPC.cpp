#include "AIBehaviorSpawnNPC.hpp"

#include "Engine\Core\HenryFunctions.hpp"

#include "GameCode\NPC\NPCFactory.hpp"
#include "GameCode\GameCommon.hpp"


namespace Henry
{

AIBehaviorRegistration AIBehaviorSpawnNPC::s_spawnNPCRegistration("SpawnNPC",&AIBehaviorSpawnNPC::CreateAndGetBehavior);


AIBehavior* AIBehaviorSpawnNPC::CreateAndGetBehavior(const TiXmlElement* elem)
{
	return ( new AIBehaviorSpawnNPC(elem) );
}


AIBehaviorSpawnNPC::AIBehaviorSpawnNPC(const TiXmlElement* xmlElem) : AIBehavior(xmlElem)
{
	m_npcName = xmlElem->Attribute("npcName");
	m_spawnThreshold = std::atoi(xmlElem->Attribute("spawnThreshold"));
	m_changeToSpawn = (float)std::atof(xmlElem->Attribute("chanceToSpawn"));
}


AIBehaviorSpawnNPC::~AIBehaviorSpawnNPC(void)
{
}


bool AIBehaviorSpawnNPC::Think()
{
	if(m_numOfSameTypeNPCNearBy < m_spawnThreshold)
	{
		float rollDice = getRandomPercent();
		if(rollDice < m_changeToSpawn)
		{
			NPCFactory* factory = NPCFactory::FindFactory(m_npcName);
			if(factory)
			{
				bool spawnSuccess = false;
				int loopTime = 0;
				while(!spawnSuccess && loopTime < 10000)
				{
					int xOffset = random(0,2)-1;
					int yOffset = random(0,2)-1;
					Vec2i spawnLocation = m_agent->m_position + Vec2i(xOffset,yOffset);
					NPC* npc = factory->SpawnNPC(m_agent->m_map,spawnLocation);
					if(npc)
					{
						GameCommon::s_newNPCs->push_back(npc);
						GameCommon::s_UI->PostMessageToLog("The %s summons a %s to fight for him!",RGBA(),m_agent->m_name.c_str(),m_npcName.c_str());
						spawnSuccess = true;
					}
					loopTime++;
				}
			}
		}
	}
	
	return true;
}


AIBehavior* AIBehaviorSpawnNPC::Clone()
{
	return new AIBehaviorSpawnNPC(*this);
}


float AIBehaviorSpawnNPC::Evaluate()
{
	m_numOfSameTypeNPCNearBy = 0;

	float evaluateValue = 0.0f;
	bool enemyNearBy = false;
	
	Vec2i currentPosition = m_agent->m_position;

	if(m_agent->m_agentsInSight.size() != 0)
	{
		for(size_t index=0; index < m_agent->m_agentsInSight.size(); index++)
		{
			Agent* currentAgent = m_agent->m_agentsInSight[index];
			int relationship = m_agent->m_factionRelationship->GetRelationshipValue(currentAgent->m_faction);
			
			if(currentAgent->m_name == m_npcName)
			{
				m_numOfSameTypeNPCNearBy++;
			}

			if(relationship < 0)
			{
				enemyNearBy = true;
			}
		}
	}

	if(enemyNearBy)
	{
		evaluateValue = (m_spawnThreshold * 5.0f) - (m_numOfSameTypeNPCNearBy * 5.0f);
	}

	return evaluateValue;
}

};