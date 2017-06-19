#include "p3d_ConsoleQueue.h"

namespace p3d
{
	namespace util
	{
		ConsoleQueue::ConsoleQueue()
		{
		}

		ConsoleQueue::~ConsoleQueue()
		{
		}

		void ConsoleQueue::push(CONSOLE_OUT_TYPE outType, const std::string& data)
		{
			_data.push({ outType, data });
		}

		void ConsoleQueue::push(CONSOLE_OUT_TYPE outType, const char* data)
		{
			_data.push({ outType, data });
		}

		bool ConsoleQueue::pop(std::pair<CONSOLE_OUT_TYPE, std::string>& data)
		{
			if (_data.empty())
				return false;

			return _data.try_pop(data);
		}

		void ConsoleQueue::clear()
		{
			_data.clear();
		}
	}

}
