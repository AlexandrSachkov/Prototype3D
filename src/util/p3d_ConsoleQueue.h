#pragma once

#include <queue>
namespace p3d
{
	namespace util
	{
#if defined(DEBUG) || defined(_DEBUG)
#define CONSOLE_DEBUG(queue, data) queue.push(NLE::OUTPUT_TYPE::DEBUG, data);
#else
#define CONSOLE_DEBUG(queue, data)
#endif

		enum CONSOLE_OUT_TYPE
		{
			DEBUG		= 0,
			STANDARD	= 1,
			WARNING		= 2,
			ERR			= 3
		};

		class ConsoleQueue
		{
		public:
			ConsoleQueue();
			~ConsoleQueue();

			void push(CONSOLE_OUT_TYPE outType, const std::string& data);
			void push(CONSOLE_OUT_TYPE outType, const char* data);

			bool pop(std::pair<CONSOLE_OUT_TYPE, std::string>& data);
			void clear();
		private:
			std::queue <std::pair<CONSOLE_OUT_TYPE, std::string>> _data;
		};
	}

}

