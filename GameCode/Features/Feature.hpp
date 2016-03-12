#pragma once

#include "GameCode\GameEntity.hpp"

#include <vector>

namespace Henry
{

class Feature : public GameEntity
{
public:
	Feature(std::string& name,Map* currentMap);
	~Feature(void);
	void Update();
	void SaveToXML(TiXmlElement* element);
	void MoveTo(const Vec2i location);
	bool CanMoveTo(const Vec2i location);

	bool m_isWalkable;
	bool m_isActivate;
	bool m_isBroke;

	char m_glyphOn;
	char m_glyphOff;

	std::string m_type;
	int m_radius;
	int m_affectValue;
	
private:
	Vec2i m_drawSize;
};

};