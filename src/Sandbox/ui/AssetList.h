#pragma once

#include <Ares.h>
#include <imgui.h>

class AssetListElement : public Ares::ImGuiElement
{
public:
	AssetListElement();

	void Draw() override;

private:
	bool OnAssetLoaded(Ares::AssetLoadedEvent& result);

private:
	std::vector<std::pair<std::string, std::string>> m_AssetList;
	int32_t m_ActiveTooltipIndex = -1;
};