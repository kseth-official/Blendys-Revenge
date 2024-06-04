#pragma once

#include <vector>

#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "world_init.hpp"

enum class ShooterState {
	Approach,
	Dodge,
};



class AISystem
{
public:
	void init(RenderSystem* renderer_arg);
	void step(float elapsed_ms);
private:
	void shoot(Entity shooterEntity, const vec2& playerPosition, float elapsed_ms);
	void boss_shoot(Boss& boss, Motion& motion, const vec2& player_pos, float elapsed_ms);
	void updateCharger(Entity chargerEntity, vec2 chase_direction, Minion& enemy, Motion& motion, float elapsed_ms, vec2 player_pos);
	void updateSniper(Entity sniperEntity, vec2 chase_direction, Minion& enemy, Motion& motion, float elapsed_ms, vec2 player_pos);
	void updateTank(Entity sniperEntity, vec2 chase_direction, Minion& enemy, Motion& motion, float elapsed_ms, vec2 player_pos);
	void updateCleaner(Entity cleanerEntity, vec2 chase_direction, Minion& enemy, Motion& motion, float elapsed_ms);
	void updateBoss(Entity bossEntity, vec2 chase_direction, Minion& enemy, Motion& motion, float elapsed_ms, vec2 player_pos);
	Entity findClosestPowerUp(vec2 cleanerPos);
	Entity findClosestSniper(vec2 player_pos);

	RenderSystem* renderer;
};