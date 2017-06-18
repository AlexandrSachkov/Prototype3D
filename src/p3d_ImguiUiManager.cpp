#include "p3d_ImguiUiManager.h"

#include <assert.h>
#include <string>
#include <iostream>
#include <algorithm>

namespace p3d
{
	namespace util
	{
		ImguiUiManager::ImguiUiManager() :
			_showConsole(true),
			_styleCount(0)
		{}
		ImguiUiManager::~ImguiUiManager(){}

		bool ImguiUiManager::initialize(
			std::function<void(unsigned int level, std::string data)> consoleOutCbk,
			std::function<bool(unsigned int& level, std::string& data)> consoleReadCbk,
			std::function<void(std::string script)> runScriptCbk,
			unsigned int logBufferSize
			)
		{
			_consoleOutCbk = consoleOutCbk;
			_consoleReadCbk = consoleReadCbk;
			_runScriptCbk = runScriptCbk;

			_consoleLogs = CircularBuffer<std::pair<unsigned int, std::string>>(logBufferSize);

			_windowBgColor = ImVec4(0.0f, 0.0f, 0.0f, 0.392f);
			_textColor = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
			_borderColor = ImVec4(0.0f, 1.0f, 1.0f, 0.129f);
			_itemColor = ImVec4(0.0f, 0.435f, 0.796f, 0.392f);
			_itemHoverColor = ImVec4(0.0f, 0.675f, 0.820f, 0.263f);
			_itemActiveColor = ImVec4(0.0f, 0.384f, 0.620f, 0.690f);
			_selectionColor = ImVec4(0.290f, 0.290f, 0.290f, 0.690f);

			_commandBuffer = CharBuffer(1024);

			return true;
		}
		
		ImDrawData* ImguiUiManager::getDrawData()
		{
			return ImGui::GetDrawData();
		}

		ImGuiIO& ImguiUiManager::getIO()
		{
			return ImGui::GetIO();
		}	

		void ImguiUiManager::update(double deltaT, int screenWidth, int screenHeight)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize = ImVec2((float)screenWidth, (float)screenHeight);
			io.DeltaTime = (float)(deltaT * 0.000000001);

			//Need crossplatform implementation
			// Hide OS mouse cursor if ImGui is drawing it
			//SetCursor(io.MouseDrawCursor ? NULL : LoadCursor(NULL, IDC_ARROW));

			ImGui::NewFrame();
			drawUI();
			ImGui::Render();
		}

		void ImguiUiManager::drawUI()
		{
			showCommandPrompt();
		}

		void ImguiUiManager::applyColorScheme(bool root)
		{
			if(root)
				pushStyleColor(ImGuiCol_WindowBg, ImColor(0.0f, 0.0f, 0.0f, 0.0f));
			else
				pushStyleColor(ImGuiCol_WindowBg, _windowBgColor);

			pushStyleColor(ImGuiCol_MenuBarBg, _windowBgColor);
			pushStyleColor(ImGuiCol_Text, _textColor);
			pushStyleColor(ImGuiCol_Border, _borderColor);
			pushStyleColor(ImGuiCol_Button, _itemColor);
			pushStyleColor(ImGuiCol_ButtonHovered, _itemHoverColor);
			pushStyleColor(ImGuiCol_ButtonActive, _itemActiveColor);
			pushStyleColor(ImGuiCol_Header, _itemColor);
			pushStyleColor(ImGuiCol_HeaderHovered, _itemHoverColor);
			pushStyleColor(ImGuiCol_HeaderActive, _itemActiveColor);
			pushStyleColor(ImGuiCol_FrameBg, _itemColor);
			pushStyleColor(ImGuiCol_FrameBgHovered, _itemHoverColor);
			pushStyleColor(ImGuiCol_FrameBgActive, _itemActiveColor);
			pushStyleColor(ImGuiCol_TitleBg, _itemColor);
			pushStyleColor(ImGuiCol_TitleBgCollapsed, _itemColor);
			pushStyleColor(ImGuiCol_TitleBgActive, _itemActiveColor);
			pushStyleColor(ImGuiCol_CloseButton, _itemColor);
			pushStyleColor(ImGuiCol_CloseButtonHovered, _itemHoverColor);
			pushStyleColor(ImGuiCol_CloseButtonActive, _itemActiveColor);
			pushStyleColor(ImGuiCol_TextSelectedBg, _selectionColor);
		}
		
		void ImguiUiManager::pushStyleColor(ImGuiCol id, ImVec4 color)
		{
			ImGui::PushStyleColor(id, color);
			_styleCount++;
		}

		void ImguiUiManager::restoreColorScheme()
		{
			for (unsigned int i = 0; i < _styleCount; i++)
			{
				ImGui::PopStyleColor();
			}
			_styleCount = 0;
		}

		void ImguiUiManager::showCommandPrompt()
		{
			unsigned int level;
			std::string consoleEntry;
			while (_consoleReadCbk(level, consoleEntry))
			{
				_consoleLogs.push({ level, consoleEntry });
			}

			ImGuiWindowFlags windowFlags = 0;
			//windowFlags |= ImGuiWindowFlags_NoSavedSettings;
			windowFlags |= ImGuiWindowFlags_ShowBorders;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 2.0f);
			applyColorScheme(false);
			ImGui::Begin("Command Prompt", &_showConsole, windowFlags);

			ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
			ImGuiListClipper clipper((int)_consoleLogs.size());
			while (clipper.Step())
			{
				for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i)
				{
					auto entry = _consoleLogs[i];
					switch (entry.first)
					{
					case 0:
					case 1:
						ImGui::PushStyleColor(ImGuiCol_Text, _textColor);
						break;
					case 2:
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, _textColor.w));
						break;
					default:
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, _textColor.w));
						break;
					}
					ImGui::TextUnformatted(entry.second.c_str());
					ImGui::PopStyleColor();
				}
			}

			ImGui::EndChild();

			ImGui::PushItemWidth(ImGui::GetWindowWidth() - 80);
			ImGui::InputText("", &_commandBuffer[0], _commandBuffer.getSize(), 0);
			ImGui::SameLine();
			if (ImGui::Button("Run", ImVec2(50, 0))) {
				_runScriptCbk(_commandBuffer.getText());
			}
			ImGui::End();
			restoreColorScheme();
			ImGui::PopStyleVar();
		}
	}
}