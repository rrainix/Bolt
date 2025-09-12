#pragma once
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>  // FEHLTE! F�r std::unordered_map
#include "Bolt/Scene/SceneDefinition.hpp"  // FEHLTE! F�r SceneDefinition

namespace Bolt {
    class PhysicsSystem;
}

namespace Bolt {
    class Scene;

    class SceneManager {
    public:
        /**
         * Registriert eine neue Scene-Definition.
         * Die Definition kann sp�ter mehrfach instanziiert werden.
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
         * L�dt eine Scene basierend auf ihrer registrierten Definition.
         * Entl�dt automatisch die aktuelle Scene (au�er sie ist persistent).
         */
        static Scene& LoadScene(const std::string& name);

        /**
         * L�dt eine Scene additiv (zus�tzlich zu bereits geladenen Scenes).
         * N�tzlich f�r z.B. UI-Overlays oder persistente Manager-Scenes.
         */
        static Scene& LoadSceneAdditive(const std::string& name);

        /**
         * L�dt die aktuelle Scene neu (destroy + neu laden)
         */
        static Scene& ReloadScene(const std::string name);

        /**
         * Entl�dt eine spezifische Scene
         */
        static void UnloadScene(const std::string& name);

        /**
         * Entl�dt alle nicht-persistenten Scenes
         */
        static void UnloadAllScenes(bool includePersistent = false);

        /**
         * Gibt eine geladene Scene zur�ck
         */
        static Scene& GetLoadedScene(const std::string& name);

        /**
         * Gibt die aktive Scene zur�ck
         */
        static Scene& GetActiveScene();

        /**
         * Setzt eine geladene Scene als aktiv
         */
        static void SetActiveScene(const std::string& name);

        /**
         * Pr�ft ob eine Scene-Definition existiert
         */
        static bool HasSceneDefinition(const std::string& name);

        /**
         * Pr�ft ob eine Scene geladen ist
         */
        static bool IsSceneLoaded(const std::string& name);

        /**
         * Gibt alle registrierten Scene-Namen zur�ck
         */
        static std::vector<std::string> GetRegisteredSceneNames();

        /**
         * Gibt alle geladenen Scene-Namen zur�ck
         */
        static std::vector<std::string> GetLoadedSceneNames();

    private:
        static void UpdateScenes();
        static void FixedUpdateScenes();
        static void InitializeStartupScenes();

        // Scene-Definitionen (Templates/Baupl�ne)
        static std::unordered_map<std::string, std::unique_ptr<SceneDefinition>> s_SceneDefinitions;

        // Geladene Scene-Instanzen
        static std::vector<std::unique_ptr<Scene>> s_LoadedScenes;

        // Aktive Scene
        static Scene* s_ActiveScene;

        friend class Application;
        friend class PhysicsSystem;
    };
}