#pragma once

#include "AIBehavior.hpp"
#include "AIBehaviorRegistration.hpp"

#include "GameCode\PathFinding.hpp"

namespace Henry
{

class AIBehaviorChase : public AIBehavior
{
public:
	AIBehaviorChase(const TiXmlElement* xmlElem);
	~AIBehaviorChase(void);
	AIBehavior* Clone();
	bool Think();
	float Evaluate();

	static AIBehavior* CreateAndGetBehavior(const TiXmlElement* elem);
	bool m_chasing;
protected:
	static AIBehaviorRegistration s_chaseRegistration;
	
private:
	Vec2i m_goal;
	PathFinding m_pathFinding;
};

};