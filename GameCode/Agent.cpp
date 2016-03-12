#include "Agent.hpp"
#include "GameCode\Map.hpp"
#include "GameCommon.hpp"

#include <sstream>


namespace Henry
{

Agent::Agent(std::string& name,Map* currentMap) 
	:	GameEntity(name,currentMap) 
	,	m_factionRelationship(nullptr) 
	,	m_prevCombat(nullptr)
{
	for(int index = 0; index < NUM_OF_SLOTS; index++)
		m_equipmentSlot[index] = nullptr;
}


Agent::~Agent(void)
{
	delete m_prevCombat;
}


bool Agent::CanMoveTo(const Vec2i location)
{
	if(m_map->GetCellType(location.x,location.y) == AIR || m_map->GetCellType(location.x,location.y) == ICE )
	{
		if(m_map->GetAgent(location.x,location.y) == nullptr)
		{
			if(m_map->GetFeature(location))
			{
				if(m_map->GetFeature(location)->m_isWalkable)
					return true;
				else
					return false;
			}
			else
				return true;
		}
	}
	return false;
}


bool Agent::MoveTo(const Vec2i location)
{
	if(CanMoveTo(location))
	{
		m_map->SetAgent(m_position.x,m_position.y,nullptr);
		m_map->SetAgent(location.x,location.y,this);
		m_position = location;
		return true;
	}

	return false;
}


bool Agent::TryGoToDirection(const Vec2i direction)
{
	bool moveSuccess = false;
	Vec2i newLocation = m_position + direction;
	
	if(CanMoveTo(newLocation))
	{
		MoveTo(newLocation);
		moveSuccess = true;
	}

	return moveSuccess;
}


void Agent::AutoEquip()
{
	if(m_weaponInventory.size() != 0)
	{
		std::vector<Item*>::iterator iter = m_weaponInventory.begin();
		Item* weapon = *iter;
		EquipmentSlot slotForThisItem = weapon->m_equipmentSlot;
		if(m_equipmentSlot[slotForThisItem] == nullptr)
		{
			m_equipmentSlot[slotForThisItem] = weapon;
			m_weaponInventory.erase(iter);
		}
	}

	if(m_armorInventory.size() != 0)
	{
		std::vector<Item*>::iterator iter = m_armorInventory.begin();
		Item* armor = *iter;
		EquipmentSlot slotForThisItem = armor->m_equipmentSlot;
		if(m_equipmentSlot[slotForThisItem] == nullptr)
		{
			m_equipmentSlot[slotForThisItem] = armor;
			m_armorInventory.erase(iter);
		}
	}
}


void Agent::ResolveEvent(AttackData atk)
{
	int attackerRelationship = m_factionRelationship->GetRelationshipValue(atk.attacker->m_name);
	int defenderRelationship = m_factionRelationship->GetRelationshipValue(atk.target->m_name);
	
	bool attackerInSight = false;
	bool defenderInSight = false;
	
	std::vector<Agent*>::iterator iter = m_agentsInSight.begin();
	while(iter!=m_agentsInSight.end())
	{
		Agent* agent = *iter;

		if(agent == atk.attacker)
			attackerInSight = true;

		if(agent == atk.target)
			defenderInSight = true;

		if(attackerInSight && defenderInSight)
			break;

		iter++;
	}

	if(!attackerInSight || !defenderInSight)
		return;

	if(atk.target == this)
	{
		m_factionRelationship->AdjustRelationship(atk.attacker->m_faction,-2);
	}

	if(attackerRelationship > 0)
	{
		m_factionRelationship->AdjustRelationship(((Agent*)atk.target)->m_faction,-2);
	}

	if(defenderRelationship > 0)
	{
		m_factionRelationship->AdjustRelationship(atk.attacker->m_faction,-2);
	}
}


void Agent::ApplyDamage(AttackData atkData)
{
	m_currentHP -= atkData.actualDamageToHealth;
	m_factionRelationship->AdjustRelationship(atkData.attacker->m_faction,-1);
	if(m_currentHP <= 0 && atkData.target->m_map->GetVisibility(atkData.target->m_position))
	{
		GameCommon::s_UI->PostMessageToLog("'%s' died!",RGBA(),m_name.c_str());
		m_isDead = true;
	}

 	m_prevCombat = new AttackData(atkData);
}


void Agent::SaveToXML(TiXmlElement* element)
{
	GameEntity::SaveToXML(element);
	if(m_factionRelationship)
		m_factionRelationship->SaveToXML(element);

	element->SetAttribute("faction" , m_faction.c_str() );

	std::ostringstream equipment;
	std::ostringstream durability;

	for(int index=0; index < NUM_OF_SLOTS; ++index)
	{
		if(m_equipmentSlot[index])
		{
			equipment	<< m_equipmentSlot[index]->m_id;
			durability	<< m_equipmentSlot[index]->m_durability;
		}
		else
		{
			equipment	<< "-1";
			durability	<< "-1";
		}

		if(index != NUM_OF_SLOTS - 1)
		{
			equipment << ",";
			durability << ",";
		}
	}

	element->SetAttribute("equipment" , equipment.str().c_str());
	element->SetAttribute("durability" , durability.str().c_str());

	if(m_weaponInventory.size() != 0)
	{
		TiXmlElement* sub_element_weapons = new TiXmlElement("Weapons");
		element->LinkEndChild(sub_element_weapons);
		
		for(size_t index = 0; index < m_weaponInventory.size(); ++index)
		{
			TiXmlElement* weapon = new TiXmlElement("Weapon");
			weapon->SetAttribute( "name"		, m_weaponInventory[index]->m_name.c_str() );
			weapon->SetAttribute( "id"			, m_weaponInventory[index]->m_id );
			weapon->SetAttribute( "durability"	, m_weaponInventory[index]->m_durability );
			sub_element_weapons->LinkEndChild(weapon);
		}
	}

	if(m_armorInventory.size() != 0)
	{
		TiXmlElement* sub_element_armors = new TiXmlElement("Armors");
		element->LinkEndChild(sub_element_armors);

		for(size_t index = 0; index < m_armorInventory.size(); ++index)
		{
			TiXmlElement* armor = new TiXmlElement("Armor");
			armor->SetAttribute( "name"			, m_armorInventory[index]->m_name.c_str() );
			armor->SetAttribute( "id"			, m_armorInventory[index]->m_id );
			armor->SetAttribute( "durability"	, m_armorInventory[index]->m_durability );
			sub_element_armors->LinkEndChild(armor);
		}
	}

	if(m_potionInventory.size() != 0)
	{
		TiXmlElement* sub_element_potions = new TiXmlElement("Potions");
		element->LinkEndChild(sub_element_potions);

		for(size_t index = 0; index < m_potionInventory.size(); ++index)
		{
			TiXmlElement* potion = new TiXmlElement("Potion");
			potion->SetAttribute( "name"	, m_potionInventory[index]->m_name.c_str() );
			potion->SetAttribute( "id"		, m_potionInventory[index]->m_id );
			sub_element_potions->LinkEndChild(potion);
		}
	}

	if(m_wandInventory.size() != 0)
	{
		TiXmlElement* sub_element_wand = new TiXmlElement("Wands");
		element->LinkEndChild(sub_element_wand);

		for(size_t index = 0; index < m_wandInventory.size(); ++index)
		{
			TiXmlElement* wand = new TiXmlElement("Wand");
			wand->SetAttribute( "name"	, m_wandInventory[index]->m_name.c_str() );
			wand->SetAttribute( "id"	, m_wandInventory[index]->m_id );
			sub_element_wand->LinkEndChild(wand);
		}
	}
}


};