#pragma once
#include "myve_renderer.hpp"

namespace myve
{
	class Scene
	{
	public:
		Scene();

		void run();

		virtual void loadResources() = 0;
		virtual void update() = 0;
		
		~Scene();
	
	protected:
		void createGameObject();
	private:
		bool inflight = true;
		Renderer renderer;
	};
}