#include "UI.hpp"

#include "Engine\Core\VertexStruct.hpp"
#include "Engine\Renderer\OpenGLRenderer.hpp"

#include "GameCode\GameCommon.hpp"
#include "GameCode\Player.hpp"

#include <stdarg.h>
#include <sstream>

namespace Henry
{

UI::UI(void)
{
	m_healthBar.min = Vec2f(100.0f,20.0f);
	m_healthBar.max = Vec2f(400.0f,50.0f);
	m_currentStartDrawingLineNum = 0;
	m_font = new BitmapFont(".\\Data\\BMPFonts\\buxton_nomal.fnt");
}


UI::~UI(void)
{
}


void UI::Draw()
{
	DrawHealthBar();
	DrawMessage();
	DrawEquipmentInfo();
}


void UI::DrawHealthBar()
{
	std::string name("Name : ");
	name += m_displayAgent->m_name;

	std::ostringstream potionInfo;
	potionInfo << "    Potions X " << m_displayAgent->m_potionInventory.size();

	m_font->Draw((name + potionInfo.str()),Vec2f(m_healthBar.min.x - 30 , m_healthBar.max.y),40,RGBA(),GameCommon::s_windowSize);
	
	std::ostringstream turnInfo;
	turnInfo << "Total Turns : " << ((Player*)(m_displayAgent))->m_totalTurns;
	m_font->Draw(turnInfo.str(),Vec2f(1300,880),50,RGBA(),GameCommon::s_windowSize);

	float percentage = m_displayAgent->m_currentHP / m_displayAgent->m_maxHP;
	Vertex_PosColor HPBarBackground[4];
	Vertex_PosColor HPBarContent[4];

	HPBarBackground[0].position = Vec3f(m_healthBar.min.x,m_healthBar.min.y,0.0f);
	HPBarBackground[1].position = Vec3f(m_healthBar.max.x,m_healthBar.min.y,0.0f);
	HPBarBackground[2].position = Vec3f(m_healthBar.max.x,m_healthBar.max.y,0.0f);
	HPBarBackground[3].position = Vec3f(m_healthBar.min.x,m_healthBar.max.y,0.0f);

	HPBarContent[0].position = Vec3f( m_healthBar.min.x															, m_healthBar.min.y,0.0f);
	HPBarContent[1].position = Vec3f( m_healthBar.min.x + (m_healthBar.max.x - m_healthBar.min.x) * percentage	, m_healthBar.min.y,0.0f);
	HPBarContent[2].position = Vec3f( m_healthBar.min.x + (m_healthBar.max.x - m_healthBar.min.x) * percentage	, m_healthBar.max.y,0.0f);
	HPBarContent[3].position = Vec3f( m_healthBar.min.x															, m_healthBar.max.y,0.0f);

	for(int index=0; index<4; index++)
	{
		HPBarBackground[index].color = RGBA(0.8f,0.2f,0.2f,1.0f);
		HPBarContent[index].color = RGBA(0.2f,0.8f,0.2f,1.0f);
	}
	
	OpenGLRenderer::BindWhiteTexture();
	OpenGLRenderer::DrawVertexWithVertexArray2D(HPBarBackground,4,OpenGLRenderer::SHAPE_QUADS);
	OpenGLRenderer::DrawVertexWithVertexArray2D(HPBarContent,4,OpenGLRenderer::SHAPE_QUADS);
}


void UI::PostMessageToLog(const std::string& msg , RGBA color)
{
	MessageLogString logMsg(msg,color);
	m_nextMsgBar.push_back(logMsg);
}


void UI::PostMessageToLog(const char* msg , RGBA color /* = RGBA()  */, ...)
{
	va_list args;
	va_start( args , color );
	std::string finalMsg = PrintToConsoleVariable( msg , args );
	va_end(args);

	MessageLogString logMsg(finalMsg,color);
	m_nextMsgBar.push_back(logMsg);
}


void UI::DrawMessage()
{
	Vec2f startPosition(200,750);
	Vec2f lineHeight(0.0f,65.0f);
	
	if(m_msgBar.size() > 0)
		m_font->Draw( m_msgBar[0].msg , lineHeight * 1.5f + startPosition , 60 , m_msgBar[0].color , Vec2i(1600,900) );
	if(m_msgBar.size() > 1)
		m_font->Draw( "- more -" , lineHeight + startPosition + Vec2f(300,0) , 40 , RGBA() , Vec2i(1600,900) );
}


void UI::ReadNextMessage()
{
	Vec2f startPosition(200,750);
	Vec2f lineHeight(0.0f,65.0f);
	std::vector<MessageLogString>::iterator iter = m_msgBar.begin();
	if(m_msgBar.size() > 0)
		m_msgBar.erase(iter);
}


bool UI::AllMessageRead()
{
	if(m_msgBar.size() <= 1)
		return true;
	return false;
}


std::string UI::PrintToConsoleVariable(const char*message , va_list args)
{
	const int buffer_size = 2048;
	char buffer[buffer_size];
	memset(buffer , 0 , buffer_size);
	_vsnprintf_s(buffer,buffer_size,buffer_size-1,message,args);
	std::string msg(buffer);
	return msg;
}


void UI::Update()
{
	std::vector<MessageLogString> empty;
	m_msgBar = empty;

	for( int index = m_nextMsgBar.size() - 1; index >= 0; --index )
		m_msgBar.push_back(m_nextMsgBar[index]);

	m_nextMsgBar = empty;
}


void UI::DrawEquipmentInfo()
{
	Vec2f startPosition(500,0);
	float fontSize = 40.0f;
	std::string weaponInfo("Weapon : ");
	weaponInfo.append(m_displayAgent->m_equipmentSlot[SLOT_WEAPON] ? m_displayAgent->m_equipmentSlot[SLOT_WEAPON]->m_name : "");

	std::string chestInfo("Chest : ");
	chestInfo.append(m_displayAgent->m_equipmentSlot[SLOT_CHEST] ? m_displayAgent->m_equipmentSlot[SLOT_CHEST]->m_name : "");

	std::string helmetInfo("Helmet : ");
	helmetInfo.append(m_displayAgent->m_equipmentSlot[SLOT_HELMET] ? m_displayAgent->m_equipmentSlot[SLOT_HELMET]->m_name : "");

	std::string pantsInfo("Pants : ");
	pantsInfo.append(m_displayAgent->m_equipmentSlot[SLOT_PANTS] ? m_displayAgent->m_equipmentSlot[SLOT_PANTS]->m_name : "");

	std::string glovesInfo("Gloves : ");
	glovesInfo.append(m_displayAgent->m_equipmentSlot[SLOT_GLOVES] ? m_displayAgent->m_equipmentSlot[SLOT_GLOVES]->m_name : "");

	std::string bootsInfo("Boots : ");
	bootsInfo.append(m_displayAgent->m_equipmentSlot[SLOT_BOOTS] ? m_displayAgent->m_equipmentSlot[SLOT_BOOTS]->m_name : "");

	m_font->Draw(weaponInfo , startPosition + Vec2f(0,50)	, fontSize , RGBA() , GameCommon::s_windowSize);
	m_font->Draw(chestInfo  , startPosition + Vec2f(0,0)		, fontSize , RGBA() , GameCommon::s_windowSize);
	m_font->Draw(helmetInfo , startPosition + Vec2f(400,50)	, fontSize , RGBA() , GameCommon::s_windowSize);
	m_font->Draw(pantsInfo  , startPosition + Vec2f(400,0)	, fontSize , RGBA() , GameCommon::s_windowSize);
	m_font->Draw(glovesInfo , startPosition + Vec2f(800,50)	, fontSize , RGBA() , GameCommon::s_windowSize);
	m_font->Draw(bootsInfo  , startPosition + Vec2f(800,0)	, fontSize , RGBA() , GameCommon::s_windowSize);
}


void UI::SetDisplayAgent(Agent* agent)
{
	m_displayAgent = agent;
}

};