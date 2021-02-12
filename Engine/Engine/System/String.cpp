#include "Engine/System/Definition.h"
#include "Engine/System/String.h"
#include "Engine/System/Exception.h"
#include "Engine/Collection/Iterator.h"
#include "Engine/System/IEnumerable.h"
#include <string>
#include <cstring>


namespace Engine {
	StringData::StringData(const char* data, int32 count) {
		length = count + 1;
		this->data = new char[length];
		// Copy data
		std::memcpy(this->data, data, count);
		// Add NULL
		std::memset(this->data + count, 0, sizeof(char));
	}
	StringData::~StringData() {
		delete[] data;
	}
	std::shared_ptr<StringData> StringData::empty = std::make_shared<StringData>("", sizeof(""));

	String::String(const char* string) {
		PrepareData(string, std::strlen(string));
	}
	String::String(const std::string& string) {
		PrepareData(string.c_str(), string.length());
	}
	String& String::operator=(const char* string) {
		PrepareData(string, std::strlen(string));
		return *this;
	}

	bool String::IsIndividual() const {
		return (refStart == 0 && refCount == data->length - 1);
	}

	String String::ToIndividual() const {
		if (IsIndividual()) {
			return *this;
		}
		return String(data->data + refStart, refCount);
	}

	ReadonlyIterator<char> String::operator[](int32 index) const {
		if (index >= data->length) {
			throw ArgumentOutOfRangeException("index", "out of the range of the string.");
		}
		return ReadonlyIterator<char>(&(data->data[index]));
	}

	int32 String::GetCount() const {
		return refCount;
	}

	const char* String::GetRawArray() const {
		return data->data;
	}

	int32 String::IndexOf(const String& pattern) const {
		if (GetCount() < pattern.GetCount()) {
			return -1;
		}
		return searcher.Search(GetStartPtr(), GetCount(), pattern.GetStartPtr(), pattern.GetCount());
	}

	String String::Substring(int32 startIndex, int32 count) const {
		if (startIndex < 0) {
			throw ArgumentOutOfRangeException("startIndex", "Out of range.");
		}
		if ((count < 0) || count > (GetCount() - startIndex)) {
			throw ArgumentOutOfRangeException("count", "Out of range.");
		}
		String substr = *this;
		substr.refStart = startIndex;
		substr.refCount = count;
		return substr;
	}

	bool String::StartsWith(const String& pattern) const {
		if (GetCount() < pattern.GetCount()) {
			return false;
		}
		return (Substring(0, pattern.GetCount()) == pattern);
	}
	bool String::EndsWith(const String& pattern) const {
		if (GetCount() < pattern.GetCount()) {
			return false;
		}
		return (Substring(GetCount() - pattern.GetCount(), pattern.GetCount()) == pattern);
	}

	bool String::IsEqual(const String& obj) const {
		if (GetCount() != obj.GetCount()) {
			return false;
		}

		// TODO: Quick hash

		const char* ptrA = GetStartPtr();
		const char* ptrB = obj.GetStartPtr();
		for (int i = 0; i < GetCount(); i += 1, ptrA += 1, ptrB += 1) {
			if (*ptrA != *ptrB) {
				return false;
			}
		}

		return true;
	}

	bool String::operator==(const String& obj) const {
		return IsEqual(obj);
	}
	bool String::operator!=(const String& obj) const {
		return !IsEqual(obj);
	}

	String String::ToString() const {
		return *this;
	}

	String::String(const char* string, int32 count) {
		PrepareData(string, count);
	}

	int32 String::GetStartIndex() const {
		return refStart;
	}
	const char* String::GetStartPtr() const {
		return data->data + refStart;
	}

	void String::PrepareData(const char* string,int32 count) {
		// Use public empty string.
		if (count <= 0) {
			data = StringData::empty;
			return;
		}
		data = std::make_shared<StringData>(string,count);
		refStart = 0;
		refCount = count;
	}

	String operator+(const String& left,const String& right) {
		return String::Format("{0}{1}", left, right);
	}

	StringSearcherSunday String::searcher{};
}