#pragma once

#ifndef AIBEHAVIORWANDER_HPP 
#define AIBEHAVIORWANDER_HPP

#include "AIBehavior.hpp"
#include "AIBehaviorRegistration.hpp"

namespace Henry
{

class AIBehaviorDrains : public AIBehavior
{
public:
	AIBehaviorDrains(const TiXmlElement* xmlElem);
	~AIBehaviorDrains(void);
	bool Think();
	AIBehavior* Clone();
	float Evaluate();
	static AIBehavior* CreateAndGetBehavior(const TiXmlElement* elem);

protected:
	static AIBehaviorRegistration s_drainsRegistration;

private:
	float m_minDrain;
	float m_maxDrain;
	Agent* m_target;
};

};

#endif