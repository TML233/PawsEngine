#pragma once

#include <cstdint>

#define STATIC_CLASS(name)					\
name() = delete;							\
~name() = delete;							\
name(const name&) = delete;					\
name& operator=(const name&) = delete;		\
name(name&&) = delete;						\
name& operator=(name&&) = delete

namespace Engine {
	// Represents a signed 8-bit integer.
	using sbyte = std::int8_t;
	// Represents an unsigned 8-bit integer.
	using byte = std::uint8_t;
	// Represents a signed 16-bit integer.
	using int16 = std::int16_t;
	// Represents an unsigned 16-bit integer.
	using uint16 = std::uint16_t;
	// Represents a signed 32-bit integer.
	using int32 = std::int32_t;
	// Represents an unsigned 32-bit integer.
	using uint32 = std::uint32_t;
	// Represents a signed 64-bit integer.
	using int64 = std::int64_t;
	// Represents an unsigned 64-bit integer.
	using uint64 = std::uint64_t;
	// Represents a local machine sized integer which can hold the memory size.
	using sizeint = size_t;

	class String;
}