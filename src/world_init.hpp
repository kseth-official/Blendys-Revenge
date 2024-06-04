#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"

// ENTITY TEXTURE CONSTANTS
const float BLENDY_BB_WIDTH = 0.2f * 423.f;
const float BLENDY_BB_HEIGHT = 0.2F * 800.f;
const float MINION_BB_WIDTH = 0.2f * 346.f;
const float MINION_BB_HEIGHT = 0.2f * 593.f;
// const float BACKGROUND_BB_WIDTH = 0.62f * 2131.f;
// const float BACKGROUND_BB_HEIGHT = 0.62f * 1563.f;
const float BACKGROUND_BB_WIDTH = 1900.f * 0.9;
const float BACKGROUND_BB_HEIGHT = 1080.f * 0.9;
const float DIRECTIONAL_LIGHT_BB_WIDTH = 0.1f * 512.f;
const float DIRECTIONAL_LIGHT_BB_HEIGHT = 0.1f * 512.f;

Entity createLine(vec2 position, vec2 scale);

Entity createHelpScreen(RenderSystem* renderer, vec2 pos, vec2 bounds);

Entity createHealthBar(RenderSystem* renderer, vec2 pos, vec2 bounds);

Entity createBullet(RenderSystem* renderer, vec2 pos, vec2 velocity, float angle);

// the background
Entity create_background(RenderSystem* renderer, const vec2& position, const vec2& bounds);

// the player (Blendy)
Entity create_blendy(RenderSystem* renderer, const vec2& position, const vec2& bounds);

// the directional light for Blinn-Phong
Entity create_directional_light(RenderSystem* renderer, const vec2& position, const vec2& bounds, const vec3& camera_position);

// the fps counter on the screen
Entity create_fps_counter(RenderSystem* renderer, const vec2& position, const vec2& bounds, const vec3& color);

// the score counter on the screen
Entity create_score_counter(RenderSystem* renderer, const vec2& position, const vec2& bounds, const vec3& color);

Entity create_charger(RenderSystem* renderer, const vec2& position, const vec2& bounds);

// a particle emitter that emits particles
Entity create_particle_emitter(const vec2& position, const vec2& scale, const float& particle_lifetime, const float& centre_offset, const vec3& start_color, const vec3& end_color, const float& outward_velocity_magnitude, const float& particle_size);

// the minions
Entity create_minion(RenderSystem* renderer, const vec2& position, const vec2& bounds);

Entity create_battery_powerup(RenderSystem* renderer, const vec2& position, const vec2& bounds);

Entity create_dodger(RenderSystem* renderer, const vec2& position, const vec2& bounds);

Entity create_enemy_bullet(RenderSystem* renderer, vec2 pos, vec2 velocity, float angle, int damage = 25, vec3 color = { 0,40,0 });

Entity create_roamer(RenderSystem* renderer, const vec2& position, const vec2& bounds);
Entity create_cleaner(RenderSystem* renderer, const vec2& position, const vec2& bounds);
Entity create_sniper(RenderSystem* renderer, const vec2& position, const vec2& bounds);
Entity create_tank(RenderSystem* renderer, const vec2& position, const vec2& bounds);

Entity create_giant(RenderSystem* renderer, const vec2& position, const vec2& bounds, int score);

Entity create_protein_powerup(RenderSystem* renderer, const vec2& position, const vec2& bounds);

Entity create_fast_bullet(RenderSystem* renderer, vec2 pos, vec2 velocity, float angle);

Entity create_grape_powerup(RenderSystem* renderer, const vec2& position, const vec2& bounds);

Entity create_lemon_powerup(RenderSystem* renderer, const vec2& position, const vec2& bounds);

Entity create_cherry_powerup(RenderSystem* renderer, const vec2& position, const vec2& bounds);

Entity create_shield_powerup(RenderSystem* renderer, const vec2& position, const vec2& bounds);

Entity create_shield_health(RenderSystem* renderer, const vec2& position, const vec2& bounds);

Entity create_bullet(RenderSystem* renderer, vec2 pos, vec2 velocity, float angle, int type);

Entity create_lemon_bullet(RenderSystem* renderer, vec2 pos, vec2 velocity, float angle);

Entity create_cursor(RenderSystem* renderer, const vec2& position);
Entity create_split_shooter(RenderSystem* renderer, const vec2& position, const vec2& bounds);
Entity create_boss(RenderSystem* renderer, const vec2& position, const vec2& bounds);
Entity create_cactus_powerup(RenderSystem* renderer, const vec2& position, const vec2& bounds);
Entity create_cactus_bullet(RenderSystem* renderer, vec2 pos, vec2 velocity, float angle);

Entity create_mesh(RenderSystem* renderer, vec2 pos, vec2 velocity, vec2 scale, float angle, Entity object_entity, TEXTURE_ASSET_ID texture_id_one, TEXTURE_ASSET_ID texture_id_two, GEOMETRY_BUFFER_ID geometry_id, bool display);

Entity create_mesh(RenderSystem* renderer, vec2 pos, vec2 scale, float angle, Entity object_entity, TEXTURE_ASSET_ID texture_id_one, TEXTURE_ASSET_ID texture_id_two, GEOMETRY_BUFFER_ID geometry_id, bool display);


Entity createCutScene(RenderSystem* renderer, vec2 pos, vec2 bounds, int stage);

Entity create_orange_bullet(RenderSystem* renderer, vec2 pos, vec2 velocity, float angle);

