#include "ui/Editor/EntityList.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <imgui.h>

#include "assets/icons/IconsMaterialDesign.h"

EntityListElement::EntityListElement()
	: m_CurrentScene(nullptr), m_CurrentEntity(0)
{
}

void EntityListElement::Draw()
{
	/*
	ImGui::Begin("Entity List");
	ImGui::Text("Entities: %u", m_ComponentMap.size());
	ImGui::Separator();
	for (auto& [entityId, components] : m_ComponentMap)
	{
		if (m_CurrentScene != nullptr)
		{
			std::string entityName = m_CurrentScene->GetEntityManager()->GetEntityName(entityId);
			bool isOpen = ImGui::TreeNode(entityName.c_str());
			ImGui::PushID(entityId);
			const float framePaddingY = ImGui::GetStyle().FramePadding.y;
			const float labelHeight = ImGui::CalcTextSize("My Tree Node").y + framePaddingY * 2;
			const float indentWidth = ImGui::GetTreeNodeToLabelSpacing();
			ImGui::SameLine();
			float buttonX = ImGui::GetContentRegionAvail().x + ImGui::GetCursorPosX() - labelHeight;
			ImGui::SetCursorPosX(buttonX);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, -5));
			if (ImGui::Button("+", ImVec2(13, 13)))
			{
				// Handle button click
			}
			ImGui::PopStyleVar();
			ImGui::PopID();
			if (isOpen)
			{
				uint32_t componentIndex = 0;
				static ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
				if (ImGui::BeginTable("table" + componentIndex, 2, tableFlags))
				{
					ImGui::TableSetupColumn("Type");
					ImGui::TableSetupColumn("Info");

					for (std::type_index& componentType : components)
					{
						std::string componentName = Ares::Utility::Type::ExtractClassName(componentType.name());

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text(componentName.c_str());
						ImGui::TableSetColumnIndex(1);
						float cellWidth = ImGui::GetContentRegionAvail().x;
						ImGui::PushItemWidth(cellWidth);
						Ares::ECS::EntityManager* entityManager = m_CurrentScene->GetEntityManager();
						if (componentName == "Mesh")
						{
							Ares::ECS::Components::Mesh* meshComponent = entityManager->GetComponent<Ares::ECS::Components::Mesh>(entityId);
							ImGui::Text(meshComponent->GetName().c_str());
						}
						else if (componentName == "Material")
						{
							Ares::ECS::Components::Material* materialComponent = entityManager->GetComponent<Ares::ECS::Components::Material>(entityId);
							ImGui::Text(materialComponent->GetShaderName().c_str());
						}
						else if (componentName == "Position")
						{
							Ares::ECS::Components::Position* positionComponent = entityManager->GetComponent<Ares::ECS::Components::Position>(entityId);
							ImGui::DragFloat3("##Position", positionComponent->GetDataPointer(), 0.1f);
						}
						else if (componentName == "Orientation")
						{
							Ares::ECS::Components::Orientation* orientationComponent = entityManager->GetComponent<Ares::ECS::Components::Orientation>(entityId);
							ImGui::DragFloat3("##Rotation", &m_EntityRotations[entityId][0], 0.1f);
							orientationComponent->SetOrientation(m_EntityRotations[entityId]);
						}
						ImGui::PopItemWidth();
						componentIndex++;
					}
					ImGui::EndTable();
				}
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();
	*/
	Ares::ECS::EntityManager* entityManager = m_CurrentScene->GetEntityManager();
	ImGui::Begin("Entities");
	if (m_CurrentScene != nullptr)
	{
		static int32_t selected = -1;
		int32_t index = 0;
		for (auto& [entityId, componentVec] : m_ComponentMap)
		{
			const char* icon = ICON_MD_VIEW_IN_AR;
			for (std::type_index& component : componentVec)
			{
				if (component == typeid(Ares::ECS::Components::Camera))
					icon = ICON_MD_VIDEOCAM;
				if (component == typeid(Ares::ECS::Components::Light))
					icon = ICON_MD_LIGHT_MODE;
			}
			std::string entityName = entityManager->GetEntityName(entityId);
			ImGui::PushID(index);
			ImGui::SetNextItemAllowOverlap();
			if (ImGui::Selectable("", selected == index))
			{
				selected = index;
				m_CurrentEntity = entityId;
			}
			ImGui::SameLine();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.7f);
			ImGui::Text(icon); ImGui::SameLine(); ImGui::Text(entityName.c_str());
			ImGui::PopID();
			index++;
		}
	}
	ImGui::End();
	ImGui::Begin("Entity Properties");
	if (m_CurrentEntity != 0)
	{
		std::vector<std::type_index>& componentVec = m_ComponentMap[m_CurrentEntity];
		for (std::type_index& type : componentVec)
		{
			std::string label;
			std::vector<std::string> componentInfo;
			std::function<void()> infoCallback = nullptr;
			if (type == typeid(Ares::ECS::Components::Camera))
			{
				label = "Camera";
				infoCallback = [=]() {
					Ares::ECS::Components::Camera* camera = entityManager->GetComponent<Ares::ECS::Components::Camera>(m_CurrentEntity);
					std::string typeLabel;
					
					const char* lightTypes[] = { "Perspective", "Orthographic" };
					int32_t lightSelected = 0;

					switch (camera->GetMode())
					{
					case Ares::ECS::Components::Camera::Perspective:
					{
						typeLabel = "Perspective";
						lightSelected = 0;
						break;
					}
					case Ares::ECS::Components::Camera::Orthographic:
					{
						typeLabel = "Orthographic";
						lightSelected = 1;
						break;
					}
					default: typeLabel = "Error";
					}

					if (ImGui::BeginTable("CameraTable", 2, ImGuiTableFlags_PadOuterX))
					{
						// Set column width for labels (make them fixed)
						ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthFixed, 100.0f);
						ImGui::TableSetupColumn("##Input", ImGuiTableColumnFlags_WidthStretch);

						// Type Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
						ImGui::Text("Type:");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::Combo("##CameraTypeCombo", &lightSelected, lightTypes, IM_ARRAYSIZE(lightTypes)))
						{
							if (lightSelected == 0)
							{
								camera->SetPerspective(
									camera->GetViewportSize(),
									camera->GetPerspectiveFov(),
									camera->GetNearFarPlanes().x,
									camera->GetNearFarPlanes().y
								);
							}
							else if (lightSelected == 1)
							{
								camera->SetOrthographic(
									camera->GetViewportSize(),
									camera->GetOrthoZoom(),
									camera->GetNearFarPlanes().x,
									camera->GetNearFarPlanes().y
								);
							}
						}
						ImGui::PopItemWidth();

						// Type specific row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						if (lightSelected == 0)
						{
							float cameraFov = camera->GetPerspectiveFov();
							ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
							ImGui::Text("FOV:");
							ImGui::TableSetColumnIndex(1);
							ImGui::PushItemWidth(-1.0f);
							if (ImGui::DragFloat("##CameraFOV", &cameraFov, 0.1f, 0.0f, 180.0f))
								camera->SetPerspectiveFov(cameraFov);
							ImGui::PopItemWidth();
						}
						else if (lightSelected == 1)
						{
							float cameraZoom = camera->GetOrthoZoom();
							ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
							ImGui::Text("Zoom:");
							ImGui::TableSetColumnIndex(1);
							ImGui::PushItemWidth(-1.0f);
							if (ImGui::DragFloat("##CameraZoom", &cameraZoom, 0.01f, 0.0f, 180.0f))
								camera->SetOrthoZoom(cameraZoom);
							ImGui::PopItemWidth();
						}

						// Near/Far planes
						glm::vec2 nearFarPlanes = camera->GetNearFarPlanes();
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
						ImGui::Text("Near Plane:");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::DragFloat("##CameraNearPlane", &nearFarPlanes.x, 0.01f, 0.01f, 200.0f))
							camera->SetNearFarPlanes(nearFarPlanes);
						ImGui::PopItemWidth();
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
						ImGui::Text("Far Plane:");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::DragFloat("##CameraFarPlane", &nearFarPlanes.y, 0.01f, 0.01f, 200.0f))
							camera->SetNearFarPlanes(nearFarPlanes);
						ImGui::PopItemWidth();

						ImGui::EndTable();
					}
				};
			}
			else if (type == typeid(Ares::ECS::Components::Mesh))
			{
				label = "Mesh";
				infoCallback = [=]() {
					Ares::ECS::Components::Mesh* mesh = entityManager->GetComponent<Ares::ECS::Components::Mesh>(m_CurrentEntity);
					std::string meshSize = Ares::Utility::FormatSize(mesh->GetMeshSize());
					if (ImGui::BeginTable("MeshTable", 2, ImGuiTableFlags_PadOuterX))
					{
						// Set column width for labels (make them fixed)
						ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthFixed, 100.0f);
						ImGui::TableSetupColumn("##Input", ImGuiTableColumnFlags_WidthStretch);

						// Name Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("Mesh Name:");
						ImGui::TableSetColumnIndex(1);
						ImGui::Text(mesh->GetMeshName().c_str());

						// Size Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("Mesh Size:");
						ImGui::TableSetColumnIndex(1);
						ImGui::Text(meshSize.c_str());

						ImGui::EndTable();
					}
				};
			}
			else if (type == typeid(Ares::ECS::Components::Material))
			{
				label = "Material";
				infoCallback = [=]() {
					Ares::ECS::Components::Material* material = entityManager->GetComponent<Ares::ECS::Components::Material>(m_CurrentEntity);
					Ares::ECS::Components::MaterialProperties props = material->GetProperties();
					glm::vec4 color = glm::vec4(props.Basic.Color, props.Basic.Alpha);
					std::string shaderSize = Ares::Utility::FormatSize(material->GetShaderSize());
					if (ImGui::BeginTable("MatTable", 2, ImGuiTableFlags_PadOuterX))
					{
						// Set column width for labels (make them fixed)
						ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthFixed, 100.0f);
						ImGui::TableSetupColumn("##Input", ImGuiTableColumnFlags_WidthStretch);

						// Name Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("Shader Name:");
						ImGui::TableSetColumnIndex(1);
						ImGui::Text(material->GetShaderName().c_str());

						// Name Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("Shader Size:");
						ImGui::TableSetColumnIndex(1);
						ImGui::Text(shaderSize.c_str());

						// Color Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
						ImGui::Text("Color:");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::ColorEdit4("##MatColor", &color.r))
						{
							props.Basic.Color = glm::vec3(color.r, color.g, color.b);
							props.Basic.Alpha = color.a;
							material->SetProperties(props);
						}
						ImGui::PopItemWidth();

						// Roughness Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
						ImGui::Text("Roughness:");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::DragFloat("##MatRoughness", &props.Surface.Roughness, 0.001f, 0.0f, 0.999f))
						{
							material->SetProperties(props);
						}
						ImGui::PopItemWidth();

						// Metallic Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
						ImGui::Text("Metallic:");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::DragFloat("##MatMetallic", &props.Surface.Metallic, 0.001f, 0.0f, 0.999f))
						{
							material->SetProperties(props);
						}
						ImGui::PopItemWidth();

						// Reflectivity Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
						ImGui::Text("Reflectivity:");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::DragFloat("##MatReflectivity", &props.Surface.Reflectivity, 0.001f, 0.0f, 0.999f))
						{
							material->SetProperties(props);
						}
						ImGui::PopItemWidth();

						// Emissive Color Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
						ImGui::Text("Emissive Color:");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::ColorEdit3("##MatEmissiveCol", &props.Surface.EmissiveColor.r))
						{
							material->SetProperties(props);
						}
						ImGui::PopItemWidth();

						// Emissive Intensity Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
						ImGui::Text("Emissive Intensity:");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::DragFloat("##MatEmissiveInt", &props.Surface.EmissiveIntensity, 0.001f, 0.0f, 1.0f))
						{
							material->SetProperties(props);
						}
						ImGui::PopItemWidth();

						ImGui::EndTable();
					}
				};
			}
			else if (type == typeid(Ares::ECS::Components::Transform))
			{
				label = "Transform";
				infoCallback = [=]() {
					Ares::ECS::Components::Transform* transform = entityManager->GetComponent<Ares::ECS::Components::Transform>(m_CurrentEntity);
					glm::vec3 currentPos = transform->GetPosition();
					glm::quat currentRot = transform->GetRotation();
					glm::vec3 rotationAngle = glm::degrees(glm::eulerAngles(currentRot));
					glm::vec3 currentScale = transform->GetScale();
					float currentAllScale = currentScale.x;
					if (ImGui::BeginTable("TransformTable", 2, ImGuiTableFlags_PadOuterX))
					{
						// Set column width for labels (make them fixed)
						ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthFixed, 100.0f);
						ImGui::TableSetupColumn("##Input", ImGuiTableColumnFlags_WidthStretch);

						// Position Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
						ImGui::Text("Position:");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::DragFloat3("##Pos", glm::value_ptr(currentPos), 0.005f, -100000.0f, 100000.0f))
							transform->SetPosition(currentPos);
						ImGui::PopItemWidth();

						// Rotation Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
						ImGui::Text("Rotation:");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::DragFloat3("##Rot", glm::value_ptr(rotationAngle), 0.1f, -360.0f, 360.0f))
						{
							transform->SetRotation(rotationAngle);
						}
						ImGui::PopItemWidth();

						// Scale Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
						ImGui::Text("Scale:");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::BeginTable("NestedTable", 1))
						{
							ImGui::TableNextRow();
							ImGui::TableSetColumnIndex(0);
							ImGui::PushItemWidth(-1.0f);
							if (ImGui::DragFloat3("##Scale", glm::value_ptr(currentScale), 0.01f, 0.0f, 10.0f))
								transform->SetScale(currentScale);
							ImGui::PopItemWidth();

							ImGui::TableNextRow();
							ImGui::TableSetColumnIndex(0);
							ImGui::PushItemWidth(-1.0f);
							if (ImGui::DragFloat("##AllScale", &currentAllScale, 0.01f, 0.0f, 10.0f))
								transform->SetScale({ currentAllScale, currentAllScale, currentAllScale });
							ImGui::PopItemWidth();

							ImGui::EndTable();
						}

						ImGui::EndTable();
					}
				};
			}
			else if (type == typeid(Ares::ECS::Components::Light))
			{
				label = "Light";
				infoCallback = [=]() {
					Ares::ECS::Components::Light* light = entityManager->GetComponent<Ares::ECS::Components::Light>(m_CurrentEntity);
					std::string typeLabel;

					const char* types[] = { "Point", "Directional" };
					int32_t typeSelected = 0;

					glm::vec4 lightColor = light->GetColor();
					glm::vec4 lightProps = light->GetProperties();
					float lightIntensity = lightColor.w;
					float lightRange = lightProps.x;
					float lightFalloff = lightProps.y;
					float lightInnerAngle = lightProps.z;
					float lightOuterAngle = lightProps.w;

					switch (light->GetType())
					{
					case Ares::ECS::Components::Light::Directional:
					{
						typeLabel = "Directional";
						typeSelected = 1;
						break;
					}
					case Ares::ECS::Components::Light::Point:
					{
						typeLabel = "Point";
						typeSelected = 0;
						break;
					}
					default: typeLabel = "Error!";
					}

					if (ImGui::BeginTable("LightTable", 2, ImGuiTableFlags_PadOuterX))
					{
						// Set column width for labels (make them fixed)
						ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthFixed, 100.0f);
						ImGui::TableSetupColumn("##Input", ImGuiTableColumnFlags_WidthStretch);

						// Type Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
						ImGui::Text("Type:");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::Combo("##LightTypeCombo", &typeSelected, types, IM_ARRAYSIZE(types)))
						{
							if (typeSelected == 0)
								light->SetType(Ares::ECS::Components::Light::Point);
							if (typeSelected == 1)
								light->SetType(Ares::ECS::Components::Light::Directional);
						}
						ImGui::PopItemWidth();

						// Color Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
						ImGui::Text("Color:");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::ColorEdit3("##LightColor", &lightColor.x))
						{
							light->SetColor(lightColor);
						}
						ImGui::PopItemWidth();

						// Intensity Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
						ImGui::Text("Intensity:");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::DragFloat("##LightIntensity", &lightIntensity, 0.01f, 0.0f, 10.0f))
						{
							light->SetIntensity(lightIntensity);
						}
						ImGui::PopItemWidth();

						// Range Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
						ImGui::Text("Range:");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::DragFloat("##LightRange", &lightRange, 0.01f, 0.0f, 10.0f))
						{
							light->SetRange(lightRange);
						}
						ImGui::PopItemWidth();

						// Falloff Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
						ImGui::Text("Falloff:");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::DragFloat("##LightFalloff", &lightFalloff, 0.01f, 0.0f, 10.0f))
						{
							light->SetFalloff(lightFalloff);
						}
						ImGui::PopItemWidth();

						// Inner Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
						ImGui::Text("Inner Angle:");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::DragFloat("##LightInnerAngle", &lightInnerAngle, 0.01f, 0.0f, 10.0f))
						{
							light->SetInnerAngle(lightInnerAngle);
						}
						ImGui::PopItemWidth();

						// Outer Row
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
						ImGui::Text("Outer Angle:");
						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::DragFloat("##LightOuterAngle", &lightOuterAngle, 0.01f, 0.0f, 10.0f))
						{
							light->SetOuterAngle(lightOuterAngle);
						}
						ImGui::PopItemWidth();

						ImGui::EndTable();
					}
				};
			}
			else
			{
				label = "Error!";
			}
			if (ImGui::CollapsingHeader(label.c_str()))
			{
				if (infoCallback)
					infoCallback();
				for (std::string& infoLabel : componentInfo)
				{
					ImGui::Text(infoLabel.c_str());
				}
			}
		}
	}
	ImGui::End();
}

void EntityListElement::OnUpdate(const Ares::Timestep& ts)
{
	if (m_CurrentScene != nullptr)
	{
		Ares::ECS::EntityManager* entityManager = m_CurrentScene->GetEntityManager();
		auto& componentMap = entityManager->GetEntityComponents();

		// Iterate through the Entity Manager map
		for (const auto& [entityId, components] : componentMap)
		{
			// Get or create the entry in the local map
			std::vector<std::type_index>& typeIndices = m_ComponentMap[entityId];

			// Create a temporary vector of type indices from the Entity Manager map
			std::vector<std::type_index> newTypeIndices;
			newTypeIndices.reserve(components.size());
			for (const auto& [typeIndex, _] : components)
			{
				newTypeIndices.push_back(typeIndex);
			}

			// Sort both vectors for comparison (std::unordered_map does not guarantee order)
			std::sort(typeIndices.begin(), typeIndices.end());
			std::sort(newTypeIndices.begin(), newTypeIndices.end());

			// Update only if there is a difference
			if (typeIndices != newTypeIndices)
			{
				// Efficiently move the new vector
				typeIndices = std::move(newTypeIndices);
			}
		}

		// Remove keys from the local map that are no longer in the Entity Manager map
		for (auto it = m_ComponentMap.begin(); it != m_ComponentMap.end();)
		{
			if (componentMap.find(it->first) == componentMap.end())
			{
				it = m_ComponentMap.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
}

void EntityListElement::SetScene(Ares::ECS::Scene* scene)
{
	m_CurrentScene = scene;
}