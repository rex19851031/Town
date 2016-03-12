#pragma once

#ifndef ITEMFACTORY_HPP
#define ITEMFACTORY_HPP

#include "GameCode\Item\Item.hpp"

#include <set>

namespace Henry
{

class ItemFactory;
typedef std::set<ItemFactory*> ItemFactorySet;

class ItemFactory
{
public:
	ItemFactory();
	~ItemFactory();
	Item* SpawnItem(Map* theMap , const Vec2i location);
	static ItemFactory* FindFactory(const std::string& name);
	static void LoadAndParseAllFilesUnder(const char* path);
	static ItemFactorySet s_itemFactorySet;
private:
	static ItemCategory GetCatagoryOfItem(std::string& category);
	static EquipmentSlot GetSlotOfItem(std::string& slot);
	Item* m_itemTemplate;
};

};

#endif