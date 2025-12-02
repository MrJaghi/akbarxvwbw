#pragma once
namespace unknowncheats {
namespace offsets {
	namespace c_base_entity {
		inline DWORD health = 0x34C;
		inline DWORD team_id = 0x3EB;
		inline DWORD flags = 0x3F8;
		inline DWORD vec_origin = 0x15B0;
		inline DWORD game_scene_node = 0x330;
		inline DWORD model_state = 0x170;
		inline DWORD pawn = 0x62C;
		inline DWORD player_pawn = 0x6B4;
		inline DWORD sanitized_player_name = 0x778;
		inline DWORD flash_duration = 0x140C;
		inline DWORD flash_bang_time = 0x13F8;
		inline DWORD flash_overlay_alpha = 0x1400;
		inline DWORD shots_fired = 0x23FC;
		inline DWORD aim_punch_cache = 0x15A8;
		inline DWORD is_scoped = 0x23E8;
		inline DWORD entity = 0x10;
		inline DWORD designer_name = 0x20;
		inline DWORD id_ent_index = 0x1458;
		inline DWORD attribute_manager = 0x1148;
		inline DWORD item = 0x50;
		inline DWORD item_definition_index = 0x1BA;
		inline DWORD clipping_weapon = 0x1620;
		inline DWORD glow = 0xC00;
		inline DWORD glow_type = 0x30;
		inline DWORD glow_color_override = 0x40;
		inline DWORD glowing = 0x51;
		inline DWORD vec_velocity = 0x3D8;
        
        // THIS LINE IS ADDED FOR COMPATIBILITY WITH OLDER CODE
        inline DWORD bone_array = 0x1E0; 
	}
	// Aliases for compatibility
	namespace c_base_player_controler {
		inline DWORD pawn_alive = 0x818;
		inline DWORD ping = 0x73C;
		inline DWORD has_defuser = 0x1F88;
		inline DWORD has_hemlet = 0x1F94;
		inline DWORD player_name = 0x778;
		inline DWORD player_pawn = 0x7FC;
		inline DWORD money_services = 0x700;
		inline DWORD tracking_services = 0x710;
		inline DWORD damage_services = 0x718;
	}
	namespace pawn {
		inline DWORD camera_services = 0x10E0;
		inline DWORD processing_value = 0x1F4;
		inline DWORD health = 0x34C;
		inline DWORD vec_old_origin = 0x15B0;
		inline DWORD spotted = 0x1644;
		inline DWORD viewangle = 0x1518;
		inline DWORD camera_pos = 0x1294;
		inline DWORD ping_services = 0x1268;
		inline DWORD max_alpha = 0x146C;
		inline DWORD flash_Duration = 0x1470;
		inline DWORD flashbang_time = 0x145C;
		inline DWORD emit_sound_time = 0x1404;
		inline DWORD dormant = 0xE7;
		inline DWORD ent_index = 0x1544;
	}
	namespace controller {
		inline DWORD money_services = 0x700;
		inline DWORD tracking_services = 0x710;
		inline DWORD damage_services = 0x718;
	}
	namespace damage_services {
		inline DWORD i_dmg = 0x58;
	}
	namespace tracking_services {
		inline DWORD total_damage = 0x110;
	}
	namespace money_services {
		inline DWORD i_account = 0x40;
	}
	namespace item_services {
		inline DWORD item_services_pawn = 0x10B0;
		inline DWORD has_defuser = 0x40;
		inline DWORD has_heavy_armor = 0x42;
	}
	namespace ping_services {
		inline DWORD player_ping = 0x40;
	}
	namespace match_making {
		inline DWORD m_i_competitive_wins = 0x774;
	}
	namespace c_game_scene_mode {
		inline DWORD vec_origin = 0x80;
	}
namespace c_base_player_pawn {
inline DWORD vec_velocity = 0x3D8;
inline DWORD player_state = 0x138C;
inline DWORD killed_by_hs = 0x1668;
inline DWORD spotted = 0x1644;
inline DWORD bone_array = 0x1E0;
inline DWORD viewangle = 0x1518;
}
namespace c_post_processing {
inline DWORD exposure_control = 0xD05;
inline DWORD min_exposure = 0xCEC;
inline DWORD max_exposure = 0xCF0;
}
namespace c_item_services {
inline DWORD item_services_pawn = 0x10B0;
inline DWORD has_defuser = 0x40;
inline DWORD has_helmet = 0x41;
inline DWORD has_heavy_armor = 0x42;
}
namespace c_ping_services {
inline DWORD player_ping = 0x40;
}
namespace c_base_weapon {
inline DWORD clipping_weapon = 0x12B0;
inline DWORD wpn_data_ptr = 0x360;
inline DWORD weapon_services = 0x10A8;
inline DWORD active_weapon = 0x60;
inline DWORD clip_1 = 0x1570;
inline DWORD cycle_time = 0xC34;
inline DWORD max_clip = 0x1FC;
inline DWORD penetration = 0xD3C;
inline DWORD weapon_type = 0x240;
inline DWORD zs_name = 0xC18;
inline DWORD inaccuracy = 0xC7C;
}
namespace c_base_smoke {
inline DWORD effect_begin = 0x1108;
inline DWORD effect_spawn = 0x1149;
inline DWORD smoke_effect = 0x110C;
inline DWORD smoke_color = 0x1114;
}
namespace c_controller {
inline DWORD money_services = 0x700;
inline DWORD tracking_services = 0x710;
inline DWORD damage_services = 0x718;
}
namespace c_money_services {
inline DWORD i_account = 0x40;
}
namespace c_damage_services {
inline DWORD i_dmg = 0x58;
}
namespace c_match_making {
inline DWORD i_competitive_ranking = 0x770;
inline DWORD i_competitive_wins = 0x774;
inline DWORD i_competitive_rank_type = 0x778;
inline DWORD i_competitive_ranking_predicted_win = 0x77C;
inline DWORD i_competitive_ranking_predicted_loss = 0x780;
inline DWORD i_competitive_ranking_predicted_tie = 0x784;
}
namespace c_tracking_services {
inline DWORD total_damage = 0x110;
}
namespace c_modulation {
inline DWORD sky3d = 0x1168;
}
namespace c_glow {
inline DWORD glow_property = 0xB98;
inline DWORD glow_color = 0x8;
inline DWORD glow_type = 0x30;
inline DWORD glow_color_override = 0x40;
inline DWORD glowing = 0x51;
}
namespace c_game_scene_node {
inline DWORD vec_origin = 0x80;
}
}
namespace dragged_offsets {
inline DWORD entity_list = 0x1D11D78;  // Updated from output: dwEntityList
inline DWORD matrix = 0x1E30450;  // Updated from output: dwViewMatrix
inline DWORD view_angle = 0x1E3A880;  // Updated from output: dwViewAngles
inline DWORD local_player_controller = 0x1E1BC58;  // Updated from output: dwLocalPlayerController
inline DWORD local_player_pawn = 0x1BECF38;  // Updated from output: dwLocalPlayerPawn
inline DWORD force_jump = 0x1E3A1D0;  // Updated from output: dwCSGOInput (used for force jump)
inline DWORD global_vars = 0x1BE21C0;  // Updated from output: dwGlobalVars
inline DWORD planted_c4 = 0x1E34C68;  // Updated from output: dwPlantedC4
inline DWORD inventory_services = 0x10B0;
}
namespace signatures {
const std::string inventory_services = "E8 ?? ?? ?? ?? 8B 45 D0 48 8B 55 D8";
const std::string global_vars = "48 89 0D ?? ?? ?? ?? 48 89 41";
const std::string entity_list = "48 8B 0D ?? ?? ?? ?? 48 89 7C 24 ?? 8B FA C1 EB";
const std::string local_player_controller = "48 8B 05 ?? ?? ?? ?? 48 85 C0 74 ?? 8B 88";
const std::string view_angles = "48 8B 0D ?? ?? ?? ?? E9 ?? ?? ?? ?? CC CC CC CC 40 55";
const std::string view_matrix = "48 8D 05 ?? ?? ?? ?? 48 8B D3 4C 8D 05";
const std::string local_player_pawn = "48 8D 05 ?? ?? ?? ?? C3 CC CC CC CC CC CC CC CC 48 83 EC ?? 8B 0D";
const std::string force_jump = "48 8B 05 ?? ?? ?? ?? 48 8D 1D ?? ?? ?? ?? 48 89 45";
const std::string planted_c4 = "48 8B 15 ?? ?? ?? ?? FF C0 48 8D 4C 24 40";
const std::string util_say_text_filter_windows = "\x48\x89\x5C\x24\x2A\x48\x89\x74\x24\x2A\x55\x57\x41\x56\x48\x8D\x6C\x24\x2A\x48\x81\xEC\x2A\x2A\x2A\x2A\x41\x0F\xB6\xF8";
const std::string util_say_text_filter_linux = "\x55\x48\x8D\x05\x2A\x2A\x2A\x2A\x48\x89\xE5\x41\x57\x41\x56\x4C\x8D\x75\x2A\x41\x55\x41\x89\xCD";
const std::string util_say_text2_filter_windows = "\x48\x89\x5C\x24\x2A\x48\x89\x74\x24\x2A\x55\x57\x41\x56\x48\x8D\x6C\x24\x2A\x48\x81\xEC\x2A\x2A\x2A\x2A\x41\x0F\xB6\xF8";
const std::string util_say_text2_filter_linux = "\x55\x48\x8D\x05\x2A\x2A\x2A\x2A\x48\x89\xE5\x41\x57\x41\x89\xD7\x31\xD2";
const std::string is_hearing_client_windows = "\x40\x53\x48\x83\xEC\x2A\x48\x8B\xD9\x3B\x51";
const std::string is_hearing_client_linux = "\x55\x48\x89\xE5\x41\x56\x41\x55\x41\x54\x53\x48\x89\xFB\x39\x77";
const std::string trigger_push_touch_windows = "\x40\x55\x53\x57\x48\x8D\x6C\x24\x2A\x48\x81\xEC\x2A\x2A\x2A\x2A\x48\x8B\x02\x48\x8B\xF9";
const std::string trigger_push_touch_linux = "\x55\x48\x89\xE5\x41\x57\x41\x56\x41\x55\x49\x89\xF5\x41\x54\x53\x48\x89\xFB\x48\x83\xEC\x2A\xE8\x2A\x2A\x2A\x2A\x84\xC0";
const std::string set_ground_entity_windows = "\x48\x89\x5C\x24\x2A\x48\x89\x6C\x24\x2A\x56\x57\x41\x54\x48\x83\xEC\x2A\x44\x8B\x89";
const std::string set_ground_entity_linux = "\x55\x48\x8D\x87\x2A\x2A\x2A\x2A\x48\x89\xE5\x41\x57\x41\x56\x49\x89\xD6";
const std::string server_movement_unlock_windows = "\x0F\x86\xB0\x2A\x2A\x2A\xF3\x0F\x58\xD3";
const std::string server_movement_unlock_linux = "\x0F\x87\x2A\x2A\x2A\x2A\xF3\x0F\x10\x35\x2A\x2A\x2A\x2A\xF3\x0F\x11\xB5\x2A\x2A\x2A\x2A\x4C\x89\xEE";
const std::string ccs_player_controller_switch_team_windows = "\x40\x53\x57\x48\x81\xEC\x2A\x2A\x2A\x2A\x48\x8B\xD9\x8B\xFA";
const std::string ccs_player_controller_switch_team_linux = "\x55\x48\x89\xE5\x41\x54\x49\x89\xFC\x89\xF7";
const std::string check_jump_button_water_windows = "\xC8\x42\xEB\x2A\x4C\x39\x67\x30";
const std::string check_jump_button_water_linux = "\xC8\x42\x41\xC7\x85\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x41\xC7\x85\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x2A\xE9";
const std::string util_remove_windows = "\x48\x85\xC9\x74\x2A\x48\x8B\xD1\x48\x8B\x0D\x2A\x2A\x2A\x2A";
const std::string util_remove_linux = "\x48\x89\xFE\x48\x85\xFF\x74\x2A\x48\x8D\x05\x2A\x2A\x2A\x2A\x48";
const std::string c_entity_system_add_entity_io_event_windows = "\x48\x89\x5C\x24\x18\x4C\x89\x4C\x24\x20\x48\x89\x4C\x24\x08\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x20";
const std::string c_entity_system_add_entity_io_event_linux = "\x55\x48\x89\xE5\x41\x55\x49\x89\xCD\x41\x54\x49\x89\xFC";
const std::string c_entity_instance_accept_input_windows = "\x48\x89\x5C\x24\x2A\x48\x89\x74\x24\x2A\x57\x48\x83\xEC\x2A\x49\x8B\xF0\x48\x8B\xD9\x48\x8B\x0D";
const std::string c_entity_instance_accept_input_linux = "\x55\x48\x89\xF0\x48\x89\xE5\x41\x57\x49\x89\xFF\x41\x56\x48\x8D\x7D";
const std::string c_entity_identity_accept_input_windows = "\x48\x89\x5C\x24\x2A\x48\x89\x54\x24\x2A\x48\x89\x4C\x24\x2A\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x2A\x2A\x2A\x2A\x48\x81\xEC\x2A\x2A\x2A\x2A\x48\x8B\x02";
const std::string c_entity_identity_accept_input_linux = "\x55\x49\x89\xD3\x48\x89\xE5\x41\x57\x45\x89\xCF\x41\x56";
const std::string c_entity_io_output_fire_output_internal_windows = "\x4C\x89\x4C\x24\x2A\x48\x89\x4C\x24\x2A\x53\x56";
const std::string c_entity_io_output_fire_output_internal_linux = "\x55\x48\x89\xE5\x41\x57\x49\x89\xFF\x41\x56\x41\x55\x41\x54\x49\x89\xD4\x53\x48\x89\xF3";
const std::string c_game_entity_system_find_entity_by_class_name_windows = "\x48\x83\xEC\x68\x45\x33\xC9";
const std::string c_game_entity_system_find_entity_by_class_name_linux = "\x55\x45\x31\xC0\x31\xC9\x48\x89\xE5\x53\x48\x8D\x5D\x2A\x48\x83\xEC\x2A\x48\x89\xDF";
const std::string c_game_entity_system_find_entity_by_name_windows = "\x48\x81\xEC\x88\x2A\x2A\x2A\x4D\x85\xC0";
const std::string c_game_entity_system_find_entity_by_name_linux = "\x55\x48\x89\xE5\x41\x54\x53\x48\x83\xEC\x2A\x48\x85\xD2";
const std::string c_base_entity_take_damage_old_windows = "\x40\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x2A\x2A\x2A\x2A\x48\x8D\x6C\x24\x2A\x48\x89\x9D\x2A\x2A\x2A\x2A\x45\x33\xED";
const std::string c_base_entity_take_damage_old_linux = "\x55\x48\x89\xE5\x41\x57\x41\x56\x49\x89\xF6\x41\x55\x41\x54\x49\x89\xFC\x53\x48\x89\xD3\x48\x83\xEC\x2A\x48\x85\xD2";
const std::string c_base_entity_set_gravity_scale_windows = "\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x2A\xF3\x0F\x10\x81\x04\x04\x00\x00";
const std::string c_base_entity_set_gravity_scale_linux = "\x55\x48\x89\xE5\x41\x57\x41\x56\x41\x55\x41\x54\x53\x48\x89\xFB\x48\x81\xEC\x2A\x2A\x2A\x2A\x0F\x2E\x87\x2A\x2A\x2A\x2A\x7A\x2A\x75\x2A\x48\x81\xC4\x2A\x2A\x2A\x2A\x5B\x41\x5C\x41\x5D\x41\x5E\x41\x5F\x5D\xC3\x0F\x1F\x40\x2A\x31\xC9\x31\xF6\x31\xFF\xF3\x0F\x11\x85\x2A\x2A\x2A\x2A\x66\x89\x8D\x2A\x2A\x2A\x2A\xBA\x2A\x2A\x2A\x2A\xB9\x2A\x2A\x2A\x2A\x66\x0F\xEF\xC9\x48\xC7\x85\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x48\xC7\x85\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x48\xC7\x85\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x48\xC7\x85\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x0F\x29\x8D\x2A\x2A\x2A\x2A\x48\xC7\x85\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x2A\xC7\x85\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x2A\xE8\x2A\x2A\x2A\x2A\xF3\x0F\x10\x85\x2A\x2A\x2A\x2A\x85\xC0\x41\x89\xC4\x0F\x8E\x2A\x2A\x2A\x2A\x48\x63\x8D\x2A\x2A\x2A\x2A\x48\x63\xD0\x31\xF6\xF3\x0F\x11\x85\x2A\x2A\x2A\x2A\x48\xC1\xE2\x2A\x48\x8B\xBD\x2A\x2A\x2A\x2A\x48\xC1\xE1\x2A\x81\xBD\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x40\x0F\x96\xC6\xE8\x2A\x2A\x2A\x2A\x8B\x95\x2A\x2A\x2A\x2A\xF3\x0F\x10\x85\x2A\x2A\x2A\x2A\x48\x89\x85\x2A\x2A\x2A\x2A\x81\xFA\x2A\x2A\x2A\x2A\x0F\x87";
const std::string i_game_system_init_all_systems_p_first_windows = "\x48\x8B\x1D\x2A\x2A\x2A\x2A\x48\x85\xDB\x0F\x84\x2A\x2A\x2A\x2A\xBD";
const std::string i_game_system_init_all_systems_p_first_linux = "\x4C\x8B\x35\x2A\x2A\x2A\x2A\x4D\x85\xF6\x75\x2A\xE9";
const std::string i_game_system_loop_post_init_all_systems_p_event_dispatcher_windows = "\x48\x39\x1D\x2A\x2A\x2A\x2A\x74\x2A\x39\x05";
const std::string i_game_system_loop_post_init_all_systems_p_event_dispatcher_linux = "\x4C\x8B\x25\x2A\x2A\x2A\x2A\x48\x8B\x05\x2A\x2A\x2A\x2A\x8B\x35";
const std::string i_game_system_loop_destroy_all_systems_s_game_systems_windows = "\x8B\x05\x2A\x2A\x2A\x2A\x83\xE8\x2A\x48\x63\xF8\x0F\x88";
const std::string i_game_system_loop_destroy_all_systems_s_game_systems_linux = "\x8B\x05\x2A\x2A\x2A\x2A\x89\xC2\x83\xEA\x2A\x0F\x88\x2A\x2A\x2A\x2A\x4C\x8D\x3D";
}
}