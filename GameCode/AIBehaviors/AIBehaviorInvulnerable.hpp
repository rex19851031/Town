#pragma once

#ifndef AIBEHAVIORWANDER_HPP 
#define AIBEHAVIORWANDER_HPP

#include "AIBehavior.hpp"
#include "AIBehaviorRegistration.hpp"

namespace Henry
{

class AIBehaviorInvulnerable : public AIBehavior
{
public:
	AIBehaviorInvulnerable(const TiXmlElement* xmlElem);
	~AIBehaviorInvulnerable(void);
	bool Think();
	AIBehavior* Clone();
	float Evaluate();
	static AIBehavior* CreateAndGetBehavior(const TiXmlElement* elem);

protected:
	static AIBehaviorRegistration s_invulnerableRegistration;

private:
	std::string m_weakness;
};

};

#endif