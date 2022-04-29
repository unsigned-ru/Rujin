#pragma once
#include "ResourceManager.h"

namespace rujin
{
	using clip_id = uint16_t;

	class AudioClip final
	{
	public:
		inline static clip_id s_InstanceCount{ 0 };

		AudioClip() = delete;

		AudioClip(const AudioClip& other) = delete;
		AudioClip(AudioClip&& other) noexcept = delete;
		AudioClip& operator=(const AudioClip& other) = delete;
		AudioClip& operator=(AudioClip&& other) noexcept = delete;

		~AudioClip();

		clip_id GetId() const;

	private:
		//friends!
		friend std::shared_ptr<AudioClip> ResourceManager::LoadAudio(const std::string& file);

		// Only use for re-creating previous unloaded resource with already assigned ID.
		explicit AudioClip(clip_id id, Mix_Chunk* pChunk);
		explicit AudioClip(Mix_Chunk* pChunk);

		//friend of audio player. No one else touches this.
		Mix_Chunk* GetChunk() const;

		//Member Vars
		const clip_id m_Id;
		Mix_Chunk* m_pChunk;
	};
}
