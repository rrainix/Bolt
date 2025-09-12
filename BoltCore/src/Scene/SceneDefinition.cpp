#include "Bolt/btpch.hpp"
#include "Bolt/Scene/SceneDefinition.hpp"
#include "Bolt/Debug/Logger.hpp"
#include "Bolt/Scene/Scene.hpp"


namespace Bolt {
	std::unique_ptr<Scene> SceneDefinition::instantiate() const {
		// Erstelle neue Scene-Instanz (Konstruktor ist friend)
		auto scene = std::unique_ptr<Scene>(
			new Scene(m_name, this, m_isPersistent)
		);

		// Erstelle alle Systems aus den Factories
		for (const auto& factory : m_systemFactories) {
			try {
				auto system = factory();
				if (system) {
					scene->m_Systems.push_back(std::move(system));
				}
				else {
					Logger::Error("Failed to create system for scene with name " + m_name);
				}
			}
			catch (const std::exception& e) {
				Logger::Error("Exception creating system for scene with name '" +
					m_name + "': " + e.what());
			}
		}

		// Rufe Initialize-Callbacks auf (einmalig beim Erstellen)
		for (const auto& callback : m_initializeCallbacks) {
			try {
				callback(*scene);
			}
			catch (const std::exception& e) {
				Logger::Error("Exception in initialize callback for scene with name '" +
					m_name + "': " + e.what());
			}
		}

		// Awake alle Systems
		scene->AwakeSystems();

		// Start alle Systems
		scene->StartSystems();
		return scene;
	}
}