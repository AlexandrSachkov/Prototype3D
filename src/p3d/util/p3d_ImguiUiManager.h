#pragma once

#include "internal/p3d_CircularBuffer.h"
#include "internal/p3d_CharBuffer.h"

#include <imgui.h>
#include <functional>

namespace p3d
{
	namespace util
	{
		class ImguiUiManager
		{
		public:
			ImguiUiManager();
			~ImguiUiManager();

			bool initialize(
				std::function<void(unsigned int level, std::string data)> consoleOutCbk,
				std::function<bool(unsigned int& level, std::string& data)> consoleReadCbk,
				std::function<void(std::string script)> runScriptCbk,
				unsigned int logBufferSize
				);
			ImDrawData* getDrawData();
			ImGuiIO& getIO();

			void update(double deltaT, int screenWidth, int screenHeight);
			

		private:
			void drawUI();
			void applyColorScheme(bool root);
			void pushStyleColor(ImGuiCol, ImVec4);
			void restoreColorScheme();
			void showCommandPrompt();

			bool _showConsole;
			unsigned int _styleCount;

			std::function<void(unsigned int level, std::string data)> _consoleOutCbk;
			std::function<bool(unsigned int& level, std::string& data)> _consoleReadCbk;
			std::function<void(std::string script)> _runScriptCbk;

			CircularBuffer<std::pair<unsigned int, std::string>> _consoleLogs;

			ImVec4 _windowBgColor;
			ImVec4 _textColor;
			ImVec4 _borderColor;
			ImVec4 _itemColor;
			ImVec4 _itemHoverColor;
			ImVec4 _itemActiveColor;
			ImVec4 _selectionColor;

			CharBuffer _commandBuffer;
		};
	}
}

