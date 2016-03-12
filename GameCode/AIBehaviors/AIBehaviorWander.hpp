#pragma once

#ifndef AIBEHAVIORWANDER_HPP 
#define AIBEHAVIORWANDER_HPP

#include "AIBehavior.hpp"
#include "AIBehaviorRegistration.hpp"

namespace Henry
{

class AIBehaviorWander : public AIBehavior
{
public:
	AIBehaviorWander(const TiXmlElement* xmlElem);
	~AIBehaviorWander(void);
	bool Think();
	AIBehavior* Clone();
	float Evaluate();
	static AIBehavior* CreateAndGetBehavior(const TiXmlElement* elem);

protected:
	static AIBehaviorRegistration s_wanderRegistration;

private:
	float m_chanceOfMove;
	float m_chanceOfRest;
};

};

#endif