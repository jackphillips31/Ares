#pragma once
#include <Ares.h>

class AssetListElement : public Ares::ImGuiElement
{
public:
	AssetListElement(Ares::Application& app);

	void Draw() override;

private:
	bool OnAssetLoaded(Ares::AssetLoadedEvent& result);

private:
	eastl::vector<Ares::Ref<Ares::Asset>> m_AssetList;
	int32_t m_ActiveTooltipIndex = -1;
	Ares::Systems::AssetManager* m_AssetManager;
};