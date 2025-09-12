#include "Bolt/btpch.hpp"

#include "Bolt/Audio/AudioManager.hpp"
#include "Bolt/Audio/AudioSource.hpp"
#include "Bolt/Audio/Audio.hpp"

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio/miniaudio.h>

namespace Bolt {
	ma_engine AudioManager::s_Engine{};
	bool AudioManager::s_IsInitialized = false;
	std::unordered_map<AudioHandle::HandleType, std::unique_ptr<Audio>> AudioManager::s_audioMap;
	AudioHandle::HandleType AudioManager::s_nextHandle = 1;
	std::vector<AudioManager::SoundInstance> AudioManager::s_soundInstances;
	std::vector<uint32_t> AudioManager::s_freeInstanceIndices;
	float AudioManager::s_masterVolume = 1.0f;
	glm::vec3 AudioManager::s_listenerPosition{ 0.0f };
	glm::vec3 AudioManager::s_listenerVelocity{ 0.0f };
	glm::vec3 AudioManager::s_listenerForward{ 0.0f, 0.0f, -1.0f };
	glm::vec3 AudioManager::s_listenerUp{ 0.0f, 1.0f, 0.0f };

	uint32_t AudioManager::s_maxConcurrentSounds = MAX_CONCURRENT_SOUNDS;
	uint32_t AudioManager::s_maxSoundsPerFrame = MAX_SOUNDS_PER_FRAME;
	uint32_t AudioManager::s_soundsPlayedThisFrame = 0;
	uint32_t AudioManager::s_activeSoundCount = 0;
	std::priority_queue<AudioManager::SoundRequest> AudioManager::s_soundQueue;
	std::unordered_map<AudioHandle::HandleType, AudioManager::SoundLimitData> AudioManager::s_soundLimits;


	bool AudioManager::Initialize() {
		if (s_IsInitialized) {
			return true;
		}

		// Initialize miniaudio engine with default configuration
		ma_result result = ma_engine_init(nullptr, &s_Engine);
		if (result != MA_SUCCESS) {
			std::cerr << "AudioManager: Failed to initialize miniaudio engine. Error: " << result << std::endl;
			return false;
		}

		// Reserve space for sound instances to avoid frequent reallocations
		s_soundInstances.reserve(256);
		s_freeInstanceIndices.reserve(256);

		// Set initial listener properties
		UpdateListener();

		s_IsInitialized = true;
		return true;
	}

	void AudioManager::Shutdown() {
		if (!s_IsInitialized) {
			return;
		}

		// Stop and cleanup all active sound instances
		for (auto& instance : s_soundInstances) {
			if (instance.IsValid) {
				ma_sound_stop(&instance.Sound);
				ma_sound_uninit(&instance.Sound);
			}
		}
		s_soundInstances.clear();
		s_freeInstanceIndices.clear();

		// Unload all audio resources
		UnloadAllAudio();

		// Shutdown miniaudio engine
		ma_engine_uninit(&s_Engine);
		s_IsInitialized = false;
	}

	void AudioManager::Update() {
		if (!s_IsInitialized) {
			return;
		}

		// Reset frame counter
		s_soundsPlayedThisFrame = 0;

		// Process queued sound requests
		ProcessSoundQueue();

		// Clean up finished sounds
		CleanupFinishedSounds();

		// Update listener and sound instances
		UpdateListener();
		UpdateSoundInstances();

		// Update active sound count
		s_activeSoundCount = 0;
		for (const auto& instance : s_soundInstances) {
			if (instance.IsValid && ma_sound_is_playing(&instance.Sound)) {
				s_activeSoundCount++;
			}
		}
	}

	void AudioManager::PlayOneShotLimited(const AudioHandle& handle, float volume, float priority) {
		if (!s_IsInitialized || !handle.IsValid()) {
			return;
		}

		// Check if we can play this sound
		if (!CanPlaySound(handle, priority)) {
			return;
		}

		// Check throttling
		if (IsThrottled(handle)) {
			return;
		}

		// Play immediately if under limits
		if (s_soundsPlayedThisFrame < s_maxSoundsPerFrame && s_activeSoundCount < s_maxConcurrentSounds) {
			PlayOneShot(handle, volume);
			s_soundsPlayedThisFrame++;
			ThrottleSound(handle);
		}
		else {
			// Queue for later processing
			SoundRequest request;
			request.Handle = handle;
			request.Volume = volume;
			request.Priority = priority;
			request.Is3D = false;
			request.RequestTime = std::chrono::steady_clock::now();

			s_soundQueue.push(request);
		}
	}

	void AudioManager::PlayOneShotAtPositionLimited(const AudioHandle& handle, const glm::vec3& position,
		float volume, float priority) {
		if (!s_IsInitialized || !handle.IsValid()) {
			return;
		}

		if (!CanPlaySound(handle, priority) || IsThrottled(handle)) {
			return;
		}

		if (s_soundsPlayedThisFrame < s_maxSoundsPerFrame && s_activeSoundCount < s_maxConcurrentSounds) {
			PlayOneShotAtPosition(handle, position, volume);
			s_soundsPlayedThisFrame++;
			ThrottleSound(handle);
		}
		else {
			SoundRequest request;
			request.Handle = handle;
			request.Volume = volume;
			request.Priority = priority;
			request.Position = position;
			request.Is3D = true;
			request.RequestTime = std::chrono::steady_clock::now();

			s_soundQueue.push(request);
		}
	}

	void AudioManager::PlayBatchOneShots(const std::vector<std::pair<AudioHandle, float>>& sounds) {
		for (const auto& [handle, volume] : sounds) {
			if (s_soundsPlayedThisFrame >= s_maxSoundsPerFrame) {
				break; // Stop processing if frame limit reached
			}
			PlayOneShotLimited(handle, volume, 1.0f);
		}
	}

	bool AudioManager::CanPlaySound(const AudioHandle& handle, float priority) {
		// Always allow high priority sounds
		if (priority >= 2.0f) {
			return true;
		}

		// Check global limits
		if (s_activeSoundCount >= s_maxConcurrentSounds) {
			return priority > 1.5f; // Only allow higher priority sounds
		}

		if (s_soundsPlayedThisFrame >= s_maxSoundsPerFrame) {
			return priority > 1.8f; // Very restrictive for frame limits
		}

		return true;
	}

	void AudioManager::ProcessSoundQueue() {
		uint32_t processed = 0;
		const uint32_t maxProcessPerFrame = 4; // Limit queue processing

		while (!s_soundQueue.empty() && processed < maxProcessPerFrame &&
			s_soundsPlayedThisFrame < s_maxSoundsPerFrame &&
			s_activeSoundCount < s_maxConcurrentSounds) {

			SoundRequest request = s_soundQueue.top();
			s_soundQueue.pop();

			// Skip old requests (older than 200ms)
			auto now = std::chrono::steady_clock::now();
			auto age = std::chrono::duration_cast<std::chrono::milliseconds>(now - request.RequestTime);
			if (age.count() > 200) {
				processed++;
				continue;
			}

			// Check if still throttled
			if (IsThrottled(request.Handle)) {
				processed++;
				continue;
			}

			// Play the sound
			if (request.Is3D) {
				PlayOneShotAtPosition(request.Handle, request.Position, request.Volume);
			}
			else {
				PlayOneShot(request.Handle, request.Volume);
			}

			s_soundsPlayedThisFrame++;
			ThrottleSound(request.Handle);
			processed++;
		}
	}

	void AudioManager::ThrottleSound(const AudioHandle& handle) {
		auto& limitData = s_soundLimits[handle.GetHandle()];
		limitData.LastPlayTime = std::chrono::steady_clock::now();
		limitData.FramePlayCount++;
	}

	bool AudioManager::IsThrottled(const AudioHandle& handle) {
		auto it = s_soundLimits.find(handle.GetHandle());
		if (it == s_soundLimits.end()) {
			return false;
		}

		auto now = std::chrono::steady_clock::now();
		auto timeSinceLastPlay = std::chrono::duration_cast<std::chrono::milliseconds>(
			now - it->second.LastPlayTime);

		return timeSinceLastPlay.count() < (MIN_SOUND_INTERVAL * 1000);
	}

	void AudioManager::SetMaxConcurrentSounds(uint32_t maxSounds) {
		s_maxConcurrentSounds = std::min(maxSounds, 128u); // Cap at 128
	}

	void AudioManager::SetMaxSoundsPerFrame(uint32_t maxPerFrame) {
		s_maxSoundsPerFrame = std::min(maxPerFrame, 16u); // Cap at 16
	}

	uint32_t AudioManager::GetActiveSoundCount() {
		return s_activeSoundCount;
	}

	AudioHandle AudioManager::LoadAudio(const std::string& filepath) {
		if (!s_IsInitialized) {
			Logger::Error("AudioManager: Cannot load audio - AudioManager not initialized");
			return AudioHandle();
		}

		auto audio = std::make_unique<Audio>();

		if (!audio->LoadFromFile(filepath)) {
			Logger::Error("AudioManager: Failed to load audio from file: " + filepath);
			return AudioHandle();
		}

		AudioHandle::HandleType handle = GenerateHandle();
		s_audioMap[handle] = std::move(audio);
		return AudioHandle(handle);
	}

	void AudioManager::UnloadAudio(const AudioHandle& handle) {
		if (!handle.IsValid()) {
			return;
		}

		auto it = s_audioMap.find(handle.GetHandle());
		if (it != s_audioMap.end()) {
			// Stop any playing instances of this audio
			for (auto& instance : s_soundInstances) {
				if (instance.IsValid && instance.AudioHandle == handle) {
					ma_sound_stop(&instance.Sound);
					ma_sound_uninit(&instance.Sound);
					instance.IsValid = false;
				}
			}

			s_audioMap.erase(it);
		}
	}

	void AudioManager::UnloadAllAudio() {
		// Stop all sound instances first
		for (auto& instance : s_soundInstances) {
			if (instance.IsValid) {
				ma_sound_stop(&instance.Sound);
				ma_sound_uninit(&instance.Sound);
				instance.IsValid = false;
			}
		}

		s_audioMap.clear();
		s_nextHandle = 1;
	}

	void AudioManager::PlayAudioSource(AudioSource& source) {
		if (!s_IsInitialized || !source.GetAudioHandle().IsValid()) {
			return;
		}

		// Stop current instance if already playing
		if (source.GetInstanceId() != 0) {
			StopAudioSource(source);
		}

		// Create new sound instance
		uint32_t instanceId = CreateSoundInstance(source.GetAudioHandle());
		if (instanceId == 0) {
			std::cerr << "AudioManager: Failed to create sound instance" << std::endl;
			return;
		}

		source.SetInstanceId(instanceId);
		SoundInstance* instance = GetSoundInstance(instanceId);

		if (instance) {
			// Apply all source settings
			ma_sound_set_volume(&instance->Sound, source.GetVolume() * s_masterVolume);
			ma_sound_set_pitch(&instance->Sound, source.GetPitch());
			ma_sound_set_looping(&instance->Sound, source.IsLooping());

			if (source.IsSpatial()) {
				ma_sound_set_positioning(&instance->Sound, ma_positioning_absolute);
				const auto& pos = source.GetPosition();
				ma_sound_set_position(&instance->Sound, pos.x, pos.y, pos.z);

				const auto& vel = source.GetVelocity();
				ma_sound_set_velocity(&instance->Sound, vel.x, vel.y, vel.z);

				ma_sound_set_min_distance(&instance->Sound, source.GetMinDistance());
				ma_sound_set_max_distance(&instance->Sound, source.GetMaxDistance());
				ma_sound_set_attenuation_model(&instance->Sound, static_cast<ma_attenuation_model>(source.GetAttenuationModel()));
			}
			else {
				ma_sound_set_positioning(&instance->Sound, ma_positioning_relative);
			}

			// Start playback
			ma_result result = ma_sound_start(&instance->Sound);
			if (result != MA_SUCCESS) {
				std::cerr << "AudioManager: Failed to start sound playback. Error: " << result << std::endl;
				DestroySoundInstance(instanceId);
				source.SetInstanceId(0);
			}
		}
	}

	void AudioManager::PauseAudioSource(AudioSource& source) {
		if (!s_IsInitialized || source.GetInstanceId() == 0) {
			return;
		}

		SoundInstance* instance = GetSoundInstance(source.GetInstanceId());
		if (instance && instance->IsValid) {
			ma_sound_stop(&instance->Sound);
		}
	}

	void AudioManager::StopAudioSource(AudioSource& source) {
		if (!s_IsInitialized || source.GetInstanceId() == 0) {
			return;
		}

		DestroySoundInstance(source.GetInstanceId());
		source.SetInstanceId(0);
	}

	void AudioManager::ResumeAudioSource(AudioSource& source) {
		if (!s_IsInitialized || source.GetInstanceId() == 0) {
			return;
		}

		SoundInstance* instance = GetSoundInstance(source.GetInstanceId());
		if (instance && instance->IsValid) {
			ma_sound_start(&instance->Sound);
		}
	}

	void AudioManager::SetMasterVolume(float volume) {
		s_masterVolume = std::max(0.0f, volume);

		if (s_IsInitialized) {
			ma_engine_set_volume(&s_Engine, s_masterVolume);
		}
	}

	void AudioManager::SetListenerPosition(const glm::vec3& position) {
		s_listenerPosition = position;

		if (s_IsInitialized) {
			ma_engine_listener_set_position(&s_Engine, 0, position.x, position.y, position.z);
		}
	}

	void AudioManager::SetListenerVelocity(const glm::vec3& velocity) {
		s_listenerVelocity = velocity;

		if (s_IsInitialized) {
			ma_engine_listener_set_velocity(&s_Engine, 0, velocity.x, velocity.y, velocity.z);
		}
	}

	void AudioManager::SetListenerOrientation(const glm::vec3& forward, const glm::vec3& up) {
		s_listenerForward = forward;
		s_listenerUp = up;

		if (s_IsInitialized) {
			ma_engine_listener_set_direction(&s_Engine, 0, forward.x, forward.y, forward.z);
			ma_engine_listener_set_world_up(&s_Engine, 0, up.x, up.y, up.z);
		}
	}

	void AudioManager::PlayOneShot(const AudioHandle& handle, float volume) {
		if (!s_IsInitialized || !handle.IsValid()) {
			return;
		}

		const Audio* audio = GetAudio(handle);
		if (!audio) {
			return;
		}

		// Create a temporary sound for one-shot playback
		ma_sound sound;
		ma_result result = ma_sound_init_from_file(&s_Engine, audio->GetFilepath().c_str(), 0, nullptr, nullptr, &sound);

		if (result == MA_SUCCESS) {
			ma_sound_set_volume(&sound, volume * s_masterVolume);
			ma_sound_start(&sound);

			// Note: The sound will automatically be cleaned up by miniaudio when it finishes
		}
		else {
			std::cerr << "AudioManager: Failed to create one-shot sound. Error: " << result << std::endl;
		}
	}

	void AudioManager::PlayOneShotAtPosition(const AudioHandle& handle, const glm::vec3& position, float volume) {
		if (!s_IsInitialized || !handle.IsValid()) {
			return;
		}

		const Audio* audio = GetAudio(handle);
		if (!audio) {
			return;
		}

		// Create a temporary positioned sound for one-shot playback
		ma_sound sound;
		ma_result result = ma_sound_init_from_file(&s_Engine, audio->GetFilepath().c_str(), 0, nullptr, nullptr, &sound);

		if (result == MA_SUCCESS) {
			ma_sound_set_volume(&sound, volume * s_masterVolume);
			ma_sound_set_positioning(&sound, ma_positioning_absolute);
			ma_sound_set_position(&sound, position.x, position.y, position.z);
			ma_sound_start(&sound);
		}
		else {
			std::cerr << "AudioManager: Failed to create positioned one-shot sound. Error: " << result << std::endl;
		}
	}

	bool AudioManager::IsAudioLoaded(const AudioHandle& handle) {
		if (!handle.IsValid()) {
			return false;
		}

		return s_audioMap.find(handle.GetHandle()) != s_audioMap.end();
	}

	const Audio* AudioManager::GetAudio(const AudioHandle& handle) {
		if (!handle.IsValid()) {
			return nullptr;
		}

		auto it = s_audioMap.find(handle.GetHandle());
		return (it != s_audioMap.end()) ? it->second.get() : nullptr;
	}

	// Private methods implementation

	AudioHandle::HandleType AudioManager::GenerateHandle() {
		return s_nextHandle++;
	}

	uint32_t AudioManager::CreateSoundInstance(const AudioHandle& audioHandle) {
		if (!audioHandle.IsValid()) {
			return 0;
		}

		const Audio* audio = GetAudio(audioHandle);
		if (!audio || !audio->IsLoaded()) {
			return 0;
		}

		uint32_t instanceId;

		// Reuse a free instance if available
		if (!s_freeInstanceIndices.empty()) {
			instanceId = s_freeInstanceIndices.back();
			s_freeInstanceIndices.pop_back();
		}
		else {
			// Create new instance
			instanceId = static_cast<uint32_t>(s_soundInstances.size());
			s_soundInstances.emplace_back();
		}

		SoundInstance& instance = s_soundInstances[instanceId];

		// Initialize sound from the audio file
		ma_result result = ma_sound_init_from_file(&s_Engine, audio->GetFilepath().c_str(),
			0, nullptr, nullptr, &instance.Sound);

		if (result != MA_SUCCESS) {
			std::cerr << "AudioManager: Failed to create sound instance. Error: " << result << std::endl;
			s_freeInstanceIndices.push_back(instanceId);
			return 0;
		}

		instance.AudioHandle = audioHandle;
		instance.IsValid = true;

		return instanceId + 1; // Add 1 so that 0 can represent invalid ID
	}

	void AudioManager::DestroySoundInstance(uint32_t instanceId) {
		if (instanceId == 0) {
			return;
		}

		uint32_t index = instanceId - 1; // Subtract 1 because we added 1 in CreateSoundInstance

		if (index >= s_soundInstances.size()) {
			return;
		}

		SoundInstance& instance = s_soundInstances[index];
		if (instance.IsValid) {
			ma_sound_stop(&instance.Sound);
			ma_sound_uninit(&instance.Sound);
			instance.IsValid = false;
			instance.AudioHandle = AudioHandle();

			s_freeInstanceIndices.push_back(index);
		}
	}

	AudioManager::SoundInstance* AudioManager::GetSoundInstance(uint32_t instanceId) {
		if (instanceId == 0) {
			return nullptr;
		}

		uint32_t index = instanceId - 1;

		if (index >= s_soundInstances.size()) {
			return nullptr;
		}

		SoundInstance& instance = s_soundInstances[index];
		return instance.IsValid ? &instance : nullptr;
	}

	void AudioManager::CleanupFinishedSounds() {
		for (size_t i = 0; i < s_soundInstances.size(); ++i) {
			SoundInstance& instance = s_soundInstances[i];

			if (instance.IsValid) {
				// Check if sound has finished playing
				if (!ma_sound_is_playing(&instance.Sound)) {
					ma_sound_uninit(&instance.Sound);
					instance.IsValid = false;
					instance.AudioHandle = AudioHandle();
					s_freeInstanceIndices.push_back(static_cast<uint32_t>(i));
				}
			}
		}
	}

	void AudioManager::UpdateListener() {
		if (!s_IsInitialized) {
			return;
		}

		// Update listener properties (already set individually, but could batch here if needed)
		// This is called every frame to allow for smooth listener updates
	}

	void AudioManager::UpdateSoundInstances() {
		if (!s_IsInitialized) {
			return;
		}

		// Update any dynamic properties of sound instances
		// This could include things like doppler effects, distance calculations, etc.
		// For now, most updates are handled when properties are set directly
	}

} // namespace Bolt