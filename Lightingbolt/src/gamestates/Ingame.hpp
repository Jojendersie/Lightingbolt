#pragma once

#include "GameState.hpp"
#include "../predecl.hpp"

namespace GameStates {

	class Ingame: public IGameState
	{
	public:
		Ingame();
		~Ingame();
		virtual void MouseMove(int _dx, int _dy) override;
		virtual void KeyDown(int _key) override;
		virtual void KeyUp(int _key) override;
		virtual void Scroll(int _delta) override;

		virtual void Render( double _time, double _deltaTime,
			Graphic::RenderTargetList& _renderTargets,
			Graphic::ShaderList& _shaders,
			Graphic::UniformBuffer* _ShaderConstants) override;

		virtual void Update( double _time, double _deltaTime ) override;

		const Map::Map* getMap() {return map;}
	private:
		Graphic::VertexBuffer* m_vertexBuffer;
		Map::Map *map;
		Graphic::PhotonMapper* m_photonMapper;
		Graphic::RenderTarget* m_mapTexture;
		int m_minEnemies;
		int m_maxEnemies;
	};

} // namespace GameStates