#pragma once

#include <vector>

namespace p3d
{
	namespace util
	{
		template<typename T>
		class CircularBuffer
		{
		public:
			CircularBuffer() : CircularBuffer(1) {}
			CircularBuffer(size_t size) :
				_buffer(size),
				_index(0),
				_start(0),
				_numElements(0)
			{}

			~CircularBuffer() {}

			CircularBuffer(const CircularBuffer& other)
			{
				_index = other._index;
				_start = other._start;
				_numElements = other._numElements;
				_buffer = other._buffer;
			}

			CircularBuffer& operator=(const CircularBuffer& other)
			{
				_index = other._index;
				_start = other._start;
				_numElements = other._numElements;
				_buffer = other._buffer;
				return *this;
			}

			void push(T val)
			{
				if (_index == _start && _numElements > 0)
					_start = (_start + 1) % _buffer.size();

				_buffer[_index] = val;
				_index = (_index + 1) % _buffer.size();

				if (_numElements < _buffer.size())
					++_numElements;
			}

			size_t size()
			{
				return _numElements;
			}

			size_t capacity()
			{
				return _buffer.size();
			}

			T& operator [](unsigned int i) {
				return _buffer[(_start + i) % _buffer.size()];
			}
		private:
			unsigned int _index;
			unsigned int _start;
			size_t _numElements;
			std::vector<T> _buffer;
		};
	}
}