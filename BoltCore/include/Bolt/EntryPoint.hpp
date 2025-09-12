#pragma once

bool g_ApplicationRunning = true;

namespace Bolt {

	int main(int argc, char** argv) {
		while (g_ApplicationRunning)
		{
			InitializeCore();
			Application* app = CreateApplication(argc, argv);
			HZ_CORE_ASSERT(app, "Client Application is null!");
			app->Run();
			delete app;
			ShutdownCore();
		}
		return 0;
	}
}