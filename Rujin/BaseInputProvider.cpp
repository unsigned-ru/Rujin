#include "RujinPCH.h"
#include "BaseInputProvider.h"

#include <array>
#include <numeric>

#include "PlayerInput.h"
#include "Rujin.h"

rujin::BaseInputProvider::BaseInputProvider()
	: InputService()
	, m_DeviceScanTimer{ s_DeviceScanDelay, true }
{
	//Set up iota data for player index queue.
	std::array<unsigned char, s_MaxPlayerCount> tempArr{};
	std::iota(tempArr.begin(), tempArr.end(), static_cast<unsigned char>(0u));

	//create player index queue
	m_PlayerIndexQueue = std::priority_queue<PlayerIndex, std::vector<PlayerIndex>, std::greater<PlayerIndex>>(tempArr.begin(), tempArr.end());
}

rujin::BaseInputProvider::~BaseInputProvider() = default;

void rujin::BaseInputProvider::Start()
{
	RegisterNewDevices();
	ProcessInput();
}

void rujin::BaseInputProvider::ProcessInput()
{
	//device scanning. (every 3 seconds)
	if (m_DeviceScanTimer.Tick(Rujin::Get()->GetDeltaTime()))
		RegisterNewDevices();

	//update connected devices
	for (auto it = m_Players.begin(); it != m_Players.end(); ++it)
	{
		it->second.Update();
	}
}

rujin::PlayerIndex rujin::BaseInputProvider::RegisterPlayer()
{
	//get next player index
	const PlayerIndex playerIndex = m_PlayerIndexQueue.top();
	m_PlayerIndexQueue.pop();

	//Create player and add to map
	m_Players.emplace(playerIndex, playerIndex);

	return playerIndex;
}

void rujin::BaseInputProvider::UnRegisterPlayer(PlayerIndex idx)
{
	m_Players.erase(idx);
}

void rujin::BaseInputProvider::AddInputAction(PlayerIndex player, uint32_t inputAction, const InputActionKeybinds& keybinds)
{
	if (const auto it = m_Players.find(player); it != m_Players.end())
		it->second.AddInputAction(inputAction, keybinds);
	else
		LOG_WARNING_("Could not find player with index '{}'", player);
}

void rujin::BaseInputProvider::AddAxisAction(PlayerIndex player, uint32_t axisAction, const AxisActionKeybinds& keybinds)
{
	if (const auto it = m_Players.find(player); it != m_Players.end())
		it->second.AddAxisAction(axisAction, keybinds);
	else
		LOG_WARNING_("Could not find player with index '{}'", player);
}

void rujin::BaseInputProvider::SetLeftVibration(PlayerIndex player, float intensity)
{
	if (const auto it = m_Players.find(player); it != m_Players.end())
		it->second.SetLeftVibration(intensity);
	else
		LOG_WARNING_("Could not find player with index '{}'", player);
}

void rujin::BaseInputProvider::SetRightVibration(PlayerIndex player, float intensity)
{
	if (const auto it = m_Players.find(player); it != m_Players.end())
		it->second.SetRightVibration(intensity);
	else
		LOG_WARNING_("Could not find player with index '{}'", player);
}

void rujin::BaseInputProvider::SetVibration(PlayerIndex player, float intensity)
{
	if (const auto it = m_Players.find(player); it != m_Players.end())
		it->second.SetVibration(intensity);
	else
		LOG_WARNING_("Could not find player with index '{}'", player);
}

bool rujin::BaseInputProvider::IsInputActionTriggered(PlayerIndex player, uint32_t inputAction) const
{
	if (const auto it = m_Players.find(player); it != m_Players.end())
		return it->second.IsInputActionTriggered(inputAction);

	LOG_WARNING_("Could not find player with index '{}'", player);
	return false;
}

bool rujin::BaseInputProvider::IsAxisActionTriggered(PlayerIndex player, const uint32_t axisAction, float* pIntensity) const
{
	if (const auto it = m_Players.find(player); it != m_Players.end())
		return it->second.IsAxisActionTriggered(axisAction, pIntensity);

	LOG_WARNING_("Could not find player with index '{}'", player);
	return false;
}

glm::ivec2 rujin::BaseInputProvider::GetMouseMovement() const
{
	if (m_pKeyboardSession)
		return m_pKeyboardSession->GetMouseMovement();

	LOG_WARNING_("Tried to get mouse position, but no keyboard and mouse input session was detected.");
	return glm::ivec2();
}

glm::ivec2 rujin::BaseInputProvider::GetMousePosition(const bool previousFrame) const
{
	if (m_pKeyboardSession)
		return m_pKeyboardSession->GetMousePosition(previousFrame);

	LOG_WARNING_("Tried to get mouse position, but no keyboard and mouse input session was detected.");
	return glm::ivec2();
}
