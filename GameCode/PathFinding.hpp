#pragma once

#ifndef PATHFINDING_HPP 
#define PATHFINDING_HPP

#include "Engine\Math\Vec2.hpp"
#include "GameCode\Map.hpp"

#include <list>

namespace Henry
{

struct PathNode
{
	PathNode() : g(0.0f) , h(0.0f) , parentNode(nullptr) {};
	PathNode(Vec2i _pos,float _g = 0, float _h = 0) : position(_pos) , g(_g) , h(_h) , parentNode(nullptr) {};
	Vec2i position;
	float g;
	float h;
	PathNode* parentNode;
};

class PathFinding
{
public:
	PathFinding(void);
	~PathFinding(void);
	void FindPath(Vec2i startPoint,Vec2i endPoint,Map* theMap);
	
	std::vector<Vec2i> m_path;
private:
	float GetHeuristic(Vec2i startPoint,Vec2i endPoint);
	bool IsOnCloseList(Vec2i location);
	bool IsOnOpenList(Vec2i location , PathNode*& node);
	void ClearList();
	void GeneratePathVector(PathNode* endNode);

	std::list<PathNode*> m_openList;
	std::list<PathNode*> m_closeList;
};

};

#endif