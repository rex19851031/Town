#pragma once

#ifndef PLAYER_HPP 
#define PLAYER_HPP

#include "Agent.hpp"

#include "Engine\Renderer\BitmapFont.hpp"

#include "GameCode\Item\Item.hpp"

namespace Henry
{

class Player :	public Agent
{
public:
	Player(std::string& name,Map* currentMap);
	~Player(void);
	void Input();
	void Update();
	void Render();
	void Attack();
	void ApplyDamage(AttackData atkData);
	void SaveToXML(TiXmlElement* element);
	bool MoveTo(const Vec2i location);

	bool m_doAnything;
	int m_totalTurns;
	int m_numOfMonsterKilled;
	int m_numOfItemPicked;

private:
	void PickAndSortItem(std::vector<Item*> entityList);
	Vec2i m_characterSize;
	float m_minDamage;
	float m_maxDamage;
	float m_chanceToHit;
};

};

#endif