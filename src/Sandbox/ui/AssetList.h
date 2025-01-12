#pragma once
#include <Ares.h>

class AssetListElement : public Ares::ImGuiElement
{
public:
	AssetListElement();

	void Draw() override;

private:
	bool OnAssetLoaded(Ares::AssetLoadedEvent& result);

private:
	std::vector<Ares::Ref<Ares::Asset>> m_AssetList;
	int32_t m_ActiveTooltipIndex = -1;
};