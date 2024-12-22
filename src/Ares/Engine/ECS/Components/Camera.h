#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Engine/ECS/Core/Component.h"

namespace Ares::ECS {

	namespace Components {

		struct Camera : public Component
		{
		public:
			enum Mode : uint8_t
			{
				Orthographic,
				Perspective
			};
		public:
			// Constructor
			Camera(Mode mode = Perspective)
				: mode(mode),
				viewportDimensions(1280.0f, 720.0f),
				orthoZoom(1.0f),
				perspectiveFov(90.0f),
				nearPlane(0.1f), farPlane(100.0f)
			{
				CalculateViewProjectionMatrix();
			}

			// Getters
			const Mode GetMode() const
			{
				return mode;
			}
			const glm::vec3& GetPosition() const
			{
				return position;
			}
			const glm::quat& GetOrientation() const
			{
				return orientation;
			}
			const glm::vec2 GetNearFarPlanes() const
			{
				return glm::vec2(nearPlane, farPlane);
			}
			const glm::vec2& GetViewportDimensions() const
			{
				return viewportDimensions;
			}
			const float& GetOrthoZoom() const
			{
				return orthoZoom;
			}
			const float& GetPerspectiveFov() const
			{
				return perspectiveFov;
			}
			const glm::mat4& GetViewProjectionMatrix()
			{
				CalculateViewProjectionMatrix();
				return viewProjectionMatrix;
			}


			// Setters
			void SetPosition(const glm::vec3& newPosition)
			{
				if (!(position == newPosition))
				{
					position = newPosition;
					viewMatrixDirty = true;
				}
			}
			void SetPosition(float x, float y, float z)
			{
				SetPosition({ x, y, z });
			}
			void SetOrientation(const glm::quat& newOrientation)
			{
				if (!(orientation == newOrientation))
				{
					orientation = newOrientation;
					viewMatrixDirty = true;
				}
			}
			void SetOrientation(const glm::vec3& eulerAngles)
			{
				SetOrientation(glm::quat(glm::radians(eulerAngles)));
			}
			void SetOrientation(float x, float y, float z)
			{
				orientation = glm::quat(glm::radians(glm::vec3(x, y, z)));
				viewMatrixDirty = true;
			}
			void SetNearFarPlanes(const float& newNearPlane, const float& newFarPlane)
			{
				if (nearPlane != newNearPlane || farPlane != newFarPlane)
				{
					nearPlane = newNearPlane;
					farPlane = newFarPlane;
					projectionMatrixDirty = true;
				}
			}
			void SetNearFarPlanes(const glm::vec2& newPlanes)
			{
				SetNearFarPlanes(newPlanes.x, newPlanes.y);
			}
			void SetViewportDimensions(const glm::vec2& newDimensions)
			{
				if (!(viewportDimensions == newDimensions))
				{
					viewportDimensions = newDimensions;
					projectionMatrixDirty = true;
				}
			}
			void SetViewportDimensions(float x, float y)
			{
				SetViewportDimensions({ x, y });
			}
			void SetViewportDimensions(uint32_t x, uint32_t y)
			{
				SetViewportDimensions({ static_cast<float>(x), static_cast<float>(y) });
			}
			void SetOrthoZoom(float newZoom)
			{
				orthoZoom = newZoom;
				projectionMatrixDirty = true;
			}
			void SetPerspectiveFov(float newFov)
			{
				perspectiveFov = newFov;
				projectionMatrixDirty = true;
			}

			// Set mode
			void SetOrthographic(const glm::vec2& dimensions, float newZoom, float newNearPlane, float newFarPlane)
			{
				viewportDimensions = dimensions;
				orthoZoom = newZoom;
				nearPlane = newNearPlane;
				farPlane = newFarPlane;
				projectionMatrixDirty = true;
				mode = Mode::Orthographic;
			}
			void SetPerspective(const glm::vec2& dimensions, float newFov, float newNearPlane, float newFarPlane)
			{
				viewportDimensions = dimensions;
				perspectiveFov = newFov;
				nearPlane = newNearPlane;
				farPlane = newFarPlane;
				projectionMatrixDirty = true;
				mode = Mode::Perspective;
			}

		private:
			void CalculateViewMatrix()
			{
				if (viewMatrixDirty)
				{
					// Translate to the position
					glm::mat4 translation = glm::translate(glm::mat4(1.0f), -position);

					// Rotate according to the quaternion
					glm::mat4 rotation = glm::toMat4(glm::conjugate(orientation));

					// Combine rotation and translation
					viewMatrix = rotation * translation;

					viewMatrixDirty = false;
					viewProjectionMatrixDirty = true;
				}
			}
			void CalculateProjectionMatrix()
			{
				if (projectionMatrixDirty)
				{
					if (mode == Mode::Orthographic)
					{
						float aspectRatio = viewportDimensions.x / viewportDimensions.y;

						projectionMatrix = glm::ortho(
							-aspectRatio * orthoZoom,
							aspectRatio * orthoZoom,
							-orthoZoom,
							orthoZoom,
							nearPlane,
							farPlane
						);

						projectionMatrixDirty = false;
						viewProjectionMatrixDirty = true;
					}
					else if (mode == Mode::Perspective)
					{
						projectionMatrix = glm::perspective(
							glm::radians(perspectiveFov),
							viewportDimensions.x / viewportDimensions.y,
							nearPlane, farPlane
						);

						projectionMatrixDirty = false;
						viewProjectionMatrixDirty = true;
					}
				}
			}
			void CalculateViewProjectionMatrix()
			{
				CalculateViewMatrix();
				CalculateProjectionMatrix();

				if (viewProjectionMatrixDirty)
				{
					viewProjectionMatrix = projectionMatrix * viewMatrix;
					viewProjectionMatrixDirty = false;
				}
			}

		private:
			// Projection type
			Mode mode;

			// View parameters
			glm::vec3 position = glm::vec3(0.0f);
			glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

			// Parameters for both types
			float nearPlane, farPlane;
			glm::vec2 viewportDimensions;

			// Orthographic parameters
			float orthoZoom;

			// Perspective parameters
			float perspectiveFov;

			// Matrices
			glm::mat4 viewMatrix;
			glm::mat4 projectionMatrix;
			glm::mat4 viewProjectionMatrix;

			// Are matrices dirty
			bool viewMatrixDirty = true;
			bool projectionMatrixDirty = true;
			bool viewProjectionMatrixDirty = true;
		};

	}

}