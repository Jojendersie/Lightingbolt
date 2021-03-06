#include "map.hpp"
#include <cstdlib>
#include "../math/math.hpp"
#include "../ai/GameObject.hpp"
#include "../ai/Enemy.hpp"
#include "../generator/Random.hpp"
#include <cmath>

extern Generators::Random* g_rand;

namespace Map
{
	Map::Map(int _width, int _height)
	{
		m_width = _width;
		m_height = _height;

		m_densityMap = new float[_width*_height];
		
		m_player = new Ai::GameObject(Math::Vec2(0.0,0.0),0.3f,this);
		m_player->setShape(0,0,0);
		m_player->setMaterialIndex(0);
		//m_player->setAlive(true);
		InitMap();
	}

	Map::~Map()
	{
		delete m_densityMap;
		delete m_player;
	}

	void Map::InitMap()
	{
		float amplitude = 5.0f;
		float offset = 0.5f;
		// build density map
		for(int y=0;y<m_height;++y)
		{
			for(int x=0;x<m_width;++x)
			{
				m_densityMap[m_width*y+x] = offset;
				m_densityMap[m_width*y+x]  += amplitude - GaussAtCoordinate(Math::Vec2(x,y),Math::Vec2(m_width/2,m_height/2),Math::Vec2(m_width/2,m_height/2),amplitude);
				/*m_densityMap[m_width*y+x]  += GaussAtCoordinate(Math::Vec2(x,y),Math::Vec2(m_width/4,m_height/2),Math::Vec2(50,30),3);
				m_densityMap[m_width*y+x]  += GaussAtCoordinate(Math::Vec2(x,y),Math::Vec2(500,m_height/3),Math::Vec2(200,30),-0.5);
				m_densityMap[m_width*y+x]  += g_rand->Uniform(-0.4f,0.5f);*/
				//m_densityMap[m_width*y+x] += (sin(sqrt((m_width-x)*(m_width-x))*0.03)) *0.25; //+ cos((m_height-y)*(m_height-y))) * 0.5;
				//m_densityMap[m_width*y+x] += (cos(sqrt((m_height-y)*(m_height-y))*0.05)) *0.25;
				m_densityMap[m_width*y+x]  /= amplitude + offset;
			} // for x
		} // for y
	}

	float Map::GaussAtCoordinate(const Math::Vec2& _coordinate, const Math::Vec2& _center, const Math::Vec2& spread, float _amplidude)
	{
		Math::Vec2 sub = _coordinate - _center;
		return _amplidude * pow(2.71828f,-(((sub.x*sub.x)/(2*spread.x*spread.x))+((sub.y*sub.y)/(2*spread.y*spread.y))));
	}

	void Map::Update( double _deltaTime)
	{
		if(!getPlayer()->isAlive()) return;
		bool rescale=false;
		// update the player
		//if(m_player->isAlive())
		//{
			m_player->update(_deltaTime);
			if(m_player->getEnergy()>1) rescale = true;
		//}
		// update all other objects
		for(int i=0;i<m_objects.size();++i)
		{
			if(m_objects[i]->isAlive())
			{
				m_objects[i]->update(_deltaTime);
				if(m_objects[i]->getEnergy()>1) rescale = true;
			}
			else
				m_objects.remove(i--);
		}// objects

		if(rescale)
		{
			for(int i=0;i<m_objects.size();++i)
			{
				m_objects[i]->setEnergy(m_objects[i]->getEnergy()*0.75f);
			}
			m_player->setEnergy(m_player->getEnergy()*0.75f);
		}
	}

	float Map::Density( const Math::Vec2& _position)
	{
		return m_densityMap[m_width*(int)_position.y+(int)_position.x];
	}

	void Map::addEnemy(const Math::Vec2& _position, float _radius)
	{
		m_objects.append(Ai::Enemy(_position, _radius,this));
	}

} // namespace Map
