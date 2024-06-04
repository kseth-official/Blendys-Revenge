#pragma once

#include <array>
#include <map>
#include <utility>
#include <iostream>
#include "common.hpp"
#include "components.hpp"
#include "tiny_ecs.hpp"

// Fonts
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>

#include "particle_system.hpp"


struct Character {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
	char character;
};

// System responsible for setting up OpenGL and for rendering all the
// visual entities in the game
class RenderSystem {
	/**
	 * The following arrays store the assets the game will use. They are loaded
	 * at initialization and are assumed to not be modified by the render loop.
	 *
	 * Whenever possible, add to these lists instead of creating dynamic state
	 * it is easier to debug and faster to execute for the computer.
	 */
	std::array<GLuint, texture_count> texture_gl_handles;
	std::array<ivec2, texture_count> texture_dimensions;
	// Make sure these paths remain in sync with the associated enumerators.
	// Associated id with .obj path
	const std::vector < std::pair<GEOMETRY_BUFFER_ID, std::string>> mesh_paths =
	{
			std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::BLENDY, mesh_path("Blendy-Reduced.obj")),
			std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::MINION, mesh_path("Minion-Reduced.obj")),
			std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::BULLET, mesh_path("Bullet-color.obj")),
			std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::MINION_BULLET, mesh_path("Minion_Bullet-color.obj")),
		  
			std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::GRAPE, mesh_path("grape.obj")),

			std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::LEMON, mesh_path("lemon.obj")),

			std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::ORANGE, mesh_path("orange.obj")),

			std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::PROTEIN_POWER, mesh_path("protein_powder.obj")),

			std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::BATTERY, mesh_path("battery.obj")),

			std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::BOSS, mesh_path("boss_down.obj")),

		  // specify meshes of other assets here
	};

	// Make sure these paths remain in sync with the associated enumerators.
	const std::array<std::string, texture_count> texture_paths = {
			textures_path("blendy-flat.png"),
			textures_path("blendy-flat-nm.png"),
			textures_path("minion-standing.png"),
			textures_path("minion-standing-nm.png"),
			textures_path("background.png"),
			textures_path("directional-light.png"),
			textures_path("blendy_left_0.png"),
			textures_path("blendy_left_1.png"),
			textures_path("blendy_left_2.png"),
			textures_path("blendy_left_3.png"),
			textures_path("blendy_right_0.png"),
			textures_path("blendy_right_1.png"),
			textures_path("blendy_right_2.png"),
			textures_path("blendy_right_3.png"),
			textures_path("blendy_down_0.png"),
			textures_path("blendy_down_1.png"),
			textures_path("blendy_down_2.png"),
			textures_path("blendy_down_3.png"),
			textures_path("blendy_up_0.png"),
			textures_path("blendy_up_1.png"),
			textures_path("blendy_up_2.png"),
			textures_path("blendy_up_3.png"),
			textures_path("blendy_left_0_nm.png"),
			textures_path("blendy_left_1_nm.png"),
			textures_path("blendy_left_2_nm.png"),
			textures_path("blendy_left_3_nm.png"),
			textures_path("blendy_right_0_nm.png"),
			textures_path("blendy_right_1_nm.png"),
			textures_path("blendy_right_2_nm.png"),
			textures_path("blendy_right_3_nm.png"),
			textures_path("blendy_down_0_nm.png"),
			textures_path("blendy_down_1_nm.png"),
			textures_path("blendy_down_2_nm.png"),
			textures_path("blendy_down_3_nm.png"),
			textures_path("blendy_up_0_nm.png"),
			textures_path("blendy_up_1_nm.png"),
			textures_path("blendy_up_2_nm.png"),
			textures_path("blendy_up_3_nm.png"),
			textures_path("bullet.png"),
			textures_path("bullet_nm.png"),
			textures_path("fullHealthBar.png"),
			textures_path("HelpScreen.png"),
			textures_path("HealthBarFrame.png"),
			textures_path("cutscene_1_1.png"),
			textures_path("cutscene_1_2.png"),
			textures_path("cutscene_1_3.png"),
			textures_path("cutscene_1_4.png"),
			textures_path("cutscene_2_1.png"),
			textures_path("cutscene_2_2.png"),
			textures_path("cutscene_3_1.png"),
			textures_path("title_screen.png"),
			textures_path("m_left_0.png"),
			textures_path("m_left_1.png"),
			textures_path("m_left_2.png"),
			textures_path("m_right_0.png"),
			textures_path("m_right_1.png"),
			textures_path("m_right_2.png"),
			textures_path("m_up_0.png"),
			textures_path("m_up_1.png"),
			textures_path("m_up_2.png"),
			textures_path("m_down_0.png"),
			textures_path("m_down_1.png"),
			textures_path("m_down_2.png"),
			textures_path("m_left_0_n.png"),
			textures_path("m_left_1_n.png"),
			textures_path("m_left_2_n.png"),
			textures_path("m_right_0_n.png"),
			textures_path("m_right_1_n.png"),
			textures_path("m_right_2_n.png"),
			textures_path("m_up_0_n.png"),
			textures_path("m_up_1_n.png"),
			textures_path("m_up_2_n.png"),
			textures_path("m_down_0_n.png"),
			textures_path("m_down_1_n.png"),
			textures_path("m_down_2_n.png"),

			textures_path("battery.png"),
			textures_path("orange.png"),
			textures_path("lemon.png"),
			textures_path("grape.png"),
			textures_path("protein powder.png"),
			textures_path("battery_nm.png"),
			textures_path("orange_nm.png"),
			textures_path("lemon_nm.png"),
			textures_path("grape_nm.png"),
			textures_path("protein_powder_nm.png"),
			textures_path("bright_bot_0.png"),
			textures_path("bright_bot_1.png"),
			textures_path("bright_bot_2.png"),
			textures_path("bright_bot_3.png"),
			textures_path("bright_top_0.png"),
			textures_path("bright_top_1.png"),
			textures_path("bright_top_2.png"),
			textures_path("bright_top_3.png"),
			textures_path("bleft_bot_0.png"),
			textures_path("bleft_bot_1.png"),
			textures_path("bleft_bot_2.png"),
			textures_path("bleft_bot_3.png"),
			textures_path("bleft_top_0.png"),
			textures_path("bleft_top_1.png"),
			textures_path("bleft_top_2.png"),
			textures_path("bleft_top_3.png"),

			textures_path("bright_bot_0_n.png"),
			textures_path("bright_bot_1_n.png"),
			textures_path("bright_bot_2_n.png"),
			textures_path("bright_bot_3_n.png"),
			textures_path("bright_top_0_n.png"),
			textures_path("bright_top_1_n.png"),
			textures_path("bright_top_2_n.png"),
			textures_path("bright_top_3_n.png"),
			textures_path("bleft_bot_0_n.png"),
			textures_path("bleft_bot_1_n.png"),
			textures_path("bleft_bot_2_n.png"),
			textures_path("bleft_bot_3_n.png"),
			textures_path("bleft_top_0_n.png"),
			textures_path("bleft_top_1_n.png"),
			textures_path("bleft_top_2_n.png"),
			textures_path("bleft_top_3_n.png"),

			textures_path("healer_d0.png"),
			textures_path("healer_d0_n.png"),
			textures_path("healer_d1.png"),
			textures_path("healer_d1_n.png"),
			textures_path("healer_d2.png"),
			textures_path("healer_d2_n.png"),

			textures_path("healer_l0.png"),
			textures_path("healer_l0_n.png"),
			textures_path("healer_l1.png"),
			textures_path("healer_l1_n.png"),
			textures_path("healer_l2.png"),
			textures_path("healer_l2_n.png"),

			textures_path("healer_r0.png"),
			textures_path("healer_r0_n.png"),
			textures_path("healer_r1.png"),
			textures_path("healer_r1_n.png"),
			textures_path("healer_r2.png"),
			textures_path("healer_r2_n.png"),

			textures_path("healer_u0.png"),
			textures_path("healer_u0_n.png"),
			textures_path("healer_u1.png"),
			textures_path("healer_u1_n.png"),
			textures_path("healer_u2.png"),
			textures_path("healer_u2_n.png"),

			textures_path("roamer_d0.png"),
			textures_path("roamer_d0_n.png"),
			textures_path("roamer_d1.png"),
			textures_path("roamer_d1_n.png"),
			textures_path("roamer_d2.png"),
			textures_path("roamer_d2_n.png"),

			textures_path("roamer_l0.png"),
			textures_path("roamer_l0_n.png"),
			textures_path("roamer_l1.png"),
			textures_path("roamer_l1_n.png"),
			textures_path("roamer_l2.png"),
			textures_path("roamer_l2_n.png"),

			textures_path("roamer_r0.png"),
			textures_path("roamer_r0_n.png"),
			textures_path("roamer_r1.png"),
			textures_path("roamer_r1_n.png"),
			textures_path("roamer_r2.png"),
			textures_path("roamer_r2_n.png"),

			textures_path("roamer_u0.png"),
			textures_path("roamer_u0_n.png"),
			textures_path("roamer_u1.png"),
			textures_path("roamer_u1_n.png"),
			textures_path("roamer_u2.png"),
			textures_path("roamer_u2_n.png"),

			textures_path("shooter_d0.png"),
			textures_path("shooter_d0_n.png"),
			textures_path("shooter_d1.png"),
			textures_path("shooter_d1_n.png"),
			textures_path("shooter_d2.png"),
			textures_path("shooter_d2_n.png"),

			textures_path("shooter_l0.png"),
			textures_path("shooter_l0_n.png"),
			textures_path("shooter_l1.png"),
			textures_path("shooter_l1_n.png"),
			textures_path("shooter_l2.png"),
			textures_path("shooter_l2_n.png"),

			textures_path("shooter_r0.png"),
			textures_path("shooter_r0_n.png"),
			textures_path("shooter_r1.png"),
			textures_path("shooter_r1_n.png"),
			textures_path("shooter_r2.png"),
			textures_path("shooter_r2_n.png"),

			textures_path("shooter_u0.png"),
			textures_path("shooter_u0_n.png"),
			textures_path("shooter_u1.png"),
			textures_path("shooter_u1_n.png"),
			textures_path("shooter_u2.png"),
			textures_path("shooter_u2_n.png"),


			textures_path("Sniper_d0.png"),
			textures_path("Sniper_d0_n.png"),
			textures_path("Sniper_d1.png"),
			textures_path("Sniper_d1_n.png"),
			textures_path("Sniper_d2.png"),
			textures_path("Sniper_d2_n.png"),

			textures_path("Sniper_l0.png"),
			textures_path("Sniper_l0_n.png"),
			textures_path("Sniper_l1.png"),
			textures_path("Sniper_l1_n.png"),
			textures_path("Sniper_l2.png"),
			textures_path("Sniper_l2_n.png"),

			textures_path("Sniper_r0.png"),
			textures_path("Sniper_r0_n.png"),
			textures_path("Sniper_r1.png"),
			textures_path("Sniper_r1_n.png"),
			textures_path("Sniper_r2.png"),
			textures_path("Sniper_r2_n.png"),

			textures_path("Sniper_u0.png"),
			textures_path("Sniper_u0_n.png"),
			textures_path("Sniper_u1.png"),
			textures_path("Sniper_u1_n.png"),
			textures_path("Sniper_u2.png"),
			textures_path("Sniper_u2_n.png"),

			textures_path("cursor.png"),
			textures_path("cherry.png"),
			textures_path("nut.png"),
			textures_path("cactus.png"),

			textures_path("Boss_d0.png"),
			textures_path("Boss_d0_n.png"),
			textures_path("Boss_d1.png"),
			textures_path("Boss_d1_n.png"),
			textures_path("Boss_d2.png"),
			textures_path("Boss_d2_n.png"),
			textures_path("Boss_d3.png"),
			textures_path("Boss_d3_n.png"),

			textures_path("Boss_l0.png"),
			textures_path("Boss_l0_n.png"),
			textures_path("Boss_l1.png"),
			textures_path("Boss_l1_n.png"),
			textures_path("Boss_l2.png"),
			textures_path("Boss_l2_n.png"),
			textures_path("Boss_l3.png"),
			textures_path("Boss_l3_n.png"),

			textures_path("Boss_r0.png"),
			textures_path("Boss_r0_n.png"),
			textures_path("Boss_r1.png"),
			textures_path("Boss_r1_n.png"),
			textures_path("Boss_r2.png"),
			textures_path("Boss_r2_n.png"),
			textures_path("Boss_r3.png"),
			textures_path("Boss_r3_n.png"),

			textures_path("Boss_u0.png"),
			textures_path("Boss_u0_n.png"),
			textures_path("Boss_u1.png"),
			textures_path("Boss_u1_n.png"),
			textures_path("Boss_u2.png"),
			textures_path("Boss_u2_n.png"),
			textures_path("Boss_u3.png"),
			textures_path("Boss_u3_n.png"),

			textures_path("Boss_dl0.png"),
			textures_path("Boss_dl0_n.png"),
			textures_path("Boss_dl1.png"),
			textures_path("Boss_dl1_n.png"),
			textures_path("Boss_dl2.png"),
			textures_path("Boss_dl2_n.png"),
			textures_path("Boss_dl3.png"),
			textures_path("Boss_dl3_n.png"),

			textures_path("Boss_dr0.png"),
			textures_path("Boss_dr0_n.png"),
			textures_path("Boss_dr1.png"),
			textures_path("Boss_dr1_n.png"),
			textures_path("Boss_dr2.png"),
			textures_path("Boss_dr2_n.png"),
			textures_path("Boss_dr3.png"),
			textures_path("Boss_dr3_n.png"),

			textures_path("Boss_ul0.png"),
			textures_path("Boss_ul0_n.png"),
			textures_path("Boss_ul1.png"),
			textures_path("Boss_ul1_n.png"),
			textures_path("Boss_ul2.png"),
			textures_path("Boss_ul2_n.png"),
			textures_path("Boss_ul3.png"),
			textures_path("Boss_ul3_n.png"),

			textures_path("Boss_ur0.png"),
			textures_path("Boss_ur0_n.png"),
			textures_path("Boss_ur1.png"),
			textures_path("Boss_ur1_n.png"),
			textures_path("Boss_ur2.png"),
			textures_path("Boss_ur2_n.png"),
			textures_path("Boss_ur3.png"),
			textures_path("Boss_ur3_n.png"),
			
			textures_path("cactus_bullet.png")

	};

	std::array<GLuint, effect_count> effects;
	// Make sure these paths remain in sync with the associated enumerators.
	const std::array<std::string, effect_count> effect_paths = {
		shader_path("coloured"),
		shader_path("egg"),
		shader_path("chicken"),
		shader_path("textured"),
		shader_path("wind"),
		shader_path("healthBar"),
		shader_path("particles")
	};

	std::array<GLuint, geometry_count> vertex_buffers;
	std::array<GLuint, geometry_count> index_buffers;
	std::array<Mesh, geometry_count> meshes;

	// Fonts
	std::map<char, Character> m_ftCharacters;
	GLuint m_font_shaderProgram;
	GLuint m_font_VAO;
	GLuint m_font_VBO;

	const std::string FONT_FOLDER_PATH = "..//..//..//data//fonts//";
	const std::string DOT_TTF = ".ttf";
	const std::string Kenney_Pixel_Square = "Kenney_Pixel_Square";
	const std::string Kenney_Future_Narrow = "Kenney_Future_Narrow";
	const unsigned int FONT_DEFAULT_SIZE = 48;


	// Dummy VAO
	GLuint dummy_vao;

	// Particle System
	Emitter emitter;

public:
	// Initialize the window
	bool init(GLFWwindow* window);

	template <class T>
	void bindVBOandIBO(GEOMETRY_BUFFER_ID gid, std::vector<T> vertices, std::vector<uint16_t> indices);

	void initializeGlTextures();

	void initializeGlEffects();

	void initializeGlMeshes();

	Mesh& getMesh(GEOMETRY_BUFFER_ID id) { return meshes[(int)id]; }

	void initializeGlGeometryBuffers();

	//void initializeParticleSystem();

	// Initialize the screen texture used as intermediate render target
	// The draw loop first renders to this texture, then it is used for the wind
	// shader
	bool initScreenTexture();

	// Destroy resources associated to one or all entities created by the system
	~RenderSystem();

	// Draw all entities
	void draw();

	mat3 createProjectionMatrix();

	// Directional Light
	Entity& getDirectionalLight() { return directional_light; }

	void setDirectionalLight(const Entity& light) { directional_light = light; }

	// Fonts
	bool initializeFonts();
	bool fontInit_internal(const std::string& font_filename, unsigned font_default_size);
	void renderText(const std::string& text, float x, float y, float scale, const glm::vec3& color, const glm::mat4& trans);

	// Particle System
	void particles_step(const float& elapsed_ms);
private:
	// Internal drawing functions for each entity type
	void drawTexturedMesh(Entity entity, const mat3& projection);
	void drawToScreen();


	// Helpers for drawTexturedMesh
	void handle_textured_rendering(Entity entity, GLuint program, const RenderRequest& render_request);
	void handle_normal_map_uniform(Entity entity, GLuint program);
	void setUsesNormalMap(bool cond, const GLuint program);
	void handle_chicken_or_egg_effect_rendering(const RenderRequest& render_request, GLuint program);
	void configure_base_uniforms(::Entity entity, const mat3& projection, Transform transform, const GLuint program, GLsizei& out_num_indices, const RenderRequest& render_request);
	void handle_giant_uniform(const Entity entity, const GLuint program);
	                             
	void handle_health_bar_rendering(const RenderRequest& render_request, GLuint program);
	void handle_particle_rendering(const RenderRequest& render_request, const GLuint& program, const mat3& projection, const Transform& transform);


	// Debugging FPS
	void debug_fps(const mat3& projection);

	// Score Counter
	void display_score();

	void display_text();

	// Window handle
	GLFWwindow* window;

	// Screen texture handles
	GLuint frame_buffer;
	GLuint off_screen_render_buffer_color;
	GLuint off_screen_render_buffer_depth;

	Entity screen_state_entity;
	Entity directional_light;
};

bool loadEffectFromFile(
	const std::string& vs_path, const std::string& fs_path, GLuint& out_program);
