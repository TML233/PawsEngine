#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/Exception.h"
#include <memory>
#include <new>
#include <type_traits>

// Partly referenced Godot Engine 3.2.3 source code.
// https://www.github.com/godotengine/godot

namespace Engine {
	class Memory final {
		STATIC_CLASS(Memory);
	public:
		// Allocate a memory of the specific size.
		static void* Allocate(sizeint size);
		// Resize a memory block.
		static void* Reallocate(void* ptr, sizeint newSize);
		// Free a memory block.
		static void Deallocate(void* ptr);

		template<typename T>
		static constexpr bool IsDestructionNeeded() {
			return !__has_trivial_destructor(T);
		}

		// Constructs a object on an existing memory.
		template<typename T,typename ... Args>
		static void Construct(T* ptr, Args&& ... args) {
			// Placement new
			new (ptr) T(Object::Forward<Args>(args)...);
		}
		// Deconstructs a object but doesn't deallocate the memory.
		template<typename T>
		static void Destruct(T* ptr) {
			if (ptr == nullptr) {
				return;
			}
			// Only do necessary destructor calls
			if (IsDestructionNeeded<T>()) {
				ptr->~T();
			}
		}

		// Use MEMDEL(ptr) instead.
		template<typename T>
		static void Delete(T* ptr) {
			if (ptr == nullptr) {
				return;
			}
			// Call destructor if necessary.
			Destruct(ptr);
			// Free memory.
			Deallocate(ptr);
		}

		template<typename T,typename ... Args>
		static T* NewArray(sizeint count, Args&& ... args) {
			if (count <= 0) {
				throw ArgumentOutOfRangeException("count", "must be larger than 0.");
			}

			// Reserve a few bytes of size_t for saving the count data.
			sizeint size = sizeof(T) * count + sizeof(sizeint);

			// Allocate memory and write count data.
			sizeint* rawPtr = (sizeint*)Allocate(size);
			rawPtr[0] = count;

			// Do constructions.
			T* ptr = (T*)(rawPtr + 1);
			for (sizeint i = 0; i < count; i += 1) {
				Construct(ptr + i, Object::Forward<Args>(args)...);
			}

			return ptr;
		}

		template<typename T>
		static void DeleteArray(T* ptr) {
			if (ptr == nullptr) {
				return;
			}

			// Get count data.
			sizeint* rawPtr = ((sizeint*)ptr) - 1;
			sizeint count = rawPtr[0];

			// Do destructions if necessary.
			if (IsDestructionNeeded<T>()) {
				for (sizeint i = 0; i < count; i += 1) {
					Destruct(ptr + i);
				}
			}

			// Free memory.
			Deallocate(rawPtr);
		}

		static sizeint GetHeapArrayElementCount(void* ptr);
	};
}

#define MEMNEW(type) new (true) type
#define MEMDEL(ptr) ::Engine::Memory::Delete(ptr)
#define MEMNEWARR(type,count) ::Engine::Memory::NewArray<type>(count)
#define MEMDELARR(ptr) ::Engine::Memory::DeleteArray(ptr)

void* operator new(size_t size, bool reserved);
// Paired operator delete for freeing memory when exception is thrown in ctor.
void operator delete(void* ptr, bool reserved);
