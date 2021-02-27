#pragma once

#include "Engine/System/Exception.h"
#include "Engine/System/Memory.h"

namespace Engine {
	// Default deleter which calls delete
	template<typename T>
	class DefaultDeleter {
	public:
		void operator()(T* ptr) {
			MEMDEL(ptr);
		}
	};

	// Default deleter for arrays which calls delete[]
	template<typename T>
	class DefaultDeleter<T[]> {
	public:
		void operator()(T* ptr) {
			MEMDELARR(ptr);
		}
	};

	// UniquePtr
	template<typename T, typename TDeleter = DefaultDeleter<T>>
	class UniquePtr final {
	public:
		explicit UniquePtr(T* ptr = nullptr) :ptr(ptr) {}
		~UniquePtr() {
			Reset();
		}
		
		// No copying
		UniquePtr(const UniquePtr& obj) = delete;
		UniquePtr& operator=(const UniquePtr obj) = delete;
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
		operator bool() {
			return ptr != nullptr;
		}
	private:
		T* ptr = nullptr;
		TDeleter deleter;
	};

	// UniquePtr for arrays
	template<typename T, typename TDeleter>
	class UniquePtr<T[], TDeleter> {
	public:
		explicit UniquePtr(T* ptr = nullptr) :ptr(ptr) {}
		~UniquePtr() {
			Reset();
		}

		// No copying
		UniquePtr(const UniquePtr& obj) = delete;
		UniquePtr& operator=(const UniquePtr obj) = delete;
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
		operator bool() {
			return ptr != nullptr;
		}
	private:
		T* ptr = nullptr;
		TDeleter deleter;
	};
}