// Copyright 2019 Genten Studios
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

/***
 * @file ImGuiHelpers.hpp
 * @describe
 *   defines objects that implement alternative ImGui interfaces for easier
 *   UI implementation.
 */

#pragma once

#include <imgui.h>

#include <vector>
#include <string>
#include <sstream>


// NOTE: consider transfering to an ANSI.h for submodule context, this code
//   could possibly be used with an alternative GUI API as well and could
//   benefit from being standalone.
#define ANSI_RED IM_COL32(255,0,0,255)

namespace q2 {
	namespace ImGuiHelpers {
		class TerminalBase
		{
			private:
				const char* _name;
			protected:
				const char* outputName;
				bool renderFocus; // for when the focus method is invoked
				//std::string buffer(); // this is the input text buffer for the term
				ImVec2 defaultPadding = ImVec2(5.0f, 5.0f);

				// XXX: this is a manual hack to get around the fact we have no
				//   way to query the pre-existing window padding for responsive
				//   design. Perhaps ImGui expects us to design something from scratch?
				//   I say fuck that for now.
				ImVec2* currentPadding;

				ImGuiWindowFlags currentFlags = ImGuiWindowFlags(
					ImGuiWindowFlags_NoScrollbar |
					ImGuiWindowFlags_NoScrollWithMouse
				);

				virtual void drawInputField();
				virtual void drawOutputField();
				inline void begin(ImVec2* size, ImVec2* pos, ImVec2* padding, bool* p_open, ImGuiWindowFlags flags)
					{
						currentFlags = currentFlags | flags; // save current flags;
						currentPadding = ( padding != NULL ? padding : &defaultPadding );
						ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, *currentPadding);
						ImGui::Begin(_name, p_open, currentFlags);
						if (pos != NULL) ImGui::SetWindowPos(*((const ImVec2*) pos), ImGuiCond_Once);
						if (size != NULL) ImGui::SetWindowSize(*((const ImVec2*) size), ImGuiCond_Once);
					};
				inline void end()
					{ ImGui::End(); ImGui::PopStyleVar();};
			public:
				std::ostringstream output;

				// NOTE:
				//   ImGui limitation; The `name` must be used for internal refference ie.
				//   all widgets are searched via a public hash function. There no current
				//   way to use the ImGui internal IDs which would be slightly faster.
				inline TerminalBase(const char* name)
					{ _name = name; outputName = std::string(_name).append("_Output").c_str(); };
				inline TerminalBase(const char* name, std::string initial_contents)
					{ _name = name; outputName = std::string(_name).append("_Output").c_str(); };

				// NOTE: define overrides in base class for ease of implementation down the road.
				// TODO: vertical shrink with templates
				virtual void drawEx(ImVec2* size, ImVec2* pos, ImVec2* padding, bool* p_open, ImGuiWindowFlags flags);
				inline void draw(ImVec2 size, ImVec2 pos, ImVec2 padding, bool* p_open, ImGuiWindowFlags flags)
					{ drawEx(&size, &pos, &padding, p_open, flags); };
				inline void draw(ImVec2 size, ImVec2 pos)
					{ drawEx(&size, &pos, NULL, NULL, ImGuiWindowFlags_None); };
				inline void draw(ImVec2 size){ drawEx(&size, NULL, NULL, NULL, ImGuiWindowFlags_None); };
				inline void draw(ImVec2 size, bool* p_open){ drawEx(&size, NULL, NULL, p_open, ImGuiWindowFlags_None); };
				inline void draw(ImVec2 size, ImGuiWindowFlags flags){ drawEx(&size, NULL, NULL, NULL, flags); };
				inline void draw(){ drawEx(NULL, NULL, NULL, NULL, ImGuiWindowFlags_None); };

				void focus(){ renderFocus = true; };
				virtual void onCommand();
		};

		class BasicTerminal : public TerminalBase
		{
			protected:
				void drawInputField();
				void drawOutputField();
			public:
				using TerminalBase::TerminalBase;
				//BasicTerminal();
				void drawEx(ImVec2* size, ImVec2* pos, ImVec2* padding, bool* p_open, ImGuiWindowFlags extra_flags);
				void onCommand();
		};
	};
};
