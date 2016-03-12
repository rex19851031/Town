#pragma once

#ifndef GAMEENTITY_HPP 
#define GAMEENTITY_HPP

#include "Engine\Input\InputSystem.hpp"
#include "Engine\Math\GeneralStruct.hpp"
#include "Engine\Math\Vec2.hpp"
#include "Engine\Parsing\TinyXML\tinyxml.h"


namespace Henry
{

struct AttackData;
class Map;

class GameEntity
{
public:
	GameEntity(std::string& name,Map* currentMap);
	GameEntity(const GameEntity& entity);
	~GameEntity(void);
	virtual void Input();
	virtual void Update();
	virtual void Render();
	virtual void ApplyDamage(AttackData atkData);
	virtual void RemoveFromMap();
	virtual void AddToMap();
	virtual void Destory();
	void SaveToXML(TiXmlElement* element);

	static int s_nextGameEntityID;
	Vec2i m_position;
	std::string m_name;
	int m_id;
	int m_prev_id;
	char m_glyph;
	RGBA m_color;
	float m_maxHP;
	float m_currentHP;
	//int m_degugLevel;
	Map* m_map;
	bool m_isDead;
};

};

#endif