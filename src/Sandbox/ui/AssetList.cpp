#include "ui/AssetList.h"

AssetListElement::AssetListElement()
{
	m_AssetList = Ares::AssetManager::GetCompleteList();
	Ares::EventQueue::AddListener<Ares::AssetLoadedEvent>(AR_BIND_EVENT_FN(AssetListElement::OnAssetLoaded));
}

void AssetListElement::Draw()
{
	ImGui::Begin("Asset List");

	float buttonWidth = ImGui::GetContentRegionAvail().x;

	if (ImGui::Button("Refresh List", ImVec2(buttonWidth, 0)))
	{
		m_AssetList = Ares::AssetManager::GetCompleteList();
	}

	ImGui::Separator();

	ImGui::Text("List of Assets:");
	for (size_t i = 0; i < m_AssetList.size(); i++)
	{
		ImGui::BulletText("%s", m_AssetList[i].second.c_str());

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup(("Popup" + std::to_string(i)).c_str());
		}

		if (ImGui::BeginPopup(("Popup" + std::to_string(i)).c_str()))
		{
			ImGui::Text("Filepath:");
			ImGui::Text(m_AssetList[i].first.c_str());
			ImGui::Separator();
			if (ImGui::MenuItem("Open File"))
			{
				AR_BUG("Open File does not work yet!");
			}

			ImGui::EndPopup();
		}
	}

	ImGui::End();
}

bool AssetListElement::OnAssetLoaded(Ares::AssetLoadedEvent& result)
{
	m_AssetList = Ares::AssetManager::GetCompleteList();
	return false;
}