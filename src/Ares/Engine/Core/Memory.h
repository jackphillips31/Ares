#pragma once
#include "Engine/Core/Core.h"

namespace Ares {

	namespace Internal {

		template <typename ObjectType>
		class SmartPointerBase
		{
		protected:
			struct ControlBlock
			{
				size_t StrongRefCount = 0;
				size_t WeakRefCount = 0;
			};

			SmartPointerBase(ObjectType* object, ControlBlock* controlBlock) : m_Object(object), m_ControlBlock(controlBlock) {}
			SmartPointerBase(ObjectType* object)
				: m_Object(object)
			{
				m_ControlBlock = new (Allocate(sizeof(ControlBlock))) ControlBlock();
				if (!m_ControlBlock) throw std::bad_alloc();
				m_ControlBlock->StrongRefCount++;
			}

			inline void AddReference()
			{
				if (m_ControlBlock)
					m_ControlBlock->StrongRefCount++;
			}

			inline void ReleaseReference()
			{
				if (m_ControlBlock && --m_ControlBlock->StrongRefCount == 0)
				{
					m_ControlBlock->~ControlBlock();
					Deallocate(m_ControlBlock, sizeof(ControlBlock));
					m_ControlBlock = nullptr;

					if (!m_Object) return;
					m_Object->~ObjectType();
					Deallocate(m_Object, sizeof(ObjectType));
					m_Object = nullptr;
				}
			}

			ObjectType* m_Object = nullptr;
			ControlBlock* m_ControlBlock = nullptr;
			mutable std::shared_mutex m_Mutex;

		public:
			virtual ~SmartPointerBase()
			{
				ReleaseReference();
			}

			inline ObjectType* operator->() {
				std::shared_lock lock(m_Mutex);
				if (!m_Object)
					throw Internal::Exception(ErrorCode::DEREF_NULL_POINTER);
				return m_Object;
			}
		};

	}

	template <typename ObjectType>
	class AppScope : public Internal::SmartPointerBase<ObjectType>
	{
	public:
		AppScope(ObjectType* obj) : Internal::SmartPointerBase<ObjectType>(obj) {}
		AppScope(const AppScope<ObjectType>&) = delete;
		AppScope& operator=(const AppScope<ObjectType>&) = delete;
		AppScope(AppScope<ObjectType>&& other) noexcept
			: Internal::SmartPointerBase<ObjectType>(nullptr)
		{
			std::unique_lock lock(other.m_Mutex);
			this->m_Object = other.m_Object;
			this->m_ControlBlock = other.m_ControlBlock;
			other.m_Object = nullptr;
			other.m_ControlBlock = nullptr;
		}
		AppScope& operator=(AppScope<ObjectType>&& other) noexcept
		{
			if (this != &other)
			{
				std::unique_lock lock1(this->m_Mutex, std::defer_lock);
				std::unique_lock lock2(other.m_Mutex, std::defer_lock);
				std::lock(lock1, lock2);
				this->m_Object = other.m_Object;
				this->m_ControlBlock = other.m_ControlBlock;
				other.m_Object = nullptr;
				other.m_ControlBlock = nullptr;
			}
			return *this;
		}

		inline ObjectType* Release() {
			std::unique_lock lock(this->m_Mutex);
			ObjectType* result = this->m_Object;
			this->m_Object = nullptr;
			this->ReleaseReference();
			return result;
		}
	};

	template <typename ObjectType>
	class AppRef : public Internal::SmartPointerBase<ObjectType>
	{
	public:
		AppRef(ObjectType* obj) : Internal::SmartPointerBase<ObjectType>(obj) {}
		AppRef(AppRef<ObjectType>&&) = delete;
		AppRef& operator=(AppRef<ObjectType>&&) = delete;
		AppRef(const AppRef<ObjectType>& other)
			: Internal::SmartPointerBase<ObjectType>(nullptr, nullptr)
		{
			std::unique_lock lock(other.m_Mutex);
			this->m_Object = other.m_Object;
			this->m_ControlBlock = other.m_ControlBlock;
			this->AddReference();
		}
		AppRef& operator=(const AppRef<ObjectType>& other)
		{
			if (this != &other)
			{
				std::unique_lock lock1(this->m_Mutex, std::defer_lock);
				std::unique_lock lock2(other.m_Mutex, std::defer_lock);
				std::lock(lock1, lock2);
				this->m_Object = other.m_Object;
				this->m_ControlBlock = other.m_ControlBlock;
				this->AddReference();
			}
			return *this;
		}
	};

}