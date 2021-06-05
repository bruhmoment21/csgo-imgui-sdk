#include "pch.hpp"
#include "menu.hpp"

#include "utilities.hpp"
#include "config.hpp"
#include "hooks.hpp"
#include "sdk.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_internal.h"

namespace menu
{
	bool initialize() noexcept
	{
		auto& io{ImGui::GetIO()};
		io.IniFilename = nullptr;
		io.LogFilename = nullptr;
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

		ImGui::StyleColorsDark();

		auto& style{ImGui::GetStyle()};
		style.ScrollbarSize = 13.0f;
		style.WindowTitleAlign = ImVec2{0.5f, 0.5f};
		style.WindowRounding = 6.f;

		return true;
	}

	void render() noexcept
	{
		auto& style = ImGui::GetStyle();
		const auto& io = ImGui::GetIO();

		static float alpha = 0.f;

		const auto& draw_list = ImGui::GetBackgroundDrawList();
		draw_list->AddRectFilled(ImVec2{0.f, 0.f}, io.DisplaySize, ImColor{0.f, 0.f, 0.f, alpha});

		if (!is_open)
		{
			alpha = std::fmax(alpha - io.DeltaTime * 3.0f, 0.f);
			style.Alpha = std::fmax(style.Alpha - io.DeltaTime * 6.0f, 0.f);
			return;
		}

		alpha = std::fmin(alpha + io.DeltaTime * 3.0f, 0.5f);
		style.Alpha = std::fmin(style.Alpha + io.DeltaTime * 6.0f, 1.f);

		static std::uint32_t selected_tab = 1;
		ImGui::SetNextWindowSize(ImVec2{512.f, 815.f}, ImGuiCond_Once);
		ImGui::Begin("jet csgo sdk", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

#ifdef _DEBUG
		ImGui::ShowDemoWindow();
#endif

		ImGui::BeginChild("Tabs Panel", ImVec2{150, 0}, true);
		static constexpr const char* tabs[] {
			"Visuals", 
			"Miscellaneous", 
			"Configs"
#ifdef _DEBUG
			, "Debug Tab"
#endif
		};
		for (std::size_t i = 0; i < IM_ARRAYSIZE(tabs); ++i)
		{
			if (ImGui::Selectable(tabs[i], selected_tab == i))
			{
				selected_tab = i;
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginGroup();
		switch (selected_tab)
		{
			case 1:
				ImGui::Text("Miscellaneous settings");
				ImGui::Separator();

				ImGui::Checkbox("Auto accept", &config::auto_accept);
				ImGui::Checkbox("Block server advertisements", &config::block_advertisements);
				ImGui::Checkbox("Bypass invite cooldown", &config::bypass_invite_cooldown);
				ImGui::Checkbox("Fake prime", &config::fake_prime);

				break;
			case 2:
				{			
					static int config_index = -1;
					static char buffer[10];

					ImGui::Text("Configs");
					ImGui::Separator();

					ImGui::SetNextItemWidth(338.f);
					ImGui::InputTextWithHint("##cfg_name", "enter config name", buffer, 10);

					static ImGuiTextFilter configs_filter;
					configs_filter.Draw("##cfg_filter", 338.f);

					ImGui::Text("Available configs");
					ImGui::Separator();

					ImGui::SetNextItemWidth(338.f);
					if (ImGui::BeginListBox("##config_listbox"))
					{
						const auto& configs_vector = config::configs;
						for (std::size_t i = 0u; i < configs_vector.size(); ++i)
						{
							const auto& config_name = configs_vector[i].data();
							if (configs_filter.PassFilter(config_name))
							{
								if (ImGui::Selectable(config_name, i == config_index))
								{
									config_index = i;
								}
							}
						}

						ImGui::EndListBox();
					}

					if (ImGui::Button("Create Config", ImVec2{338.f, 19.f}))
					{
						config::create_file(buffer);
					}

					if (config_index != -1 && config_index < static_cast<int>(config::configs.size()))
					{
						if (ImGui::Button("Save", ImVec2{165.f, 19.f}))
						{
							config::save(config_index);
						}

						ImGui::SameLine();

						if (ImGui::Button("Load", ImVec2{165.f, 19.f}))
						{
							config::load(config_index);
							if (sdk::engine->is_connected() && local_player::entity->is_alive())
							{
								sdk::console->get_convar("cl_fullupdate")->change_callback();
							}
						}

						if (ImGui::Button("Delete Config", ImVec2{338.f, 19.f}))
						{
							config::delete_file(config_index);
						}
					}

					if (ImGui::Button("Update Configs", ImVec2{338.f, 19.f}))
					{
						config::update_configs();
					}

					if (ImGui::Button("Unload", ImVec2{338.f, 19.f}))
					{
						if (auto* const unload_thread = CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(hooks::release), nullptr, 0, nullptr))
						{
							CloseHandle(unload_thread);
						}
					}

					if (ImGui::Button("Unlock All Achievements", ImVec2{338.f, 19.f}))
					{
						const auto& user_stats = sdk::engine->get_steam_context()->steam_user_stats;
						user_stats->request_current_stats();
						for (std::uint32_t i = 0u; i < user_stats->achievements_count(); ++i)
						{
							user_stats->set_achievement(user_stats->get_achievement_name(i));
						}
						user_stats->store_stats();
					}

					if (ImGui::Button("Lock All Achievements", ImVec2{338.f, 19.f}))
					{
						const auto& user_stats = sdk::engine->get_steam_context()->steam_user_stats;
						user_stats->request_current_stats();
						for (std::uint32_t i = 0u; i < user_stats->achievements_count(); ++i)
						{
							user_stats->clear_achievement(user_stats->get_achievement_name(i));
						}
						user_stats->store_stats();
					}

					break;
				}
#ifdef _DEBUG
			case IM_ARRAYSIZE(tabs) - 1:
				ImGui::Text("This tab is for debugging.");
				ImGui::Separator();

				ImGui::Text("Current heap allocations: %u", hooks::alloc_count - hooks::freed_alloc_count);
				ImGui::Separator();
				ImGui::Text("Total heap allocations: %u", hooks::alloc_count);
				ImGui::Text("Freed heap allocations: %u", hooks::freed_alloc_count);
				ImGui::Separator();
				ImGui::Text("ImGui Version: %s", IMGUI_VERSION);

				break;
#endif
		}
		ImGui::EndGroup();

		ImGui::End();
	}
}