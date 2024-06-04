#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"
#include <iostream>
#include <random>

Entity createLine(vec2 position, vec2 scale)
{
	Entity entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT,
			TEXTURE_ASSET_ID::TEXTURE_COUNT,
			EFFECT_ASSET_ID::EGG,
			GEOMETRY_BUFFER_ID::DEBUG_LINE });

	// Create motion
	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = scale;

	registry.debugComponents.emplace(entity);
	return entity;
}

Entity create_particle_emitter(const vec2& position, const vec2& scale, const float& particle_lifetime, const float& centre_offset, const vec3& start_color, const vec3& end_color, const float& outward_velocity_magnitude, const float& particle_size)
{
	Entity entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT,
			TEXTURE_ASSET_ID::TEXTURE_COUNT,
			EFFECT_ASSET_ID::PARTICLES,
			GEOMETRY_BUFFER_ID::SCREEN_TRIANGLE });

	// Create motion
	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = scale;

	ParticleEmitter& particle_emitter = registry.particleEmitters.emplace(entity);
	particle_emitter.emitter_instance.init(position, particle_lifetime, centre_offset, outward_velocity_magnitude);
	particle_emitter.particle_start_color = start_color;
	particle_emitter.particle_end_color = end_color;
	particle_emitter.particle_size = particle_size;

	EmitterTimer& emitter_timer = registry.emitterTimers.emplace(entity);
	emitter_timer.counter_ms = particle_lifetime;

	return entity;
}


Entity createHelpScreen(RenderSystem* renderer, vec2 pos, vec2 bounds)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = bounds;

	registry.helpScreens.emplace(entity);
	// Create a render request for the 
	// 
	// 
	// 
	// 
	//  Screen
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::HELP_SCREEN,
			TEXTURE_ASSET_ID::TEXTURE_COUNT,
		EFFECT_ASSET_ID::TEXTURED,
		GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}


Entity createHealthBar(RenderSystem* renderer, vec2 pos, vec2 bounds)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = bounds;

	// add health bar to panel component
	registry.panel.emplace(entity);

	// Create a render request for the health bar
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::HEALTH_BAR_FRAME,
			TEXTURE_ASSET_ID::TEXTURE_COUNT,
		EFFECT_ASSET_ID::TEXTURED,
		GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity create_shield_health(RenderSystem* renderer, const vec2& position, const vec2& bounds)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = bounds;

	registry.shields.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::SHIELD_POWERUP,
			TEXTURE_ASSET_ID::TEXTURE_COUNT,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createBullet(RenderSystem* renderer, vec2 pos, vec2 velocity, float angle) {
	auto entity = Entity();
	// Store a reference to the potentially re-used mesh object, like createChicken
	//Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::BULLET);
	//registry.meshPtrs.emplace(entity, &mesh);
	registry.mesh_collision.emplace(entity);
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = angle;
	motion.velocity = velocity;
	// Vicky M1: scale could change after render decided 
	motion.scale = vec2(100.0f, 100.0f);
	registry.bullets.emplace(entity);

	create_mesh(renderer, pos, velocity, { 0.4 * motion.scale.x, 0.8 * motion.scale.y }, angle, entity, TEXTURE_ASSET_ID::BULLET, TEXTURE_ASSET_ID::BULLET_NM, GEOMETRY_BUFFER_ID::BULLET, false);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BULLET,
			TEXTURE_ASSET_ID::BULLET_NM,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	return entity;
}



Entity create_background(RenderSystem* renderer, const vec2& position, const vec2& bounds)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;

	motion.velocity = { 0.f, 0.f };
	motion.position = position;


	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ bounds.x, bounds.y });

	// Create an (empty) background
	registry.backgrounds.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BACKGROUND,
			TEXTURE_ASSET_ID::TEXTURE_COUNT,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}


Entity create_blendy(RenderSystem* renderer, const vec2& position, const vec2& bounds)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::BLENDY);
	registry.mesh_collision.emplace(entity);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -bounds.x, bounds.y });
	
	// Create an (empty) Blendy component to be able to refer to Blendy
	registry.players.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BLENDY,
			TEXTURE_ASSET_ID::BLENDY_NM,
		 EFFECT_ASSET_ID::TEXTURED,
		GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity create_directional_light(RenderSystem* renderer, const vec2& position, const vec2& bounds, const vec3& camera_position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -bounds.x, bounds.y });

	// Create a directional light
	auto& directional_light = registry.lightSources.emplace(entity);
	directional_light.light_color = { 1.0f,1.0f,1.0f };
	directional_light.shininess = 4.f;
	directional_light.ambientIntensity = 0.00f;
	directional_light.z_depth = 500.f;
	directional_light.camera_position = camera_position;

	renderer->setDirectionalLight(entity);

	return entity;
}

Entity create_fps_counter(RenderSystem* renderer, const vec2& position, const vec2& bounds, const vec3& color)
{
	auto entity = Entity();

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -bounds.x, bounds.y });

	// Create an Fps Counter component to be able to refer to Blendy
	registry.fpsCounters.emplace(entity);
	auto& fps_text_color = registry.colors.emplace(entity);
	fps_text_color = color;

	return entity;
}

Entity create_score_counter(RenderSystem* renderer, const vec2& position, const vec2& bounds, const vec3& color)
{
	auto entity = Entity();

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -bounds.x, bounds.y });

	// Create an score counter to display the game score
	registry.scoreCounters.emplace(entity);
	auto& fps_text_color = registry.colors.emplace(entity);
	fps_text_color = color;

	return entity;
}

Entity create_minion(RenderSystem* renderer, const vec2& position, const vec2& bounds)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	//Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MINION);
	registry.mesh_collision.emplace(entity);

	//registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;
	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -bounds.x, bounds.y });
	create_mesh(renderer, motion.position, motion.velocity, motion.scale, motion.angle, entity, TEXTURE_ASSET_ID::MINION, TEXTURE_ASSET_ID::MINION_NM, GEOMETRY_BUFFER_ID::MINION, false);

	// Create and (empty) Minion component to be able to refer to all minions
	registry.minions.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MINION,
			TEXTURE_ASSET_ID::MINION_NM,
		 EFFECT_ASSET_ID::TEXTURED,
		GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity create_giant(RenderSystem* renderer, const vec2& position, const vec2& bounds, int score)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	//Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MINION);
	registry.mesh_collision.emplace(entity);

	//registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;
	motion.scale = vec2({ -bounds.x, bounds.y });
	auto& minion = registry.minions.emplace(entity);
	minion.score = 200;
	minion.health = score / 7;
	minion.max_health = score / 7;
	minion.speed = 50;
	create_mesh(renderer, position, motion.velocity, motion.scale, motion.angle, entity, TEXTURE_ASSET_ID::MINION, TEXTURE_ASSET_ID::MINION_NM, GEOMETRY_BUFFER_ID::MINION, false);
	registry.loots.emplace(entity);
	registry.giants.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MINION,
			TEXTURE_ASSET_ID::MINION_NM,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}




Entity create_battery_powerup(RenderSystem* renderer, const vec2& position, const vec2& bounds)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	//Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MINION);
	//registry.meshPtrs.emplace(entity, &mesh);
	registry.mesh_collision.emplace(entity);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 25.f;
	motion.velocity = { 0, 0 };
	motion.position = position;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -bounds.x, bounds.y });

	create_mesh(renderer, motion.position, motion.scale, 50.f, entity, TEXTURE_ASSET_ID::BATTERY_POWERUP, TEXTURE_ASSET_ID::TEXTURE_COUNT, GEOMETRY_BUFFER_ID::BATTERY, false);
	// Create and (empty) powerup component to be able to refer to all minions
	auto& powerup = registry.powerUps.emplace(entity);
	powerup.type = POWERUP_TYPE::BATTERY;
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BATTERY_POWERUP,
			TEXTURE_ASSET_ID::TEXTURE_COUNT,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity create_protein_powerup(RenderSystem* renderer, const vec2& position, const vec2& bounds)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	//Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MINION);
	//registry.meshPtrs.emplace(entity, &mesh);
	registry.mesh_collision.emplace(entity);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 25.f;
	motion.velocity = { 0, 0 };
	motion.position = position;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -bounds.x, bounds.y });
	create_mesh(renderer, motion.position, motion.scale, motion.angle, entity, TEXTURE_ASSET_ID::PROTEIN_POWERUP, TEXTURE_ASSET_ID::TEXTURE_COUNT, GEOMETRY_BUFFER_ID::PROTEIN_POWER, false);

	// Create and (empty) powerup component to be able to refer to all minions
	auto& powerup = registry.powerUps.emplace(entity);
	powerup.type = POWERUP_TYPE::PROTEIN;
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::PROTEIN_POWERUP,
			TEXTURE_ASSET_ID::TEXTURE_COUNT,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity create_grape_powerup(RenderSystem* renderer, const vec2& position, const vec2& bounds)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	//Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MINION);
	//registry.meshPtrs.emplace(entity, &mesh);
	registry.mesh_collision.emplace(entity);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -bounds.x, bounds.y });
	create_mesh(renderer, motion.position, motion.scale, motion.angle, entity, TEXTURE_ASSET_ID::GRAPE_POWERUP, TEXTURE_ASSET_ID::TEXTURE_COUNT, GEOMETRY_BUFFER_ID::GRAPE, false);

	// Create and (empty) powerup component to be able to refer to all minions
	auto& powerup = registry.powerUps.emplace(entity);
	powerup.type = POWERUP_TYPE::GRAPE;
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::GRAPE_POWERUP,
			TEXTURE_ASSET_ID::TEXTURE_COUNT,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity create_lemon_powerup(RenderSystem* renderer, const vec2& position, const vec2& bounds)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MINION);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.mesh_collision.emplace(entity);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 25.f;
	motion.velocity = { 0, 0 };
	motion.position = position;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -bounds.x, bounds.y });
	create_mesh(renderer, motion.position, motion.scale, motion.angle, entity, TEXTURE_ASSET_ID::LEMON_POWERUP, TEXTURE_ASSET_ID::TEXTURE_COUNT, GEOMETRY_BUFFER_ID::LEMON, false);

	// Create and (empty) powerup component to be able to refer to all minions
	auto& powerup = registry.powerUps.emplace(entity);
	powerup.type = POWERUP_TYPE::LEMON;
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::LEMON_POWERUP,
			TEXTURE_ASSET_ID::TEXTURE_COUNT,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity create_cherry_powerup(RenderSystem* renderer, const vec2& position, const vec2& bounds)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MINION);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.mesh_collision.emplace(entity);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -bounds.x, bounds.y });
	// Create and (empty) powerup component to be able to refer to all minions
	auto& powerup = registry.powerUps.emplace(entity);
	powerup.type = POWERUP_TYPE::CHERRY;

	create_mesh(renderer, motion.position, motion.scale, motion.angle, entity, TEXTURE_ASSET_ID::LEMON_POWERUP, TEXTURE_ASSET_ID::TEXTURE_COUNT, GEOMETRY_BUFFER_ID::LEMON, false);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::CHERRY_POWERUP,
			TEXTURE_ASSET_ID::TEXTURE_COUNT,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity create_shield_powerup(RenderSystem* renderer, const vec2& position, const vec2& bounds)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MINION);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.mesh_collision.emplace(entity);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -bounds.x, bounds.y });
	// Create and (empty) powerup component to be able to refer to all minions
	auto& powerup = registry.powerUps.emplace(entity);
	powerup.type = POWERUP_TYPE::SHIELD;
	create_mesh(renderer, motion.position, motion.velocity, motion.scale, motion.angle, entity, TEXTURE_ASSET_ID::BATTERY_POWERUP, TEXTURE_ASSET_ID::BATTERY_POWERUP_NM, GEOMETRY_BUFFER_ID::BATTERY, false);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::SHIELD_POWERUP,
			TEXTURE_ASSET_ID::TEXTURE_COUNT,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity create_cactus_powerup(RenderSystem* renderer, const vec2& position, const vec2& bounds)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MINION);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.mesh_collision.emplace(entity);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -bounds.x, bounds.y });
	// Create and (empty) powerup component to be able to refer to all minions
	auto& powerup = registry.powerUps.emplace(entity);
	powerup.type = POWERUP_TYPE::CACTUS;
	create_mesh(renderer, motion.position, motion.velocity, motion.scale, motion.angle, entity, TEXTURE_ASSET_ID::BATTERY_POWERUP, TEXTURE_ASSET_ID::BATTERY_POWERUP_NM, GEOMETRY_BUFFER_ID::BATTERY, false);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::CACTUS_POWERUP,
			TEXTURE_ASSET_ID::TEXTURE_COUNT,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}
Entity create_lemon_bullet(RenderSystem* renderer, vec2 pos, vec2 velocity, float angle) {
	auto entity = Entity();
	// Store a reference to the potentially re-used mesh object, like createChicken
	//Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::BULLET);
	//registry.meshPtrs.emplace(entity, &mesh);

	registry.mesh_collision.emplace(entity);
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = angle;
	motion.velocity = velocity;
	motion.scale = vec2(100.0f, 100.0f);
	auto& bullet = registry.bullets.emplace(entity);

	vec3 color = { 40,40,0 };
	registry.colors.insert(entity, color);
	create_mesh(renderer, pos, velocity, { 0.4 * motion.scale.x, 0.8 * motion.scale.y }, angle, entity, TEXTURE_ASSET_ID::BULLET, TEXTURE_ASSET_ID::BULLET_NM, GEOMETRY_BUFFER_ID::BULLET, false);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BULLET,
			TEXTURE_ASSET_ID::BULLET_NM,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	return entity;
}



Entity create_fast_bullet(RenderSystem* renderer, vec2 pos, vec2 velocity, float angle) {
	auto entity = Entity();
	// Store a reference to the potentially re-used mesh object, like createChicken
	//Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::BULLET);
	//registry.meshPtrs.emplace(entity, &mesh);

	registry.mesh_collision.emplace(entity);
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = angle;
	motion.velocity = velocity;
	motion.scale = vec2(100.0f, 100.0f);
	auto& bullet = registry.bullets.emplace(entity);

	vec3 color = { 40,0,0 };
	registry.colors.insert(entity, color);
	create_mesh(renderer, pos, velocity, { 0.4 * motion.scale.x, 0.8 * motion.scale.y }, angle, entity, TEXTURE_ASSET_ID::BULLET, TEXTURE_ASSET_ID::BULLET_NM, GEOMETRY_BUFFER_ID::BULLET, false);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BULLET,
			TEXTURE_ASSET_ID::BULLET_NM,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	return entity;
}

Entity create_cactus_bullet(RenderSystem* renderer, vec2 pos, vec2 velocity, float angle) {
	auto entity = Entity();
	// Store a reference to the potentially re-used mesh object, like createChicken
	//Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::BULLET);
	//registry.meshPtrs.emplace(entity, &mesh);

	registry.mesh_collision.emplace(entity);
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = angle;
	motion.velocity = velocity;
	motion.scale = vec2(50.f, 50.f);
	auto& bullet = registry.bullets.emplace(entity);

	create_mesh(renderer, pos, velocity, { 0.4 * motion.scale.x, 0.8 * motion.scale.y }, angle, entity, TEXTURE_ASSET_ID::BULLET, TEXTURE_ASSET_ID::BULLET_NM, GEOMETRY_BUFFER_ID::BULLET, false);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::CACTUS_BULLET,
			TEXTURE_ASSET_ID::BULLET_NM,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	return entity;
}

Entity create_orange_bullet(RenderSystem* renderer, vec2 pos, vec2 velocity, float angle) {
	auto entity = Entity();
	// Store a reference to the potentially re-used mesh object, like createChicken
	//Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::BULLET);
	//registry.meshPtrs.emplace(entity, &mesh);

	registry.mesh_collision.emplace(entity);
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = angle;
	motion.velocity = velocity;
	motion.scale = vec2(100.0f, 100.0f);
	auto& bullet = registry.bullets.emplace(entity);

	vec3 color = { 3,1,0 };
	registry.colors.insert(entity, color);
	create_mesh(renderer, pos, velocity, { 0.4 * motion.scale.x, 0.8 * motion.scale.y }, angle, entity, TEXTURE_ASSET_ID::BULLET, TEXTURE_ASSET_ID::BULLET_NM, GEOMETRY_BUFFER_ID::BULLET, false);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BULLET,
			TEXTURE_ASSET_ID::BULLET_NM,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	return entity;
}

Entity create_dodger(RenderSystem* renderer, const vec2& position, const vec2& bounds) {
	auto entity = Entity();


	//Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MINION);
	registry.mesh_collision.emplace(entity);
	//registry.meshPtrs.emplace(entity, &mesh);
	auto& motion = registry.motions.emplace(entity);
	auto& minion = registry.minions.emplace(entity);
	minion.type = Enemy_TYPE::SHOOTER;
	minion.score = 25;
	motion.angle = 0.f;
	motion.velocity = { 0, 0.f };
	motion.position = position;
	motion.scale = vec2({ -bounds.x, bounds.y });
	/*vec3 color = { 0.8,0.8,0.0 };
	registry.colors.insert(entity, color);*/
	create_mesh(renderer, position, motion.velocity, motion.scale, motion.angle, entity, TEXTURE_ASSET_ID::MINION, TEXTURE_ASSET_ID::MINION_NM, GEOMETRY_BUFFER_ID::MINION, false);


	auto& dodger = registry.shooters.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MINION,
		  TEXTURE_ASSET_ID::MINION_NM,
		  EFFECT_ASSET_ID::TEXTURED,
		GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity create_split_shooter(RenderSystem* renderer, const vec2& position, const vec2& bounds) {
	auto entity = Entity();


	registry.mesh_collision.emplace(entity);
	auto& motion = registry.motions.emplace(entity);
	auto& minion = registry.minions.emplace(entity);
	minion.type = Enemy_TYPE::SPLIT_SHOOTER;
	minion.score = 40;
	motion.angle = 0.f;
	motion.velocity = { 0, 0.f };
	motion.position = position;
	motion.scale = vec2({ -bounds.x, bounds.y });

	create_mesh(renderer, position, motion.velocity, motion.scale, motion.angle, entity, TEXTURE_ASSET_ID::MINION, TEXTURE_ASSET_ID::MINION_NM, GEOMETRY_BUFFER_ID::MINION, false);
	auto& dodger = registry.shooters.emplace(entity);
	registry.splitShooters.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MINION,
		  TEXTURE_ASSET_ID::MINION_NM,
		  EFFECT_ASSET_ID::TEXTURED,
		GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity create_charger(RenderSystem* renderer, const vec2& position, const vec2& bounds) {
	auto entity = Entity();

	//Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MINION);
	registry.mesh_collision.emplace(entity);
	//registry.meshPtrs.emplace(entity, &mesh);
	auto& motion = registry.motions.emplace(entity);
	auto& minion = registry.minions.emplace(entity);
	minion.type = Enemy_TYPE::CHARGER;
	minion.score = 40;
	motion.angle = 0.f;
	minion.health = 40;
	minion.max_health = 40;
	minion.armor = 15;
	motion.velocity = { 0, 0.f };
	motion.position = position;
	motion.scale = vec2({ -bounds.x, bounds.y });
	vec3 color = { 0.5,0.2,0.2 };
	registry.colors.insert(entity, color);
	create_mesh(renderer, position, motion.velocity, motion.scale, motion.angle, entity, TEXTURE_ASSET_ID::MINION, TEXTURE_ASSET_ID::MINION_NM, GEOMETRY_BUFFER_ID::MINION, false);

	auto& charger = registry.chargers.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MINION,
		  TEXTURE_ASSET_ID::MINION_NM,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity create_boss(RenderSystem* renderer, const vec2& position, const vec2& bounds) {
	auto entity = Entity();

	//Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MINION);
	registry.mesh_collision.emplace(entity);
	//registry.meshPtrs.emplace(entity, &mesh);
	auto& motion = registry.motions.emplace(entity);
	auto& minion = registry.minions.emplace(entity);
	auto& cleaner = registry.cleaners.emplace(entity);

	auto& boss = registry.boss.emplace(entity);
	minion.type = Enemy_TYPE::BOSS;
	minion.score = 10000;
	motion.angle = 0.f;
	minion.damage = 1000;
	minion.health = 2500;
	minion.max_health = 2500;
	minion.armor = 0;
	minion.speed = 200;
	motion.velocity = { 0, 0.f };
	motion.position = position;
	motion.scale = vec2({ -bounds.x, bounds.y });
	/*vec3 color = { 0.5,0.2,0.2 };
	registry.colors.insert(entity, color);*/

	create_mesh(renderer, position, motion.velocity, {motion.scale.x * 1.5, motion.scale.y * 1.5}, motion.angle, entity, TEXTURE_ASSET_ID::BOSS_D0, TEXTURE_ASSET_ID::BOSS_D0_N, GEOMETRY_BUFFER_ID::BOSS, false);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BOSS_D0,
		  TEXTURE_ASSET_ID::BOSS_D0_N,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity create_tank(RenderSystem* renderer, const vec2& position, const vec2& bounds)
{
	auto entity = Entity();

	//Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MINION);
	registry.mesh_collision.emplace(entity);
	//registry.meshPtrs.emplace(entity, &mesh);
	auto& motion = registry.motions.emplace(entity);
	auto& minion = registry.minions.emplace(entity);
	minion.type = Enemy_TYPE::TANK;
	minion.score = 80;
	minion.armor = 20;
	motion.angle = 0.f;
	minion.health = 100;
	minion.max_health = minion.health;
	minion.speed = 50;
	motion.velocity = { 0, 0.f };
	motion.position = position;
	motion.scale = vec2({ -bounds.x, bounds.y });
	vec3 color = { 0,0,0 };
	registry.colors.insert(entity, color);
	create_mesh(renderer, position, motion.velocity, motion.scale, motion.angle, entity, TEXTURE_ASSET_ID::MINION, TEXTURE_ASSET_ID::MINION_NM, GEOMETRY_BUFFER_ID::MINION, false);
	auto& tank = registry.tanks.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MINION,
		  TEXTURE_ASSET_ID::MINION_NM,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity create_sniper(RenderSystem* renderer, const vec2& position, const vec2& bounds)
{
	auto entity = Entity();
	//Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MINION);
	registry.mesh_collision.emplace(entity);
	//registry.meshPtrs.emplace(entity, &mesh);
	auto& motion = registry.motions.emplace(entity);
	auto& minion = registry.minions.emplace(entity);
	minion.type = Enemy_TYPE::SNIPER;
	minion.score = 50;
	motion.angle = 0.f;
	motion.velocity = { 0, 0.f };
	minion.health = 50;
	minion.max_health = 50;
	motion.position = position;
	motion.scale = vec2({ -bounds.x, bounds.y });
	/*vec3 color = { 0.2,0.8,0.8 };
	registry.colors.insert(entity, color);*/
	create_mesh(renderer, position, motion.velocity, motion.scale, motion.angle, entity, TEXTURE_ASSET_ID::MINION, TEXTURE_ASSET_ID::MINION_NM, GEOMETRY_BUFFER_ID::MINION, false);

	auto& dodger = registry.snipers.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MINION,
		  TEXTURE_ASSET_ID::MINION_NM,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}



Entity create_roamer(RenderSystem* renderer, const vec2& position, const vec2& bounds)
{
	auto entity = Entity();


	//Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MINION);
	registry.mesh_collision.emplace(entity);
	//registry.meshPtrs.emplace(entity, &mesh);
	auto& motion = registry.motions.emplace(entity);
	auto& minion = registry.minions.emplace(entity);
	minion.type = Enemy_TYPE::ROAMER;
	minion.score = 30;
	minion.speed = 100;
	motion.angle = 0.f;

	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_real_distribution<> distr(100, 150);
	std::uniform_int_distribution<> signDistr(0, 1);

	float velocityX = distr(eng) * (signDistr(eng) * 2 - 1);
	float velocityY = distr(eng) * (signDistr(eng) * 2 - 1);

	motion.velocity = { velocityX, velocityY };

	motion.position = position;
	motion.scale = vec2({ -bounds.x, bounds.y });
	/*vec3 color = { 0,1,0 };
	registry.colors.insert(entity, color);*/

	create_mesh(renderer, position, motion.velocity, motion.scale, motion.angle, entity, TEXTURE_ASSET_ID::MINION, TEXTURE_ASSET_ID::MINION_NM, GEOMETRY_BUFFER_ID::MINION, false);

	auto& dodger = registry.roamers.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MINION,
		  TEXTURE_ASSET_ID::MINION_NM,
		  EFFECT_ASSET_ID::TEXTURED,
		GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity create_cleaner(RenderSystem* renderer, const vec2& position, const vec2& bounds)
{
	auto entity = Entity();


	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MINION);
	registry.mesh_collision.emplace(entity);
	registry.meshPtrs.emplace(entity, &mesh);
	auto& motion = registry.motions.emplace(entity);
	auto& minion = registry.minions.emplace(entity);
	minion.type = Enemy_TYPE::CLEANER;
	minion.score = 50;
	minion.health = 150;
	minion.speed = 70;
	motion.angle = 0.f;

	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_real_distribution<> distr(60, 70);
	std::uniform_int_distribution<> signDistr(0, 1);

	float velocityX = distr(eng) * (signDistr(eng) * 2 - 1);
	float velocityY = distr(eng) * (signDistr(eng) * 2 - 1);

	motion.velocity = { velocityX, velocityY };

	motion.position = position;
	motion.scale = vec2({ -bounds.x, bounds.y });
	vec3 color = { 0.8,0.8,0.0 };
	registry.colors.insert(entity, color);

	registry.roamers.emplace(entity);
	registry.cleaners.emplace(entity);
	create_mesh(renderer, motion.position, motion.velocity, motion.scale, motion.angle, entity, TEXTURE_ASSET_ID::BATTERY_POWERUP, TEXTURE_ASSET_ID::BATTERY_POWERUP_NM, GEOMETRY_BUFFER_ID::BATTERY, false);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MINION,
		  TEXTURE_ASSET_ID::MINION_NM,
		  EFFECT_ASSET_ID::TEXTURED,
		GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}



Entity create_enemy_bullet(RenderSystem* renderer, vec2 pos, vec2 velocity, float angle, int damage, vec3 color) {
	auto entity = Entity();
	// Store a reference to the potentially re-used mesh object, like createChicken
	//Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MINION_BULLET);
	registry.enemyBullets.emplace(entity);
	//registry.meshPtrs.emplace(entity, &mesh);

	registry.mesh_collision.emplace(entity);
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = angle;
	motion.velocity = velocity;
	motion.scale = vec2(100.0f, 100.0f);
	auto& bullet = registry.bullets.emplace(entity);
	bullet.friendly = false;
	bullet.damage = damage;
	registry.colors.insert(entity, color);
	create_mesh(renderer, pos, velocity, { 0.4 * motion.scale.x, 0.8 * motion.scale.y }, angle, entity, TEXTURE_ASSET_ID::BULLET, TEXTURE_ASSET_ID::BULLET_NM, GEOMETRY_BUFFER_ID::BULLET, false);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BULLET,
			TEXTURE_ASSET_ID::BULLET_NM,
		 EFFECT_ASSET_ID::TEXTURED,
		GEOMETRY_BUFFER_ID::SPRITE });
	return entity;
}


Entity create_mesh(RenderSystem* renderer, vec2 pos, vec2 velocity, vec2 scale, float angle, Entity object_entity, TEXTURE_ASSET_ID texture_id_one, TEXTURE_ASSET_ID texture_id_two, GEOMETRY_BUFFER_ID geometry_id, bool display) {
	auto entity = Entity();
	Mesh& mesh = renderer->getMesh(geometry_id);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.Mesh_entity.emplace(entity);
	registry.Entity_Mesh_Entity.insert(object_entity, entity);
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = angle;
	motion.velocity = velocity;
	motion.scale = scale;
	if (display) {
		registry.renderRequests.insert(
			entity,
			{ texture_id_one,
				texture_id_two,
			 EFFECT_ASSET_ID::CHICKEN,
			geometry_id });
	}
	return entity;
}



Entity create_bullet(RenderSystem* renderer, vec2 pos, vec2 velocity, float angle, int type) {
	if (type == 0) {
		velocity = velocity;
		return createBullet(renderer, pos, velocity, angle);
	}
	else if (type == 1) {
		velocity = velocity;
		return create_lemon_bullet(renderer, pos, velocity, angle);
	}
	else {
		velocity = velocity;
		return create_cactus_bullet(renderer, pos, velocity, angle);
	}
}


Entity create_mesh(RenderSystem* renderer, vec2 pos, vec2 scale, float angle, Entity object_entity, TEXTURE_ASSET_ID texture_id_one, TEXTURE_ASSET_ID texture_id_two, GEOMETRY_BUFFER_ID geometry_id, bool display) {
	auto entity = Entity();
	Mesh& mesh = renderer->getMesh(geometry_id);
	registry.meshPtrs.emplace(entity, &mesh);
	registry.Entity_Mesh_Entity.insert(object_entity, entity);
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = angle;
	motion.scale = scale;
	motion.velocity = { 0.f, 0.f };
	if (display) {
		registry.renderRequests.insert(
			entity,
			{ texture_id_one,
				texture_id_two,
			 EFFECT_ASSET_ID::CHICKEN,
			geometry_id });
	}
	return entity;
}

Entity create_cursor(RenderSystem* renderer, const vec2& position)
{
	auto entity = Entity();
	registry.cursor.emplace(entity);
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0,0 };
	motion.position = position;
	motion.scale = vec2({ 50,50 });

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::CURSOR,
			TEXTURE_ASSET_ID::TEXTURE_COUNT,
		 EFFECT_ASSET_ID::TEXTURED,
		GEOMETRY_BUFFER_ID::SPRITE });


	return entity;
}

Entity createCutScene(RenderSystem* renderer, vec2 pos, vec2 bounds, int stage)
{
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = bounds;

	// add to component
	auto& cutScene = registry.cutScenes.emplace(entity);
	cutScene.stage = stage;

	if (stage == 1) {
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::START_SCREEN,
				TEXTURE_ASSET_ID::TEXTURE_COUNT,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE });
	}
	else if (stage == 2) {
		// 1.1
		cutScene.text[0] = "Meet Blendy!";
		cutScene.text[1] = "He works at";
		cutScene.text[2] = "a smoothie shop.";
		cutScene.text[3] = "Press 'C' to skip";
		cutScene.text_position[0].x = 50;
		cutScene.text_position[0].y = window_height_px - 230;
		cutScene.text_position[1].x = 1210;
		cutScene.text_position[1].y = window_height_px - 250;
		cutScene.text_position[2].x = 1150;
		cutScene.text_position[2].y = window_height_px - 320;
		cutScene.text_position[3].x = 1250;
		cutScene.text_position[3].y = window_height_px - 50;
		cutScene.text_color[0] = vec3(0.16f, 0.302f, 0.322f);
		cutScene.text_color[1] = vec3(0.16f, 0.302f, 0.322f);
		cutScene.text_color[2] = vec3(0.16f, 0.302f, 0.322f);
		cutScene.text_color[3] = vec3(0.6f, 0.247f, 0.435f);
		// 0.443, 0.612, 0.639
		cutScene.text_scale[0] = 1.1;
		cutScene.text_scale[1] = 1.1;
		cutScene.text_scale[2] = 1.1;
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::CUTSCENE_1_1,
				TEXTURE_ASSET_ID::TEXTURE_COUNT,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE });
	}
	else if (stage == 3) {
		// 1.2
		cutScene.text[0] = "One day,";
		cutScene.text[1] = "the manager brings";
		cutScene.text[2] = "in a replacement...";
		cutScene.text[3] = "Press 'C' to skip";
		cutScene.text_position[0].x = 240;
		cutScene.text_position[0].y = window_height_px - 90;
		cutScene.text_position[1].x = 80;
		cutScene.text_position[1].y = window_height_px - 160;
		cutScene.text_position[2].x = 90;
		cutScene.text_position[2].y = window_height_px - 230;
		cutScene.text_position[3].x = 1250;
		cutScene.text_position[3].y = window_height_px - 50;
		cutScene.text_color[0] = vec3(0.16f, 0.302f, 0.322f);
		cutScene.text_color[1] = vec3(0.16f, 0.302f, 0.322f);
		cutScene.text_color[2] = vec3(0.16f, 0.302f, 0.322f);
		cutScene.text_color[3] = vec3(0.6f, 0.247f, 0.435f);
		cutScene.text_scale[0] = 1.2;
		cutScene.text_scale[1] = 1.2;
		cutScene.text_scale[2] = 1.2;
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::CUTSCENE_1_2,
				TEXTURE_ASSET_ID::TEXTURE_COUNT,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE });
	}
	else if (stage == 4) {
		// 1.3
		cutScene.text[0] = "...and throws";
		cutScene.text[1] = "Blendy out.";
		cutScene.text[3] = "Press 'C' to skip";
		cutScene.text_position[0].x = 100;
		cutScene.text_position[0].y = window_height_px - 210;
		cutScene.text_position[1].x = 130;
		cutScene.text_position[1].y = window_height_px - 280;
		cutScene.text_position[3].x = 1250;
		cutScene.text_position[3].y = window_height_px - 50;
		cutScene.text_scale[0] = 1.2;
		cutScene.text_scale[1] = 1.2;
		cutScene.text_color[0] = vec3(0.16f, 0.302f, 0.322f);
		cutScene.text_color[1] = vec3(0.16f, 0.302f, 0.322f);
		cutScene.text_color[3] = vec3(0.6f, 0.247f, 0.435f);
		cutScene.text_scale[0] = 1.2;
		cutScene.text_scale[1] = 1.2;
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::CUTSCENE_1_3,
				TEXTURE_ASSET_ID::TEXTURE_COUNT,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE });
	}
	else if (stage == 5) {
		// 1.4
		cutScene.text[0] = "Blendy vows to get revenge.";
		cutScene.text[3] = "Press 'C' to skip";
		cutScene.text_position[0].x = 750;
		cutScene.text_position[0].y = window_height_px - 180;
		cutScene.text_position[3].x = 1250;
		cutScene.text_position[3].y = window_height_px - 50;
		cutScene.text_color[0] = vec3(0.16f, 0.302f, 0.322f);
		cutScene.text_color[3] = vec3(0.6f, 0.247f, 0.435f);
		cutScene.text_scale[0] = 1.15;
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::CUTSCENE_1_4,
				TEXTURE_ASSET_ID::TEXTURE_COUNT,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE });
	}
	else if (stage == 7) {
		// 2.1
		cutScene.text[0] = "Blendy's replacement";
		cutScene.text[1] = "begins to grow in power.";
		cutScene.text[3] = "Press 'C' to skip";
		cutScene.scale = 1.5;
		cutScene.text_position[0].x = 850;
		cutScene.text_position[0].y = window_height_px - 600;
		cutScene.text_position[1].x = 850;
		cutScene.text_position[1].y = window_height_px - 700;
		cutScene.text_position[3].x = 1250;
		cutScene.text_position[3].y = window_height_px - 50;
		cutScene.text_color[3] = vec3(1.f, 0.95f, 0.776f);
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::CUTSCENE_2_1,
				TEXTURE_ASSET_ID::TEXTURE_COUNT,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE });
	}
	else if (stage == 8)
	{
		// 2.2
		cutScene.text[3] = "Press 'C' to skip";
		cutScene.text_position[3].x = 1250;
		cutScene.text_position[3].y = window_height_px - 50;
		cutScene.text_color[3] = vec3(1.f, 0.95f, 0.776f);
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::CUTSCENE_2_2,
				TEXTURE_ASSET_ID::TEXTURE_COUNT,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE });
	}
	else if (stage == 10) {
		// 3.1
		cutScene.text[0] = "Press 'R' to restart!";
		cutScene.scale = 1.4;
		cutScene.text_position[0].x = 1000;
		cutScene.text_position[0].y = window_height_px - 120;
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::CUTSCENE_3_1,
				TEXTURE_ASSET_ID::TEXTURE_COUNT,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE });
	}

	// create render request
	return entity;
}
