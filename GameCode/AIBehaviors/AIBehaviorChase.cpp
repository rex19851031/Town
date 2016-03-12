#include "AIBehaviorChase.hpp"

#include "GameCode\GameCommon.hpp"

namespace Henry
{


AIBehaviorRegistration AIBehaviorChase::s_chaseRegistration("Chase",&AIBehaviorChase::CreateAndGetBehavior);


AIBehavior* AIBehaviorChase::CreateAndGetBehavior(const TiXmlElement* elem)
{
	return ( new AIBehaviorChase(elem) );
}


AIBehaviorChase::AIBehaviorChase(const TiXmlElement* xmlElem) : AIBehavior(xmlElem)
{

}


AIBehaviorChase::~AIBehaviorChase(void)
{
}


bool AIBehaviorChase::Think()
{	
	if(m_chasing)
	{
		m_pathFinding.FindPath(m_agent->m_position,m_goal,m_agent->m_map);

		size_t index = m_pathFinding.m_path.size()-2;
		Vec2i nextStep;

		if(index < m_pathFinding.m_path.size())
		{
			nextStep = m_pathFinding.m_path[index];
			if(m_agent->CanMoveTo(nextStep))
			{
				m_agent->MoveTo(nextStep);
			}
		}
	}

	return true;
}


AIBehavior* AIBehaviorChase::Clone()
{
	return new AIBehaviorChase(*this);
}


float AIBehaviorChase::Evaluate()
{
	float evaluateValue = 0.0f;
	m_chasing = false;
	int lowerestRelationship = 10000;

	if(m_agent->m_agentsInSight.size() != 0)
	{
		for(size_t index=0; index < m_agent->m_agentsInSight.size(); index++)
		{
			std::string targetFaction = m_agent->m_agentsInSight[index]->m_faction;
			int targetRelationship = m_agent->m_factionRelationship->GetRelationshipValue(targetFaction);
			if( targetRelationship < 0 && targetRelationship < lowerestRelationship)
			{
				m_chasing = true;
				m_goal = m_agent->m_agentsInSight[index]->m_position;
				evaluateValue = 2.0f;
				lowerestRelationship = targetRelationship;
			}
		}
	}

	if(!m_chasing)
		evaluateValue = 0.0f;

	return evaluateValue;
}


};

