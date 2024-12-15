#include "ui/AssetList.h"

AssetListElement::AssetListElement()
{
	m_AssetList = Ares::AssetManager::GetCompleteList();
	Ares::EventQueue::AddListener<Ares::AssetEvent>(AR_BIND_EVENT_FN(AssetListElement::OnAssetLoaded));
}

void AssetListElement::Draw()
{
	ImGui::Begin("Asset List", nullptr, ImGuiWindowFlags_NoScrollbar);

	float buttonWidth = ImGui::GetContentRegionAvail().x;

	if (ImGui::Button("Refresh List", ImVec2(buttonWidth, 0)))
	{
		m_AssetList = Ares::AssetManager::GetCompleteList();
	}

	ImGui::Separator();

	float windowHeight = ImGui::GetContentRegionAvail().y;
	float buttonHeight = ImGui::GetTextLineHeightWithSpacing();
	float seperatorHeight = ImGui::GetStyle().ItemSpacing.y;
	float availableHeight = windowHeight - buttonHeight - 2 * (buttonHeight + seperatorHeight);

	ImGui::Text("List of Assets:");
	ImGui::BeginChild("ScrollableList", ImVec2(0, availableHeight), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
	for (size_t i = 0; i < m_AssetList.size(); i++)
	{
		Ares::Ref<Ares::AssetInfo> currentAsset = m_AssetList[i];
		ImGui::BulletText("%s - %s", currentAsset->TypeName.c_str(), currentAsset->Name.c_str());

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup(("Popup" + std::to_string(i)).c_str());
		}

		if (ImGui::BeginPopup(("Popup" + std::to_string(i)).c_str()))
		{
			ImGui::Text("Asset ID: %u", currentAsset->AssetId);
			std::string filepath = currentAsset->Filepath != "" ? currentAsset->Filepath : "N/A";
			ImGui::Text("Filepath:");
			ImGui::Text(filepath.c_str());
			if (currentAsset->HasFilepath)
			{
				ImGui::Separator();
				if (ImGui::MenuItem("Open File"))
				{
					AR_BUG("Open File does not work yet!");
				}
			}

			ImGui::EndPopup();
		}
	}

	ImGui::EndChild();
	
	ImGui::Separator();
	
	if (ImGui::Button("Get Count", ImVec2(buttonWidth, 0)))
	{
		AR_TRACE("Count: {}", m_AssetList[0]->Asset.use_count());
	}

	ImGui::End();
}

bool AssetListElement::OnAssetLoaded(Ares::AssetEvent& result)
{
	m_AssetList = Ares::AssetManager::GetCompleteList();
	return false;
}