#include "first_app.hpp"

#include <memory>
#include "myve_scene.hpp"
#include "input_handler.hpp"

namespace myve 
{

	FirstApp::FirstApp()
	{
	}
	void FirstApp::update()
	{
        
        Mouse m = InputHandler::getMouseD();

	}
	void FirstApp::loadResources()
	{
		createGameObject();
	}
	
}

