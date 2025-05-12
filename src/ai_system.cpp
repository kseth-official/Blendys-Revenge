#include "ai_system.hpp"
#include "world_init.hpp"
#include <random>

static int frame_count = 0;

// AI Configuration
const int UPDATE_FREQUENCY = 50;
const float IDEAL_RANGE_FROM_PLAYER = 400.0f;
const float APPROACH_SPEED_FACTOR = 1.0f;
const float DODGE_SPEED_FACTOR = 1.5f;
const float CHARGER_AGGRO_RANGE = 450.0f;
const float CHARGER_AIM_TIME = 50.0f;
const float CHARGER_REST_TIME = 80.0f;
const float CHARGER_CHARGE_SPEED = 4.0f;
const float BOSS_CHARGE_SPEED = 2.0f;
const float BOSS_SHOOT_RANGE = 500.f;
const float PICK_UP_RANGE = 600.f;
const float BOSS_AIM_TIME = 40.0f;
const float BOSS_REST_TIME = 150.0f;
const float GRAPE_BULLET_SPEED = 400.f;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> angleDistr(-M_PI / 2, M_PI / 2);
std::uniform_int_distribution<> distr(-2000, 100);

float calculateDistance(const vec2& pos1, const vec2& pos2) {
	vec2 diff = pos1 - pos2;
	return sqrt(diff.x * diff.x + diff.y * diff.y);
}

vec2 calculateInterceptPosition(vec2 sniperPos, vec2 playerPos, float factor) {
	// Calculates a position factor% of the way from playerPos to sniperPos
	return playerPos + (sniperPos - playerPos) * factor;
}

Entity AISystem::findClosestSniper(vec2 tank_pos) {
	float minDistance = std::numeric_limits<float>::max();
	Entity closestSniper = registry.players.entities[0];

	for (auto sniperEntity : registry.snipers.entities) {
		if (registry.protections.has(sniperEntity)) {
			continue;
		}
		auto& sniperMotion = registry.motions.get(sniperEntity);
		float distance = calculateDistance(tank_pos, sniperMotion.position);

		if (distance < minDistance) {
			minDistance = distance;
			closestSniper = sniperEntity;
		}
	}

	return closestSniper;
}

Entity AISystem::findClosestPowerUp(vec2 pos) {
	float minDistance = std::numeric_limits<float>::max();
	Entity closestPowerUp = registry.players.entities[0];

	for (auto powerUpEntity : registry.powerUps.entities) {
		auto& powerUpMotion = registry.motions.get(powerUpEntity);
		float distance = calculateDistance(pos, powerUpMotion.position);

		if (distance < minDistance) {
			minDistance = distance;
			closestPowerUp = powerUpEntity;
		}
	}

	return closestPowerUp;
}

void AISystem::shoot(Entity shooterEntity, const vec2& playerPosition, float elapsed_ms) {
	auto& shooter = registry.shooters.get(shooterEntity);
	Motion& motion = registry.motions.get(shooterEntity);
	if (registry.splitShooters.has(shooterEntity)) {
		shooter.time_since_last_shot_ms += elapsed_ms / 2;
	}
	else {
		shooter.time_since_last_shot_ms += elapsed_ms;
	}

	shooter.time_since_last_shot_ms += elapsed_ms;
	if (shooter.time_since_last_shot_ms >= shooter.shoot_interval_ms) {
		vec2 bullet_direction = normalize(playerPosition - motion.position);

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

		if (registry.splitShooters.has(shooterEntity)) {
			vec2 side_direction = vec2(-bullet_direction.y, bullet_direction.x);
			create_enemy_bullet(renderer, motion.position, (bullet_direction + side_direction * 0.2f) * 280.0f, angle_diff);
			create_enemy_bullet(renderer, motion.position, (bullet_direction - side_direction * 0.2f) * 280.0f, angle_diff);
		}
		else {
			create_enemy_bullet(renderer, motion.position, bullet_direction * 280.0f, angle_diff);
		}
		shooter.time_since_last_shot_ms = static_cast<float>(distr(gen));
	}
}



void shootGrapeBullets(RenderSystem* renderer, vec2 pos, vec2 velocity, float up_angle, float angle_diff) {
	const int num_bullets = 12;
	const float angle_increment = 2 * M_PI / num_bullets;
	for (int i = 0; i < num_bullets; ++i) {
		if (angle_diff < -M_PI) {
			angle_diff += 2 * M_PI;
		}
		else if (angle_diff > M_PI) {
			angle_diff -= 2 * M_PI;
		}
		float angle = i * angle_increment;
		vec2 velocity = { cos(angle) * GRAPE_BULLET_SPEED, sin(angle) * GRAPE_BULLET_SPEED };
		float final_angle = up_angle + angle_diff + angle;
		create_enemy_bullet(renderer, pos, velocity, final_angle, 20, { 1,0,1 });
	}
}




void AISystem::boss_shoot(Boss& boss, Motion& motion, const vec2& player_pos, float elapsed_ms) {
	if (!boss.is_shooting) return;
	boss.time_since_last_shot_ms += elapsed_ms;
	/*std::cout << boss.time_since_last_shot_ms << std::endl;*/
	if (boss.time_since_last_shot_ms < boss.shoot_interval_ms / 4) return;
	vec2 bullet_direction = normalize(player_pos - motion.position);
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
	boss.powerup_duration_ms -= elapsed_ms;

	switch (boss.bstate) {
	case BulletState::DEFAULT: {
		if (boss.time_since_last_shot_ms >= boss.shoot_interval_ms) {
			create_enemy_bullet(renderer, motion.position, bullet_direction * 320.0f, angle_diff, 25, { 1,0.5,0 });
			boss.time_since_last_shot_ms = 0;
		}
		return;
	}
	case BulletState::GRAPE: {
		if (boss.time_since_last_shot_ms >= boss.shoot_interval_ms) {
			angle_diff = -3.02989;
			shootGrapeBullets(renderer, motion.position, bullet_direction, up_angle, angle_diff);
			boss.time_since_last_shot_ms = 0;
		}
		break;
	}
	case BulletState::CACTUS: {
		if (boss.time_since_last_shot_ms >= boss.shoot_interval_ms * 1.5) {
			//todo:change_shape
			create_enemy_bullet(renderer, motion.position, bullet_direction * 580.0f, angle_diff, 25, { 1,0,0 });
			boss.time_since_last_shot_ms = 0;
		}
		break;
	}
	case BulletState::CHERRY: {
		if (boss.time_since_last_shot_ms >= boss.shoot_interval_ms * 2) {
			vec2 side_direction = vec2(-bullet_direction.y, bullet_direction.x);
			create_enemy_bullet(renderer, motion.position, (bullet_direction + side_direction * 0.3f) * 320.0f, angle_diff, 15, { 1,0.5,0 });
			create_enemy_bullet(renderer, motion.position, (bullet_direction - side_direction * 0.3f) * 320.0f, angle_diff, 15, { 1,0.5,0 });
			create_enemy_bullet(renderer, motion.position, bullet_direction * 320.0f, angle_diff, 15, { 1,0.5,0 });
			boss.time_since_last_shot_ms = 0;
		}
		break;
	}
	case BulletState::PROTEIN: {
		if (boss.time_since_last_shot_ms >= boss.shoot_interval_ms / 4) {
			create_enemy_bullet(renderer, motion.position, bullet_direction * 290.0f, angle_diff, 10, { 1,0.5,0 });
			boss.time_since_last_shot_ms = 0;
		}
		break;
	}
	case BulletState::LEMON: {
		if (boss.time_since_last_shot_ms >= boss.shoot_interval_ms / 2) {
			create_enemy_bullet(renderer, motion.position, bullet_direction * 320.f, angle_diff, 50, { 0.5,1,0 });
			boss.time_since_last_shot_ms = 0;
		}
		break;
	}
	}

	if (boss.powerup_duration_ms < 0) boss.bstate = BulletState::DEFAULT;
}


void AISystem::updateBoss(Entity bossEntity, vec2 chase_direction,
	Minion& enemy, Motion& motion, float elapsed_ms, vec2 player_pos) {
	Boss& boss = registry.boss.get(bossEntity);
	float distanceToPlayer = calculateDistance(motion.position, player_pos);
	vec2 target_direction = chase_direction;
	Entity closestPowerUp = findClosestPowerUp(motion.position);



	switch (boss.state) {
	case BossState::Default: {
		boss.is_shooting = true;
		if (boss.rest_timer < 0) {
			boss.state = BossState::Aiming;
			boss.aim_timer = BOSS_AIM_TIME;
			break;
		}
		boss.rest_timer -= elapsed_ms;
		if (!registry.players.has(closestPowerUp)) {
			auto& power = registry.motions.get(closestPowerUp);
			vec2 goalPos = calculateInterceptPosition(power.position, motion.position, 1);
			target_direction = normalize(goalPos - motion.position);

		}
		else if (distanceToPlayer < 400.f) {
			vec2 awayDirection = -chase_direction;
			float angleOffset = angleDistr(gen);
			vec2 newDirection;
			newDirection.x = awayDirection.x * cos(angleOffset) - awayDirection.y * sin(angleOffset);
			newDirection.y = awayDirection.x * sin(angleOffset) + awayDirection.y * cos(angleOffset);
			target_direction = newDirection;
		}
		motion.velocity = target_direction * enemy.speed;
		break;
	}
	case BossState::Aiming: {
		boss.is_shooting = false;
		motion.velocity = { 0, 0 };
		boss.aim_timer -= elapsed_ms;
		if (boss.aim_timer <= 0) {
			boss.aim_timer = 0;
			boss.state = BossState::Charging;
			if (!registry.players.has(closestPowerUp)) {
				auto& power = registry.motions.get(closestPowerUp);
				vec2 goalPos = calculateInterceptPosition(power.position, motion.position, 1);
				target_direction = normalize(goalPos - motion.position);
			}
			boss.charge_direction = target_direction;
		}
		break;
	}
	case BossState::Charging: {
		boss.is_shooting = false;
		motion.velocity = boss.charge_direction * BOSS_CHARGE_SPEED * enemy.speed;
		boss.rest_timer += elapsed_ms * 2;
		if (boss.rest_timer >= CHARGER_REST_TIME) {
			boss.rest_timer = BOSS_REST_TIME;
			boss.state = BossState::Default;
		}
		break;
	}
	case BossState::Shooting: {
		boss.is_shooting = true;
		boss.powerup_duration_ms -= elapsed_ms;
		if (boss.powerup_duration_ms < 0) {
			boss.state = BossState::Default;
		}
		motion.velocity = { 0, 0 };
		break;
	}
	}

	boss_shoot(boss, motion, player_pos, elapsed_ms);
}


void AISystem::updateCleaner(Entity cleanerEntity, vec2 chase_direction,
	Minion& enemy, Motion& motion, float elapsed_ms) {
	Entity closestPowerUp = findClosestPowerUp(motion.position);

	if (!registry.players.has(closestPowerUp)) {
		auto& powerUpMotion = registry.motions.get(closestPowerUp);
		vec2 directionToPowerUp = normalize(powerUpMotion.position - motion.position);

		if (calculateDistance(motion.position, powerUpMotion.position) < PICK_UP_RANGE) {
			motion.velocity = directionToPowerUp * enemy.speed;
		}
	}
	else {
		vec2 direction = normalize(motion.velocity);
		motion.velocity = direction * enemy.speed;
	}
}

void AISystem::updateTank(Entity tankEntity, vec2 chase_direction,
	Minion& enemy, Motion& motion, float elapsed_ms, vec2 player_pos) {
	auto& tank = registry.tanks.get(tankEntity);
	float distanceToPlayer = calculateDistance(motion.position, player_pos);

	switch (tank.state) {
	case TankState::DEFAULT: {
		if (registry.snipers.entities.size() != 0) {
			Entity closestSniper = findClosestSniper(motion.position);
			if (!registry.players.has(closestSniper)) {
				auto& sniper_protect = registry.protections.emplace(closestSniper);
				sniper_protect.link = tankEntity;
				auto& tank_protect = registry.protections.emplace(tankEntity);
				tank_protect.link = closestSniper;
				tank.state = TankState::PROTECTING;
			}
		}
		motion.velocity = chase_direction * enemy.speed;
		break;
	}
	case TankState::PROTECTING: {
		auto& protect = registry.protections.get(tankEntity);
		if (!registry.protections.has(protect.link)) {
			registry.protections.remove(tankEntity);
			tank.state = TankState::DEFAULT;
			motion.velocity = chase_direction * enemy.speed;
			break;
		}
		else {
			auto& sniperMotion = registry.motions.get(protect.link);
			vec2 protectivePos = calculateInterceptPosition(sniperMotion.position, player_pos, 0.7f);
			vec2 directionToProtectivePos = normalize(protectivePos - motion.position);
			motion.velocity = directionToProtectivePos * enemy.speed;
		}
		break;
	}
	}

}


void AISystem::updateSniper(Entity sniperEntity, vec2 chase_direction,
	Minion& enemy, Motion& motion, float elapsed_ms, vec2 player_pos) {
	auto& sniper = registry.snipers.get(sniperEntity);

	float distanceToPlayer = calculateDistance(motion.position, player_pos);
	float aimDistance = 700.0f;
	float avoidDistance = 500.0f;


	if (registry.protections.has(sniperEntity)) {
		auto& protect = registry.protections.get(sniperEntity);
		if (!registry.protections.has(protect.link)) {
			registry.protections.remove(sniperEntity);
		}
	}

	switch (sniper.state) {
	case SniperState::AVOIDING:
		if (distanceToPlayer < avoidDistance) {
			vec2 flee_direction = normalize(motion.position - player_pos);
			motion.velocity = -chase_direction * enemy.speed;
		}
		else if (distanceToPlayer >= avoidDistance && distanceToPlayer <= aimDistance) {
			sniper.state = SniperState::AIMING;
			sniper.aim_timer = CHARGER_AIM_TIME;
		}
		else {
			motion.velocity = chase_direction * enemy.speed;
		}
		break;
	case SniperState::AIMING: {
		motion.velocity = { 0, 0 };
		sniper.aim_timer -= elapsed_ms;
		float color_offset = ((50 - sniper.aim_timer) / 50) / 4;
		vec3 color = { 0.2 - color_offset ,0.8 - color_offset,0.8 + color_offset };
		registry.colors.remove(sniperEntity);
		registry.colors.insert(sniperEntity, color);
		if (sniper.aim_timer <= 0) {
			sniper.state = SniperState::SHOOTING;
		}
		break;
	}
	case SniperState::SHOOTING: {
		vec2 bullet_direction = normalize(player_pos - motion.position);

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
		vec3 color = { 0.2,0.8,0.8 };
		registry.colors.remove(sniperEntity);
		registry.colors.insert(sniperEntity, color);
		create_enemy_bullet(renderer, motion.position, bullet_direction * 500.0f, angle_diff, 50, { 0,0,0 });
		sniper.state = SniperState::AVOIDING;
		break;
	}
	}
}


void AISystem::updateCharger(Entity chargerEntity, vec2 chase_direction,
	Minion& enemy, Motion& motion, float elapsed_ms, vec2 player_pos) {
	auto& charger = registry.chargers.get(chargerEntity);

	float distanceToPlayer = calculateDistance(motion.position, player_pos);


	switch (charger.state) {
	case ChargerState::Approaching:
		if (distanceToPlayer <= CHARGER_AGGRO_RANGE) {
			charger.state = ChargerState::Aiming;
			charger.aim_timer = CHARGER_AIM_TIME;
		}
		else {
			motion.velocity = chase_direction * enemy.speed;

		}
		break;
	case ChargerState::Aiming:
	{
		motion.velocity = { 0, 0 };

		charger.aim_timer -= elapsed_ms;
		float color_r = ((50 - charger.aim_timer) / 50) + 0.5;
		vec3 color = { color_r,0.2,0.2 };
		registry.colors.remove(chargerEntity);
		registry.colors.insert(chargerEntity, color);
		if (charger.aim_timer <= 0) {
			charger.aim_timer = 0;
			charger.state = ChargerState::Charging;
			charger.charge_direction = chase_direction;
		}
	}
	break;
	case ChargerState::Charging:
		motion.velocity = charger.charge_direction * CHARGER_CHARGE_SPEED * enemy.speed;
		charger.rest_timer += elapsed_ms * 2;
		if (charger.rest_timer >= CHARGER_REST_TIME) {
			charger.rest_timer = 80;
			charger.state = ChargerState::Resting;
		}
		break;
	case ChargerState::Resting:
	{
		motion.velocity = chase_direction * enemy.speed * ((80 - charger.rest_timer) / 80);

		float color_r = (charger.rest_timer / 160) + 0.5;
		vec3 color = { color_r,0.2,0.2 };
		registry.colors.remove(chargerEntity);
		registry.colors.insert(chargerEntity, color);
		charger.rest_timer -= elapsed_ms;
		if (charger.rest_timer <= 0) {
			charger.state = ChargerState::Approaching;
		}
	}
	break;
	}
}

ShooterState decideShooterState(const vec2& enemyPos, const vec2& playerPos, float idealRange) {
	float distance = calculateDistance(enemyPos, playerPos);
	if (distance > idealRange) {
		return ShooterState::APPROACH;
	}
	else {
		return ShooterState::DODGE;
	}
}



void AISystem::init(RenderSystem* renderer_arg) {
	this->renderer = renderer_arg;
}


void AISystem::step(float elapsed_ms)
{
	frame_count++;
	if (frame_count % UPDATE_FREQUENCY != 0) return;

	auto& motions_registry = registry.motions;
	auto& player_registry = registry.players;
	auto& enemy_registry = registry.minions;
	Motion& player_motion = motions_registry.get(player_registry.entities[0]);
	vec2 player_position = player_motion.position;
	vec2 player_velocity = player_motion.velocity;


	float prediction_time = UPDATE_FREQUENCY / 1000;
	vec2 predicted_player_pos = player_position + player_velocity * prediction_time;


	for (Entity enemy_enitiy : registry.minions.entities) {
		Motion& motion = motions_registry.get(enemy_enitiy);
		Minion& enemy = enemy_registry.get(enemy_enitiy);
		vec2 minions_pos = motion.position;
		vec2 direction_to_player = predicted_player_pos - minions_pos;
		float original_speed = enemy.speed;
		vec2 chase_direction = normalize(predicted_player_pos - minions_pos);

		if (enemy.type == EnemyType::BASIC) {
			motion.velocity = chase_direction * original_speed;
		}
		else if (enemy.type == EnemyType::ROAMER) {
			vec2 direction = normalize(motion.velocity);
			motion.velocity = direction * (original_speed);
		}
		else if (enemy.type == EnemyType::SHOOTER || enemy.type == EnemyType::SPLIT_SHOOTER) {
			ShooterState state = decideShooterState(motion.position, predicted_player_pos, IDEAL_RANGE_FROM_PLAYER);
			switch (state) {
			case ShooterState::APPROACH:
				motion.velocity = chase_direction * (original_speed * APPROACH_SPEED_FACTOR);
				break;
			case ShooterState::DODGE:
				motion.velocity = -chase_direction * (original_speed * DODGE_SPEED_FACTOR);
				break;
			}
			shoot(enemy_enitiy, player_position, elapsed_ms * UPDATE_FREQUENCY);
		}
		else if (enemy.type == EnemyType::CHARGER) {
			updateCharger(enemy_enitiy, chase_direction, enemy, motion, elapsed_ms, player_position);
		}
		else if (enemy.type == EnemyType::SNIPER) {
			updateSniper(enemy_enitiy, chase_direction, enemy, motion, elapsed_ms, player_position);
		}
		else if (enemy.type == EnemyType::TANK) {
			updateTank(enemy_enitiy, chase_direction, enemy, motion, elapsed_ms, player_position);
		}
		else if (enemy.type == EnemyType::CLEANER) {
			updateCleaner(enemy_enitiy, chase_direction, enemy, motion, elapsed_ms);
		}
		else if (enemy.type == EnemyType::BOSS) {
			updateBoss(enemy_enitiy, chase_direction, enemy, motion, elapsed_ms, player_position);
		}
	}
}