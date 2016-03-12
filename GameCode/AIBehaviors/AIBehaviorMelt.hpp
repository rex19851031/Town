#pragma once

#ifndef AIBEHAVIORWANDER_HPP 
#define AIBEHAVIORWANDER_HPP

#include "AIBehavior.hpp"
#include "AIBehaviorRegistration.hpp"

namespace Henry
{

class AIBehaviorMelt : public AIBehavior
{
public:
	AIBehaviorMelt(const TiXmlElement* xmlElem);
	~AIBehaviorMelt(void);
	bool Think();
	AIBehavior* Clone();
	float Evaluate();
	static AIBehavior* CreateAndGetBehavior(const TiXmlElement* elem);

protected:
	static AIBehaviorRegistration s_meltRegistration;
	int m_affectRdius;
};

};

#endif