#include "AIBehaviorDrains.hpp"

#include "Engine\Core\HenryFunctions.hpp"

#include "GameCode\GameCommon.hpp"


namespace Henry
{

AIBehaviorRegistration AIBehaviorDrains::s_drainsRegistration("Drains",&AIBehaviorDrains::CreateAndGetBehavior);


AIBehavior* AIBehaviorDrains::CreateAndGetBehavior(const TiXmlElement* elem)
{
	return ( new AIBehaviorDrains(elem) );
}


AIBehaviorDrains::AIBehaviorDrains(const TiXmlElement* xmlElem) : AIBehavior(xmlElem) , m_target(nullptr)
{
	m_minDrain = (float)std::atof(xmlElem->Attribute("minDrain"));
	m_maxDrain = (float)std::atof(xmlElem->Attribute("maxDrain"));
}


AIBehaviorDrains::~AIBehaviorDrains(void)
{
}


bool AIBehaviorDrains::Think()
{
	if(m_target)
	{
		float drainHealth = m_maxDrain * getRandomPercent();
		if(drainHealth < m_minDrain)
			drainHealth = m_minDrain;

		if(m_agent->m_currentHP < m_agent->m_maxHP)
		{
			m_agent->m_currentHP += drainHealth;
			if(m_agent->m_currentHP > m_agent->m_maxHP)
			{
				m_agent->m_currentHP = m_agent->m_maxHP;
			}

			GameCommon::s_UI->PostMessageToLog("The %s appears stronger.",RGBA(),m_agent->m_name.c_str());
		}

		m_target->m_currentHP -= drainHealth;
		GameCommon::s_UI->PostMessageToLog("%s feel drained.",RGBA(),m_target->m_name.c_str());
		GameCommon::s_UI->PostMessageToLog("The %s touches %s.",RGBA(),m_agent->m_name.c_str(),m_target->m_name.c_str());
	}

	m_target = nullptr;
	return true;
}


AIBehavior* AIBehaviorDrains::Clone()
{
	return new AIBehaviorDrains(*this);
}


float AIBehaviorDrains::Evaluate()
{
	m_target = nullptr;

	float evaluateValue = 0.0f;
	Vec2i allDirection[8] = {	Vec2i(-1,-1) ,	Vec2i(0,-1) ,	Vec2i(1,-1), 
								Vec2i(-1,0)  ,					Vec2i(1,0) , 
								Vec2i(-1,1)  ,	Vec2i(0,1)  ,	Vec2i(1,1) };

	Map* currentMap = m_agent->m_map;
	Vec2i currentPosition = m_agent->m_position;
	Agent* agent = nullptr;
	int relationship = 0;

	for(int index=0; index < 8; index++)
	{
		Vec2i adjacentLocation = currentPosition + allDirection[index];
		agent = currentMap->GetAgent(adjacentLocation.x,adjacentLocation.y);
		if( agent && m_agent->m_factionRelationship->GetRelationshipValue(agent->m_faction) < 0 && m_agent->m_factionRelationship->GetRelationshipValue(agent->m_faction) < relationship)
		{
			m_target = agent;
			relationship = m_agent->m_factionRelationship->GetRelationshipValue(agent->m_faction);
		}
	}

	if(m_target)
	{
		evaluateValue = 5.0f * (m_agent->m_maxHP - m_agent->m_currentHP) + 1.0f;
	}

	return evaluateValue;
}

};