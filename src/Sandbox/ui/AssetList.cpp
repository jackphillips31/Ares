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

	ImGui::Text("List of Assets:");
	ImGui::BeginChild("ScrollableList", ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
	for (size_t i = 0; i < m_AssetList.size(); i++)
	{
		Ares::Ref<Ares::Asset>& currentAsset = m_AssetList[i];
		ImGui::BulletText("%s - %s", currentAsset->GetTypeName().c_str(), currentAsset->GetName().c_str());

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup(("pop up" + std::to_string(i)).c_str());
		}

		if (ImGui::BeginPopup(("pop up" + std::to_string(i)).c_str()))
		{
			ImGui::Text("Asset ID: %u", currentAsset->GetAssetId());
			std::string filepath = currentAsset->HasFilepath() ? currentAsset->GetFilepath() : "N/A";
			ImGui::Text("Filepath:");
			ImGui::Text(filepath.c_str());
			ImGui::Text("Asset Ref Count: %u", currentAsset->GetAsset().use_count());
			ImGui::Text("Asset Wrapper Ref Count: %u", currentAsset.use_count());
			if (currentAsset->HasFilepath())
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
	
	ImGui::End();
}

bool AssetListElement::OnAssetLoaded(Ares::AssetEvent& result)
{
	m_AssetList = Ares::AssetManager::GetCompleteList();
	return false;
}