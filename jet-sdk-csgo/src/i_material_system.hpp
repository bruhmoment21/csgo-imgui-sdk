#pragma once

#include "signatures.hpp"
#include "utilities.hpp"

#include "c_materials.hpp"

constexpr const char* TEXTURE_GROUP_LIGHTMAP = "Lightmaps";
constexpr const char* TEXTURE_GROUP_WORLD = "World textures";
constexpr const char* TEXTURE_GROUP_MODEL = "Model textures";
constexpr const char* TEXTURE_GROUP_VGUI = "VGUI textures";
constexpr const char* TEXTURE_GROUP_PARTICLE = "Particle textures";
constexpr const char* TEXTURE_GROUP_DECAL = "Decal textures";
constexpr const char* TEXTURE_GROUP_SKYBOX = "SkyBox textures";
constexpr const char* TEXTURE_GROUP_CLIENT_EFFECTS = "ClientEffect textures";
constexpr const char* TEXTURE_GROUP_OTHER = "Other textures";
constexpr const char* TEXTURE_GROUP_PRECACHED = "Precached";
constexpr const char* TEXTURE_GROUP_CUBE_MAP = "CubeMap textures";
constexpr const char* TEXTURE_GROUP_RENDER_TARGET = "RenderTargets";
constexpr const char* TEXTURE_GROUP_UNACCOUNTED = "Unaccounted textures";
constexpr const char* TEXTURE_GROUP_STATIC_VERTEX_BUFFER = "Static Vertex";
constexpr const char* TEXTURE_GROUP_STATIC_INDEX_BUFFER = "Static Indices";
constexpr const char* TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP = "Displacement Verts";
constexpr const char* TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR = "Lighting Verts";
constexpr const char* TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD = "World Verts";
constexpr const char* TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS = "Model Verts";
constexpr const char* TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER = "Other Verts";
constexpr const char* TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER = "Dynamic Indices";
constexpr const char* TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER = "Dynamic Verts";
constexpr const char* TEXTURE_GROUP_DEPTH_BUFFER = "DepthBuffer";
constexpr const char* TEXTURE_GROUP_VIEW_MODEL = "ViewModel";
constexpr const char* TEXTURE_GROUP_PIXEL_SHADERS = "Pixel Shaders";
constexpr const char* TEXTURE_GROUP_VERTEX_SHADERS = "Vertex Shaders";
constexpr const char* TEXTURE_GROUP_RENDER_TARGET_SURFACE = "RenderTarget Surfaces";
constexpr const char* TEXTURE_GROUP_MORPH_TARGETS = "Morph Targets";

class c_key_values;

struct texture_t
{
	IDirect3DTexture9* texture()
	{
		return *reinterpret_cast<IDirect3DTexture9**>(reinterpret_cast<std::uintptr_t>(this) + 0xC);
	}
};

class i_texture
{
public:
	int get_actual_width()
	{
		return utilities::call_virtual<int, 3>(this);
	}

	int get_actual_height()
	{
		return utilities::call_virtual<int, 4>(this);
	}

	texture_t** texture_handle()
	{
		return *reinterpret_cast<texture_t***>(reinterpret_cast<std::uintptr_t>(this) + 0x50);
	}
};

class i_mat_render_context
{
public:
	int release()
	{
		return utilities::call_virtual<int, 1>(this);
	}

	void set_render_target(i_texture* texture)
	{
		return utilities::call_virtual<void, 6, i_texture*>(this, texture);
	}

	void push_render_target_and_viewport()
	{
		return utilities::call_virtual<void, 119>(this);
	}

	void pop_render_target_and_viewport()
	{
		return utilities::call_virtual<void, 120>(this);
	}
};

class i_material_system
{
public:
	std::byte pad[0x2C88]; // "80 B9 ? ? ? ? ? 74 0F" + 2
	bool game_started;

	image_format_t get_back_buffer_format()
	{
		return utilities::call_virtual<image_format_t, 36>(this);
	}

	i_material* create_material(const char* name, c_key_values* key_values)
	{
		return utilities::call_virtual<i_material*, 83, const char*, c_key_values*>(this, name, key_values);
	}

	i_material* find_material(const char* material_name, const char* group_name, bool complain = true, const char* complain_prefix = nullptr)
	{
		return utilities::call_virtual<i_material*, 84, const char*, const char*, bool, const char*>(this, material_name, group_name, complain, complain_prefix);
	}

	material_handle_t first_material()
	{
		return utilities::call_virtual<material_handle_t, 86>(this);
	}

	material_handle_t next_material(material_handle_t handle)
	{
		return utilities::call_virtual<material_handle_t, 87, material_handle_t>(this, handle);
	}

	material_handle_t invalid_material_handle()
	{
		return utilities::call_virtual<material_handle_t, 88>(this);
	}

	i_material* get_material(material_handle_t handle)
	{
		return utilities::call_virtual<i_material*, 89, material_handle_t>(this, handle);
	}

	int	get_materials_count()
	{
		return utilities::call_virtual<int, 90>(this);
	}

	void begin_render_target_allocation()
	{
		return utilities::call_virtual<void, 94>(this);
	}

	void end_render_target_allocation()
	{
		return utilities::call_virtual<void, 95>(this);
	}

	i_texture* create_named_render_target_texture_ex(const char* name, int w, int h, int size_mode, image_format_t format, int depth, int texture_flags, int render_target_flags)
	{
		return utilities::call_virtual<i_texture*, 99, const char*, int, int, int, image_format_t, int, int, int>(this, name, w, h, size_mode, format, depth, texture_flags, render_target_flags);
	}

	i_mat_render_context* get_render_context()
	{
		return utilities::call_virtual<i_mat_render_context*, 115>(this);
	}

	void force_begin_render_target_allocation()
	{
		const bool old_state = game_started;
		if (!old_state)
		{
			begin_render_target_allocation();
			return;
		}

		game_started = false;
		begin_render_target_allocation();
		game_started = old_state;
	}

	void force_end_render_target_allocation()
	{
		const bool old_state = game_started;
		if (!old_state)
		{
			end_render_target_allocation();
			return;
		}

		game_started = false;
		end_render_target_allocation();
		game_started = old_state;
	}

	i_texture* create_full_render_frame_target(const char* name)
	{
		return create_named_render_target_texture_ex(name, 1, 1, 4, get_back_buffer_format(), 0, 4 | 8, 1);
	}
};
