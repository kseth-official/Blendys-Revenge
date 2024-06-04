// Header
#include "world_system.hpp"
#include "world_init.hpp"
#include <iostream>
// stlib
#include <cassert>
#include <sstream>

#include "physics_system.hpp"
#include <fstream>



// Game configuration
const size_t MAX_TOTAL_MINIONS = 30;
const size_t MAX_MINIONS = 10;
const size_t MAX_DODGERS = 3;
const size_t MAX_ROAMER = 2;
const size_t MAX_CHARGER = 3;
const size_t MAX_TANK = 2;
const size_t MAX_SNIPER = 2;
const size_t MAX_HEALER = 1;
const size_t MAX_GIANT = 1;
const size_t MAX_CLEANER = 1;
const size_t MINION_DELAY_MS = 200 * 6;
const float LIGHT_SOURCE_MOVEMENT_DISTANCE = 50.0f;
const size_t MAX_BATTERY_POWERUPS = 3;
const size_t MAX_PROTEIN_POWDER_POWERUPS = 3;
const size_t MAX_GRAPE_POWERUPS = 3;
const size_t MAX_LEMON_POWERUPS = 3;
const size_t MAX_CHERRY_POWERUPS = 3;
const size_t MAX_SHIELD_POWERUPS = 6;
const size_t MAX_CACTUS_POWERUPS = 3;
const size_t POWERUP_DELAY_MS = 200 * 3;
const int boss_spawn_score = 5000;
const float PLAYER_POWERUP_SPAWN_DISTANCE = 150.0f;
const float protein_coef = 1.6;
const float grape_coef = 0.6f;
const float cherry_coef = 0.8f;

// UI
const vec3 BLENDY_COLOR = { 0.78f, 0.39f, 0.62f };
const vec3 MAGENTA = { 0.78f, 0.39f, 0.62f };
const vec3 RED = { 1.f, 0.f, 0.f };
const vec3 WHITE = vec3{ 1.f,1.f,1.f };


// DEFAULT START POSITIONS
const vec2 TOP_LEFT_OF_SCREEN = { 0.f,0.f };
const vec2 CENTER_OF_SCREEN = { window_width_px / 2, window_height_px / 2 };
const vec2 BOTTOM_RIGHT_OF_SCREEN = { window_width_px, window_height_px };
const vec2 BOTTOM_LEFT_OF_SCREEN = { 0, window_height_px };
const vec2 BOTTOM_RIGHT_OF_SCREEN_DIRECTIONAL_LIGHT = { window_width_px / 2, 0 };
const vec2 BLENDY_START_POSITION = { window_width_px / 2, window_height_px / 2 };
const vec2 HEALTH_BAR_POSITION = { 140.f, 25.f };
const vec2 HEALTH_BAR_FRAME_POSITION = { 120.f, 25.f };
const vec2 SHIELD_POSITION_1 = { 270.f, 25.f };
const vec2 SHIELD_POSITION_2 = { 320.f, 25.f };
const vec2 SHIELD_POSITION_3 = { 370.f, 25.f };

// BOUNDS
const vec2 BLENDY_BOUNDS = { BLENDY_BB_WIDTH * 0.9, BLENDY_BB_HEIGHT * 0.9 };
const vec2 BOSS_BOUNDS = { BLENDY_BB_WIDTH * 1.2, BLENDY_BB_HEIGHT * 1.2 };
const vec2 DIRECTIONAL_LIGHT_BOUNDS = { DIRECTIONAL_LIGHT_BB_WIDTH, DIRECTIONAL_LIGHT_BB_HEIGHT };
const vec2 BACKGROUND_BOUNDS = { BACKGROUND_BB_WIDTH, BACKGROUND_BB_HEIGHT };
const vec2 MINION_BOUNDS = { MINION_BB_WIDTH, MINION_BB_HEIGHT };
const vec2 HEALTH_BAR_BOUNDS = { 175.f, 32.f };
const vec2 HEALTH_BAR_FRAME_BOUNDS = { 230.f, 55.f };
const vec2 HELP_SCREEN_BOUNDS = { BACKGROUND_BB_WIDTH, BACKGROUND_BB_HEIGHT };
const vec2 BATTERY_POWERUP_BOUNDS = { 60.f, 80.f };
const vec2 PROTEIN_POWDER_POWERUP_BOUNDS = { 70.f, 80.f };
const vec2 LEMON_POWERUP_BOUNDS = { 70.f, 70.f };
const vec2 GRAPE_POWERUP_BOUNDS = { 80.f, 70.f };
const vec2 SHIELD_POWERUP_BOUNDS = { 70.f, 70.f };
const vec2 CACTUS_POWERUP_BOUNDS = { 70.f, 70.f };
const vec2 SHIELD_HEALTH_BOUNDS = { 40.f, 40.f };
bool is_dead = false;
const vec2 dead_velocity = { 0, 100.0f };
const float dead_angle = 3.0f;
const vec2 dead_scale = { 0, 0 };

// ANIMATION VALUES
const size_t BLENDY_FRAME_DELAY = 20 * 3;
const size_t MINION_FRAME_DELAY = 20 * 6;

// EYE POSITION (For Lighting Purposes)
const float CAMERA_Z_DEPTH = 1500.f;
const vec3 CAMERA_POSITION = { window_width_px / 2, window_height_px / 2, CAMERA_Z_DEPTH };

// FPS COUNTER
const vec2 FPS_COUNTER_TRANSLATION_FROM_BOTTOM_LEFT_OF_SCREEN = { 0.f, 0.f };
const vec2 FPS_COUNTER_SCALE = { 1.f,1.f };
const vec3 FPS_TEXT_COLOR = BLENDY_COLOR;

// SCORE COUNTER
const float SCORE_COUNTER_X = 30.f;
const float SCORE_COUNTER_Y = window_height_px - 107.f;
const vec2 SCORE_COUNTER_TRANSLATION_FROM_BOTTOM_LEFT_OF_SCREEN = { SCORE_COUNTER_X, SCORE_COUNTER_Y };
const vec2 SCORE_COUNTER_SCALE = { 1.f,1.f };
const vec3 SCORE_TEXT_COLOR = BLENDY_COLOR;

// CUTSCENE STUFF
const int FIRST_CUT_SCENE_END = 6;
const int SECOND_CUT_SCENE_END = 9;
const int THIRD_CUT_SCORE = 3000;

// MUSIC
const unsigned int MUSIC_SPEEDUP_THRESHOLD = 1000;

// GAME BALANCE

const float MULTIPLIER = 1;
const unsigned int DODGER_SPAWN_SCORE = 100 * MULTIPLIER;
const unsigned int ROAMER_SPAWN_SCORE = 300 * MULTIPLIER;
const unsigned int CHARGER_SPAWN_SCORE = 800 * MULTIPLIER;
const unsigned int SNIPER_SPAWN_SCORE = 1300 * MULTIPLIER;
const unsigned int TANK_SPAWN_SCORE = 1700 * MULTIPLIER;
const unsigned int GIANT_SPAWN_SCORE = 2000 * MULTIPLIER;
const unsigned int SPLIT_SHOOTER_SPAWN_SCORE = 2500 * MULTIPLIER;
const unsigned int BOSS_SPAWN_SCORE = 6000 * MULTIPLIER;

const unsigned int BATTERY_SPAWN_SCORE = 0 * MULTIPLIER;
const unsigned int PROTEIN_SPAWN_SCORE = 300 * MULTIPLIER;
const unsigned int LEMON_SPAWN_SCORE = 600 * MULTIPLIER;
const unsigned int GRAPE_SPAWN_SCORE = 1500 * MULTIPLIER;
const unsigned int SHIELD_SPAWN_SCORE = 600 * MULTIPLIER;
const unsigned int CHERRY_SPAWN_SCORE = 1100 * MULTIPLIER;
const unsigned int CACTUS_SPAWN_SCORE = 1800 * MULTIPLIER;

const int SECOND_CUT_SCORE = BOSS_SPAWN_SCORE / 2;
/* Point Counts:
 * dodger 25
 * roamer 30
 * charger 40
 * sniper 50
 * tank 80
 * giant 200
 * split_shooter 40
 * cleaner 50
 */

// Create the bug world
WorldSystem::WorldSystem()
	: points(0)
{
	// Seeding rng with random device
	rng = std::default_random_engine(std::random_device()());
}

WorldSystem::~WorldSystem() {
	// Destroy music components
	if (background_music != nullptr)
		Mix_FreeMusic(background_music);
	if (background_music_sped_up != nullptr)
		Mix_FreeMusic(background_music_sped_up);
	if (dead_sound != nullptr)
		Mix_FreeChunk(dead_sound);
	if (get_point != nullptr)
		Mix_FreeChunk(get_point);
	if (powerup_pickup_battery != nullptr)
		Mix_FreeChunk(powerup_pickup_battery);
	if (powerup_pickup_grape != nullptr)
		Mix_FreeChunk(powerup_pickup_grape);
	if (powerup_pickup_lemon != nullptr)
		Mix_FreeChunk(powerup_pickup_lemon);
	if (powerup_pickup_protein != nullptr)
		Mix_FreeChunk(powerup_pickup_protein);
	if (player_hurt != nullptr)
		Mix_FreeChunk(player_hurt);
	if (minion_hurt != nullptr)
		Mix_FreeChunk(minion_hurt);
	if (minion_dead != nullptr)
		Mix_FreeChunk(minion_dead);
	Mix_CloseAudio();

	// Destroy all created components
	registry.clear_all_components();

	// Close the window
	glfwDestroyWindow(window);
}

void WorldSystem::update_game_music()
{
	if (registry.score >= MUSIC_SPEEDUP_THRESHOLD && game_music_state == MusicState::Ordinary)
	{
		Mix_FadeInMusic(background_music_sped_up, -1, 1000);
		game_music_state = MusicState::SpedUp;
	}
}

// Debugging
namespace {
	void glfw_err_cb(int error, const char *desc) {
		fprintf(stderr, "%d: %s", error, desc);
	}
}

// World initialization
// Note, this has a lot of OpenGL specific things, could be moved to the renderer
GLFWwindow* WorldSystem::create_window() {
	///////////////////////////////////////
	// Initialize GLFW
	glfwSetErrorCallback(glfw_err_cb);
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW");
		return nullptr;
	}

	//-------------------------------------------------------------------------
	// If you are on Linux or Windows, you can change these 2 numbers to 4 and 3 and
	// enable the glDebugMessageCallback to have OpenGL catch your mistakes for you.
	// GLFW / OGL Initialization
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, 0);

	// Create the main window (for rendering, keyboard, and mouse input)
	window = glfwCreateWindow(window_width_px, window_height_px, "Blendy's Revenge", nullptr, nullptr);
	if (window == nullptr) {
		fprintf(stderr, "Failed to glfwCreateWindow");
		return nullptr;
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	// Setting callbacks to member functions (that's why the redirect is needed)
	// Input is handled using GLFW, for more info see
	// http://www.glfw.org/docs/latest/input_guide.html
	glfwSetWindowUserPointer(window, this);
	auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((WorldSystem*)glfwGetWindowUserPointer(wnd))->on_key(_0, _1, _2, _3); };
	//auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1) { ((WorldSystem*)glfwGetWindowUserPointer(wnd))->on_mouse_move({ _0, _1 }); };
	glfwSetKeyCallback(window, key_redirect);
	//glfwSetCursorPosCallback(window, cursor_pos_redirect);

	//////////////////////////////////////
	// Loading music and sounds with SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		fprintf(stderr, "Failed to initialize SDL Audio");
		return nullptr;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
		fprintf(stderr, "Failed to open audio device");
		return nullptr;
	}

	background_music = Mix_LoadMUS(audio_path("music.wav").c_str());
	dead_sound = Mix_LoadWAV(audio_path("dead_effect.wav").c_str());
	get_point = Mix_LoadWAV(audio_path("get_point.wav").c_str());

	background_music_sped_up = Mix_LoadMUS(audio_path("music_sped_up.wav").c_str());
	powerup_pickup_battery = Mix_LoadWAV(audio_path("powerup_pickup_battery.wav").c_str());
	powerup_pickup_grape = Mix_LoadWAV(audio_path("powerup_pickup_grape.wav").c_str());
	powerup_pickup_lemon = Mix_LoadWAV(audio_path("powerup_pickup_lemon.wav").c_str());
	powerup_pickup_protein = Mix_LoadWAV(audio_path("powerup_pickup_protein.wav").c_str());
	player_hurt = Mix_LoadWAV(audio_path("hurt_sound.wav").c_str());
	minion_hurt = Mix_LoadWAV(audio_path("button_press.wav").c_str());
	Mix_VolumeChunk(minion_hurt, 48);
	minion_dead = Mix_LoadWAV(audio_path("minion_dead_sound.wav").c_str());
	//Mix_VolumeChunk(minion_dead, 64);

	if (background_music == nullptr 
		|| dead_sound == nullptr 
		|| get_point == nullptr
		|| background_music_sped_up == nullptr
		|| powerup_pickup_battery == nullptr
		|| powerup_pickup_grape == nullptr
		|| powerup_pickup_lemon == nullptr
		|| powerup_pickup_protein == nullptr
		|| player_hurt == nullptr
		|| minion_hurt == nullptr
		|| minion_dead == nullptr
		) {
		fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n make sure the data directory is present",
			audio_path("music.wav").c_str(),
			audio_path("dead_effect.wav").c_str(),
			audio_path("get_point.wav").c_str()),
			audio_path("music_sped_up.wav").c_str(),
			audio_path("powerup_pickup_battery.wav").c_str(),
			audio_path("powerup_pickup_grape.wav").c_str(),
			audio_path("powerup_pickup_lemon.wav").c_str(),
			audio_path("powerup_pickup_protein.wav").c_str(),
			audio_path("hurt_sound.wav").c_str(),
			audio_path("button_press.wav").c_str(),
			audio_path("minion_dead_sound.wav").c_str();
		return nullptr;
	}

	return window;
}

void WorldSystem::init(RenderSystem* renderer_arg) {
	this->renderer = renderer_arg;

	// Set all states to default
    restart_game();
}

// Updates Health bar when blendy gets hit
void WorldSystem::update_health_bar()
{
	while (registry.debugComponents.entities.size() > 0)
		registry.remove_all_components_of(registry.debugComponents.entities.back());

	auto& blendy = registry.players.get(player_blendy);
	
	float current_width = HEALTH_BAR_BOUNDS.x * static_cast<float>(blendy.health) / static_cast<float>(blendy.max_health);

	float offset_to_center = (current_width - HEALTH_BAR_BOUNDS.x) / 2.0f;

	vec2 health_bar_center = { HEALTH_BAR_POSITION.x + offset_to_center, HEALTH_BAR_POSITION.y };

	vec2 health_bar_scale = { current_width, HEALTH_BAR_BOUNDS.y };

	createLine(health_bar_center, health_bar_scale);

	// Clear existing shield entities
	if (registry.shields.has(shield_1) || registry.shields.has(shield_2) || registry.shields.has(shield_3)) {
		registry.remove_all_components_of(shield_1);
		registry.remove_all_components_of(shield_2);
		registry.remove_all_components_of(shield_3);
	}

	// Update shield display based on blendy's current shields
	switch (blendy.shield) {
	case 3:
		shield_3 = create_shield_health(renderer, SHIELD_POSITION_3, SHIELD_HEALTH_BOUNDS);

	case 2:
		shield_2 = create_shield_health(renderer, SHIELD_POSITION_2, SHIELD_HEALTH_BOUNDS);

	case 1:
		shield_1 = create_shield_health(renderer, SHIELD_POSITION_1, SHIELD_HEALTH_BOUNDS);
		break;
	default:
		// No shields
		break;
	}
}

// make powerups spawn randomly on the map
void WorldSystem::update_powerups(float elapsed_ms_since_last_update)
{
	// Decrement the cooldown for each type of powerup
	next_battery_powerup_spawn -= elapsed_ms_since_last_update * current_speed;
	next_protein_powerup_spawn -= elapsed_ms_since_last_update * current_speed;
	next_grape_powerup_spawn -= elapsed_ms_since_last_update * current_speed;
	next_lemon_powerup_spawn -= elapsed_ms_since_last_update * current_speed;
	next_cherry_powerup_spawn -= elapsed_ms_since_last_update * current_speed;
	next_shield_powerup_spawn -= elapsed_ms_since_last_update * current_speed;
	next_cactus_powerup_spawn -= elapsed_ms_since_last_update * current_speed;

	// Get the position of the player
	Motion& player_motion = registry.motions.get(player_blendy);
	vec2 player_pos = player_motion.position;

	// Spawn battery powerup 

	if (registry.powerUps.components.size() <= MAX_BATTERY_POWERUPS && next_battery_powerup_spawn < 0.f && registry.score > BATTERY_SPAWN_SCORE) {
		next_battery_powerup_spawn = (POWERUP_DELAY_MS * 20) + uniform_dist(rng) * POWERUP_DELAY_MS;

		// Generate a random position, excluding the player's position
		vec2 random_pos;
		do {
			random_pos = vec2(50.f + uniform_dist(rng) * (window_width_px - 150.f), 50.f + uniform_dist(rng) * (window_height_px - 300.f) + 150.f);
		} while (length(random_pos - player_pos) < PLAYER_POWERUP_SPAWN_DISTANCE);

		create_battery_powerup(renderer, random_pos, BATTERY_POWERUP_BOUNDS);
	}

	// Spawn grape powerup
	if (registry.powerUps.components.size() <= MAX_GRAPE_POWERUPS && next_grape_powerup_spawn < 0.f && registry.score > GRAPE_SPAWN_SCORE) {
		next_grape_powerup_spawn = POWERUP_DELAY_MS * 20 + uniform_dist(rng) * POWERUP_DELAY_MS;

		vec2 random_pos;
		do {
			random_pos = vec2(50.f + uniform_dist(rng) * (window_width_px - 150.f), 50.f + uniform_dist(rng) * (window_height_px - 300.f) + 150.f);
		} while (length(random_pos - player_pos) < PLAYER_POWERUP_SPAWN_DISTANCE);

		create_grape_powerup(renderer, random_pos, GRAPE_POWERUP_BOUNDS);
	}

	// Spawn lemon powerup
	if (registry.powerUps.components.size() <= MAX_LEMON_POWERUPS && next_lemon_powerup_spawn < 0.f && registry.score > LEMON_SPAWN_SCORE) {
		next_lemon_powerup_spawn = POWERUP_DELAY_MS * 20 + uniform_dist(rng) * POWERUP_DELAY_MS;

		vec2 random_pos;
		do {
			random_pos = vec2(50.f + uniform_dist(rng) * (window_width_px - 150.f), 50.f + uniform_dist(rng) * (window_height_px - 300) + 150.f);
		} while (length(random_pos - player_pos) < PLAYER_POWERUP_SPAWN_DISTANCE);

		create_lemon_powerup(renderer, random_pos, LEMON_POWERUP_BOUNDS);
	}

	// Spawn protein powder powerup
	if (registry.powerUps.components.size() <= MAX_PROTEIN_POWDER_POWERUPS && next_protein_powerup_spawn < 0.f && registry.score > PROTEIN_SPAWN_SCORE) {
		next_protein_powerup_spawn = POWERUP_DELAY_MS * 20 + uniform_dist(rng) * POWERUP_DELAY_MS;

		vec2 random_pos;
		do {
			random_pos = vec2(50.f + uniform_dist(rng) * (window_width_px - 150.f), 50.f + uniform_dist(rng) * (window_height_px - 300.f) + 150.f);
		} while (length(random_pos - player_pos) < PLAYER_POWERUP_SPAWN_DISTANCE);

		create_protein_powerup(renderer, random_pos, PROTEIN_POWDER_POWERUP_BOUNDS);
	}

	// Spawn cherry powerup
	if (registry.powerUps.components.size() <= MAX_CHERRY_POWERUPS && next_cherry_powerup_spawn < 0.f && registry.score > CHERRY_SPAWN_SCORE) {
		next_cherry_powerup_spawn = POWERUP_DELAY_MS * 20 + uniform_dist(rng) * POWERUP_DELAY_MS;

		vec2 random_pos;
		do {
			random_pos = vec2(50.f + uniform_dist(rng) * (window_width_px - 150.f), 50.f + uniform_dist(rng) * (window_height_px - 300.f) + 150.f);
		} while (length(random_pos - player_pos) < PLAYER_POWERUP_SPAWN_DISTANCE);

		create_cherry_powerup(renderer, random_pos, PROTEIN_POWDER_POWERUP_BOUNDS);
	}

	// Spawn shield powerup
	if (registry.powerUps.components.size() <= MAX_SHIELD_POWERUPS && next_shield_powerup_spawn < 0.f && registry.score > SHIELD_SPAWN_SCORE) {
		next_shield_powerup_spawn = POWERUP_DELAY_MS * 20 + uniform_dist(rng) * POWERUP_DELAY_MS;

		vec2 random_pos;
		do {
			random_pos = vec2(50.f + uniform_dist(rng) * (window_width_px - 150.f), 50.f + uniform_dist(rng) * (window_height_px - 300.f) + 150.f);
		} while (length(random_pos - player_pos) < PLAYER_POWERUP_SPAWN_DISTANCE);

		create_shield_powerup(renderer, random_pos, SHIELD_POWERUP_BOUNDS);
	}

	// Spawn cactus powerup
	if (registry.powerUps.components.size() <= MAX_CACTUS_POWERUPS && next_cactus_powerup_spawn < 0.f && registry.score > CACTUS_SPAWN_SCORE) {
		next_cactus_powerup_spawn = POWERUP_DELAY_MS * 20 + uniform_dist(rng) * POWERUP_DELAY_MS;

		vec2 random_pos;
		do {
			random_pos = vec2(50.f + uniform_dist(rng) * (window_width_px - 150.f), 50.f + uniform_dist(rng) * (window_height_px - 300.f) + 150.f);
		} while (length(random_pos - player_pos) < PLAYER_POWERUP_SPAWN_DISTANCE);

		create_cactus_powerup(renderer, random_pos, CACTUS_POWERUP_BOUNDS);
	}
}


vec2 generateRandomEdgePosition(float window_width_px, float window_height_px, std::uniform_real_distribution<float>& uniform_dist, std::mt19937& rng) {
	// Randomly select an edge: 0 for bottom, 1 for left, 2 for right
	int edge = std::uniform_int_distribution<int>(0, 2)(rng);

	switch (edge) {
	case 0: // Bottom edge (spawn just below visible area)
		return vec2(50.f + uniform_dist(rng) * (window_width_px - 100.f), window_height_px+200);
	case 1: // Left edge (spawn just to the left of visible area)
		return vec2(-200.f, 40.f + uniform_dist(rng) * (window_height_px - 80.f));
	case 2: // Right edge (spawn just to the right of visible area)
		return vec2(window_width_px+200, 40.f + uniform_dist(rng) * (window_height_px - 80.f));
	default:
		return vec2(); // This should never happen
	}
}


void WorldSystem::spawn_minions(float elapsed_ms_since_last_update)
{
	elapsed_ms = elapsed_ms_since_last_update;
	next_minion_spawn -= elapsed_ms_since_last_update * current_speed;
	next_dodger_spawn -= elapsed_ms_since_last_update * current_speed;
	next_roamer_spawn -= elapsed_ms_since_last_update * current_speed;
	next_charger_spawn -= elapsed_ms_since_last_update * current_speed;
	next_sniper_spawn -= elapsed_ms_since_last_update * current_speed;
	next_tank_spawn -= elapsed_ms_since_last_update * current_speed;
	next_giant_spawn -= elapsed_ms_since_last_update * current_speed;

	if (registry.boss_spawned == false) {
		if (registry.score >= BOSS_SPAWN_SCORE)
		{
			vec2 spawnPos = generateRandomEdgePosition(window_width_px, window_height_px, uniform_dist, rng);
			boss = create_boss(renderer, spawnPos, BOSS_BOUNDS);
			registry.boss_spawned = true;
		}
	}

	if (registry.minions.components.size() < MAX_MINIONS && next_minion_spawn < 0.f ) {
		next_minion_spawn = MINION_DELAY_MS + uniform_dist(rng) * MINION_DELAY_MS;
		vec2 spawnPos = generateRandomEdgePosition(window_width_px, window_height_px, uniform_dist, rng);
		create_minion(renderer, spawnPos, MINION_BOUNDS);
	}
	if (registry.shooters.components.size() < MAX_DODGERS && next_dodger_spawn < 0.f && registry.score >= DODGER_SPAWN_SCORE) {
		next_dodger_spawn = MINION_DELAY_MS * 3 + 2 * uniform_dist(rng) * (MINION_DELAY_MS);
		vec2 spawnPos = generateRandomEdgePosition(window_width_px, window_height_px, uniform_dist, rng);
		if (registry.score >= SPLIT_SHOOTER_SPAWN_SCORE) {
			create_split_shooter(renderer, spawnPos, MINION_BOUNDS);
		}
		else {
			create_dodger(renderer, spawnPos, MINION_BOUNDS);
		}
	}
	if (registry.roamers.components.size() < MAX_ROAMER && next_roamer_spawn < 0.f && registry.score >= ROAMER_SPAWN_SCORE) {
		next_roamer_spawn = MINION_DELAY_MS * 3 + 2 * uniform_dist(rng) * (MINION_DELAY_MS);
		vec2 spawnPos = generateRandomEdgePosition(window_width_px, window_height_px, uniform_dist, rng);
		create_roamer(renderer, spawnPos, MINION_BOUNDS);
	}

	if (registry.chargers.components.size() < MAX_CHARGER && next_charger_spawn < 0.f && registry.score >= CHARGER_SPAWN_SCORE) {
		next_charger_spawn = MINION_DELAY_MS * 5 + 2 * uniform_dist(rng) * (MINION_DELAY_MS);
		vec2 spawnPos = generateRandomEdgePosition(window_width_px, window_height_px, uniform_dist, rng);
		create_charger(renderer, spawnPos, MINION_BOUNDS);
	}

	if (registry.snipers.components.size() < MAX_SNIPER && next_sniper_spawn < 0.f && registry.score >= SNIPER_SPAWN_SCORE) {
		next_sniper_spawn = MINION_DELAY_MS * 5 + 3 * uniform_dist(rng) * (MINION_DELAY_MS);
		vec2 spawnPos = generateRandomEdgePosition(window_width_px, window_height_px, uniform_dist, rng);
		create_sniper(renderer, spawnPos, MINION_BOUNDS);
	}

	if (registry.tanks.components.size() < MAX_SNIPER && next_tank_spawn < 0.f && registry.score >= TANK_SPAWN_SCORE) {
		next_tank_spawn = MINION_DELAY_MS * 5 + 5 * uniform_dist(rng) * (MINION_DELAY_MS);
		vec2 spawnPos = generateRandomEdgePosition(window_width_px, window_height_px, uniform_dist, rng);
		create_tank(renderer, spawnPos, MINION_BOUNDS);
	}


	if (registry.giants.components.size() < MAX_GIANT && next_giant_spawn < 0.f && registry.score >= GIANT_SPAWN_SCORE) {
		next_giant_spawn = MINION_DELAY_MS * 5 + 2 * uniform_dist(rng) * (MINION_DELAY_MS);
		vec2 bound = { MINION_BOUNDS.x*1.5, MINION_BOUNDS.y*1.5 };
		vec2 spawnPos = generateRandomEdgePosition(window_width_px, window_height_px, uniform_dist, rng);
		create_giant(renderer, spawnPos, bound, registry.score);
	}

}

void WorldSystem::update_blendy_animation(float elapsed_ms_since_last_update) {
	Player& blendy = registry.players.get(player_blendy);
	Motion& blendy_motion = registry.motions.get(player_blendy);

	blendy.counter_ms -= elapsed_ms_since_last_update;
	if (blendy.counter_ms < 0.f) {
		blendy.counter_ms = BLENDY_FRAME_DELAY;
		if (blendy.going_up < 0) {
			blendy.frame_stage += 1;
			if (blendy.frame_stage > 4) {
				blendy.frame_stage = 4;
				blendy.going_up = 1;
			}
		}
		else {
			blendy.frame_stage -= 1;
			if (blendy.frame_stage < 0) {
				blendy.frame_stage = 0;
				blendy.going_up = -1;
			}
		}
	}
	// get what the render request status should be
	if (blendy_motion.velocity.x == 0 && blendy_motion.velocity.y == 0) {
		// just keep the current image
		registry.renderRequests.remove(player_blendy);
		if (!blendy.up && !blendy.down && !blendy.right && !blendy.left) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::BLENDY,
					TEXTURE_ASSET_ID::BLENDY_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else {
			get_blendy_render_request(blendy.up, blendy.down, blendy.right, blendy.left, blendy.frame_stage);
		}
		blendy.going_up = 1;
		blendy_motion.y_animate = 0.f;
	}
	else {
		// blendy is moving - calculate appropriate frame to put in render request
		registry.renderRequests.remove(player_blendy);
		get_blendy_render_request(blendy.up, blendy.down, blendy.right, blendy.left, blendy.frame_stage);
		blendy_motion.y_animate = get_y_animate(blendy.frame_stage, blendy.going_up,player_blendy);
	}
}

void WorldSystem::update_boss_animation(float elapsed_ms_since_last_update) {

	if (registry.boss_spawned == false || registry.is_winning) return;
	
	Boss& final_boss = registry.boss.get(boss);
	
	Minion& boss_minion = registry.minions.get(boss);
	Motion& boss_motion = registry.motions.get(boss);
	boss_minion.up = false;
	boss_minion.down = false;
	boss_minion.left = false;
	boss_minion.right = false;
	if (boss_motion.velocity.x < 0 && boss_motion.velocity.x < -50) {
		boss_minion.right = true;
	}
	else if (boss_motion.velocity.x > 0 && boss_motion.velocity.x > 50) {
		boss_minion.left = true;
	}
	if (boss_motion.velocity.y > 0 && boss_motion.velocity.y > 50) {
		boss_minion.down = true;
	}
	else if (boss_motion.velocity.y < 0 && boss_motion.velocity.y < -50) {
		boss_minion.up = true;
	}
	
	boss_minion.counter_ms -= elapsed_ms_since_last_update;
	if (boss_minion.counter_ms < 0.f) {
		boss_minion.counter_ms = BLENDY_FRAME_DELAY;
		if (final_boss.going_up < 0) {
			boss_minion.frame_stage += 1;
			if (boss_minion.frame_stage > 3) {
				boss_minion.frame_stage = 3;
				final_boss.going_up = 1;
			}
		}
		else {
			boss_minion.frame_stage -= 1;
			if (boss_minion.frame_stage < 0) {
				boss_minion.frame_stage = 0;
				final_boss.going_up = -1;
			}
		}
	}
	// get what the render request status should be
	if (boss_motion.velocity.x == 0 && boss_motion.velocity.y == 0) {
		// just keep the current image
		if (!boss_minion.up && !boss_minion.down && !boss_minion.right && !boss_minion.left) {
			RenderRequest request = registry.renderRequests.get(boss);
			registry.renderRequests.remove(boss);
			registry.renderRequests.insert(
				boss,
				{ request.used_texture, request.used_normal_map,request.used_effect,request.used_geometry });
		}
		else {
			registry.renderRequests.remove(boss);
			get_minion_render_request(boss_minion.up, boss_minion.down, boss_minion.right, boss_minion.left, boss_minion.frame_stage,Enemy_TYPE::BOSS,boss);
		}
		final_boss.going_up = 1;
		boss_motion.y_animate = 0.f;
	}
	else {
		registry.renderRequests.remove(boss);
		get_minion_render_request(boss_minion.up, boss_minion.down, boss_minion.right, boss_minion.left, boss_minion.frame_stage, Enemy_TYPE::BOSS, boss);
		boss_motion.y_animate = get_y_animate(boss_minion.frame_stage, final_boss.going_up,boss);
	}

}

void WorldSystem::update_minion_animation(float elapsed_ms_since_last_update) {

	for (int j = 0; j < registry.minions.entities.size(); j++) {

		Minion& minion = registry.minions.get(registry.minions.entities[j]);
		Motion& minion_motion = registry.motions.get(registry.minions.entities[j]);
		if (minion.type == Enemy_TYPE::BOSS)
			continue;
		// get what the render request status should be
		// if minion is not moving, render original image
		if (minion_motion.velocity.x == 0 && minion_motion.velocity.y == 0) {
			// just keep the current image
			RenderRequest request = registry.renderRequests.get(registry.minions.entities[j]);
			registry.renderRequests.remove(registry.minions.entities[j]);
			registry.renderRequests.insert(
				registry.minions.entities[j],
				{ request.used_texture, request.used_normal_map,request.used_effect,request.used_geometry });
		}
		else {
			// minion is moving - calculate appropriate frame to put in render request
			registry.renderRequests.remove(registry.minions.entities[j]);
			get_minion_render_request(minion.up, minion.down, minion.right, minion.left, minion.frame_stage, minion.type, registry.minions.entities[j]);
		}
	}
}


void WorldSystem::shootGrapeBullets(RenderSystem* renderer, vec2 pos, vec2 velocity,float up_angle, float angle_diff,int type) {
	auto& blendy = registry.players.get(player_blendy);
	const int num_bullets = 12; 
	const float angle_increment = 2 * M_PI / num_bullets; // Angle increment for each bullet


	for (int i = 0; i < num_bullets; ++i) {
		// Calculate the angle for the current bullet
		float angle = i * angle_increment;

		// Calculate the velocity based on the angle
		vec2 velocity = { cos(angle) * bullet_speed, sin(angle) * bullet_speed };

		// calculate final angle for bullet
		float final_angle = up_angle + angle_diff + angle;

		create_bullet(renderer, pos, velocity, final_angle, type);
	}	
}




// Update our game world
vec2 WorldSystem::getCurrentMousePosition() {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos); 
	return vec2{ (float)xpos, (float)ypos }; 
}
void WorldSystem::update_bullets(float elapsed_ms_since_last_update) {

	Motion& motion = registry.motions.get(player_blendy);
	vec2& blendy_pos = motion.position;
	vec2 mouse_position = getCurrentMousePosition();
	auto& blendy = registry.players.get(player_blendy);
	if (!is_dead) {
		
			if (bullet_timer <= 0.0f) {
				vec2 bullet_direction = normalize(mouse_position - blendy_pos);
				vec2 up_vector{ 0.0f, -1.0f };
				float bullet_angle = std::atan2(bullet_direction.y, bullet_direction.x);
				float up_angle = std::atan2(up_vector.y, up_vector.x);
				float angle_diff = bullet_angle - up_angle;
				if (angle_diff < -M_PI) {
					angle_diff += 2 * M_PI;
				}
				else if (angle_diff > M_PI) {
					angle_diff -= 2 * M_PI;
				}

				if (blendy.pattern_type == 0) {
					create_bullet(renderer, blendy_pos, bullet_direction * bullet_speed, angle_diff, blendy.bullet_type);
					bullet_timer = bullet_launch_interval;
				}
				else if (blendy.pattern_type == 1) {
					angle_diff = -3.02989;
					shootGrapeBullets(renderer, blendy_pos, bullet_direction, angle_diff, up_angle, blendy.bullet_type);
					bullet_timer = bullet_launch_interval;
				}
				else {

					vec2 side_direction = vec2(-bullet_direction.y, bullet_direction.x); 
					create_bullet(renderer, blendy_pos, bullet_direction * bullet_speed, angle_diff, blendy.bullet_type);
					create_bullet(renderer, blendy_pos, (bullet_direction + side_direction * 0.2f) * bullet_speed, angle_diff, blendy.bullet_type);
					create_bullet(renderer, blendy_pos, (bullet_direction - side_direction * 0.2f) * bullet_speed, angle_diff, blendy.bullet_type);
					bullet_timer = bullet_launch_interval;
				}

				if (blendy.bullet_type != 0) {
					blendy.bullet_powerup_duration_ms -= elapsed_ms_since_last_update * current_speed;
				}
				if (blendy.bullet_powerup_duration_ms < 0) {
					blendy.bullet_type = 0;
				}

				if (blendy.pattern_type != 0) {
					blendy.pattern_powerup_duration_ms -= elapsed_ms_since_last_update * current_speed;
				}
				if (blendy.pattern_powerup_duration_ms < 0) {
					blendy.pattern_type = 0;
				}
				
			}
			else if (bullet_timer > 0.0f) {
				float multi_1 = 1;
				float multi_2 = 1;
				if (blendy.pattern_type == 1) {
					multi_1 = 0.6; 
				}
				if (blendy.pattern_type == 1) {
					multi_1 = 0.8;
				}

				if (blendy.protein_powerup_duration_ms > 0) {
					blendy.protein_powerup_duration_ms -= elapsed_ms_since_last_update * current_speed;;
					multi_2 = 3;
				}

				bullet_timer -= elapsed_ms_since_last_update / 1000.0f * multi_1 * multi_2;
				//bullet_timer -= elapsed_ms_since_last_update / 1000.0f;
			}

			//std::cout << "Blendy protein powerup duration: " << blendy.protein_powerup_duration_ms << std::endl;
			//std::cout << "Bullet_timer:" << bullet_timer << std::endl;
	}
	return;
}

bool WorldSystem::step(float elapsed_ms_since_last_update) {
	if (registry.is_winning)
	{
		cutscene_active = true;
		cutscene_stage = 9;
		handle_cutScenes();
	}

	for (Entity e : registry.panel.entities) {
		registry.remove_all_components_of(e);
	}

	if (registry.boss_spawned) {
		update_boss_animation(elapsed_ms_since_last_update);
	}
	update_minion_animation(elapsed_ms_since_last_update);
	update_fps(elapsed_ms_since_last_update);
	update_score();
	update_powerups(elapsed_ms_since_last_update);
	update_bullets(elapsed_ms_since_last_update);
	update_player_movement();
	update_game_music();

	auto& motions_registry = registry.motions;

	if (is_dead) {
		Motion& player_motion = registry.motions.get(player_blendy);
		float sec_passed = elapsed_ms_since_last_update / 1000;
		player_motion.velocity = player_motion.velocity*(1 - sec_passed) + dead_velocity * sec_passed;
		player_motion.angle = player_motion.angle * (1 - sec_passed) + dead_angle * sec_passed;
		player_motion.scale= player_motion.scale * (1 - sec_passed) + dead_scale * sec_passed;
	}

	spawn_minions(elapsed_ms_since_last_update);
	

	// BLENDY ANIMATION
	update_blendy_animation(elapsed_ms_since_last_update);




	// Processing the blendy state
	assert(registry.screenStates.components.size() <= 1);
    ScreenState &screen = registry.screenStates.components[0];

    float min_counter_ms = 3000.f;
	for (Entity entity : registry.deathTimers.entities) {
		// progress timer
		DeathTimer& counter = registry.deathTimers.get(entity);
		counter.counter_ms -= elapsed_ms_since_last_update;
		
		//
		if(counter.counter_ms < min_counter_ms){
		    min_counter_ms = counter.counter_ms;
		}

		// restart the game once the death timer expired
		if (counter.counter_ms < 0) {
			registry.deathTimers.remove(entity);
			screen.darken_screen_factor = 0;
			cutscene_stage = 5;
			cutscene_active = true;
            restart_game();
			return true;
		}
	}

	float min_particle_lifetime_ms = 3000.f;
	for (Entity entity : registry.emitterTimers.entities) {
		// progress timer
		EmitterTimer& counter = registry.emitterTimers.get(entity);
		counter.counter_ms -= elapsed_ms_since_last_update;

		if (counter.counter_ms < min_particle_lifetime_ms) {
			min_particle_lifetime_ms = counter.counter_ms;
		}

		// deletes an emitter once its timer has expired
		if (counter.counter_ms < 0) {
			registry.remove_all_components_of(entity);
		}
	}


	// reduce window brightness if any of the present chickens is dying
	screen.darken_screen_factor = 1 - min_counter_ms / 3000;
	health_bar_frame = createHealthBar(renderer, HEALTH_BAR_FRAME_POSITION, HEALTH_BAR_FRAME_BOUNDS);

	if (registry.score >= SECOND_CUT_SCORE && cutscene_stage == FIRST_CUT_SCENE_END) {
		cutscene_active = true;
		handle_cutScenes();
	}

	//if (registry.score >= THIRD_CUT_SCORE && cutscene_stage == SECOND_CUT_SCENE_END) {
	//	cutscene_active = true;
	//	handle_cutScenes();
	//}

	return true;
}
void WorldSystem::render_cursor() {
	vec2 mouse_position = getCurrentMousePosition();
	if (mouse_position.x > window_width_px || mouse_position.x < 0 || mouse_position.y > window_height_px || mouse_position.y < 0) {
		return;
	}
	registry.motions.get(cursor).position = mouse_position;
	registry.renderRequests.remove(cursor);
	registry.renderRequests.insert(
		cursor,
		{ TEXTURE_ASSET_ID::CURSOR,
			TEXTURE_ASSET_ID::TEXTURE_COUNT,
		 EFFECT_ASSET_ID::TEXTURED,
		GEOMETRY_BUFFER_ID::SPRITE });

}
// Reset the world state to its initial state
void WorldSystem::restart_game() {
	game_music_state = MusicState::Ordinary;

	// Halt any playing music
	Mix_HaltMusic();
	// Playing background music
	Mix_FadeInMusic(background_music, -1, 1000.f);

	fprintf(stderr, "Loaded music\n");

	// Debugging for memory/component leaks
	registry.list_all_components();
	printf("Restarting\n");

	// Reset the game speed
	current_speed = 1.f;

	// Remove all entities that we created
	// All that have a motion, we could also iterate over all bug, eagles, ... but that would be more cumbersome
	while (registry.motions.entities.size() > 0)
	    registry.remove_all_components_of(registry.motions.entities.back());

	// Debugging for memory/component leaks
	registry.list_all_components();

	is_dead = false;
	registry.is_dead = false;
	registry.boss_spawned = false;
	registry.is_winning = false;
	registry.score = 0;
	game_background = create_background(renderer, CENTER_OF_SCREEN, BACKGROUND_BOUNDS);
	player_blendy = create_blendy(renderer, BLENDY_START_POSITION, BLENDY_BOUNDS);
	update_health_bar();
	directional_light = create_directional_light(renderer, BOTTOM_RIGHT_OF_SCREEN_DIRECTIONAL_LIGHT, DIRECTIONAL_LIGHT_BOUNDS, CAMERA_POSITION);
	fps_counter = create_fps_counter(renderer, FPS_COUNTER_TRANSLATION_FROM_BOTTOM_LEFT_OF_SCREEN, FPS_COUNTER_SCALE, FPS_TEXT_COLOR);
	score_counter = create_score_counter(renderer, SCORE_COUNTER_TRANSLATION_FROM_BOTTOM_LEFT_OF_SCREEN, SCORE_COUNTER_SCALE, SCORE_TEXT_COLOR);

	registry.boss_spawned = false;

	test_particle_emitter = create_particle_emitter(CENTER_OF_SCREEN, BACKGROUND_BOUNDS, 2000.f, 30.f, RED, WHITE, 0.05f, 5);
	//test_particle_emitter_2 = create_particle_emitter(CENTER_OF_SCREEN, BACKGROUND_BOUNDS, 2000.f, 50.f, MAGENTA, RED, 0.25f, 10);
	cutscene_active == true;
	cursor = create_cursor(renderer, { window_width_px / 2,window_height_px / 2 });
	handle_cutScenes();
}
void WorldSystem::window_minimized_callback() {
	registry.is_minimized = true;
	Mix_PauseMusic();
}
void WorldSystem::window_unminimized_callback() {
	Mix_ResumeMusic();
	registry.is_minimized = false;
}
void WorldSystem::console_debug_fps()
{
	if (debugging.show_game_fps)
	{
		// std::cout << "FPS: " << fps << std::endl;
	}
}

void WorldSystem::update_fps(float elapsed_ms_since_last_update)
{
	if (debugging.show_game_fps)
	{
		frame_count++;
		time_accumulator += elapsed_ms_since_last_update;
		if (time_accumulator >= 1000.0f) {
			fps = frame_count * 1000.0f / time_accumulator;
			frame_count = 0;
			time_accumulator = 0.0f;

			auto& fps_component = registry.fpsCounters.get(fps_counter);
			fps_component.current_fps = fps;

			console_debug_fps();
		}
	}
}

void WorldSystem::update_score()
{
	auto& score_component = registry.scoreCounters.get(score_counter);
	score_component.current_score = registry.score;
}

void WorldSystem::hit_player(const int& damage) {
	if (!registry.deathTimers.has(player_blendy)) {
		auto& player = registry.players.get(player_blendy);

		if ((player.health - damage <= 0 && player.shield == 0) || damage == 1000) {
			player.health = 0;
			update_health_bar();
			is_dead = true;
			registry.is_dead = true;
			auto& motions_registry = registry.motions;
			Motion& motion = motions_registry.get(player_blendy);
			motion.velocity.x = 0;
			motion.velocity.y = 0;
			motion.angle = { 0.0f };
			registry.deathTimers.emplace(player_blendy);
			Mix_PlayChannel(-1, dead_sound, 0);
			Mix_FadeOutMusic(1500.f);
			Mix_HaltMusic();
		}
		else if (player.shield > 0) {
			player.shield--;
			update_health_bar();
		}
		else {
			player.shield = 0;
			update_health_bar();
			player.health -= damage;
			Mix_PlayChannel(-1, player_hurt, 0);
			update_health_bar();
		}
	}
}

void WorldSystem::hit_enemy(const Entity& target, const int& damage) {
	Minion& minion = registry.minions.get(target);
	//minion.health -= std::max((damage-minion.armor),1.f);
	auto& blendy = registry.players.get(player_blendy);

	// blendy has cactus powerup
	if (blendy.bullet_type != 1 && blendy.bullet_type != 0) {
		Mix_PlayChannel(-1, minion_hurt, 0);
		int new_damage = damage * 2;
		minion.health -= std::max((new_damage - minion.armor), 1.f);
	}
	// blendy does not have cactus powerup regular attack
	else {
		Mix_PlayChannel(-1, minion_hurt, 0);
		minion.health -= std::max((damage - minion.armor), 1.f);
	}

	if (minion.health <= 0) {
		registry.score += minion.score;
		Mix_PlayChannel(-1, minion_dead, 0);
		if (registry.boss.has(target)) {
			registry.is_winning = true;
		}
		/*if (registry.loots.has(target)) {
			create_shield_powerup(renderer, registry.motions.get(target).position, SHIELD_POWERUP_BOUNDS);
		}*/
		registry.remove_all_components_of(registry.Entity_Mesh_Entity.get(target));
		registry.remove_all_components_of(target);
	} else {
		Mix_PlayChannel(-1, minion_hurt, 0);


		if (blendy.bullet_type == 1) {
			minion.armor = 0;
		}
	}
	
}


// Compute collisions between entities
void WorldSystem::handle_collisions() {
	// Loop over all collisions detected by the physics system
	auto& collisionsRegistry = registry.collisions;
	for (uint i = 0; i < collisionsRegistry.components.size(); i++) {
		// The entity and its collider
		Entity entity = collisionsRegistry.entities[i];
		Entity entity_other = collisionsRegistry.components[i].other;

		if (registry.players.has(entity)) {
			if (registry.minions.has(entity_other)) {
				int damage = registry.minions.get(entity_other).damage;
				hit_player(damage);
				if (!registry.boss.has(entity_other)) {
					registry.remove_all_components_of(registry.Entity_Mesh_Entity.get(entity_other));
					registry.remove_all_components_of(entity_other);
				}
			}
			else if (registry.bullets.has(entity_other)) {
				if (!registry.bullets.get(entity_other).friendly) {
					int damage = registry.bullets.get(entity_other).damage;
					registry.remove_all_components_of(registry.Entity_Mesh_Entity.get(entity_other));
					registry.remove_all_components_of(entity_other);
					hit_player(damage);
				}
			}
			// Check blendy - collisions with power ups
			else if (registry.powerUps.has(entity_other)) {
				PowerUp powerup = registry.powerUps.get(entity_other);
				auto& blendy = registry.players.get(player_blendy);
				if (powerup.type == POWERUP_TYPE::BATTERY) {
					blendy.health = blendy.max_health;
					update_health_bar();
					Mix_PlayChannel(-1, powerup_pickup_battery, 0);
					registry.remove_all_components_of(registry.Entity_Mesh_Entity.get(entity_other));
					registry.remove_all_components_of(entity_other);
				}
				else if (powerup.type == POWERUP_TYPE::PROTEIN) {
					blendy.protein_powerup_duration_ms = 2500.f;
					registry.remove_all_components_of(entity_other);
				}
				else if (powerup.type == POWERUP_TYPE::GRAPE) {
					blendy.pattern_type = 1;
					blendy.pattern_powerup_duration_ms = 300.f;
					registry.remove_all_components_of(entity_other);
				}
				else if (powerup.type == POWERUP_TYPE::LEMON) {
					blendy.bullet_type = 1;
					blendy.bullet_powerup_duration_ms = 300.f;
					registry.remove_all_components_of(entity_other);
				}
				else if (powerup.type == POWERUP_TYPE::CHERRY) {
					blendy.pattern_type = 2;	
					blendy.pattern_powerup_duration_ms = 300.f;
					registry.remove_all_components_of(entity_other);
				}
				else if (powerup.type == POWERUP_TYPE::CACTUS) {
					blendy.bullet_type = 2;
					blendy.bullet_powerup_duration_ms = 300.f;
					registry.remove_all_components_of(entity_other);
					
				}
				else if (powerup.type == POWERUP_TYPE::SHIELD) {
					if (blendy.shield < blendy.max_shield) {
						blendy.shield += 1;
						update_health_bar();
						registry.remove_all_components_of(entity_other);
					}
					else if (blendy.shield == blendy.max_shield) {
							blendy.shield = blendy.max_shield;
							update_health_bar();
							registry.remove_all_components_of(entity_other);
					}
					//std::cout << "Blendy shield: " << blendy.shield << std::endl;
				}
				
			}
		}
		else if (registry.cleaners.has(entity)) {
			if (registry.powerUps.has(entity_other)) {
				if (registry.boss.has(entity)) {
					PowerUp& powerup = registry.powerUps.get(entity_other);
					Boss& boss = registry.boss.get(entity);
					Minion& m = registry.minions.get(entity);
					switch (powerup.type) {
					case POWERUP_TYPE::BATTERY:
						m.health += 200;
						if (m.health > m.max_health) m.health = m.max_health;
						break;
					case POWERUP_TYPE::SHIELD:
						m.health += 800;
						if (m.health > m.max_health) m.health = m.max_health;
						break;
					case POWERUP_TYPE::LEMON:
						boss.bstate = static_cast<Bullet_State>((int)powerup.type);
						boss.powerup_duration_ms = 100;
						break;
					case POWERUP_TYPE::CHERRY:
						boss.bstate = static_cast<Bullet_State>((int)powerup.type);
						boss.powerup_duration_ms = 100;
						break;
					case POWERUP_TYPE::GRAPE:
						boss.bstate = static_cast<Bullet_State>((int)powerup.type);
						boss.state = BossState::Shooting;
						boss.powerup_duration_ms = 300;
						break;
					case POWERUP_TYPE::	PROTEIN:
						boss.bstate = static_cast<Bullet_State>((int)powerup.type);
						boss.state = BossState::Shooting;
						boss.powerup_duration_ms = 130;
						break;
					case POWERUP_TYPE::CACTUS:
						boss.bstate = static_cast<Bullet_State>((int)powerup.type);
						boss.state = BossState::Shooting;
						boss.powerup_duration_ms = 200;
						break;
					default:
						break;
					}
				}
				registry.remove_all_components_of(entity_other);
				
			}
		}
			else if (registry.bullets.has(entity)) {
			
			auto& bullet = registry.bullets.get(entity);
			if (registry.minions.has(entity_other) && bullet.friendly) {
				int damage = registry.bullets.get(entity).damage;
				hit_enemy(entity_other, damage);
				registry.remove_all_components_of(registry.Entity_Mesh_Entity.get(entity));
				registry.remove_all_components_of(entity);
			}
		}
	}
	registry.collisions.clear();
}

// Should the game be over ?
bool WorldSystem::is_over() const {
	return bool(glfwWindowShouldClose(window));
}

void WorldSystem::handle_cutScenes()
{
	cutscene_stage++;
	registry.remove_all_components_of(current_cutscene);
	auto& score_component = registry.scoreCounters.get(score_counter);

	if (cutscene_stage == FIRST_CUT_SCENE_END || cutscene_stage == SECOND_CUT_SCENE_END) {
		// if reached the end of the cut scenes, resume gameplay
		cutscene_active = false;
		registry.is_pause = false;
		score_component.show = true;
		if (cutscene_stage == FIRST_CUT_SCENE_END) handle_help_screen();

	}
	else {
		registry.is_pause = true;
		current_cutscene = createCutScene(renderer, CENTER_OF_SCREEN, BACKGROUND_BOUNDS, cutscene_stage);
		score_component.show = false;
	}
}

void WorldSystem::handle_help_screen() {
	auto& score_component = registry.scoreCounters.get(score_counter);

	if (showHelpScreen) {
		registry.is_pause = true;
		help_screen = createHelpScreen(renderer, CENTER_OF_SCREEN, HELP_SCREEN_BOUNDS);
		score_component.show = false;
	}
	else {
		registry.is_pause = false;
		registry.remove_all_components_of(help_screen);
		score_component.show = true;
	}

	showHelpScreen = !showHelpScreen;
}

void WorldSystem::move_player(vec2 direction) {
	auto& motions_registry = registry.motions;
	Motion& player_motion = motions_registry.get(player_blendy);
	float& speed = registry.players.get(player_blendy).max_speed;
	player_motion.velocity.x = direction.x * speed;
	player_motion.velocity.y = direction.y * speed;
}



void WorldSystem::update_player_movement() {
	if (is_dead) return;

	Player& blendy = registry.players.get(player_blendy);

	vec2 direction = { 0, 0 };
	if (keyWPressed) direction.y -= 1;
	if (keySPressed) direction.y += 1;
	if (keyAPressed) direction.x -= 1;
	if (keyDPressed) direction.x += 1;

	// Normalize direction to avoid faster diagonal movement
	if (direction.x != 0 || direction.y != 0) {
		float length = sqrt(direction.x * direction.x + direction.y * direction.y);
		direction.x /= length;
		direction.y /= length;
		// BLENDY ANIMATION
		blendy.up = false;
		blendy.down = false;
		blendy.left = false;
		blendy.right = false;
		if (direction.x > 0) {
			// going right
			blendy.right = true;
		}
		else if (direction.x < 0) {
			// going left
			blendy.left = true;
		}
		if (direction.y > 0) {
			// going down
			blendy.down = true;
		}
		else if (direction.y < 0) {
			// going up
			blendy.up = true;
		}
	}
	else {
		blendy.frame_stage = 4;
	}
	move_player(direction);

	
}


void WorldSystem::handlePlayerMovement(int key, int action) {
	bool isPressed = (action == GLFW_PRESS || action == GLFW_REPEAT);

	switch (key) {
	case GLFW_KEY_W: keyWPressed = isPressed; break;
	case GLFW_KEY_S: keySPressed = isPressed; break;
	case GLFW_KEY_A: keyAPressed = isPressed; break;
	case GLFW_KEY_D: keyDPressed = isPressed; break;
	}
}

void WorldSystem::on_key(int key, int, int action, int mod) {
	if (!cutscene_active) {
		handlePlayerMovement(key, action);
	}
	

	// Toggle the help screen visibility when "H" is pressed
	if (action == GLFW_RELEASE && key == GLFW_KEY_H && !cutscene_active) {
		handle_help_screen();
	}

	// switch to next cutscene
	if (action == GLFW_RELEASE && key == GLFW_KEY_C && cutscene_active && cutscene_stage < 10 ) {
		handle_cutScenes();
	}


	//// Resetting game
	//if (action == GLFW_RELEASE && key == GLFW_KEY_R && !cutscene_active) {
	//	int w, h;
	//	glfwGetWindowSize(window, &w, &h);

	//	has_restarted = true;
 //       restart_game();
	//}

	if (action == GLFW_RELEASE && key == GLFW_KEY_R && cutscene_stage == 10) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);

		cutscene_stage = 5;
		restart_game();
	}

	// Debugging
	if (key == GLFW_KEY_D) {
		if (action == GLFW_RELEASE)
			debugging.in_debug_mode = false;
		else
			debugging.in_debug_mode = true;
	}

	// FPS Toggle
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		debugging.show_game_fps = !debugging.show_game_fps;
	}

	// Control the current speed with `<` `>`
	if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) && key == GLFW_KEY_COMMA) {
		current_speed -= 0.1f;
		printf("Current speed = %f\n", current_speed);
	}
	if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) && key == GLFW_KEY_PERIOD) {
		current_speed += 0.1f;
		printf("Current speed = %f\n", current_speed);
	}
	current_speed = fmax(0.f, current_speed);

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_MINUS) {
			float currentMusicVolume = Mix_VolumeMusic(-1);
			float dead_sound_volume = Mix_VolumeChunk(dead_sound, -1);
			float get_point_volume = Mix_VolumeChunk(get_point, -1);
			Mix_VolumeMusic(currentMusicVolume - 10);
			Mix_VolumeChunk(dead_sound, dead_sound_volume - 10);
			Mix_VolumeChunk(get_point, get_point_volume - 10);
		}
		else if (key == GLFW_KEY_EQUAL) {
			float currentMusicVolume = Mix_VolumeMusic(-1);
			float dead_sound_volume = Mix_VolumeChunk(dead_sound, -1);
			float get_point_volume = Mix_VolumeChunk(get_point, -1);
			Mix_VolumeMusic(currentMusicVolume + 10);
			Mix_VolumeChunk(dead_sound, dead_sound_volume + 10);
			Mix_VolumeChunk(get_point, get_point_volume + 10);
		}
	}
}

float WorldSystem::get_y_animate(int stage, int going_up, Entity entity) {
	if (registry.deathTimers.has(entity)) {
		return 0;
	}
	if (stage == 0) {
		return 0.f * going_up;
	}
	else if (stage == 1) {
		return 0.3f * going_up;
	}
	else if (stage == 2) {
		return 0.6f * going_up;
	}
	else if (stage == 3) {
		return 1.5f * going_up;
	}
	else if (stage == 4) {
		return 1.5f * going_up;
	}
	else {
		return 0.f * going_up;
	}
}

void WorldSystem::get_blendy_render_request(bool up, bool down, bool right, bool left, int stage) {
	// BLENDY ANIMATION
	if (up && right) {
		// going up
		if (stage == 0) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::BRT_0,
					TEXTURE_ASSET_ID::BRT_0_N,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 1) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::BRT_1,
					TEXTURE_ASSET_ID::BRT_1_N,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 2) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::BRT_2,
					TEXTURE_ASSET_ID::BRT_2_N,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 3 || stage == 4) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::BRT_3,
					TEXTURE_ASSET_ID::BRT_3_N,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
	}
	else if (down && right) {
		if (stage == 0) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::BRB_0,
					TEXTURE_ASSET_ID::BRB_0_N,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 1) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::BRB_1,
					TEXTURE_ASSET_ID::BRB_1_N,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 2) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::BRB_2,
					TEXTURE_ASSET_ID::BRB_2_N,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 3 || stage == 4) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::BRB_3,
					TEXTURE_ASSET_ID::BRB_3_N,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
	}
	else if (up && left) {
		if (stage == 0) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::BLT_0,
					TEXTURE_ASSET_ID::BLT_0_N,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 1) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::BLT_1,
					TEXTURE_ASSET_ID::BLT_1_N,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 2) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::BLT_2,
					TEXTURE_ASSET_ID::BLT_2_N,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 3 || stage == 4) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::BLT_3,
					TEXTURE_ASSET_ID::BLT_3_N,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
	}
	else if (down && left) {
		// going up
		if (stage == 0) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::BLB_0,
					TEXTURE_ASSET_ID::BLB_0_N,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 1) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::BLB_1,
					TEXTURE_ASSET_ID::BLB_1_N,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 2) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::BLB_2,
					TEXTURE_ASSET_ID::BLB_2_N,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 3 || stage == 4) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::BLB_3,
					TEXTURE_ASSET_ID::BLB_3_N,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
	}
	else if (up) {
		// going up
		if (stage == 0) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::UFRAME_0,
					TEXTURE_ASSET_ID::UFRAME_0_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 1) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::UFRAME_1,
					TEXTURE_ASSET_ID::UFRAME_1_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 2) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::UFRAME_2,
					TEXTURE_ASSET_ID::UFRAME_2_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 3 || stage == 4) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::UFRAME_3,
					TEXTURE_ASSET_ID::UFRAME_3_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
		}
	}
	else if (down) {
		// going down
		if (stage == 0) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::DFRAME_0,
					TEXTURE_ASSET_ID::DFRAME_0_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 1) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::DFRAME_1,
					TEXTURE_ASSET_ID::DFRAME_1_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 2) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::DFRAME_2,
					TEXTURE_ASSET_ID::DFRAME_2_NM,
					EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 3 || stage == 4) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::DFRAME_3,
					TEXTURE_ASSET_ID::DFRAME_3_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
		}
	}
	else if (right) {
		// going right
		if (stage == 0) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::RFRAME_0,
					TEXTURE_ASSET_ID::RFRAME_0_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 1) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::RFRAME_1,
					TEXTURE_ASSET_ID::RFRAME_1_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 2) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::RFRAME_2,
					TEXTURE_ASSET_ID::RFRAME_2_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 3 || stage == 4) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::RFRAME_3,
					TEXTURE_ASSET_ID::RFRAME_3_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
		}
	}
	else if (left) {
		// going left
		if (stage == 0) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::LFRAME_0,
					TEXTURE_ASSET_ID::LFRAME_0_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 1) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::LFRAME_1,
					TEXTURE_ASSET_ID::LFRAME_1_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 2) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::LFRAME_2,
					TEXTURE_ASSET_ID::LFRAME_2_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 3 || stage == 4) {
			registry.renderRequests.insert(
				player_blendy,
				{ TEXTURE_ASSET_ID::LFRAME_3,
					TEXTURE_ASSET_ID::LFRAME_3_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
		}
	}
}

void WorldSystem::get_minion_render_request(bool up, bool down, bool right, bool left, int stage, Enemy_TYPE type,Entity minion) {
	if (type == Enemy_TYPE::HEALER || type == Enemy_TYPE::ROAMER || type == Enemy_TYPE::SHOOTER || type == Enemy_TYPE::SNIPER) {
		TEXTURE_ASSET_ID texture = TEXTURE_ASSET_ID::HEALER_D0; // healer, roamer,shooter,sniper
		if (type == Enemy_TYPE::HEALER) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 0);
		}
		else if (type == Enemy_TYPE::ROAMER) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 24);
		}
		else if (type == Enemy_TYPE::SHOOTER) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 48);
		}
		else if (type == Enemy_TYPE::SNIPER) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 72);
		}
		// down, left, right, up
		if (up) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 18);
		}
		else if (down) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 0);
		}
		else if (left) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 6);
		}
		else if (right) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 12);
		}
		if (stage == 0) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 0);
		}
		else if (stage == 1) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 2);
		}
		else {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 4);
		}
		TEXTURE_ASSET_ID normal_map = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 1);
		registry.renderRequests.insert(
			minion,
			{ texture,
				normal_map,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });
		return;
	}
	if (type == Enemy_TYPE::BOSS) {
		TEXTURE_ASSET_ID texture = TEXTURE_ASSET_ID::BOSS_D0;
		if (down && left) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 32);
		}
		else if (down && right) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 40);
		}
		else if (up && left) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 48);
		}
		else if (up && right) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 56);
		}
		else if (down) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 0);
		}
		else if (left) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 8);
		}
		else if (right) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 16);
		}
		else if (up) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 24);
		}

		if (stage == 0) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 0);
		}
		else if (stage == 1) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 2);
		}
		else if (stage == 2) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 4);
		}
		else if (stage == 3) {
			texture = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 6);
		}
		TEXTURE_ASSET_ID normal_map = static_cast<TEXTURE_ASSET_ID>(static_cast<int>(texture) + 1);
		registry.renderRequests.insert(
			minion,
			{ texture,
				normal_map,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });

		return;
	}
	if (up) {
		// going up
		if (stage == 0) {
			registry.renderRequests.insert(
				minion,
				{ TEXTURE_ASSET_ID::MUP_0,
					TEXTURE_ASSET_ID::MUP_0_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 1) {
			registry.renderRequests.insert(
				minion,
				{ TEXTURE_ASSET_ID::MUP_1,
					TEXTURE_ASSET_ID::MUP_1_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 2) {
			registry.renderRequests.insert(
				minion,
				{ TEXTURE_ASSET_ID::MUP_2,
					TEXTURE_ASSET_ID::MUP_2_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
	}
	else if (down) {
		// going down
		if (stage == 0) {
			registry.renderRequests.insert(
				minion,
				{ TEXTURE_ASSET_ID::MDOWN_0,
				 TEXTURE_ASSET_ID::MDOWN_0_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 1) {
			registry.renderRequests.insert(
				minion,
				{ TEXTURE_ASSET_ID::MDOWN_1,
					TEXTURE_ASSET_ID::MDOWN_1_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 2) {
			registry.renderRequests.insert(
				minion,
				{ TEXTURE_ASSET_ID::MDOWN_2,
					TEXTURE_ASSET_ID::MDOWN_2_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
	}
	else if (right) {
		// going right
		if (stage == 0) {
			registry.renderRequests.insert(
				minion,
				{ TEXTURE_ASSET_ID::MRIGHT_0,
					TEXTURE_ASSET_ID::MRIGHT_0_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 1) {
			registry.renderRequests.insert(
				minion,
				{ TEXTURE_ASSET_ID::MRIGHT_1,
					TEXTURE_ASSET_ID::MRIGHT_1_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 2) {
			registry.renderRequests.insert(
				minion,
				{ TEXTURE_ASSET_ID::MRIGHT_2,
					TEXTURE_ASSET_ID::MRIGHT_2_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
	}
	else if (left) {
		// going left
		if (stage == 0) {
			registry.renderRequests.insert(
				minion,
				{ TEXTURE_ASSET_ID::MLEFT_0,
					TEXTURE_ASSET_ID::MLEFT_0_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 1) {
			registry.renderRequests.insert(
				minion,
				{ TEXTURE_ASSET_ID::MLEFT_1,
					TEXTURE_ASSET_ID::MLEFT_1_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
		else if (stage == 2) {
			registry.renderRequests.insert(
				minion,
				{ TEXTURE_ASSET_ID::MLEFT_2,
					TEXTURE_ASSET_ID::MLEFT_2_NM,
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE });
		}
	}
}




