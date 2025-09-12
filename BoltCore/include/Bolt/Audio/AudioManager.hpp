#pragma once

#include <miniaudio/miniaudio.h>
#include "Bolt/Audio/AudioHandle.hpp"

#include <queue>

namespace Bolt {
	class AudioSource;
	class Audio;

	class AudioManager {
	public:
		static constexpr uint32_t MAX_CONCURRENT_SOUNDS = 64;
		static constexpr uint32_t MAX_SOUNDS_PER_FRAME = 8;
		static constexpr float MIN_SOUND_INTERVAL = 0.1f;

		// Engine lifecycle
		static bool Initialize();
		static void Shutdown();
		static void Update(); // Call once per frame

		// Audio loading
		static AudioHandle LoadAudio(const std::string& filepath);
		static void UnloadAudio(const AudioHandle& handle);
		static void UnloadAllAudio();

		// Playback control
		static void PlayAudioSource(AudioSource& source);
		static void PauseAudioSource(AudioSource& source);
		static void StopAudioSource(AudioSource& source);
		static void ResumeAudioSource(AudioSource& source);

		// Global settings
		static void SetMasterVolume(float volume);
		static void SetListenerPosition(const glm::vec3& position);
		static void SetListenerVelocity(const glm::vec3& velocity);
		static void SetListenerOrientation(const glm::vec3& forward, const glm::vec3& up);
		static float GetMasterVolume() { return s_masterVolume; }

		// Quick play functions (fire-and-forget)
		static void PlayOneShot(const AudioHandle& handle, float volume = 1.0f);
		static void PlayOneShotAtPosition(const AudioHandle& handle, const glm::vec3& position, float volume = 1.0f);

		// New optimized methods
		static void PlayOneShotLimited(const AudioHandle& handle, float volume = 1.0f, float priority = 1.0f);
		static void PlayOneShotAtPositionLimited(const AudioHandle& handle, const glm::vec3& position,
			float volume = 1.0f, float priority = 1.0f);

		// Batch operations for performance
		static void PlayBatchOneShots(const std::vector<std::pair<AudioHandle, float>>& sounds);

		// Sound limiting
		static void SetMaxConcurrentSounds(uint32_t maxSounds);
		static void SetMaxSoundsPerFrame(uint32_t maxPerFrame);
		static uint32_t GetActiveSoundCount();

		// Audio queries
		static bool IsAudioLoaded(const AudioHandle& handle);
		static const Audio* GetAudio(const AudioHandle& handle);

		// Internal access for AudioSource (friend functions would be better, but this works)
		struct SoundInstance {
			ma_sound Sound;
			AudioHandle AudioHandle;
			bool IsValid = false;
		};

		static SoundInstance* GetSoundInstance(uint32_t instanceId);

	private:
		// Delete constructors to prevent instantiation
		AudioManager() = delete;
		~AudioManager() = delete;
		AudioManager(const AudioManager&) = delete;
		AudioManager& operator=(const AudioManager&) = delete;

		// Static member variables
		static ma_engine s_Engine;
		static bool s_IsInitialized;

		struct SoundRequest {
			AudioHandle Handle;
			float Volume;
			float Priority;
			glm::vec3 Position;
			bool Is3D;
			std::chrono::steady_clock::time_point RequestTime;


			bool operator<(const SoundRequest& other) const {
				// Achtung: priority_queue ist ein MAX-Heap, d.h. das größte Element kommt nach oben.
				// Wenn du "höhere priority zuerst" willst:
				return Priority < other.Priority;
			}
		};

		struct SoundLimitData {
			std::chrono::steady_clock::time_point LastPlayTime;
			uint32_t FramePlayCount;
		};

		// Sound limiting
		static uint32_t s_maxConcurrentSounds;
		static uint32_t s_maxSoundsPerFrame;
		static uint32_t s_soundsPlayedThisFrame;
		static uint32_t s_activeSoundCount;

		// Sound request queue with priority
		static std::priority_queue<SoundRequest> s_soundQueue;

		// Per-audio throttling
		static std::unordered_map<AudioHandle::HandleType, SoundLimitData> s_soundLimits;

		// Internal methods
		static bool CanPlaySound(const AudioHandle& handle, float priority);
		static void ProcessSoundQueue();
		static void ThrottleSound(const AudioHandle& handle);
		static bool IsThrottled(const AudioHandle& handle);

		// Audio storage
		static std::unordered_map<AudioHandle::HandleType, std::unique_ptr<Audio>> s_audioMap;
		static AudioHandle::HandleType s_nextHandle;

		// Sound instances for tracking
		static std::vector<SoundInstance> s_soundInstances;
		static std::vector<uint32_t> s_freeInstanceIndices;

		// Global settings
		static float s_masterVolume;
		static glm::vec3 s_listenerPosition;
		static glm::vec3 s_listenerVelocity;
		static glm::vec3 s_listenerForward;
		static glm::vec3 s_listenerUp;

		// Internal methods
		static AudioHandle::HandleType GenerateHandle();
		static uint32_t CreateSoundInstance(const AudioHandle& audioHandle);
		static void DestroySoundInstance(uint32_t instanceId);
		static void CleanupFinishedSounds();

		// Update internal state
		static void UpdateListener();
		static void UpdateSoundInstances();

		friend class AudioSource;
	};
}