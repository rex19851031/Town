#include "Player.hpp"

#include "GameCode\TheApplication.hpp"
#include "GameCode\GameCommon.hpp"

#include "Engine\Renderer\OpenGLRenderer.hpp"
#include "Engine\Core\HenryFunctions.hpp"

#include <vector>

namespace Henry
{

extern TheApplication* g_theApplication;


Player::Player(std::string& name,Map* currentMap) : Agent(name,currentMap)
{
	m_color = RGBA(1.0f,1.0f,1.0f,1.0f);
	m_characterSize = GameCommon::s_cellSize;
	m_faction = "Human";
	m_maxHP = 15.0f;
	m_currentHP = m_maxHP;
	m_minDamage = 2.0f;
	m_maxDamage = 4.0f;
	m_chanceToHit = 0.75f;

	m_totalTurns = 0;
	m_numOfMonsterKilled = 0;
	m_numOfItemPicked = 0;
}


Player::~Player(void)
{
}


void Player::Input()
{
	m_doAnything = false;
	Vec2i intentMoveDirection;

	if(GameCommon::s_input->PressedOnce(Inputs::INPUT_SPACE))
		GameCommon::s_UI->ReadNextMessage();

	if(!GameCommon::s_UI->AllMessageRead())
		return;

	if(GameCommon::s_input->PressedOnce('A') || GameCommon::s_input->PressedOnce('H'))
		intentMoveDirection = Vec2i(-1,0);

	if(GameCommon::s_input->PressedOnce('S') || GameCommon::s_input->PressedOnce('J'))
		intentMoveDirection = Vec2i(0,-1);

	if(GameCommon::s_input->PressedOnce('W') || GameCommon::s_input->PressedOnce('K'))
		intentMoveDirection = Vec2i(0,1);

	if(GameCommon::s_input->PressedOnce('D') || GameCommon::s_input->PressedOnce('L'))
		intentMoveDirection = Vec2i(1,0);

	if(GameCommon::s_input->PressedOnce('Y'))
		intentMoveDirection = Vec2i(-1,1);

	if(GameCommon::s_input->PressedOnce('U'))
		intentMoveDirection = Vec2i(1,1);

	if(GameCommon::s_input->PressedOnce('B'))
		intentMoveDirection = Vec2i(-1,-1);

	if(GameCommon::s_input->PressedOnce('N'))
		intentMoveDirection = Vec2i(-1,1);

	if(GameCommon::s_input->PressedOnce(Inputs::INPUT_OEM_COMMA))
	{
		m_doAnything = true;
		std::vector<Item*> pickUpItems;
		m_map->GetItemsOn(m_position,pickUpItems);
		if(pickUpItems.size() == 0)
			GameCommon::s_UI->PostMessageToLog("Nothing Here To Pick");
		else
			PickAndSortItem(pickUpItems);
	}
	
	if(GameCommon::s_input->PressedOnce(Inputs::INPUT_OEM_2))
	{
		m_doAnything = true;
		if(m_wandInventory.size() > 0)
		{
			std::vector<Item*>::iterator iter = m_wandInventory.begin();
			Item* wand = *iter;
			wand->Use();
		}
		else
			GameCommon::s_UI->PostMessageToLog("You Don't Have Any Wand To Use.");
	}

	if(GameCommon::s_input->PressedOnce(Inputs::INPUT_OEM_PERIOD) || GameCommon::s_input->PressedOnce('Q'))
	{
		m_doAnything = true;
		if(m_potionInventory.size() > 0)
		{
			std::vector<Item*>::iterator iter = m_potionInventory.begin();
			Item* potion = *iter;
			m_potionInventory.erase(iter);
			potion->Consume();
			GameCommon::s_UI->PostMessageToLog("You Feel Better.");
			GameCommon::s_UI->PostMessageToLog("You Drink The Potion.");
		}
		else
			GameCommon::s_UI->PostMessageToLog("You Don't Have Any Potion To Drink.");
	}

	if( intentMoveDirection != Vec2i(0,0) && MoveTo(m_position + intentMoveDirection) )
		m_doAnything = true;
	else if( intentMoveDirection != Vec2i(0,0) )
	{
		Vec2i direction = m_position + intentMoveDirection;
		Agent* target = m_map->GetAgent(direction.x,direction.y);
		if(target)
		{
			m_doAnything = true;
			
			AttackData atk;
			atk.attacker = this;
			atk.target = target;
			float damage = m_maxDamage * getRandomPercent();
			if(damage < m_minDamage)
				damage = m_minDamage;

			atk.potentialDamageToHealth = damage;
			atk.attackType = ATTACK_WEAPON;

			if(!m_equipmentSlot[SLOT_WEAPON])
				atk.attackType = ATTACK_MELEE;

			if(m_equipmentSlot[SLOT_WEAPON])
			{
				int damage = (int)(m_equipmentSlot[SLOT_WEAPON]->m_maxEffectValue * getRandomPercent());
				if(damage < m_equipmentSlot[SLOT_WEAPON]->m_minEffectValue)
					damage = m_equipmentSlot[SLOT_WEAPON]->m_minEffectValue;

				atk.potentialDamageToHealth += damage;
				m_equipmentSlot[SLOT_WEAPON]->m_durability--;
				atk.attackType = ATTACK_WEAPON;

				if(m_equipmentSlot[SLOT_WEAPON]->m_durability <= 0)
				{
					GameCommon::s_UI->PostMessageToLog("Weapon '%s' Broke.", RGBA(1.0f,0.0f,1.0f,1.0f),m_equipmentSlot[SLOT_WEAPON]->m_name.c_str());
					//delete m_equipmentSlot[SLOT_WEAPON];
					m_equipmentSlot[SLOT_WEAPON]->m_isBroke = true;
					m_equipmentSlot[SLOT_WEAPON] = nullptr;
				}
			}
			
			float rollDice = getRandomPercent();
			if(rollDice < m_chanceToHit)
			{
				if(m_equipmentSlot[SLOT_WEAPON] && target->m_faction == m_equipmentSlot[SLOT_WEAPON]->m_againstRace)
				{
					atk.potentialDamageToHealth += 5;
					GameCommon::s_UI->PostMessageToLog("The %s is burned by the purifying light." , RGBA(0.2f,0.4f,0.8f,1.0f) , target->m_name.c_str());
					GameCommon::s_UI->PostMessageToLog("The mace glows as it hits the %s." , RGBA(0.2f,0.4f,0.8f,1.0f) , target->m_name.c_str());
				}
				else
					GameCommon::s_UI->PostMessageToLog("You Attack The %s , And Deal With %.0f Damage!!" , RGBA(0.2f,0.4f,0.8f,1.0f) , target->m_name.c_str() , damage);
				
				GameCommon::s_combatSystem.RequestCombat(atk);
			}
			else
				GameCommon::s_UI->PostMessageToLog("You Tried To Attack The %s , But Missed!!" , RGBA(0.2f,0.4f,0.8f,1.0f) , target->m_name.c_str() , damage);
			
			if(atk.defenderDies)
				m_numOfMonsterKilled++;
		}

		Feature* feature = m_map->GetFeature(direction);
		if(feature)
		{
			m_doAnything = true;
			feature->m_isActivate = true;
			feature->m_isWalkable = true;
			GameCommon::s_UI->PostMessageToLog("You Opened a door!!");
		}
	}

	if(m_doAnything)
		m_totalTurns++;
}


void Player::Update()
{
	GameCommon::PoorMansFOV(this,20,true,20);
	AutoEquip();

	if(m_currentHP > m_maxHP)
		m_currentHP = m_maxHP;

	if(m_currentHP <= 0)
	{
		m_currentHP = 0;
		m_isDead = true;
	}
}


void Player::Render()
{
	Vec2f position = Vec2f((float)(GameCommon::s_borderOffset.x + m_position.x * m_characterSize.x),(float)(GameCommon::s_borderOffset.y + m_position.y * m_characterSize.y));
	GameCommon::s_drawingFont->Draw("@",position,(float)GameCommon::s_cellSize.y,m_color,g_theApplication->m_windowSize);
}


void Player::ApplyDamage(AttackData atkData)
{
	m_currentHP -= atkData.actualDamageToHealth;
	GameCommon::s_UI->PostMessageToLog("You Lost %.0f Health From Been Attack by %s!!",RGBA(0.8f,0.2f,0.2f,1.0f),atkData.actualDamageToHealth,atkData.attacker->m_name.c_str());
}


void Player::PickAndSortItem(std::vector<Item*> entityList)
{
	for(size_t index=0; index < entityList.size(); index++)
	{
		m_numOfItemPicked++;

		Item* item = entityList[index];
		item->m_beenPicked = true;
		item->m_owner = this;
		
		if( item->m_category == ITEM_WEAPON )
		{
			m_weaponInventory.push_back( item );
		}

		if( item->m_category == ITEM_ARMOR )
		{
			m_armorInventory.push_back( item );
		}

		if( item->m_category == ITEM_POTION )
		{
			m_potionInventory.push_back( item );
		}

		if(item->m_category == ITEM_WAND)
		{
			m_wandInventory.push_back( item );
		}

		GameCommon::s_UI->PostMessageToLog("You Pick Up An Item : %s",RGBA(),item->m_name.c_str());
	}
}


bool Player::MoveTo(const Vec2i location)
{
	if(CanMoveTo(location))
	{
		m_map->SetAgent(m_position.x,m_position.y,nullptr);
		m_map->SetAgent(location.x,location.y,this);
		m_map->RevealItems(location);
		m_position = location;
		return true;
	}

	return false;
}


void Player::SaveToXML(TiXmlElement* element)
{
	element->SetAttribute("totalTurns" , m_totalTurns);
	element->SetAttribute("monsterKilled" , m_numOfMonsterKilled);
	element->SetAttribute("itemPicked" , m_numOfItemPicked);

	Agent::SaveToXML(element);
}


};