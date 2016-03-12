#include "AIBehaviorInvulnerable.hpp"

#include "Engine\Core\HenryFunctions.hpp"

namespace Henry
{

AIBehaviorRegistration AIBehaviorInvulnerable::s_invulnerableRegistration("Invulnerable",&AIBehaviorInvulnerable::CreateAndGetBehavior);


AIBehavior* AIBehaviorInvulnerable::CreateAndGetBehavior(const TiXmlElement* elem)
{
	return ( new AIBehaviorInvulnerable(elem) );
}


AIBehaviorInvulnerable::AIBehaviorInvulnerable(const TiXmlElement* xmlElem) : AIBehavior(xmlElem)
{
	m_weakness = xmlElem->Attribute("weakness");
}


AIBehaviorInvulnerable::~AIBehaviorInvulnerable(void)
{
}


bool AIBehaviorInvulnerable::Think()
{
	return true;
}


AIBehavior* AIBehaviorInvulnerable::Clone()
{
	return new AIBehaviorInvulnerable(*this);
}


float AIBehaviorInvulnerable::Evaluate()
{
	if(m_agent && m_agent->m_prevCombat)
	{
		if(m_agent->m_prevCombat->attacker != nullptr)
		{
			
			std::string attackWeapon = "";
			
			if(m_agent->m_prevCombat->attacker->m_equipmentSlot[SLOT_WEAPON])
				attackWeapon = m_agent->m_prevCombat->attacker->m_equipmentSlot[SLOT_WEAPON]->m_name;
			
			if(attackWeapon != m_weakness)
				m_agent->m_currentHP += m_agent->m_prevCombat->actualDamageToHealth;

		}
	}

	return 1.0f;
}

};