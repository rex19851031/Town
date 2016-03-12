#include "AIBehaviorWander.hpp"

#include "Engine\Core\HenryFunctions.hpp"

namespace Henry
{

AIBehaviorRegistration AIBehaviorWander::s_wanderRegistration("Wander",&AIBehaviorWander::CreateAndGetBehavior);


AIBehavior* AIBehaviorWander::CreateAndGetBehavior(const TiXmlElement* elem)
{
	return ( new AIBehaviorWander(elem) );
}


AIBehaviorWander::AIBehaviorWander(const TiXmlElement* xmlElem) : AIBehavior(xmlElem)
{
	m_chanceOfRest = (float)std::atof(xmlElem->Attribute("chanceOfRest"));
	m_chanceOfMove = (float)std::atof(xmlElem->Attribute("chanceOfMove"));
}


AIBehaviorWander::~AIBehaviorWander(void)
{
}


bool AIBehaviorWander::Think()
{
	float decision = getRandomPercent();
	if( decision < m_chanceOfRest )
	{
		//resting
	}

	if( m_chanceOfRest < decision && decision < m_chanceOfRest + m_chanceOfMove)
	{
		Vec2i direction[8] = {	Vec2i(-1,1)		,	Vec2i(0,1)	,	Vec2i(1,1) , 
								Vec2i(-1,0)		,							Vec2i(1,0) , 
								Vec2i(-1,-1)		,	Vec2i(0,-1)	,	Vec2i(1,-1) };
		float movement = getRandomPercent() * 7;
		m_agent->TryGoToDirection(direction[(int)movement]);
	}

	return true;
}


AIBehavior* AIBehaviorWander::Clone()
{
	return new AIBehaviorWander(*this);
}


float AIBehaviorWander::Evaluate()
{
	return 1.0f;
}

};