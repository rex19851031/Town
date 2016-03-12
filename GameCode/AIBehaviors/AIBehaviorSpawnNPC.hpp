#pragma once

#ifndef AIBEHAVIORWANDER_HPP 
#define AIBEHAVIORWANDER_HPP

#include "AIBehavior.hpp"
#include "AIBehaviorRegistration.hpp"

namespace Henry
{

class AIBehaviorSpawnNPC : public AIBehavior
{
public:
	AIBehaviorSpawnNPC(const TiXmlElement* xmlElem);
	~AIBehaviorSpawnNPC(void);
	bool Think();
	AIBehavior* Clone();
	float Evaluate();
	static AIBehavior* CreateAndGetBehavior(const TiXmlElement* elem);

protected:
	static AIBehaviorRegistration s_spawnNPCRegistration;

private:
	std::string m_npcName;
	int m_spawnThreshold;
	int m_numOfSameTypeNPCNearBy;
	float m_changeToSpawn;
};

};

#endif