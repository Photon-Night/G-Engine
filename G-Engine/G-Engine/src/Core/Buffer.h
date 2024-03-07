#pragma once

#include "Base.h"

namespace GEngine
{
	struct Buffer
	{
		byte* m_Data;
		uint32_t m_Size;

		Buffer()
			:m_Data(nullptr), m_Size(0)
		{

		}

		Buffer(byte* data, uint32_t size)
			:m_Data(data), m_Size(size)
		{

		}

		static Buffer Copy(void* data, uint32_t size)
		{
			Buffer buffer;
			buffer.Allocate(size);
			memcpy(buffer.m_Data, data, size);
			return buffer;
		}

		void Allocate(uint32_t size)
		{
			delete[] m_Data;
			m_Data = nullptr;

			if(size == 0) return;

			m_Data = new byte[size];
			this->m_Size = size;
		}

		void MemSetZero()
		{
			if(m_Data) memset(m_Data, 0, m_Size);
		}

		void Write(void* data, uint32_t size, uint32_t offset = 0)
		{
			GE_CORE_ASSERT(offset + size <= this->size, "Buffer overflow");
			memcpy(m_Data + offset, data, size);
		}

		operator bool() const
		{
			return m_Data;
		}

		byte& operator[](int index)
		{
			return m_Data[index];
		}

		byte operator[](int index) const
		{
			return m_Data[index];
		}

		template<typename T>
		T* As()
		{
			return (T*)m_Data;
		}

		inline uint32_t GetSize() const { return m_Size; }
	};
	
}