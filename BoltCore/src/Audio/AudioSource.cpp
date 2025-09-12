#include "Bolt/btpch.hpp"
#include "Bolt/Audio/AudioSource.hpp"
#include "Bolt/Audio/AudioManager.hpp"


namespace Bolt {

    AudioSource::AudioSource(const AudioHandle& audioHandle)
        : m_audioHandle(audioHandle)
    {
    }

    void AudioSource::Play() {
        if (!m_audioHandle.IsValid()) {
            std::cerr << "AudioSource: Cannot play - invalid audio handle" << std::endl;
            return;
        }

        // Delegate to AudioManager for actual playback

        AudioManager::PlayAudioSource(*this);
    }

    void AudioSource::Pause() {
        if (m_instanceId == 0) {
            return; // Nothing is playing
        }

        AudioManager::PauseAudioSource(*this);

    }

    void AudioSource::Stop() {
        if (m_instanceId == 0) {
            return; // Nothing is playing
        }

        AudioManager::StopAudioSource(*this);
    }

    void AudioSource::Resume() {
        if (m_instanceId == 0) {
            return; // Nothing to resume
        }

        AudioManager::ResumeAudioSource(*this);
    }

    void AudioSource::SetVolume(float volume) {
        m_Volume = std::max(0.0f, volume); // Clamp to minimum 0

        // If currently playing, update the live sound instance
        if (m_instanceId != 0) {
            auto* instance = AudioManager::GetSoundInstance(m_instanceId);
            if (instance && instance->IsValid) {
                float masterVolume = AudioManager::GetMasterVolume();
                ma_sound_set_volume(&instance->Sound, m_Volume * masterVolume);
            }
        }
    }

    void AudioSource::SetPitch(float pitch) {
        m_Pitch = std::max(0.01f, pitch); // Clamp to minimum 0.01 to avoid issues

        // If currently playing, update the live sound instance
        if (m_instanceId != 0) {
            auto* instance = AudioManager::GetSoundInstance(m_instanceId);
            if (instance && instance->IsValid) {
                ma_sound_set_pitch(&instance->Sound, m_Pitch);
            }
        }
    }

    void AudioSource::SetLoop(bool loop) {
        m_Loop = loop;

        // If currently playing, update the live sound instance
        if (m_instanceId != 0) {
            auto* instance = AudioManager::GetSoundInstance(m_instanceId);
            if (instance && instance->IsValid) {
                ma_sound_set_looping(&instance->Sound, m_Loop);
            }
        }
    }

    void AudioSource::SetSpatial(bool spatial) {
        m_Spatial = spatial;

        // If currently playing, update the live sound instance
        if (m_instanceId != 0) {
            auto* instance = AudioManager::GetSoundInstance(m_instanceId);
            if (instance && instance->IsValid) {
                if (m_Spatial) {
                    ma_sound_set_positioning(&instance->Sound, ma_positioning_absolute);
                    ma_sound_set_position(&instance->Sound, m_Position.x, m_Position.y, m_Position.z);
                    ma_sound_set_velocity(&instance->Sound, m_Velocity.x, m_Velocity.y, m_Velocity.z);
                    ma_sound_set_min_distance(&instance->Sound, m_MinDistance);
                    ma_sound_set_max_distance(&instance->Sound, m_MaxDistance);
                    ma_sound_set_attenuation_model(&instance->Sound, static_cast<ma_attenuation_model>(m_AttenuationModel));
                }
                else {
                    ma_sound_set_positioning(&instance->Sound, ma_positioning_relative);
                }
            }
        }
    }

    void AudioSource::SetPosition(const glm::vec3& position) {
        m_Position = position;

        // If spatial and currently playing, update the live sound instance
        if (m_Spatial && m_instanceId != 0) {
            auto* instance = AudioManager::GetSoundInstance(m_instanceId);
            if (instance && instance->IsValid) {
                ma_sound_set_position(&instance->Sound, m_Position.x, m_Position.y, m_Position.z);
            }
        }
    }

    void AudioSource::SetVelocity(const glm::vec3& velocity) {
        m_Velocity = velocity;

        // If spatial and currently playing, update the live sound instance
        if (m_Spatial && m_instanceId != 0) {
            auto* instance = AudioManager::GetSoundInstance(m_instanceId);
            if (instance && instance->IsValid) {
                ma_sound_set_velocity(&instance->Sound, m_Velocity.x, m_Velocity.y, m_Velocity.z);
            }
        }
    }

    void AudioSource::SetMinDistance(float distance) {
        m_MinDistance = std::max(0.0f, distance);

        // If spatial and currently playing, update the live sound instance
        if (m_Spatial && m_instanceId != 0) {
            auto* instance = AudioManager::GetSoundInstance(m_instanceId);
            if (instance && instance->IsValid) {
                ma_sound_set_min_distance(&instance->Sound, m_MinDistance);
            }
        }
    }

    void AudioSource::SetMaxDistance(float distance) {
        m_MaxDistance = std::max(m_MinDistance, distance); // Ensure max >= min

        // If spatial and currently playing, update the live sound instance
        if (m_Spatial && m_instanceId != 0) {
            auto* instance = AudioManager::GetSoundInstance(m_instanceId);
            if (instance && instance->IsValid) {
                ma_sound_set_max_distance(&instance->Sound, m_MaxDistance);
            }
        }
    }

    void AudioSource::SetAttenuationModel(int model) {
        // Validate attenuation model (miniaudio supports 0-4)
        if (model >= 0 && model <= 4) {
            m_AttenuationModel = model;

            // If spatial and currently playing, update the live sound instance
            if (m_Spatial && m_instanceId != 0) {
                auto* instance = AudioManager::GetSoundInstance(m_instanceId);
                if (instance && instance->IsValid) {
                    ma_sound_set_attenuation_model(&instance->Sound, static_cast<ma_attenuation_model>(m_AttenuationModel));
                }
            }
        }
        else {
            std::cerr << "AudioSource: Invalid attenuation model: " << model << ". Valid range is 0-4." << std::endl;
        }
    }

    bool AudioSource::IsPlaying() const {
        if (m_instanceId == 0) {
            return false;
        }

        auto* instance = AudioManager::GetSoundInstance(m_instanceId);
        if (instance && instance->IsValid) {
            return ma_sound_is_playing(&instance->Sound);
        }

        return false;
    }

    bool AudioSource::IsPaused() const {
        if (m_instanceId == 0) {
            return false;
        }

        auto* instance = AudioManager::GetSoundInstance(m_instanceId);
        if (instance && instance->IsValid) {
            // In miniaudio, a sound is paused if it's not playing but has been started
            // We need to track this state or check if the sound was manually stopped
            return !ma_sound_is_playing(&instance->Sound) && ma_sound_at_end(&instance->Sound) == MA_FALSE;
        }

        return false;
    }


    // Utility methods for easier usage
    void AudioSource::SetAudioHandle(const AudioHandle& handle) {
        // Stop current playback if switching audio
        if (IsPlaying()) {
            Stop();
        }

        m_audioHandle = handle;
    }

    void AudioSource::PlayOneShot() {
        if (!m_audioHandle.IsValid()) {
            std::cerr << "AudioSource: Cannot play one-shot - invalid audio handle" << std::endl;
            return;
        }

        if (m_Spatial) {
            AudioManager::PlayOneShotAtPosition(m_audioHandle, m_Position, m_Volume);
        }
        else {
            AudioManager::PlayOneShot(m_audioHandle, m_Volume);
        }
    }

    void AudioSource::FadeIn(float duration) {
        if (!IsPlaying()) {
            float originalVolume = m_Volume;
            SetVolume(0.0f);
            Play();

            // Note: For a complete fade implementation, you would need to implement
            // a tween system or update the volume over time in your game loop
            // This is a simplified version that just starts playback
            SetVolume(originalVolume);
        }
    }

    void AudioSource::FadeOut(float duration) {
        if (IsPlaying()) {
            // Note: For a complete fade implementation, you would gradually
            // reduce volume over time and then stop the sound
            // This is a simplified version that just stops immediately
            Stop();
        }
    }
}