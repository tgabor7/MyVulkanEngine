#pragma once

namespace myve {

	class FirstApp {
	public:
		void run();
	private:
		void initVulkan();
		void mainLoop();
		void cleanUp();
	};
}
