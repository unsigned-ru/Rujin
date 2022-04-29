#include "RujinPCH.h"
#include "AudioClip.h"


rujin::AudioClip::~AudioClip()
{
	Mix_FreeChunk(m_pChunk);
	m_pChunk = nullptr;
}

rujin::clip_id rujin::AudioClip::GetId() const
{
	return m_Id;
}

rujin::AudioClip::AudioClip(clip_id id, Mix_Chunk* pChunk)
	: m_Id(id)
	, m_pChunk(pChunk)
{
}

rujin::AudioClip::AudioClip(Mix_Chunk* pChunk)
	: m_Id(s_InstanceCount++)
	, m_pChunk(pChunk)
{
}

Mix_Chunk* rujin::AudioClip::GetChunk() const
{
	return m_pChunk;
}
