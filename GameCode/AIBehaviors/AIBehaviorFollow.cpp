#include "AIBehaviorFollow.hpp"

#include "Engine\Core\HenryFunctions.hpp"

namespace Henry
{

AIBehaviorRegistration AIBehaviorFollow::s_followRegistration("Follow",&AIBehaviorFollow::CreateAndGetBehavior);


AIBehavior* AIBehaviorFollow::CreateAndGetBehavior(const TiXmlElement* elem)
{
	return ( new AIBehaviorFollow(elem) );
}


AIBehaviorFollow::AIBehaviorFollow(const TiXmlElement* xmlElem) : AIBehavior(xmlElem) , m_leader(nullptr)
{
	m_leaderName = xmlElem->Attribute("leaderName");
	m_threshold = (float)atof(xmlElem->Attribute("distanceThreshold"));
}


AIBehaviorFollow::~AIBehaviorFollow(void)
{
}


bool AIBehaviorFollow::Think()
{
	if(m_leader)
	{
		m_pathFinding.FindPath(m_agent->m_position,m_leader->m_position,m_agent->m_map);

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


AIBehavior* AIBehaviorFollow::Clone()
{
	return new AIBehaviorFollow(*this);
}


float AIBehaviorFollow::Evaluate()
{
	float evaluateValue = 0.0f;
	float distance = 0.0f;
	if(!m_leader)
	{
		if(m_agent->m_agentsInSight.size() != 0)
		{
			for(size_t index=0; index < m_agent->m_agentsInSight.size(); index++)
			{
				std::string targetName = m_agent->m_agentsInSight[index]->m_name;
				if(targetName == m_leaderName)
				{
					m_leader = m_agent->m_agentsInSight[index];
					Vec2i vectorToLeader = m_leader->m_position - m_agent->m_position;
					distance = std::sqrt((float)(vectorToLeader.x * vectorToLeader.x + vectorToLeader.y * vectorToLeader.y));
					break;
				}
			}
		}
	}
	else
	{
		Vec2i vectorToLeader = m_leader->m_position - m_agent->m_position;
		distance = std::sqrt((float)(vectorToLeader.x * vectorToLeader.x + vectorToLeader.y * vectorToLeader.y));
		if(distance > m_threshold)
		{
			m_leader = nullptr;
		}
	}

	evaluateValue = distance * 2;
	return evaluateValue;
}

};