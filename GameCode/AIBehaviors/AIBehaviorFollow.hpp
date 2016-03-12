#pragma once

#ifndef AIBEHAVIORWANDER_HPP 
#define AIBEHAVIORWANDER_HPP

#include "AIBehavior.hpp"
#include "AIBehaviorRegistration.hpp"

#include "GameCode\PathFinding.hpp"

namespace Henry
{

class AIBehaviorFollow : public AIBehavior
{
public:
	AIBehaviorFollow(const TiXmlElement* xmlElem);
	~AIBehaviorFollow(void);
	bool Think();
	AIBehavior* Clone();
	float Evaluate();
	static AIBehavior* CreateAndGetBehavior(const TiXmlElement* elem);

protected:
	static AIBehaviorRegistration s_followRegistration;

private:
	std::string m_leaderName;
	float m_threshold;
	Agent* m_leader;
	PathFinding m_pathFinding;
};

};

#endif