#include "ItemFactory.hpp"

#include "GameCode\Map.hpp"

#include "Engine\Parsing\TinyXML\TinyXMLHelper.hpp"
#include "Engine\Parsing\FileSystem\FileSystem.hpp"

namespace Henry
{

ItemFactorySet ItemFactory::s_itemFactorySet;


ItemFactory::ItemFactory()
{

}


ItemFactory::~ItemFactory()
{
	if(m_itemTemplate)
	{
		delete m_itemTemplate;
		m_itemTemplate = nullptr;
	}
}


void ItemFactory::LoadAndParseAllFilesUnder(const char* path)
{
	std::vector<std::string> allFiles;
	FileSystem files;
	std::string fullPath(path);

	fullPath.append("\\*.xml");
	files.EnumerateFileInFolder(fullPath.c_str(),allFiles);
	for(size_t index=0; index < allFiles.size(); index++)
	{
		std::string xmlPath(path);
		xmlPath.append(allFiles[index]);
		TinyXMLHelper xmlHelper;
		xmlHelper.LoadFile(xmlPath.c_str());
		for(TiXmlElement* elem = xmlHelper.m_root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			std::string elemName = elem->Value();
			if(elemName == "Item")
			{
				std::string name(elem->Attribute("name"));
				std::string glyph(elem->Attribute("glyph"));
				std::string color(elem->Attribute("color"));
				std::string category(elem->Attribute("category"));
				std::string value(elem->Attribute("effectValue"));
				std::string durability(elem->Attribute("durability"));
				std::string slot(elem->Attribute("slot") ? elem->Attribute("slot") : "");
				std::string attackRange(elem->Attribute("attackRange") ? elem->Attribute("attackRange") : "0");
				std::string weight(elem->Attribute("weight") ? elem->Attribute("weight") : "0" );
				std::string random(elem->Attribute("randomGenerate"));
				std::string against(elem->Attribute("againstRace") ? elem->Attribute("againstRace") : "");	

				Item* item = new Item(name , nullptr); //GameCommon::s_world->GetMap());
				item->m_glyph = glyph[0];
				sscanf_s( color.c_str() , "%d,%d,%d" , &item->m_color.r , &item->m_color.g , &item->m_color.b );
				item->m_color.a = 255;
				item->m_category = GetCatagoryOfItem(category);
				sscanf_s( value.c_str() , "%d,%d" , &item->m_minEffectValue , &item->m_maxEffectValue);
				item->m_durability = atoi(durability.c_str());
				item->m_equipmentSlot = GetSlotOfItem(slot);
				item->m_attackRange = atoi(attackRange.c_str());
				item->m_weight = (float)atof(weight.c_str());
				item->m_random = (random == "true") ? true : false;
				item->m_againstRace = against;

				ItemFactory* factory = new ItemFactory(); //pass root to create NPCFactory() instead of get the value here
				factory->m_itemTemplate = item;
				s_itemFactorySet.insert(factory);
			}
		}
	}
}


Item* ItemFactory::SpawnItem(Map* theMap , const Vec2i location)
{
	Item* item = nullptr;
	if(theMap->GetCellType(location.x,location.y) == AIR)
	{
		item = new Item(*m_itemTemplate);
		item->m_map = theMap;
		item->MoveTo(location);
	}
	return item;
}


ItemCategory ItemFactory::GetCatagoryOfItem(std::string& category)
{
	if(category == "Weapon")
		return ITEM_WEAPON;
	else if(category == "Armor")
		return 	ITEM_ARMOR;
	else if(category == "Potion")
		return 	ITEM_POTION;
	else if(category == "Wand")
		return ITEM_WAND;
	else if(category == "Light")
		return ITEM_LIGHT;
	else if(category == "Tool")
		return ITEM_TOOL;
	else if(category == "Ring")
		return ITEM_RING;
	else if(category == "Container")
		return ITEM_CONTAINER;
	else if(category == "Scroll")
		return ITEM_SCROLL;
	else if(category == "Key")
		return ITEM_KEY;
	else
		return ITEM_DEFAULT;
}


EquipmentSlot ItemFactory::GetSlotOfItem(std::string& slot)
{
	if (slot == "Helmet")
		return SLOT_HELMET;
	else if(slot == "Chest")
		return SLOT_CHEST;
	else if(slot == "Pants")
		return SLOT_PANTS;
	else if(slot == "Gloves")
		return SLOT_GLOVES;
	else if(slot == "Weapon")
		return SLOT_WEAPON;
	else if(slot == "Boots")
		return SLOT_BOOTS;
	else
		return SLOT_NONE;
}


ItemFactory* ItemFactory::FindFactory(const std::string& name)
{
	ItemFactory* target_factory = nullptr;
	ItemFactorySet::iterator iter = s_itemFactorySet.begin();
	while(iter != s_itemFactorySet.end())
	{
		ItemFactory* factory = *iter;
		if(factory->m_itemTemplate->m_name == name)
		{
			target_factory = factory;
			break;
		}
		iter++;
	}

	return target_factory;
}

};