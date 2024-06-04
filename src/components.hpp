#pragma once
#include "common.hpp"
#include <vector>
#include "particle_system.hpp"
#include <unordered_map>
#include "../ext/stb_image/stb_image.h"
#include <map>

struct Mesh;
enum class GEOMETRY_BUFFER_ID;
enum class EntityType {
	Generic,
	Player,
	Bullet,
	ENEMY,
	ALL
};

enum class POWERUP_TYPE {

	BATTERY = 0,
	PROTEIN = BATTERY + 1,
	GRAPE = PROTEIN + 1,
	LEMON = GRAPE + 1,
	CHERRY = LEMON + 1,
	CACTUS = CHERRY + 1,
	SHIELD = CACTUS + 1,
};

enum class Bullet_State {
	Default = 0,
	Protein = Default+ 1,
	Grape = Protein +1,
	Lemon = Grape + 1,
	Cherry = Lemon + 1,
	Cactus = Cherry +1,
};

enum class Sniper_State {
	Avoiding,
	Aiming,
	Shooting,
	Reloading
};

enum class Tank_state {
	defualt,
	protecting,
};

enum class Enemy_TYPE {
	BASIC = 0,
	SHOOTER = BASIC + 1,
	ROAMER = SHOOTER + 1,
	CHARGER = ROAMER + 1,
	HEALER = CHARGER + 1,
	GIANT = HEALER + 1,
	SNIPER = GIANT + 1,
	TANK = SNIPER + 1,
	CLEANER = TANK + 1,
	SPLIT_SHOOTER = CLEANER + 1,
	BOSS = SPLIT_SHOOTER + 1,
};

enum class Charger_State {
	Approaching = 0,
	Aiming = Approaching + 1,
	Charging = Aiming +1,
	Resting = Charging + 1,
};

enum class BossState {
	Default,
	Aiming,
	Charging,
	PowerUpSeeking,
	Shooting,
};

struct Boss {
	BossState state = BossState::Default;
	Bullet_State bstate = Bullet_State::Default;
	float aim_timer = 0;
	float shoot_interval_ms = 20.0f;
	float time_since_last_shot_ms = 0.0f;
	bool is_shooting = true;
	float powerup_duration_ms = 0.f;
	vec2 charge_direction;
	float rest_timer = 30;
	int isAngry = 0;
	int going_up = -1;
};

struct SplitShooter {

};

struct Loot {

};

struct Cleaner {

};

// A component to represent shield
struct Shield {

};

struct ToolTip {

};

struct Cursor {

};

enum class Direction {
	Up,
	Down,
	Left,
	Right
};
// Player component

struct Player {
	float max_speed = 310.f;
	int health = 200;
	int max_health = 200;
	int max_effect = 3;
	int current_effect = 0;
	int shield = 0;
	int max_shield = 3;
	int bullet_type = 0;
	int pattern_type = 0;
	float protein_powerup_duration_ms = 0.f;
	float pattern_powerup_duration_ms = 0.f;
	float bullet_powerup_duration_ms = 0.f;
	float counter_ms = 50.f;
	int frame_stage = 0;
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	int going_up = -1;
	float invisible_counter = 0.0f;
	float max_invisible_duration = 100.f;
	std::map<Direction, Mesh> meshes;

	//static const std::map<Direction, std::string> direction_mesh;


};

static const std::map<Direction, std::string> blendy_direction_mesh = {
	{Direction::Up, mesh_path("Blendy-up.obj")},
	{Direction::Down, mesh_path("Blendy-Reduced.obj")},
	{Direction::Left, mesh_path("Blendy-left.obj")},
	{Direction::Right, mesh_path("Blendy-right.obj")}
};

static const std::map<Direction, std::string> minion_direction_mesh = {
	{Direction::Up, mesh_path("Minion-Reduced.obj")},
	{Direction::Down, mesh_path("Minion-Reduced.obj")},
	{Direction::Left, mesh_path("minion-left.obj")},
	{Direction::Right, mesh_path("minion-right.obj")}
};

static const std::map<Direction, std::string> boss_direction_mesh = {
	{Direction::Up, mesh_path("boss_up.obj")},
	{Direction::Down, mesh_path("boss_down.obj")},
	{Direction::Left, mesh_path("boss_left.obj")},
	{Direction::Right, mesh_path("boss_right.obj")}
};



struct Mesh_entity {

};
struct Roamer {

};

struct Charger {
	Charger_State state = Charger_State::Approaching;
	float aim_timer = 0;
	vec2 charge_direction;
	float rest_timer = 0;
};

struct Healer {

};

struct Giant {

};

struct Tank {
	Tank_state state = Tank_state::defualt;
};

struct Protection {
	Entity link;
};

struct Sniper {
	Sniper_State state = Sniper_State::Avoiding;
	float aim_timer = 100.f;
};


struct Panel {

};

struct EnemyBullet{
	
};

struct Mesh_collision
{


};

struct PowerUp
{
	POWERUP_TYPE type;
	float duration_ms = 0.0f;
	int count = 0;
};

struct Minion
{
	int health = 50;
	int max_health = 50;
	int damage = 50;
	float speed = 100.f;
	float armor = 0;
	int score = 10;
	float counter_ms = 50.f;
	int frame_stage = 0;
	bool up;
	bool down;
	bool left;
	bool right;
	Enemy_TYPE type = Enemy_TYPE::BASIC;
};

struct Shooter {
	float shoot_interval_ms = 2000.0f; 
	float time_since_last_shot_ms = 0.0f;
};

struct Bullet
{
	bool friendly = true;
	int penetration = 1;
	int damage = 25;
};



struct Eatable
{
	int power_up_id = -1;
};

// All data relevant to the shape and motion of entities
struct Motion {
	vec2 position = { 0, 0 };
	float angle = 0;
	vec2 velocity = { 0, 0 };
	vec2 scale = { 10, 10 };
	float y_animate = 0.f;
	EntityType type = EntityType::Generic;

	
	bool operator==(const Motion& other) const {
		return position == other.position &&
			angle == other.angle &&
			velocity == other.velocity &&
			scale == other.scale;
	}
};

// Stucture to store collision information
struct Collision
{
	// Note, the first object is stored in the ECS container.entities
	Entity other; // the second object involved in the collision
	Collision(Entity& other) { this->other = other; };
};

// Data structure for toggling debug mode
struct Debug {
	bool in_debug_mode = 0;
	bool show_game_fps = 0;
	bool in_freeze_mode = 0;
};
extern Debug debugging;

// Sets the brightness of the screen
struct ScreenState
{
	float darken_screen_factor = -1;
};

// A component to represent a help screen
struct HelpScreen
{
	
};

// A component to represent cut scene
struct CutScene
{
	int stage = 0;
	// std::string text = "";
	std::string text[4] = { "", "", "" , ""};
	vec2 text_position[4] = { {0, 0},  {0, 0} , {0, 0} , {0,0} };
	vec3 text_color[4] = { {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} };
	float text_scale[4] = { 1.f, 1.f, 1.f, 1.f };
	float scale = 1;
};

// If an entity represents an FPS counter
struct FpsCounter
{
	unsigned int current_fps = 0;
	float scale = 1.f;
};

// If an entity represents a score counter
struct ScoreCounter
{
	unsigned int current_score = 0;
	float scale = 1.f;
	bool show = true;
};

// A struct to refer to debugging graphics in the ECS
struct DebugComponent
{
	// Note, an empty struct has size 1
};

// A timer that will be associated to dying chicken
struct DeathTimer
{
	float counter_ms = 2000;
};

// A timer that will be associated to a particle emitter
struct EmitterTimer
{
	float counter_ms = 2000;
};

// Single Vertex Buffer element for non-textured meshes (coloured.vs.glsl & chicken.vs.glsl)
struct ColoredVertex
{
	vec3 position;
	vec3 color;
	bool operator==(const ColoredVertex& other) const {
		return position == other.position && color == other.color;
	}
};

// Single Vertex Buffer element for textured sprites (textured.vs.glsl)
struct TexturedVertex
{
	vec3 position;
	vec2 texcoord;
};

// Mesh datastructure for storing vertex and index buffers
struct Mesh
{
	static bool loadFromOBJFile(std::string obj_path, std::vector<ColoredVertex>& out_vertices, std::vector<uint16_t>& out_vertex_indices, vec2& out_size);
	vec2 original_size = {1,1};
	std::vector<ColoredVertex> vertices;
	std::vector<uint16_t> vertex_indices;
};

// Background component for if an entity represents a background image
struct Background
{
	
};

// A component to represent entities that are particle emitters
struct ParticleEmitter
{
	Emitter emitter_instance;
	vec3 particle_start_color;
	vec3 particle_end_color;
	float particle_size;
};


// LightSource component for entities that represent a LightSource
struct LightSource
{
	vec3 light_color;

	// Parameter for Blinn-Phong
	float shininess;

	// Intensity of ambient light to add to scene b/w [0.0, 1.0]
	float ambientIntensity;

	// Z-depth of the light
	float z_depth;

	// Position of camera for doing lighting calculations
	vec3 camera_position;
};

struct box {
	vec2 center;
	vec2 scale;
};
/**
 * The following enumerators represent global identifiers refering to graphic
 * assets. For example TEXTURE_ASSET_ID are the identifiers of each texture
 * currently supported by the system.
 *
 * So, instead of referring to a game asset directly, the game logic just
 * uses these enumerators and the RenderRequest struct to inform the renderer
 * how to structure the next draw command.
 *
 * There are 2 reasons for this:
 *
 * First, game assets such as textures and meshes are large and should not be
 * copied around as this wastes memory and runtime. Thus separating the data
 * from its representation makes the system faster.
 *
 * Second, it is good practice to decouple the game logic from the render logic.
 * Imagine, for example, changing from OpenGL to Vulkan, if the game logic
 * depends on OpenGL semantics it will be much harder to do the switch than if
 * the renderer encapsulates all asset data and the game logic is agnostic to it.
 *
 * The final value in each enumeration is both a way to keep track of how many
 * enums there are, and as a default value to represent uninitialized fields.
 */
//
enum class TEXTURE_ASSET_ID {
	BLENDY = 0,
	BLENDY_NM = BLENDY + 1,
	MINION = BLENDY_NM + 1,
	MINION_NM = MINION + 1,
	BACKGROUND = MINION_NM + 1,
	DIRECTIONAL_LIGHT = BACKGROUND + 1,
	LFRAME_0 = DIRECTIONAL_LIGHT + 1,
	LFRAME_1 = LFRAME_0 + 1,
	LFRAME_2 = LFRAME_1 + 1,
	LFRAME_3 = LFRAME_2 + 1,
	RFRAME_0 = LFRAME_3 + 1,
	RFRAME_1 = RFRAME_0 + 1,
	RFRAME_2 = RFRAME_1 + 1,
	RFRAME_3 = RFRAME_2 + 1,
	DFRAME_0 = RFRAME_3 + 1,
	DFRAME_1 = DFRAME_0 + 1,
	DFRAME_2 = DFRAME_1 + 1,
	DFRAME_3 = DFRAME_2 + 1,
	UFRAME_0 = DFRAME_3 + 1,
	UFRAME_1 = UFRAME_0 + 1,
	UFRAME_2 = UFRAME_1 + 1,
	UFRAME_3 = UFRAME_2 + 1,
	LFRAME_0_NM = UFRAME_3 + 1,
	LFRAME_1_NM = LFRAME_0_NM + 1,
	LFRAME_2_NM = LFRAME_1_NM + 1,
	LFRAME_3_NM = LFRAME_2_NM + 1,
	RFRAME_0_NM = LFRAME_3_NM + 1,
	RFRAME_1_NM = RFRAME_0_NM + 1,
	RFRAME_2_NM = RFRAME_1_NM + 1,
	RFRAME_3_NM = RFRAME_2_NM + 1,
	DFRAME_0_NM = RFRAME_3_NM + 1,
	DFRAME_1_NM = DFRAME_0_NM + 1,
	DFRAME_2_NM = DFRAME_1_NM + 1,
	DFRAME_3_NM = DFRAME_2_NM + 1,
	UFRAME_0_NM = DFRAME_3_NM + 1,
	UFRAME_1_NM = UFRAME_0_NM + 1,
	UFRAME_2_NM = UFRAME_1_NM + 1,
	UFRAME_3_NM = UFRAME_2_NM + 1,
	BULLET = UFRAME_3_NM + 1,
	BULLET_NM = BULLET + 1,
	FULL_HEALTH_BAR = BULLET_NM + 1,
	HELP_SCREEN = FULL_HEALTH_BAR + 1,
	HEALTH_BAR_FRAME = HELP_SCREEN + 1,
	CUTSCENE_1_1 = HEALTH_BAR_FRAME + 1,
	CUTSCENE_1_2 = CUTSCENE_1_1 + 1,
	CUTSCENE_1_3 = CUTSCENE_1_2 + 1,
	CUTSCENE_1_4 = CUTSCENE_1_3 + 1,
	CUTSCENE_2_1 = CUTSCENE_1_4 + 1,
	CUTSCENE_2_2 = CUTSCENE_2_1 + 1,
	CUTSCENE_3_1 = CUTSCENE_2_2 + 1,
	START_SCREEN = CUTSCENE_3_1 + 1,
	MLEFT_0 = START_SCREEN + 1,
	MLEFT_1 = MLEFT_0 + 1,
	MLEFT_2 = MLEFT_1 + 1,
	MRIGHT_0 = MLEFT_2 + 1,
	MRIGHT_1 = MRIGHT_0 + 1,
	MRIGHT_2 = MRIGHT_1 + 1,
	MUP_0 = MRIGHT_2 + 1,
	MUP_1 = MUP_0 + 1,
	MUP_2 = MUP_1 + 1,
	MDOWN_0 = MUP_2 + 1,
	MDOWN_1 = MDOWN_0 + 1,
	MDOWN_2 = MDOWN_1 + 1,
	MLEFT_0_NM = MDOWN_2 + 1,
	MLEFT_1_NM = MLEFT_0_NM + 1,
	MLEFT_2_NM = MLEFT_1_NM + 1,
	MRIGHT_0_NM = MLEFT_2_NM + 1,
	MRIGHT_1_NM = MRIGHT_0_NM + 1,
	MRIGHT_2_NM = MRIGHT_1_NM + 1,
	MUP_0_NM = MRIGHT_2_NM + 1,
	MUP_1_NM = MUP_0_NM + 1,
	MUP_2_NM = MUP_1_NM + 1,
	MDOWN_0_NM = MUP_2_NM + 1,
	MDOWN_1_NM = MDOWN_0_NM + 1,
	MDOWN_2_NM = MDOWN_1_NM + 1,

	BATTERY_POWERUP = MDOWN_2_NM + 1,
	PACFRUIT_POWERUP = BATTERY_POWERUP + 1,
	LEMON_POWERUP = PACFRUIT_POWERUP + 1,
	GRAPE_POWERUP = LEMON_POWERUP + 1,
	PROTEIN_POWERUP = GRAPE_POWERUP + 1,
	BATTERY_POWERUP_NM = PROTEIN_POWERUP+ 1,
	PACFRUIT_POWERUP_NM = BATTERY_POWERUP_NM + 1,
	LEMON_POWERUP_NM = PACFRUIT_POWERUP_NM + 1,
	GRAPE_POWERUP_NM = LEMON_POWERUP_NM + 1,
	PROTEIN_POWERUP_NM = GRAPE_POWERUP_NM + 1,

	BRB_0 = PROTEIN_POWERUP_NM + 1,
	BRB_1 = BRB_0 + 1,
	BRB_2 = BRB_1 + 1,
	BRB_3 = BRB_2 + 1,
	BRT_0 = BRB_3 + 1,
	BRT_1 = BRT_0 + 1,
	BRT_2 = BRT_1 + 1,
	BRT_3 = BRT_2 + 1,
	BLB_0 = BRT_3 + 1,
	BLB_1 = BLB_0 + 1,
	BLB_2 = BLB_1 + 1,
	BLB_3 = BLB_2 + 1,
	BLT_0 = BLB_3 + 1,
	BLT_1 = BLT_0 + 1,
	BLT_2 = BLT_1 + 1,
	BLT_3 = BLT_2 + 1,

	BRB_0_N = BLT_3 + 1,
	BRB_1_N = BRB_0_N + 1,
	BRB_2_N = BRB_1_N + 1,
	BRB_3_N = BRB_2_N + 1,
	BRT_0_N = BRB_3_N + 1,
	BRT_1_N = BRT_0_N + 1,
	BRT_2_N = BRT_1_N + 1,
	BRT_3_N = BRT_2_N + 1,
	BLB_0_N = BRT_3_N + 1,
	BLB_1_N = BLB_0_N + 1,
	BLB_2_N = BLB_1_N + 1,
	BLB_3_N = BLB_2_N + 1,
	BLT_0_N = BLB_3_N + 1,
	BLT_1_N = BLT_0_N + 1,
	BLT_2_N = BLT_1_N + 1,
	BLT_3_N = BLT_2_N + 1,
	HEALER_D0 = BLT_3_N + 1,
	HEALER_D0_N = HEALER_D0 + 1,
	HEALER_D1 = HEALER_D0_N + 1,
	HEALER_D1_N = HEALER_D1 + 1,
	HEALER_D2 = HEALER_D1_N + 1,
	HEALER_D2_N = HEALER_D2 + 1,
	HEALER_L0 = HEALER_D2_N + 1,
	HEALER_L0_N = HEALER_L0 + 1,
	HEALER_L1 = HEALER_L0_N + 1,
	HEALER_L1_N = HEALER_L1 + 1,
	HEALER_L2 = HEALER_L1_N + 1,
	HEALER_L2_N = HEALER_L2 + 1,
	HEALER_R0 = HEALER_L2_N + 1,
	HEALER_R0_N = HEALER_R0 + 1,
	HEALER_R1 = HEALER_R0_N + 1,
	HEALER_R1_N = HEALER_R1 + 1,
	HEALER_R2 = HEALER_R1_N + 1,
	HEALER_R2_N = HEALER_R2 + 1,
	HEALER_U0 = HEALER_R2_N + 1,
	HEALER_U0_N = HEALER_U0 + 1,
	HEALER_U1 = HEALER_U0_N + 1,
	HEALER_U1_N = HEALER_U1 + 1,
	HEALER_U2 = HEALER_U1_N + 1,
	HEALER_U2_N = HEALER_U2 + 1,

	ROAMER_D0 = HEALER_U2_N + 1,
	ROAMER_D0_N = ROAMER_D0 + 1,
	ROAMER_D1 = ROAMER_D0_N + 1,
	ROAMER_D1_N = ROAMER_D1 + 1,
	ROAMER_D2 = ROAMER_D1_N + 1,
	ROAMER_D2_N = ROAMER_D2 + 1,
	ROAMER_L0 = ROAMER_D2_N + 1,
	ROAMER_L0_N = ROAMER_L0 + 1,
	ROAMER_L1 = ROAMER_L0_N + 1,
	ROAMER_L1_N = ROAMER_L1 + 1,
	ROAMER_L2 = ROAMER_L1_N + 1,
	ROAMER_L2_N = ROAMER_L2 + 1,
	ROAMER_R0 = ROAMER_L2_N + 1,
	ROAMER_R0_N = ROAMER_R0 + 1,
	ROAMER_R1 = ROAMER_R0_N + 1,
	ROAMER_R1_N = ROAMER_R1 + 1,
	ROAMER_R2 = ROAMER_R1_N + 1,
	ROAMER_R2_N = ROAMER_R2 + 1,
	ROAMER_U0 = ROAMER_R2_N + 1,
	ROAMER_U0_N = ROAMER_U0 + 1,
	ROAMER_U1 = ROAMER_U0_N + 1,
	ROAMER_U1_N = ROAMER_U1 + 1,
	ROAMER_U2 = ROAMER_U1_N + 1,
	ROAMER_U2_N = ROAMER_U2 + 1,


	SHOOTER_D0 = ROAMER_U2_N + 1,
	SHOOTER_D0_N = SHOOTER_D0 + 1,
	SHOOTER_D1 = SHOOTER_D0_N + 1,
	SHOOTER_D1_N = SHOOTER_D1 + 1,
	SHOOTER_D2 = SHOOTER_D1_N + 1,
	SHOOTER_D2_N = SHOOTER_D2 + 1,
	SHOOTER_L0 = SHOOTER_D2_N + 1,
	SHOOTER_L0_N = SHOOTER_L0 + 1,
	SHOOTER_L1 = SHOOTER_L0_N + 1,
	SHOOTER_L1_N = SHOOTER_L1 + 1,
	SHOOTER_L2 = SHOOTER_L1_N + 1,
	SHOOTER_L2_N = SHOOTER_L2 + 1,
	SHOOTER_R0 = SHOOTER_L2_N + 1,
	SHOOTER_R0_N = SHOOTER_R0 + 1,
	SHOOTER_R1 = SHOOTER_R0_N + 1,
	SHOOTER_R1_N = SHOOTER_R1 + 1,
	SHOOTER_R2 = SHOOTER_R1_N + 1,
	SHOOTER_R2_N = SHOOTER_R2 + 1,
	SHOOTER_U0 = SHOOTER_R2_N + 1,
	SHOOTER_U0_N = SHOOTER_U0 + 1,
	SHOOTER_U1 = SHOOTER_U0_N + 1,
	SHOOTER_U1_N = SHOOTER_U1 + 1,
	SHOOTER_U2 = SHOOTER_U1_N + 1,
	SHOOTER_U2_N = SHOOTER_U2 + 1,

	SNIPER_D0 = SHOOTER_U2_N + 1,
	SNIPER_D0_N = SNIPER_D0 + 1,
	SNIPER_D1 = SNIPER_D0_N + 1,
	SNIPER_D1_N = SNIPER_D1 + 1,
	SNIPER_D2 = SNIPER_D1_N + 1,
	SNIPER_D2_N = SNIPER_D2 + 1,
	SNIPER_L0 = SNIPER_D2_N + 1,
	SNIPER_L0_N = SNIPER_L0 + 1,
	SNIPER_L1 = SNIPER_L0_N + 1,
	SNIPER_L1_N = SNIPER_L1 + 1,
	SNIPER_L2 = SNIPER_L1_N + 1,
	SNIPER_L2_N = SNIPER_L2 + 1,
	SNIPER_R0 = SNIPER_L2_N + 1,
	SNIPER_R0_N = SNIPER_R0 + 1,
	SNIPER_R1 = SNIPER_R0_N + 1,
	SNIPER_R1_N = SNIPER_R1 + 1,
	SNIPER_R2 = SNIPER_R1_N + 1,
	SNIPER_R2_N = SNIPER_R2 + 1,
	SNIPER_U0 = SNIPER_R2_N + 1,
	SNIPER_U0_N = SNIPER_U0 + 1,
	SNIPER_U1 = SNIPER_U0_N + 1,
	SNIPER_U1_N = SNIPER_U1 + 1,
	SNIPER_U2 = SNIPER_U1_N + 1,
	SNIPER_U2_N = SNIPER_U2 + 1,

	CURSOR = SNIPER_U2_N + 1,
	CHERRY_POWERUP = CURSOR + 1,
	SHIELD_POWERUP = CHERRY_POWERUP + 1,
	CACTUS_POWERUP = SHIELD_POWERUP + 1,

	BOSS_D0 = CACTUS_POWERUP + 1,
	BOSS_D0_N = BOSS_D0 + 1,
	BOSS_D1 = BOSS_D0_N + 1,
	BOSS_D1_N = BOSS_D1 + 1,
	BOSS_D2 = BOSS_D1_N + 1,
	BOSS_D2_N = BOSS_D2 + 1,
	BOSS_D3 = BOSS_D2_N + 1,
	BOSS_D3_N = BOSS_D3 + 1,

	BOSS_L0 = BOSS_D3_N + 1,
	BOSS_L0_N = BOSS_L0 + 1,
	BOSS_L1 = BOSS_L0_N + 1,
	BOSS_L1_N = BOSS_L1 + 1,
	BOSS_L2 = BOSS_L1_N + 1,
	BOSS_L2_N = BOSS_L2 + 1,
	BOSS_L3 = BOSS_L2_N + 1,
	BOSS_L3_N = BOSS_L3 + 1,

	BOSS_R0 = BOSS_L3_N + 1,
	BOSS_R0_N = BOSS_R0 + 1,
	BOSS_R1 = BOSS_R0_N + 1,
	BOSS_R1_N = BOSS_R1 + 1,
	BOSS_R2 = BOSS_R1_N + 1,
	BOSS_R2_N = BOSS_R2 + 1,
	BOSS_R3 = BOSS_R2_N + 1,
	BOSS_R3_N = BOSS_R3 + 1,

	BOSS_U0 = BOSS_R3_N + 1,
	BOSS_U0_N = BOSS_U0 + 1,
	BOSS_U1 = BOSS_U0_N + 1,
	BOSS_U1_N = BOSS_U1 + 1,
	BOSS_U2 = BOSS_U1_N + 1,
	BOSS_U2_N = BOSS_U2 + 1,
	BOSS_U3 = BOSS_U2_N + 1,
	BOSS_U3_N = BOSS_U3 + 1,

	BOSS_DL0 = BOSS_U3_N + 1,
	BOSS_DL0_N = BOSS_DL0 + 1,
	BOSS_DL1 = BOSS_DL0_N + 1,
	BOSS_DL1_N = BOSS_DL1 + 1,
	BOSS_DL2 = BOSS_DL1_N + 1,
	BOSS_DL2_N = BOSS_DL2 + 1,
	BOSS_DL3 = BOSS_DL2_N + 1,
	BOSS_DL3_N = BOSS_DL3 + 1,

	BOSS_DR0 = BOSS_DL3_N + 1,
	BOSS_DR0_N = BOSS_DR0 + 1,
	BOSS_DR1 = BOSS_DR0_N + 1,
	BOSS_DR1_N = BOSS_DR1 + 1,
	BOSS_DR2 = BOSS_DR1_N + 1,
	BOSS_DR2_N = BOSS_DR2 + 1,
	BOSS_DR3 = BOSS_DR2_N + 1,
	BOSS_DR3_N = BOSS_DR3 + 1,

	BOSS_UL0 = BOSS_DR3_N + 1,
	BOSS_UL0_N = BOSS_UL0 + 1,
	BOSS_UL1 = BOSS_UL0_N + 1,
	BOSS_UL1_N = BOSS_UL1 + 1,
	BOSS_UL2 = BOSS_UL1_N + 1,
	BOSS_UL2_N = BOSS_UL2 + 1,
	BOSS_UL3 = BOSS_UL2_N + 1,
	BOSS_UL3_N = BOSS_UL3 + 1,

	BOSS_UR0 = BOSS_UL3_N + 1,
	BOSS_UR0_N = BOSS_UR0 + 1,
	BOSS_UR1 = BOSS_UR0_N + 1,
	BOSS_UR1_N = BOSS_UR1 + 1,
	BOSS_UR2 = BOSS_UR1_N + 1,
	BOSS_UR2_N = BOSS_UR2 + 1,
	BOSS_UR3 = BOSS_UR2_N + 1,
	BOSS_UR3_N = BOSS_UR3 + 1,

	CACTUS_BULLET = BOSS_UR3_N + 1,

	TEXTURE_COUNT = CACTUS_BULLET + 1
};
const int texture_count = (int)TEXTURE_ASSET_ID::TEXTURE_COUNT;

enum class EFFECT_ASSET_ID {
	COLOURED = 0,
	EGG = COLOURED + 1,
	CHICKEN = EGG + 1,
	TEXTURED = CHICKEN + 1,
	WIND = TEXTURED + 1,
	HEALTH_BAR = WIND + 1,
	PARTICLES = HEALTH_BAR + 1,
	EFFECT_COUNT = PARTICLES + 1
};

const int effect_count = (int)EFFECT_ASSET_ID::EFFECT_COUNT;

enum class GEOMETRY_BUFFER_ID {
	BLENDY = 0,
	SPRITE = BLENDY + 1,
	EGG = SPRITE + 1,
	DEBUG_LINE = EGG + 1,
	SCREEN_TRIANGLE = DEBUG_LINE + 1,
	GEOMETRY_COUNT = SCREEN_TRIANGLE + 1,
	MINION = GEOMETRY_COUNT + 1,
	BULLET = MINION + 1,
	MINION_BULLET = BULLET + 1,

	BLENDY_UP = MINION_BULLET + 1,
	BLENDY_DOWN = BLENDY_UP + 1,
	BLENDY_LEFT = BLENDY_DOWN + 1,
	BLENDY_RIGHT = BLENDY_LEFT + 1,
	GRAPE = BLENDY_RIGHT + 1,
	LEMON = GRAPE + 1,
	ORANGE = LEMON + 1,
	PROTEIN_POWER = ORANGE + 1,
	BATTERY = PROTEIN_POWER + 1,
	BOSS = BATTERY + 1
};
const int geometry_count = (int)GEOMETRY_BUFFER_ID::BOSS + 1;

struct RenderRequest {
	TEXTURE_ASSET_ID used_texture = TEXTURE_ASSET_ID::TEXTURE_COUNT;
	TEXTURE_ASSET_ID used_normal_map = TEXTURE_ASSET_ID::TEXTURE_COUNT;
	EFFECT_ASSET_ID used_effect = EFFECT_ASSET_ID::EFFECT_COUNT;
	GEOMETRY_BUFFER_ID used_geometry = GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;
};

