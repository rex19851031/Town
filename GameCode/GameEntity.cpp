#include "GameEntity.hpp"
#include "GameCode\Map.hpp"
#include "GameCode\CombatSystem.hpp"
#include "GameCode\GameCommon.hpp"

#include <sstream>

namespace Henry
{

int GameEntity::s_nextGameEntityID;


GameEntity::GameEntity(std::string& name,Map* currentMap) : m_id(s_nextGameEntityID++) , m_name(name) , m_map(currentMap) , m_currentHP(100.0f) , m_maxHP(100.0f) , m_isDead(false)
{
}


GameEntity::GameEntity(const GameEntity& entity) 
	:	m_id(s_nextGameEntityID++)  
	,	m_name(entity.m_name)
	,	m_map(entity.m_map)
	,	m_isDead(false)
	,	m_glyph(entity.m_glyph)
	,	m_position(entity.m_position)
	,	m_color(entity.m_color)
	,	m_currentHP(entity.m_currentHP) 
	,	m_maxHP(entity.m_maxHP)
{
}


GameEntity::~GameEntity(void)
{
}


void GameEntity::Input()
{

}


void GameEntity::Update()
{

}


void GameEntity::Render()
{
	Vec2f position = Vec2f((float)(GameCommon::s_borderOffset.x + m_position.x * GameCommon::s_cellSize.x),(float)(GameCommon::s_borderOffset.y + m_position.y * GameCommon::s_cellSize.y));
	std::string glyph(1, m_glyph);
	GameCommon::s_drawingFont->Draw(glyph,position,(float)GameCommon::s_cellSize.y,m_color,GameCommon::s_windowSize);
}


void GameEntity::ApplyDamage(AttackData atkData)
{
	m_currentHP -= atkData.actualDamageToHealth;
	if(m_currentHP <= 0)
	{
		GameCommon::s_UI->PostMessageToLog("'%s' Broke!",RGBA(),m_name.c_str());
		m_isDead = true;
	}
}


void GameEntity::AddToMap()
{
	//m_map->SetEntity(m_position.x,m_position.y,this);
}


void GameEntity::RemoveFromMap()
{
	m_map->SetAgent(m_position.x,m_position.y,nullptr);
	m_map = nullptr;
}


void GameEntity::Destory()
{
	//m_map->setEntity(m_position.x,POSITION->y,this);
	RemoveFromMap();
	delete this;
}


void GameEntity::SaveToXML(TiXmlElement* element)
{
	std::ostringstream position;
	position << m_position.x << "," << m_position.y;

	std::ostringstream currentHealth;
	currentHealth << m_currentHP;

	element->SetAttribute( "name" , m_name.c_str() );
	element->SetAttribute( "id" , m_id );
	element->SetAttribute( "location" , position.str().c_str() );
	
	if(m_currentHP != 100)
		element->SetAttribute( "health" , currentHealth.str().c_str() );
}

};