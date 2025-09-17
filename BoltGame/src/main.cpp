#include "GameSystem.hpp"
#include "bolt/Utils/Async.hpp"

using namespace Bolt;

void runGameSystem() {
	int i = 0;

	WaitForSecondsRealtime(0.5f);
	auto& definition = SceneManager::RegisterScene("GameScene" + std::to_string(i));
	definition.AddSystem<GameSystem>();
	auto& scene = SceneManager::LoadScene("GameScene" + std::to_string(i));
	i++;
}

int main() {
	Application app = Application();
	ASYNC(runGameSystem());
	try {
		app.Run();
	}
	catch (std::exception e) {
		std::cout << e.what();
	}
	return 0;
}
