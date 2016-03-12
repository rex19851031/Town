#include "GameCode\AIBehaviors\AIBehaviorCombat.hpp"
#include "GameCode\Map.hpp"
#include "GameCode\GameCommon.hpp"

#include "Engine\Core\HenryFunctions.hpp"


namespace Henry
{

AIBehaviorRegistration AIBehaviorCombat::s_combatRegistration("Combat",&AIBehaviorCombat::CreateAndGetBehavior);


AIBehavior* AIBehaviorCombat::CreateAndGetBehavior(const TiXmlElement* elem)
{
	return ( new AIBehaviorCombat(elem) );
}


AIBehaviorCombat::AIBehaviorCombat(const TiXmlElement* xmlElem) : AIBehavior(xmlElem)
{
	std::string damageValue = xmlElem->Attribute("damage");
	sscanf_s(damageValue.c_str(),"%d,%d",&m_minDamage,&m_maxDamage);
	m_chanceToHit = (float)std::atof(xmlElem->Attribute("chanceToHit"));
	m_attackRange = std::atoi(xmlElem->Attribute("attackRange"));
}


AIBehaviorCombat::~AIBehaviorCombat(void)
{
}


bool AIBehaviorCombat::Think()
{
	AttackData atk;
	atk.attacker = m_agent;
	atk.target = m_target;
	float damage = m_maxDamage * getRandomPercent();
	if(damage < m_minDamage)
		damage = (float)m_minDamage;
	
	atk.potentialDamageToHealth = damage;
	atk.attackType = ATTACK_MELEE;

	float rollDice = getRandomPercent();
	if(rollDice < m_chanceToHit)
		GameCommon::s_combatSystem.RequestCombat(atk);
	else
		GameCommon::s_UI->PostMessageToLog("%s Tried To Attack %s, But Missed",RGBA(),m_agent->m_name.c_str(),m_target->m_name.c_str());
	
	return true;
}


AIBehavior* AIBehaviorCombat::Clone()
{
	return new AIBehaviorCombat(*this);
}


float AIBehaviorCombat::Evaluate()
{
	bool enemyNearBy = false;
	Vec2i allDirection[8] = {	Vec2i(-1,-1) ,	Vec2i(0,-1) ,	Vec2i(1,-1), 
								Vec2i(-1,0)  ,					Vec2i(1,0) , 
								Vec2i(-1,1)  ,	Vec2i(0,1)  ,	Vec2i(1,1) };
	
	Map* currentMap = m_agent->m_map;
	Vec2i currentPosition = m_agent->m_position;
	Agent* agent = nullptr;
	int relationship = 0;

	for(int index=0; index < 8; index++)
	{
		Vec2i adjacentLocation = currentPosition + allDirection[index];
		agent = currentMap->GetAgent(adjacentLocation.x,adjacentLocation.y);
		if( agent && m_agent->m_factionRelationship->GetRelationshipValue(agent->m_faction) < 0 && m_agent->m_factionRelationship->GetRelationshipValue(agent->m_faction) < relationship)
		{
			enemyNearBy = true;
			m_target = agent;
			relationship = m_agent->m_factionRelationship->GetRelationshipValue(agent->m_faction);
		}
	}

	return enemyNearBy ? 10.0f : 0.0f;
}

};