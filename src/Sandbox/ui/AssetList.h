#pragma once

#include <Ares.h>
#include <imgui.h>

class AssetListElement : public Ares::ImGuiElement
{
public:
	AssetListElement();

	void Draw() override;

private:
	bool OnAssetLoaded(Ares::AssetEvent& result);

private:
	std::vector<Ares::Ref<Ares::AssetInfo>> m_AssetList;
	int32_t m_ActiveTooltipIndex = -1;
};