#include "PathFinding.hpp"

namespace Henry
{

bool compare_nodes (const PathNode* a , const PathNode* b)
{
	return (a->g + a->h) < (b->g + b->h);
};


PathFinding::PathFinding(void)
{
}


PathFinding::~PathFinding(void)
{

}


void PathFinding::FindPath(Vec2i startPoint,Vec2i endPoint,Map* theMap)
{
	float heuristic = GetHeuristic(startPoint,endPoint);
	m_openList.push_back(new PathNode(startPoint,0,heuristic));
	
	bool done = false;
	
	while(!done)
	{
		if(m_openList.size() == 0)
			break;

		m_openList.sort(compare_nodes);
		std::list<PathNode*>::iterator iter = m_openList.begin();
		PathNode* lowest_node = (*iter);
		m_openList.erase(iter);
		m_closeList.push_back(lowest_node);
		
		if(lowest_node->position == endPoint)
		{
			GeneratePathVector(lowest_node);
			break;
		}

		for(int x = lowest_node->position.x - 1; x <= lowest_node->position.x + 1; x++)
		{
			for(int y = lowest_node->position.y - 1; y <= lowest_node->position.y + 1; y++)
			{
				if(theMap->GetCellType(x,y) == WALL)
					continue;

				if(x == lowest_node->position.x && y == lowest_node->position.y)
					continue;

				float g_cost;
				float h_cost = GetHeuristic(Vec2i(x,y),endPoint);
				PathNode* nodeInOpenList;

				if(x == lowest_node->position.x || y == lowest_node->position.y)
					g_cost = 1.0f;
				else
					g_cost = 1.4f;
				
				if(IsOnCloseList(Vec2i(x,y)))
					continue;
				else if(IsOnOpenList(Vec2i(x,y),nodeInOpenList))
				{
					float new_g_cost = lowest_node->g + g_cost;
					if(nodeInOpenList->g <= new_g_cost)
						continue;
					else
					{
						nodeInOpenList->g = new_g_cost;
						nodeInOpenList->parentNode = lowest_node;
						continue;
					}
				}
				else 
				{
					PathNode* node = new PathNode(Vec2i(x,y),lowest_node->g + g_cost,h_cost);
					node->parentNode = lowest_node;
					m_openList.push_back(node);
				}
			}
		}
	}
}


bool PathFinding::IsOnOpenList(Vec2i location,PathNode*& node)
{
	bool exist = false;
	std::list<PathNode*>::iterator iter = m_openList.begin();
	while(iter != m_openList.end())
	{
		Vec2i fecthedLocation = (*iter)->position;
		if(fecthedLocation == location)
		{
			exist = true;
			node = (*iter);
			break;
		}
		iter++;
	}
	return exist;
}


bool PathFinding::IsOnCloseList(Vec2i location)
{
	bool exist = false;
	std::list<PathNode*>::iterator iter = m_closeList.begin();
	while(iter != m_closeList.end())
	{
		Vec2i fecthedLocation = (*iter)->position;
		if(fecthedLocation == location)
		{
			exist = true;
			break;
		}
		iter++;
	}
	return exist;
}


float PathFinding::GetHeuristic(Vec2i startPoint,Vec2i endPoint)
{
	int deltaX = abs(startPoint.x - endPoint.x);
	int deltaY = abs(startPoint.y - endPoint.y);
	float heuristic = (float)(deltaX + deltaY);
	return heuristic;
}


void PathFinding::GeneratePathVector(PathNode* endNode)
{
	std::vector<Vec2i> empty;
	m_path = empty;
	while(endNode)
	{
		m_path.push_back(endNode->position);
		endNode = endNode->parentNode;
	}
	ClearList();
}


void PathFinding::ClearList()
{
	std::list<PathNode*> empty;
	m_openList = empty;
	m_closeList = empty;
}

};