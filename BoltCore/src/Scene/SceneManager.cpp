#include "Bolt/btpch.hpp"
#include "Bolt/Scene/SceneManager.hpp"
#include "Bolt/Scene/Scene.hpp"
#include "Bolt/Debug/Logger.hpp"

#include "Bolt/Components/Rigidbody2D.hpp" // WRONG
#include "Bolt/Components/BoxCollider2D.hpp"

namespace Bolt {
    // Static member initialization
    std::unordered_map<std::string, std::unique_ptr<SceneDefinition>> SceneManager::s_SceneDefinitions;
    std::vector<std::unique_ptr<Scene>> SceneManager::s_LoadedScenes;
    Scene* SceneManager::s_ActiveScene = nullptr;

    SceneDefinition& SceneManager::RegisterScene(const std::string& name) {
        // Prüfe ob Scene-Definition bereits existiert
        auto it = s_SceneDefinitions.find(name);
        if (it != s_SceneDefinitions.end()) {
            // Scene existiert bereits - gib Referenz zurück für Re-Konfiguration
            Logger::Warning("Scene definition with name '" + name +
                "' already exists. Returning existing definition for modification.");
            return *it->second;
        }

        // Erstelle neue Scene-Definition
        auto definition = std::make_unique<SceneDefinition>(name);
        SceneDefinition& ref = *definition;

        // Füge zur Map hinzu
        s_SceneDefinitions[name] = std::move(definition);
        return ref;
    }

    Scene& SceneManager::LoadScene(const std::string& name) {
        // Finde Scene-Definition
        auto defIt = s_SceneDefinitions.find(name);
        if (defIt == s_SceneDefinitions.end()) {
            throw std::runtime_error("Scene definition '" + name +
                "' not found. Call SceneManager::registerScene(\"" + name + "\") first.");
        }

        // Prüfe ob Scene bereits geladen ist
        auto loadedIt = std::find_if(s_LoadedScenes.begin(), s_LoadedScenes.end(),
            [&name](const std::unique_ptr<Scene>& scene) {
                return scene->GetName() == name;
            });

        if (loadedIt != s_LoadedScenes.end()) {
            throw std::runtime_error("Scene '" + name +
                "' is already loaded. Use loadSceneAdditive() for multiple instances.");
        }

        // Entlade alle nicht-persistenten Scenes
        UnloadAllScenes(false);

        // Instanziiere neue Scene aus Definition
        SceneDefinition* definition = defIt->second.get();
        auto newScene = definition->instantiate();

        // Rufe Load-Callbacks auf
        for (const auto& callback : definition->m_loadCallbacks) {
            callback(*newScene);
        }

        // Markiere als geladen
        newScene->m_IsLoaded = true;

        // Füge zu geladenen Scenes hinzu und setze als aktiv
        Scene& sceneRef = *newScene;
        s_LoadedScenes.push_back(std::move(newScene));
        s_ActiveScene = &sceneRef;

        return sceneRef;
    }

    Scene& SceneManager::LoadSceneAdditive(const std::string& name) {
        // Finde Scene-Definition
        auto defIt = s_SceneDefinitions.find(name);
        if (defIt == s_SceneDefinitions.end()) {
            throw std::runtime_error("Scene definition '" + name +
                "' not found. Call SceneManager::registerScene(\"" + name + "\") first.");
        }

        // Bei additiv ist es OK wenn die Scene bereits geladen ist
        // (könnte für multiple Instanzen gewollt sein)

        // Instanziiere neue Scene aus Definition
        SceneDefinition* definition = defIt->second.get();
        auto newScene = definition->instantiate();

        // Rufe Load-Callbacks auf
        for (const auto& callback : definition->m_loadCallbacks) {
            callback(*newScene);
        }

        // Markiere als geladen
        newScene->m_IsLoaded = true;

        // Füge zu geladenen Scenes hinzu
        Scene& sceneRef = *newScene;
        s_LoadedScenes.push_back(std::move(newScene));

        // Wenn keine aktive Scene, setze diese als aktiv
        if (s_ActiveScene == nullptr) {
            s_ActiveScene = &sceneRef;
        }

        return sceneRef;
    }

    Scene& SceneManager::ReloadScene(const std::string name) {
        // Finde die geladene Scene
        auto it = std::find_if(s_LoadedScenes.begin(), s_LoadedScenes.end(),
            [&name](const std::unique_ptr<Scene>& scene) {
                return scene->GetName() == name;
            });

        if (it == s_LoadedScenes.end()) {
            throw std::runtime_error("Scene '" + name + "' is not loaded.");
        }

        // Merke ob es die aktive Scene war
        bool wasActive = (s_ActiveScene == it->get());

        // Entlade die Scene
        UnloadScene(name);

        // Lade sie neu
        if (wasActive) {
            return LoadScene(name);
        }
        else {
            return LoadSceneAdditive(name);
        }
    }

    void SceneManager::UnloadScene(const std::string& name) {
        // Finde die Scene
        auto it = std::find_if(s_LoadedScenes.begin(), s_LoadedScenes.end(),
            [&name](const std::unique_ptr<Scene>& scene) {
                return scene->GetName() == name;
            });

        if (it == s_LoadedScenes.end()) {
            throw std::runtime_error("Scene '" + name + "' is not loaded.");
        }

        Scene* scene = it->get();

        // Rufe Unload-Callbacks auf
        if (scene->m_Definition) {
            for (const auto& callback : scene->m_Definition->m_unloadCallbacks) {
                callback(*scene);
            }
        }

        // Markiere als nicht geladen
        scene->m_IsLoaded = false;

        // Destroy Systems und Registry
        scene->DestroyScene();

       // for(auto [ent, rigidbody] : scene->m_Registry.view<Rigidbody2D>().each()) {
       //     rigidbody.destroy();
		//}
       /* for (auto [ent, box] : scene->m_Registry.view<BoxCollider2D>().each()) {
            box.Destroy();
        }*/

        scene->m_Registry.clear();

        // Wenn es die aktive Scene war, wähle eine neue
        if (s_ActiveScene == scene) {
            s_ActiveScene = nullptr;

            // Suche nach einer anderen geladenen Scene
            for (auto& loadedScene : s_LoadedScenes) {
                if (loadedScene.get() != scene && loadedScene->IsLoaded()) {
                    s_ActiveScene = loadedScene.get();
                    break;
                }
            }
        }

        // Entferne aus dem Vector
        s_LoadedScenes.erase(it);
    }

    void SceneManager::UnloadAllScenes(bool includePersistent) {
        // Kopiere Namen, da wir während der Iteration modifizieren
        std::vector<std::string> scenesToUnload;

        for (const auto& scene : s_LoadedScenes) {
            if (includePersistent || !scene->IsPersistent()) {
                scenesToUnload.push_back(scene->GetName());
            }
        }

        // Entlade alle markierten Scenes
        for (const auto& name : scenesToUnload) {
            UnloadScene(name);
        }
    }

    Scene& SceneManager::GetLoadedScene(const std::string& name) {
        auto it = std::find_if(s_LoadedScenes.begin(), s_LoadedScenes.end(),
            [&name](const std::unique_ptr<Scene>& scene) {
                return scene->GetName() == name;
            });

        if (it == s_LoadedScenes.end()) {
            throw std::runtime_error("Scene '" + name + "' is not loaded.");
        }

        return **it;
    }

    Scene& SceneManager::GetActiveScene() {
        if (s_ActiveScene == nullptr) {
            throw std::runtime_error("No active scene. Load a scene first.");
        }

        return *s_ActiveScene;
    }

    void SceneManager::SetActiveScene(const std::string& name) {
        auto it = std::find_if(s_LoadedScenes.begin(), s_LoadedScenes.end(),
            [&name](const std::unique_ptr<Scene>& scene) {
                return scene->GetName() == name;
            });

        if (it == s_LoadedScenes.end()) {
            throw std::runtime_error("Scene '" + name +
                "' is not loaded. Load it first before setting as active.");
        }

        s_ActiveScene = it->get();
    }

    bool SceneManager::HasSceneDefinition(const std::string& name) {
        return s_SceneDefinitions.find(name) != s_SceneDefinitions.end();
    }

    bool SceneManager::IsSceneLoaded(const std::string& name) {
        return std::any_of(s_LoadedScenes.begin(), s_LoadedScenes.end(),
            [&name](const std::unique_ptr<Scene>& scene) {
                return scene->GetName() == name;
            });
    }

    std::vector<std::string> SceneManager::GetRegisteredSceneNames() {
        std::vector<std::string> names;
        names.reserve(s_SceneDefinitions.size());

        for (const auto& [name, def] : s_SceneDefinitions) {
            names.push_back(name);
        }

        return names;
    }

    std::vector<std::string> SceneManager::GetLoadedSceneNames() {
        std::vector<std::string> names;
        names.reserve(s_LoadedScenes.size());

        for (const auto& scene : s_LoadedScenes) {
            names.push_back(scene->GetName());
        }

        return names;
    }

    void SceneManager::UpdateScenes() {
        for (auto& scene : s_LoadedScenes) {
            if (scene->IsLoaded()) {
                scene->UpdateSystems();
            }
        }
    }

    void SceneManager::FixedUpdateScenes() {
        for (auto& scene : s_LoadedScenes) {
            if (scene->IsLoaded()) {
                scene->FixedUpdateSystems();
            }
        }
    }

    void SceneManager::InitializeStartupScenes() {
        // Lade alle Scenes die als Startup markiert sind
        for (const auto& [name, definition] : s_SceneDefinitions) {
            if (definition->isStartupScene()) {
                try {
                    if (s_ActiveScene == nullptr) {
                        // Erste Startup-Scene wird normal geladen
                        LoadScene(name);
                    }
                    else {
                        // Weitere Startup-Scenes werden additiv geladen
                        LoadSceneAdditive(name);
                    }
                }
                catch (const std::exception& e) {
                    Logger::Error("Failed to load startup scene with name '" +
                        name + "': " + e.what());
                }
            }
        }
    }
}