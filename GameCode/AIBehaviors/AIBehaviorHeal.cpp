#include "AIBehaviorHeal.hpp"

#include "Engine\Core\HenryFunctions.hpp"

#include "GameCode\GameCommon.hpp"


namespace Henry
{

AIBehaviorRegistration AIBehaviorHeal::s_healRegistration("Heal",&AIBehaviorHeal::CreateAndGetBehavior);


AIBehavior* AIBehaviorHeal::CreateAndGetBehavior(const TiXmlElement* elem)
{
	return ( new AIBehaviorHeal(elem) );
}


AIBehaviorHeal::AIBehaviorHeal(const TiXmlElement* xmlElem) : AIBehavior(xmlElem) , m_target(nullptr)
{
	m_threshold = atoi(xmlElem->Attribute("healthThreshold"));
	m_min = atoi(xmlElem->Attribute("minHeal"));
	m_max = atoi(xmlElem->Attribute("maxHeal"));
}


AIBehaviorHeal::~AIBehaviorHeal(void)
{
}


bool AIBehaviorHeal::Think()
{
	if(m_target)
	{
		int healValue = random(m_min,m_max);
		m_target->m_currentHP += healValue;
		if(m_target->m_currentHP > m_target->m_maxHP)
		{
			m_target->m_currentHP = m_target->m_maxHP;
		}

		GameCommon::s_UI->PostMessageToLog("%s Heal %s With %d Health.",RGBA(0.2f,0.5f,0.2f,1.0f),m_agent->m_name.c_str(),m_target->m_name.c_str(),healValue);
	}

	m_target = nullptr;
	return true;
}


AIBehavior* AIBehaviorHeal::Clone()
{
	return new AIBehaviorHeal(*this);
}


float AIBehaviorHeal::Evaluate()
{
	m_target = nullptr;

	float evaluateValue = 0.0f;
	float lowerestHealth = 10000.0f;
	int indexOfLowerestAlly = -1;

	if(m_agent->m_agentsInSight.size() != 0)
	{
		for(size_t index=0; index < m_agent->m_agentsInSight.size(); index++)
		{
			Agent* currentAgent = m_agent->m_agentsInSight[index];
			int relationship = m_agent->m_factionRelationship->GetRelationshipValue(currentAgent->m_faction);
			if(relationship > 0 && currentAgent->m_currentHP < lowerestHealth)
			{
				lowerestHealth = currentAgent->m_currentHP;
				indexOfLowerestAlly = index;
			}
		}
	}

	if(indexOfLowerestAlly != -1)
	{
		if(m_agent->m_agentsInSight[indexOfLowerestAlly]->m_currentHP < m_threshold)
		{
			m_target = m_agent->m_agentsInSight[indexOfLowerestAlly];
		}
	}
	else
	{
		if(m_agent->m_currentHP < m_threshold)
		{
			m_target = m_agent;
		}
	}

	if(m_target)
		evaluateValue = (m_threshold - m_target->m_currentHP) * 4.0f;

	return evaluateValue;
}

};