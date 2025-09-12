#pragma once
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>  // FEHLTE! Für std::unordered_map
#include "Bolt/Scene/SceneDefinition.hpp"  // FEHLTE! Für SceneDefinition

namespace Bolt {
    class PhysicsSystem;
}

namespace Bolt {
    class Scene;

    class SceneManager {
    public:
        /**
         * Registriert eine neue Scene-Definition.
         * Die Definition kann später mehrfach instanziiert werden.
         *
         * @example
         * SceneManager::registerScene("MainMenu")
         *     .addSystem<UISystem>()
         *     .addSystem<AudioSystem>()
         *     .onLoad([](Scene& scene) {
         *         // Setup UI elements
         *     });
         */
        static SceneDefinition& RegisterScene(const std::string& name);

        /**
         * Lädt eine Scene basierend auf ihrer registrierten Definition.
         * Entlädt automatisch die aktuelle Scene (außer sie ist persistent).
         */
        static Scene& LoadScene(const std::string& name);

        /**
         * Lädt eine Scene additiv (zusätzlich zu bereits geladenen Scenes).
         * Nützlich für z.B. UI-Overlays oder persistente Manager-Scenes.
         */
        static Scene& LoadSceneAdditive(const std::string& name);

        /**
         * Lädt die aktuelle Scene neu (destroy + neu laden)
         */
        static Scene& ReloadScene(const std::string name);

        /**
         * Entlädt eine spezifische Scene
         */
        static void UnloadScene(const std::string& name);

        /**
         * Entlädt alle nicht-persistenten Scenes
         */
        static void UnloadAllScenes(bool includePersistent = false);

        /**
         * Gibt eine geladene Scene zurück
         */
        static Scene& GetLoadedScene(const std::string& name);

        /**
         * Gibt die aktive Scene zurück
         */
        static Scene& GetActiveScene();

        /**
         * Setzt eine geladene Scene als aktiv
         */
        static void SetActiveScene(const std::string& name);

        /**
         * Prüft ob eine Scene-Definition existiert
         */
        static bool HasSceneDefinition(const std::string& name);

        /**
         * Prüft ob eine Scene geladen ist
         */
        static bool IsSceneLoaded(const std::string& name);

        /**
         * Gibt alle registrierten Scene-Namen zurück
         */
        static std::vector<std::string> GetRegisteredSceneNames();

        /**
         * Gibt alle geladenen Scene-Namen zurück
         */
        static std::vector<std::string> GetLoadedSceneNames();

    private:
        static void UpdateScenes();
        static void FixedUpdateScenes();
        static void InitializeStartupScenes();

        // Scene-Definitionen (Templates/Baupläne)
        static std::unordered_map<std::string, std::unique_ptr<SceneDefinition>> s_SceneDefinitions;

        // Geladene Scene-Instanzen
        static std::vector<std::unique_ptr<Scene>> s_LoadedScenes;

        // Aktive Scene
        static Scene* s_ActiveScene;

        friend class Application;
        friend class PhysicsSystem;
    };
}