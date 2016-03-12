#pragma once

#ifndef AIBEHAVIORWANDER_HPP 
#define AIBEHAVIORWANDER_HPP

#include "AIBehavior.hpp"
#include "AIBehaviorRegistration.hpp"

namespace Henry
{

class AIBehaviorHeal : public AIBehavior
{
public:
	AIBehaviorHeal(const TiXmlElement* xmlElem);
	~AIBehaviorHeal(void);
	bool Think();
	AIBehavior* Clone();
	float Evaluate();
	static AIBehavior* CreateAndGetBehavior(const TiXmlElement* elem);

protected:
	static AIBehaviorRegistration s_healRegistration;

private:
	int m_threshold;
	int m_min;
	int m_max;
	Agent* m_target;
};

};

#endif