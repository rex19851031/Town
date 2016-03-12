#include "World.hpp"

#include "GameCode\GameCommon.hpp"

#include "Engine\Renderer\OpenGLRenderer.hpp"

#include <sstream>


namespace Henry
{

World::World(void)
{
	Map theMap;
	m_maps[0] = theMap;
	GameCommon::s_borderOffset = Vec2i(96,98);
}


World::~World(void)
{
	//delete m_generator;
}


void World::Render()
{
	int numOfCells = m_maps[0].m_mapSize.x * m_maps[0].m_mapSize.y;
	std::vector<Vertex_PosColor> m_list;
	m_list.reserve(numOfCells * 4);
	Vertex_PosColor box[4];
	for(int index=0; index < numOfCells; index++)
	{
		Cell& cell = m_maps[0].m_cells[index];

		box[0].position = Vec3f( (float)GameCommon::s_borderOffset.x	+ (index & 63) * cell.m_size.x					, (float)GameCommon::s_borderOffset.y + (index >> 6) * cell.m_size.y					, 0.0f);
		box[0].color = cell.GetColor();
		
		box[1].position = Vec3f( (float)GameCommon::s_borderOffset.x	+ (index & 63) * cell.m_size.x					, (float)GameCommon::s_borderOffset.y + (index >> 6) * cell.m_size.y + cell.m_size.y	, 0.0f);
		box[1].color = cell.GetColor();
		
		box[2].position = Vec3f( (float)GameCommon::s_borderOffset.x + (index & 63) * cell.m_size.x + cell.m_size.x		, (float)GameCommon::s_borderOffset.y + (index >> 6) * cell.m_size.y + cell.m_size.y	, 0.0f);
		box[2].color = cell.GetColor();
		
		box[3].position = Vec3f( (float)GameCommon::s_borderOffset.x + (index & 63) * cell.m_size.x + cell.m_size.x		, (float)GameCommon::s_borderOffset.y + (index >> 6) * cell.m_size.y					, 0.0f);
		box[3].color = cell.GetColor();
		
		if(	box[0].position.x <= OpenGLRenderer::WINDOW_SIZE.x - GameCommon::s_borderOffset.x && box[0].position.y <= OpenGLRenderer::WINDOW_SIZE.y - GameCommon::s_borderOffset.y &&
			box[1].position.x <= OpenGLRenderer::WINDOW_SIZE.x - GameCommon::s_borderOffset.x && box[1].position.y <= OpenGLRenderer::WINDOW_SIZE.y - GameCommon::s_borderOffset.y &&
			box[2].position.x <= OpenGLRenderer::WINDOW_SIZE.x - GameCommon::s_borderOffset.x && box[2].position.y <= OpenGLRenderer::WINDOW_SIZE.y - GameCommon::s_borderOffset.y &&
			box[3].position.x <= OpenGLRenderer::WINDOW_SIZE.x - GameCommon::s_borderOffset.x && box[3].position.y <= OpenGLRenderer::WINDOW_SIZE.y - GameCommon::s_borderOffset.y)
		{
			m_list.push_back(box[0]);
			m_list.push_back(box[1]);
			m_list.push_back(box[2]);
			m_list.push_back(box[3]);
		}
	}

	OpenGLRenderer::DrawVertexWithVertexArray(m_list,OpenGLRenderer::SHAPE_QUADS);
}


void World::ProcessOneStep()
{
	m_generator->ProcessOneStep(&m_maps[0]);
	m_steps++;
}


void World::GenerateMap()
{
	m_steps = 0;
	InitializeMap();

	if(m_generator)
		m_generator->GenerateMap(&m_maps[0]);
}


void World::SetGenerator(MapGenerator* generator)
{
	m_generator = generator;
}


void World::AutoGenerate()
{
	if(m_steps < m_generator->m_steps)
	{
		int timesOfStepLeft = m_generator->m_steps - m_steps;
		for(int steps = 0; steps < timesOfStepLeft; steps++)
			m_generator->ProcessOneStep(&m_maps[0]);
	} 
}


Map* World::GetMap(int mapIndex /* = 0 */)
{
	return &m_maps[mapIndex];
}


bool World::IsThisCellSolid(Vec2f location)
{
	if(m_maps[0].GetCellType((int)location.x,(int)location.y) != WALL)
		return true;
	return false;
}


int World::GetNumOfExploredCells()
{
	int result = 0;
	for(int index = 0; index < m_maps[0].m_mapSize.x * m_maps[0].m_mapSize.y; index++)
	{
		if(m_maps[0].m_cells[index].m_discovered)
			result++;
	}
	return result;
}


void World::SaveToXML(TiXmlElement* element)
{
	for(int y = m_maps[0].m_mapSize.y-1; y >= 0; --y)
	{
		std::ostringstream mapData;
		for(int x = 0; x < m_maps[0].m_mapSize.x; ++x)
		{
			mapData << m_maps[0].GetCellType(x,y);
		}

		TiXmlElement * sub_elem = new TiXmlElement("MapSlice");
		TiXmlText * text = new TiXmlText( mapData.str().c_str() );
		sub_elem->LinkEndChild( text );
		element->LinkEndChild(sub_elem);
		mapData.clear();
	}

	std::string visibilityData;
	for(int y = m_maps[0].m_mapSize.y-1; y >= 0; --y)
	{
		for(int x = 0; x < m_maps[0].m_mapSize.x; ++x)
		{
			if(m_maps[0].GetDiscoveredFlag(x,y))
				visibilityData.append("1");
			else
				visibilityData.append("0");
		}

		TiXmlElement * sub_elem = new TiXmlElement("Visibility");
		TiXmlText * text = new TiXmlText( visibilityData.c_str() );
		sub_elem->LinkEndChild( text );
		element->LinkEndChild(sub_elem);
		visibilityData.clear();
	}
}


void World::InitializeMap()
{
	std::vector<Item*> empty;

	for(int index = 0; index < m_maps[0].m_mapSize.x * m_maps[0].m_mapSize.y; index++)
	{
		m_maps[0].SetAgent(index,nullptr);
		m_maps[0].SetFeature(index,nullptr);
		m_maps[0].m_cells[index].m_entitys = empty;
	}
}


};