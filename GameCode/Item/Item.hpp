#pragma once

#ifndef ITEM_HPP 
#define ITEM_HPP

#include "GameCode\GameEntity.hpp"

#include <string>

namespace Henry
{

enum EquipmentSlot
{
	SLOT_NONE,
	SLOT_WEAPON,
	SLOT_HELMET,
	SLOT_CHEST,
	SLOT_PANTS,
	SLOT_GLOVES,
	SLOT_BOOTS,
	NUM_OF_SLOTS,
};

enum ItemCategory
{
	ITEM_DEFAULT,
	ITEM_WEAPON,
	ITEM_ARMOR,
	ITEM_POTION,
	ITEM_WAND,
	ITEM_LIGHT,
	ITEM_TOOL,
	ITEM_RING,
	ITEM_CONTAINER,
	ITEM_SCROLL,
	ITEM_KEY,
	NUM_OF_CATEGORY,
};

class Agent;
class Item : public GameEntity
{
public:
	Item(std::string& name,Map* currentMap);
	~Item();
	void Use();
	void Throw();
	void Consume();
	void Drop();
	void MoveTo(const Vec2i location);
	void SaveToXML(TiXmlElement* element);
	bool CanMoveTo(const Vec2i location);

public:
	ItemCategory m_category;
	EquipmentSlot m_equipmentSlot;
	float m_weight;
	int m_minEffectValue;
	int m_maxEffectValue;
	int m_durability;
	int m_attackRange;
	Agent* m_owner;
	bool m_beenPicked;
	bool m_isBroke;
	bool m_random;
	std::string m_againstRace;
};

};

#endif