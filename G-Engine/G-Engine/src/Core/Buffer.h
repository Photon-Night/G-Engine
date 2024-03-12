#pragma once

#include "Base.h"

namespace GEngine
{
	struct Buffer
	{
		byte* data;
		uint32_t m_Size;

		Buffer()
			:data(nullptr), m_Size(0)
		{

		}

		Buffer(byte* data, uint32_t size)
			:data(data), m_Size(size)
		{

		}

		static Buffer Copy(void* data, uint32_t size)
		{
			Buffer buffer;
			buffer.Allocate(size);
			memcpy(buffer.data, data, size);
			return buffer;
		}

		void Allocate(uint32_t size)
		{
			delete[] data;
			data = nullptr;

			if(size == 0) return;

			data = new byte[size];
			this->m_Size = size;
		}

		void MemSetZero()
		{
			if(data) memset(data, 0, m_Size);
		}

		void Write(void* data, uint32_t size, uint32_t offset = 0)
		{
			GE_CORE_ASSERT(offset + size <= this->size, "Buffer overflow");
			memcpy(this->data + offset, data, size);
		}

		operator bool() const
		{
			return data;
		}

		byte& operator[](int index)
		{
			return data[index];
		}

		byte operator[](int index) const
		{
			return data[index];
		}

		template<typename T>
		T* As()
		{
			return (T*)data;
		}

		inline uint32_t GetSize() const { return m_Size; }
	};
	
}