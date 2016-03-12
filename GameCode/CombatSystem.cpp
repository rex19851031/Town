#include "CombatSystem.hpp"
#include "GameCode\GameCommon.hpp"
#include "GameCode\Agent.hpp"

#include "Engine\Core\HenryFunctions.hpp"

namespace Henry
{

CombatSystem::CombatSystem(void)
{
}


CombatSystem::~CombatSystem(void)
{
}


void CombatSystem::RequestCombat(AttackData& atkData)
{
	float totalArmor = 0;
	for(int index=SLOT_HELMET; index < NUM_OF_SLOTS; index++)
	{
		Agent* target = (Agent*)(atkData.target);
		if( target->m_equipmentSlot[index])
		{
			Item* armor = target->m_equipmentSlot[index];
			totalArmor += armor->m_minEffectValue;
			armor->m_durability--;
			if(armor->m_durability <= 0)
			{
				GameCommon::s_UI->PostMessageToLog("Armor '%s' Broke." , RGBA(1.0f,0.0f,1.0f,1.0f) , target->m_equipmentSlot[index]->m_name.c_str());
				//delete target->m_equipmentSlot[index];
				target->m_equipmentSlot[index]->m_isBroke = true;
				target->m_equipmentSlot[index] = nullptr;
			}
		}
	}

	atkData.attackResult = RESULT_HIT;
	atkData.actualDamageToHealth = atkData.potentialDamageToHealth - (totalArmor * getRandomPercent());

	if(atkData.actualDamageToHealth < 0)
		atkData.actualDamageToHealth = 0;
	
	atkData.target->ApplyDamage(atkData);

	if(atkData.target->m_isDead)
		atkData.defenderDies = true;
	else
		atkData.defenderDies = false;

	std::vector<NPC*>::iterator iter = GameCommon::s_npcs->begin();
	while(iter != GameCommon::s_npcs->end())
	{
		NPC* npc = *iter;
		npc->ResolveEvent(atkData);
		iter++;
	}
}

};