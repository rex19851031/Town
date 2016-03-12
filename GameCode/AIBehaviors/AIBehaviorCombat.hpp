#pragma once

#ifndef AIBEHAVIORCOMBAT_HPP 
#define AIBEHAVIORCOMBAT_HPP

#include "GameCode\AIBehaviors\AIBehavior.hpp"
#include "GameCode\AIBehaviors\AIBehaviorRegistration.hpp"

namespace Henry
{

class AIBehaviorCombat : public AIBehavior
{
public:
	AIBehaviorCombat(const TiXmlElement* xmlElem);
	~AIBehaviorCombat();

	bool Think();
	AIBehavior* Clone();
	float Evaluate();
	static AIBehavior* CreateAndGetBehavior(const TiXmlElement* elem);

protected:
	static AIBehaviorRegistration s_combatRegistration;

private:
	int m_minDamage;
	int m_maxDamage;
	int m_attackRange;
	float m_chanceToHit;
	Agent* m_target;
};

};

#endif