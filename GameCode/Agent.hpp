#pragma once

#ifndef AGENT_HPP 
#define AGENT_HPP

#include "GameCode\GameEntity.hpp"
#include "GameCode\Item\Item.hpp"
#include "GameCode\FactionSystem.hpp"
#include "GameCode\CombatSystem.hpp"


namespace Henry
{


class Agent : public GameEntity
{
public:
	Agent(std::string& name,Map* currentMap);
	~Agent(void);
	bool MoveTo(const Vec2i location);
	bool CanMoveTo(const Vec2i location);
	bool TryGoToDirection(const Vec2i direction);
	void AutoEquip();
	void ResolveEvent(AttackData atk);
	void ApplyDamage(AttackData atkData);
	void SaveToXML(TiXmlElement* element);

	std::string m_faction;
	bool m_isInMap;
	AttackData* m_prevCombat;
	//int m_speed;
	Item* m_equipmentSlot[NUM_OF_SLOTS];

	std::vector<Agent*> m_agentsInSight;
	std::vector<Item*> m_potionInventory;
	std::vector<Item*> m_weaponInventory;
	std::vector<Item*> m_armorInventory;
	std::vector<Item*> m_wandInventory;

	FactionSystem* m_factionRelationship;
	//void DoFov();
	//void AddToMap();
	//void RemoveFromMap();
	//bool alreadyDoFov;
};

};

#endif