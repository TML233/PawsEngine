#pragma once

#include "Engine/System/Memory/Memory.h"

namespace Engine {
	/// @brief Default heap object deleter for single pointer.
	template<typename T>
	class DefaultDeleter {
	public:
		void operator()(T* ptr) {
			MEMDEL(ptr);
		}
	};

	/// @brief Default heap object deleter for pointer of array.
	template<typename T>
	class DefaultDeleter<T[]> {
	public:
		void operator()(T* ptr) {
			MEMDELARR(ptr);
		}
	};

	/// @brief Smart pointer for managing exclusive heap pointers.\n
	/// Cannot be copied, but can be moved.
	template<typename T, typename TDeleter = DefaultDeleter<T>>
	class UniquePtr final {
	public:
		template<typename ... Args>
		static UniquePtr Create(Args&& ... args) {
			return UniquePtr(MEMNEW(T(Memory::Forward<Args>(args)...)));
		}

		explicit UniquePtr(T* ptr = nullptr) :ptr(ptr) {}
		~UniquePtr() {
			Reset();
		}
		
		// No copying
		UniquePtr(const UniquePtr& obj) = delete;
		UniquePtr& operator=(const UniquePtr& obj) = delete;
		// Moving
		UniquePtr(UniquePtr&& obj) {
			ptr = obj.ptr;
			obj.ptr = nullptr;
		}
		UniquePtr& operator=(UniquePtr&& obj) {
			if (this == &obj) {
				return *this;
			}
			ptr = obj.ptr;
			obj.ptr = nullptr;
			return *this;
		}

		T* GetRaw() const {
			return ptr;
		}
		T* Release() {
			T* ptr = this->ptr;
			this->ptr = nullptr;
			return ptr;
		}
		void Reset(T* ptr = nullptr) {
			if (this->ptr != nullptr) {
				deleter(this->ptr);
			}
			this->ptr = ptr;
		}
		T& operator*() const {
			return *ptr;
		}
		T* operator->() const {
			return ptr;
		}
		bool operator==(T* ptr) const {
			return this->ptr == ptr;
		}
		bool operator!=(T* ptr) const {
			return this->ptr != ptr;
		}
		operator bool() {
			return ptr != nullptr;
		}
	private:
		T* ptr = nullptr;
		TDeleter deleter;
	};

	/// @brief Smart pointer for managing exclusive heap array pointers.\n
	/// Cannot be copied, but can be moved.
	template<typename T, typename TDeleter>
	class UniquePtr<T[], TDeleter> {
	public:
		static UniquePtr Create(sizeint length) {
			return UniquePtr(MEMNEWARR(T, length));
		}

		explicit UniquePtr(T* ptr = nullptr) :ptr(ptr) {}
		~UniquePtr() {
			Reset();
		}

		// No copying
		UniquePtr(const UniquePtr& obj) = delete;
		UniquePtr& operator=(const UniquePtr& obj) = delete;
		// Moving
		UniquePtr(UniquePtr&& obj) {
			ptr = obj.ptr;
			obj.ptr = nullptr;
		}
		UniquePtr& operator=(UniquePtr&& obj) {
			if (this == &obj) {
				return *this;
			}
			ptr = obj.ptr;
			obj.ptr = nullptr;
			return *this;
		}

		T* GetRaw() const {
			return ptr;
		}
		T* Release() {
			T* ptr = this->ptr;
			this->ptr = nullptr;
			return ptr;
		}
		void Reset(T* ptr = nullptr) {
			if (this->ptr != nullptr) {
				deleter(this->ptr);
			}
			this->ptr = ptr;
		}
		bool operator==(T* ptr) const {
			return this->ptr == ptr;
		}
		bool operator!=(T* ptr) const {
			return this->ptr != ptr;
		}
		operator bool() {
			return ptr != nullptr;
		}
	private:
		T* ptr = nullptr;
		TDeleter deleter;
	};
}