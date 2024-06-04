// internal
#include "physics_system.hpp"
#include "world_init.hpp"
#include <iostream>
#include <vector>

//vec2 normalize(const vec2&);
float duration = 0;
std::map<Direction, Mesh> PhysicsSystem::loaded_blendy_meshes;
std::map<Direction, Mesh> PhysicsSystem::loaded_minion_meshes;
std::map<Direction, Mesh> PhysicsSystem::loaded_boss_meshes;

bool isParallel(const std::vector<vec2>&, const vec2&);
std::pair<float, float> projectOntoAxis(const std::vector<vec2>&, const vec2&);
bool projectionsOverlap(const std::pair<float, float>&, const std::pair<float, float>&);
bool checkMeshCollisionSAT(Mesh*, const Motion&, Mesh*, const Motion&, const box);
std::vector<vec2> getRectangleEdge(const Motion&, std::vector<vec2>&);
box calculate_overlap_area(const vec2&, const vec2&, const vec2&, const vec2&);
bool isPointInBox(const vec2&, const box&);
void calculateMeshAxes(const std::vector<vec2>& vertices, std::vector<vec2>& axes);
bool checkForCollision(const std::vector<vec2>& shape1, const std::vector<vec2>& shape2, const std::vector<vec2>& axes);
Mesh* getMeshForEntity(const Entity& entity);
//extern Grid grid;

// Returns the local bounding coordinates scaled by the current size of the entity
vec2 get_bounding_box(const Motion& motion)
{
	// abs is to avoid negative scale due to the facing direction.
	return { abs(motion.scale.x), abs(motion.scale.y) };
}

// This is a SUPER APPROXIMATE check that puts a circle around the bounding boxes and sees
// if the center point of either object is inside the other's bounding-box-circle. You can
// surely implement a more accurate detection

//bool collides(const Motion& motion1, const Motion& motion2)
//{
//	vec2 dp = motion1.position - motion2.position;
//	float dist_squared = dot(dp, dp);
//	const vec2 other_bonding_box = get_bounding_box(motion1) / 2.f;
//	const float other_r_squared = dot(other_bonding_box, other_bonding_box);
//	const vec2 my_bonding_box = get_bounding_box(motion2) / 2.f;
//	const float my_r_squared = dot(my_bonding_box, my_bonding_box);
//	const float r_squared = max(other_r_squared, my_r_squared);
//	if (dist_squared < r_squared)
//		return true;
//	return false;
//}

//void applyBoidsRuleSeparation(Entity entity, Motion& motion) {
//	float desiredSeparation = motion.scale.y; 
//	vec2 steer = { 0.0f, 0.0f };
//	int count = 0;
//
//	for (uint j = 0; j < registry.motions.size(); j++) {
//		Motion& other_motion = registry.motions.components[j];
//		Entity other_entity = registry.motions.entities[j];
//
//		// Don't compare to itself and ensure it's a minion
//		if (entity == other_entity || !registry.minions.has(other_entity)) continue;
//
//		float d = sqrt(dot(motion.position - other_motion.position, motion.position - other_motion.position));
//		// If the distance is greater than 0 and less than an arbitrary amount (0 indicates the same position)
//		if ((d > 0) && (d < desiredSeparation)) {
//			// Calculate vector pointing away from neighbor
//			vec2 diff = motion.position - other_motion.position;
//			diff = normalize(diff);
//			diff = diff / d; // Weight by distance
//			steer = steer + diff;
//			count++; // Keep track of how many
//		}
//	}
//
//	// Average -- divide by how many
//	if (count > 0) {
//		steer = steer / float(count);
//		// Implement Reynolds: Steering = Desired - Velocity
//		steer = normalize(steer) * registry.minions.get(entity).speed - motion.velocity;
//		// Adjust the velocity
//		motion.velocity = motion.velocity + steer;
//	}
//}

bool collides(const Entity& entity1, const Entity& entity2, Motion& motion1, Motion& motion2)
{
	if (!(registry.mesh_collision.has(entity1) && registry.mesh_collision.has(entity2))) {
		return false;
	}

	//!!!!!!!note from Andrew:minion/minion sperate is added here because I want to use your function
	//plase don't touch the code in the next if statement
	if (registry.minions.has(entity1) && registry.minions.has(entity2)) {
		if (registry.boss.has(entity1) || registry.boss.has(entity2)) {
			return true;
		}
		vec2 center_dis = motion1.position - motion2.position;
		// Calculate the distance between the two entities
		float distance = sqrt(dot(center_dis, center_dis));
		// Calculate the sum of the radii
		float sum_radii = (motion1.scale.y / 2) + (motion2.scale.y / 2);

		// Check if the distance between the centers is less than the sum of the radii (collision)
		if (distance < sum_radii) {
			// Calculate how much they are overlapping
			float overlap = sum_radii - distance;


			if (distance != 0) {
				vec2 direction = { center_dis.x / distance, center_dis.y / distance };

				// Determine the separation speed. This could be a fixed value or based on the overlap
				float overlap = sum_radii - distance;
				float separationSpeed = overlap / 6;

				// Adjust the velocities to separate the minions
				// Entity1 moves away in the direction, Entity2 in the opposite
				motion1.velocity.x += direction.x * separationSpeed;
				motion1.velocity.y += direction.y * separationSpeed;
				motion2.velocity.x -= direction.x * separationSpeed;
				motion2.velocity.y -= direction.y * separationSpeed;
			}

		}
	}

	if (
		(registry.minions.has(entity1) && registry.bullets.has(entity2))
		|| (registry.minions.has(entity2) && registry.bullets.has(entity1))
		|| (registry.players.has(entity1) && registry.enemyBullets.has(entity2))
		|| (registry.players.has(entity2) && registry.enemyBullets.has(entity1))
		|| (registry.players.has(entity2) && registry.minions.has(entity1))
		|| (registry.players.has(entity1) && registry.minions.has(entity2))
		|| (registry.players.has(entity1) && registry.powerUps.has(entity2))
		|| (registry.players.has(entity2) && registry.powerUps.has(entity1))
		|| (registry.boss.has(entity1) && registry.powerUps.has(entity2))
		|| (registry.boss.has(entity2) && registry.powerUps.has(entity1))
		)
	{
		// pass
	}
	else {
		return false;
	}

	const vec2 halfBB_one = get_bounding_box(motion1) / 2.f;
	const vec2 halfBB_two = get_bounding_box(motion2) / 2.f;
	vec2 center_dis = motion1.position - motion2.position;

	// check bounding box overlap first 
	if (abs(center_dis.x) < (halfBB_two.x + halfBB_one.x)
		&& abs(center_dis.y) < (halfBB_two.y + halfBB_one.y)) {

		if (registry.boss.has(entity1) || registry.boss.has(entity2)) {
			return true;
		}
		//return true;

		//if (it_one != registry.motions.components.end() && it_two != registry.motions.components.end()) {
		box overlapBox = calculate_overlap_area(motion1.position, halfBB_one, motion2.position, halfBB_two);
		//if (registry.meshPtrs.has(entity1) && registry.meshPtrs.has(entity2)) {

		Mesh* mesh_one = NULL;
		Mesh* mesh_two = NULL;
		if (registry.Entity_Mesh_Entity.has(entity1)) {
			Entity& mesh_one_entity = registry.Entity_Mesh_Entity.get(entity1);
			if (registry.meshPtrs.has(mesh_one_entity)) {
				mesh_one = registry.meshPtrs.get(mesh_one_entity);
			}
		}
		if (registry.Entity_Mesh_Entity.has(entity2)) {
			Entity& mesh_two_entity = registry.Entity_Mesh_Entity.get(entity2);
			if (registry.meshPtrs.has(mesh_two_entity)) {
				mesh_two = registry.meshPtrs.get(mesh_two_entity);
			}
		}
		Mesh* mesh_one_temp = getMeshForEntity(entity1);
		Mesh* mesh_two_temp = getMeshForEntity(entity2);
		if (mesh_one_temp != nullptr) {
			mesh_one = mesh_one_temp;
		}
		if (mesh_two_temp != nullptr) {
			mesh_two = mesh_two_temp;
		}
		if (mesh_one != NULL && mesh_two != NULL) {
			return checkMeshCollisionSAT(mesh_one, motion1, mesh_two, motion2, overlapBox);
		}
		if (mesh_one == NULL || mesh_two == NULL) {
			//std::cout << "No mesh!!" << std::endl;
		}
	}
	return false;
}

Mesh* getMeshForEntity(const Entity& entity) {
	if (registry.players.has(entity)) {
		auto& player = registry.players.get(entity);
		return &PhysicsSystem::loaded_blendy_meshes.at(player.up ? Direction::Up :
			player.down ? Direction::Down :
			player.left ? Direction::Left :
			player.right ? Direction::Right :
			Direction::Up);
	}
	else if (registry.minions.has(entity)) {
		if (registry.boss.has(entity)) {
			auto& boss = registry.minions.get(entity);

			return &PhysicsSystem::loaded_boss_meshes.at(boss.up ? Direction::Up :
				boss.down ? Direction::Down :
				boss.left ? Direction::Left :
				boss.right ? Direction::Right :
				Direction::Down);
		}
		else {
			auto& minion = registry.minions.get(entity);
			return &PhysicsSystem::loaded_minion_meshes.at(minion.up ? Direction::Up :
				minion.down ? Direction::Down :
				minion.left ? Direction::Left :
				minion.right ? Direction::Right :
				Direction::Down);
		}
	}
	return nullptr;
}


float lerp(float start, float end, float t) {

	return start * (1 - t) + end * t;
}

void PhysicsSystem::step(float elapsed_ms)
{
	// Move bug based on how much time has passed, this is to (partially) avoid
	// having entities move at different speed based on the machine.
	auto& motion_registry = registry.motions;
	static float accumulatedTime = 0.0f;
	accumulatedTime += elapsed_ms;
	for (uint i = 0; i < motion_registry.size(); i++)
	{
		Motion& motion = motion_registry.components[i];
		Entity entity = motion_registry.entities[i];
		float step_seconds = elapsed_ms / 1000.f;

		if (registry.players.has(entity)) {
			// blendy animation
			Player& blendy = registry.players.get(entity);
			if (!registry.is_dead) {
				const float cycleDuration = 4000.0f;
				float cycleTime = fmod(accumulatedTime, cycleDuration) / cycleDuration;


				float normalizedTime;
				if (cycleTime < 0.5f) {
					normalizedTime = cycleTime / 0.5f;
				}
				else {
					normalizedTime = (1.0f - cycleTime) / 0.5f;
				}

				const float maxScale = 1.1f;

				motion.scale.x = lerp(BLENDY_BB_WIDTH, maxScale * BLENDY_BB_WIDTH, normalizedTime);
				motion.scale.y = lerp(BLENDY_BB_HEIGHT, maxScale * BLENDY_BB_HEIGHT, normalizedTime);
			}

			float new_x = motion.velocity.x * step_seconds + motion.position.x;
			float new_y = motion.velocity.y * step_seconds + motion.position.y + motion.y_animate;
			
			vec2 bounding_box = { abs(motion.scale.x), abs(motion.scale.y) };
			float half_width = bounding_box.x / 2.f;
			float half_height = bounding_box.y / 2.f;
			if (new_x - half_width > 0 && new_x + half_width < window_width_px && blendy.frame_stage != 0 && !registry.deathTimers.has(entity)) {
				motion.position.x = new_x;
			}
			else {
				if (new_x - half_width < 0 && new_x > motion.position.x) {
					motion.position.x = new_x;
				}else if (new_x + half_width > window_width_px && new_x < motion.position.x) {
					motion.position.x = new_x;
				}
			}
		
			if (new_y - half_height > 10 && new_y + half_height < window_height_px) {
				motion.position.y = new_y;
			}
			else {
				if (new_y - half_height < 10 && new_y > motion.position.y) {
					motion.position.y = new_y;
				}
				else if (new_y + half_height > window_height_px && new_y < motion.position.y) {
					motion.position.y = new_y;
				}
			}

		}

		else if(registry.minions.has(entity)){
			/*applyBoidsRuleSeparation(entity, motion);*/
			float new_x = motion.velocity.x * step_seconds + motion.position.x;
			float new_y = motion.velocity.y * step_seconds + motion.position.y + motion.y_animate;
			vec2 bounding_box = { abs(motion.scale.x), abs(motion.scale.y) };
			float half_width = bounding_box.x / 2.f;
			float half_height = bounding_box.y / 2.f;
			
			if (new_x - half_width <= 0) {
				if (registry.roamers.has(entity) && motion.velocity.x < 0) {
					motion.velocity.x *= -1;
				}
				else if (!registry.roamers.has(entity) || motion.velocity.x > 0) {
					motion.position.x = std::max(new_x, half_width);
				}
			}
			else if (new_x + half_width >= window_width_px) {
				if (registry.roamers.has(entity) && motion.velocity.x > 0) {
					motion.velocity.x *= -1;
				}
				else if (!registry.roamers.has(entity) || motion.velocity.x < 0) {
					motion.position.x = std::min(new_x, window_width_px - half_width);
				}
			}
			else {
				motion.position.x = new_x;
			}

			// Check for Y-axis boundaries and adjust the position or velocity accordingly
			if (new_y - half_height <= 70) {
				if (registry.roamers.has(entity) && motion.velocity.y < 0) {
					motion.velocity.y *= -1;
				}
				else if (!registry.roamers.has(entity) || motion.velocity.y > 0) {
					motion.position.y = std::max(new_y, 70 + half_height);
				}
			}
			else if (new_y + half_height >= window_height_px) {
				if (registry.roamers.has(entity) && motion.velocity.y > 0) {
					motion.velocity.y *= -1;
				}
				else if (!registry.roamers.has(entity) || motion.velocity.y < 0) {
					motion.position.y = std::min(new_y, window_height_px - half_height);
				}
			}
			else {
				motion.position.y = new_y;
			}
			

			//Entity& mesh_entity = registry.Entity_Mesh_Entity.get(entity);
			//Motion& mesh_motion = motion;
			//if (registry.motions.has(mesh_entity)) {
			//	mesh_motion = registry.motions.get(mesh_entity);
			//	//mesh_motion = motion;
			//	mesh_motion.position = motion.position;
			//}

			Minion& minion = registry.minions.get(entity);
			

			minion.up = false;
			minion.down = false;
			minion.left = false;
			minion.right = false;
			if (motion.velocity.x < 0 && abs(motion.velocity.y) <= abs(motion.velocity.x)) {
				// going right
				minion.right = true;
			}
			else if (motion.velocity.x > 0 && abs(motion.velocity.y) <= abs(motion.velocity.x)) {
				// going left
				minion.left = true;
			}
			else if (motion.velocity.y > 0 && abs(motion.velocity.x) <= abs(motion.velocity.y)) {
				// going down
				minion.down = true;
			}
			else if (motion.velocity.y < 0 && abs(motion.velocity.x) <= abs(motion.velocity.y)) {
				// going up
				minion.up = true;
			}
			minion.counter_ms -= elapsed_ms;
			if (minion.counter_ms < 0.f) {
				minion.counter_ms = 120;

				minion.frame_stage += 1;
				if (minion.frame_stage > 2) {
					minion.frame_stage = 0;
				}
			}

		}
		// Power ups move around the map
		/*
		else if (registry.powerUps.has(entity)) {
			float new_x = motion.velocity.x * step_seconds + motion.position.x;
			float new_y = motion.velocity.y * step_seconds + motion.position.y;
			vec2 bounding_box = { abs(motion.scale.x), abs(motion.scale.y) };
			float half_width = bounding_box.x / 2.f;
			float half_height = bounding_box.y / 2.f;
			if (new_x - half_width <= 0 || new_x + half_width >= window_width_px) {
				if (registry.powerUps.has(entity)) {
					motion.velocity.x *= -1; // Invert X velocity upon boundary collision
					new_x = motion.velocity.x * step_seconds + motion.position.x; // Recalculate new_x after velocity inversion
				}
			}
			else {
				motion.position.x = new_x; // Update position if within bounds
			}

			if (new_y <= 180 || new_y + half_height - 20 >= window_height_px) {
				if (registry.powerUps.has(entity)) {
					motion.velocity.y *= -1; // Invert Y velocity upon boundary collision
					new_y = motion.velocity.y * step_seconds + motion.position.y; // Recalculate new_y after velocity inversion
				}
			}
			else {
				motion.position.y = new_y; // Update position if within bounds
			}
		}
		*/

		else {
			if (motion.position.x < 0.f || motion.position.x > window_width_px
				|| motion.position.y < 0 || motion.position.y > window_height_px) {

				registry.remove_all_components_of(motion_registry.entities[i]);
				continue;
			}

			
			motion.position.x += motion.velocity.x * step_seconds;
			motion.position.y += motion.velocity.y * step_seconds;
		}

	}


	// Vicky TODO M1: more blood loss, the screen will trun into black, until dead
	float bloodLossPercentage;
	bloodLossPercentage = std::max(0.0f, std::min(1.0f, bloodLossPercentage)) * 100.0f;
	float alphaFactor = 1.0f - bloodLossPercentage / 100.0f;
	//render_Screen(alphaFactor);

	// Check for collisions between all moving entities

	//grid.clearAllCells();
	//
	//for (const Entity& entity : registry.mesh_collision.entities) {
	//	if (registry.motions.has(entity)) {
	//		Motion& motion = registry.motions.get(entity);
	//		grid.addEntity(entity, motion);
	//	}
	//}

	// Collision check with grid optimization
	//for (const Entity& entity_i : registry.mesh_collision.entities) {
	//	if (registry.motions.has(entity_i)) {
	//		/*Motion& motion_i = registry.motions.get(entity_i);
	//		Grid grid = *registry.grid;
	//		grid.getNearbyEntities(entity_i, motion_i);*/
	//		//auto nearbyEntities = grid.getNearbyEntities(entity_i, motion_i);
	//		/*for (const Entity& entity_j : nearbyEntities) {
	//			if (entity_i == entity_j) continue;
	//			if (registry.motions.has(entity_j)) {
	//				Motion& motion_j = registry.motions.get(entity_j);
	//				if (collides(entity_i, entity_j, motion_i, motion_j))
	//				{
	//					registry.collisions.emplace_with_duplicates(entity_i, entity_j);
	//					registry.collisions.emplace_with_duplicates(entity_j, entity_i);
	//				}
	//			}
	//		}*/
	//	}
	//}

	ComponentContainer<Mesh_collision>& mesh_Collision_container = registry.mesh_collision;
	for (uint i = 0; i < mesh_Collision_container.entities.size(); i++)
	{
		Entity entity_i = mesh_Collision_container.entities[i];
		if (registry.motions.has(entity_i)) {
			Motion& motion_i = registry.motions.get(entity_i);
			// note starting j at i+1 to compare all (i,j) pairs only once (and to not compare with itself)
			for (uint j = i + 1; j < mesh_Collision_container.components.size(); j++)
			{
				Entity entity_j = mesh_Collision_container.entities[j];
				if (registry.motions.has(entity_j)) {
					Motion& motion_j = registry.motions.get(entity_j);
					if (collides(entity_i, entity_j, motion_i, motion_j))
					{
						// Create a collisions event
						// We are abusing the ECS system a bit in that we potentially insert muliple collisions for the same entity
						registry.collisions.emplace_with_duplicates(entity_i, entity_j);
						registry.collisions.emplace_with_duplicates(entity_j, entity_i);

					}
				}

			}
		}
		
	}
}







bool checkMeshCollisionSAT(Mesh* mesh, const Motion& motion_one, Mesh* otherMesh, const Motion& motion_two, const box overlapBox) {
	std::vector<vec2> axes;
	std::vector<vec2> shape;
	std::vector<vec2> otherShape;
	bool collision = false;
	Transform transform_one;
	Transform transform_two;
	transform_one.translate(motion_one.position);
	transform_one.rotate(motion_one.angle);
	transform_one.scale(motion_one.scale);
	transform_two.translate(motion_two.position);
	transform_two.rotate(motion_two.angle);
	transform_two.scale(motion_two.scale);

	for (size_t i = 0; i < mesh->vertex_indices.size(); i += 3) {
		if (i + 2 >= mesh->vertex_indices.size()) {
			continue;
		}

		shape.clear();
		for (int j = 0; j < 3; j++) {
			const ColoredVertex& v = mesh->vertices[mesh->vertex_indices[i + j]];
			vec3 worldPos = transform_one.mat * vec3(v.position.x, v.position.y, 1.0f);
			vec2 pos = vec2(worldPos.x, worldPos.y);
			if (isPointInBox(pos, overlapBox)) {
				shape.push_back(pos);
			}
		}

		if (shape.empty()) {
			continue;
		}

		axes.clear();
		calculateMeshAxes(shape, axes);

		for (size_t index = 0; index < otherMesh->vertex_indices.size(); index += 3) {
			if (index + 2 >= otherMesh->vertex_indices.size()) {
				continue;
			}

			otherShape.clear();
			for (int j = 0; j < 3; j++) {
				const ColoredVertex& v = otherMesh->vertices[otherMesh->vertex_indices[index + j]];
				vec3 worldPos = transform_two.mat * vec3(v.position.x, v.position.y, 1.0f);
				vec2 pos = vec2(worldPos.x, worldPos.y);
				if (isPointInBox(pos, overlapBox)) {
					otherShape.push_back(pos);
				}
			}

			if (otherShape.empty()) {
				continue;
			}

			calculateMeshAxes(otherShape, axes);

			if (checkForCollision(shape, otherShape, axes)) {
				return true;
			}
		}
	}
	return collision;
}

void calculateMeshAxes(const std::vector<vec2>& vertices, std::vector<vec2>& axes) {
	for (size_t i = 0; i < vertices.size(); i++) {
		vec2 edge = vertices[(i + 1) % vertices.size()] - vertices[i];
		vec2 normal = normalize(vec2(-edge.y, edge.x));
		if (!isParallel(axes, normal)) {
			axes.push_back(normal);
		}
	}
}

bool checkForCollision(const std::vector<vec2>& shape1, const std::vector<vec2>& shape2, const std::vector<vec2>& axes) {
	for (const vec2& axis : axes) {
		std::pair<float, float> projection1 = projectOntoAxis(shape1, axis);
		std::pair<float, float> projection2 = projectOntoAxis(shape2, axis);
		if (!projectionsOverlap(projection1, projection2)) {
			return false;
		}
	}
	return true;
}


vec2 normalize(const vec2& v) {
	float length = std::sqrt(v.x * v.x + v.y * v.y);
	return { v.x / length, v.y / length };
}


bool isParallel(const std::vector<vec2>& axis, const vec2& edge) {
	for (const auto& existing_axis : axis) {
		float crossProduct = edge.x * existing_axis.y - edge.y * existing_axis.x;
		if (std::abs(crossProduct) < 0.0000001) {
			return true;
		}
	}
	return false;
}

std::vector<vec2> getRectangleEdge(const Motion& motion, std::vector<vec2>& shape) {
	std::vector<vec2> rectangle(4);
	float halfWidth = abs(motion.scale.x) / 2.0f;
	float halfHeight = abs(motion.scale.y) / 2.0f;
	vec2 topLeft = { motion.position.x - halfWidth, motion.position.y + halfHeight };
	vec2 topRight = { motion.position.x + halfWidth, motion.position.y + halfHeight };
	vec2 bottomLeft = { motion.position.x - halfWidth, motion.position.y - halfHeight };
	vec2 bottomRight = { motion.position.x + halfWidth, motion.position.y - halfHeight };
	shape.push_back(topLeft);
	shape.push_back(topRight);
	shape.push_back(bottomLeft);
	shape.push_back(bottomRight);

	rectangle[0] = normalize(topRight - topLeft); // Top edge
	rectangle[1] = normalize(bottomRight - topRight); // Right edge
	rectangle[2] = normalize(bottomLeft - bottomRight); // Bottom edge
	rectangle[3] = normalize(topLeft - bottomLeft); // Left edge
	return rectangle;
}

//projectOntoAxis function reference https://dyn4j.org/2010/01/sat/
std::pair<float, float> projectOntoAxis(const std::vector<vec2>& shape, const vec2& axis) {
	float minProj = dot(shape[0], axis);
	float maxProj = minProj;
	for (const auto& point : shape) {
		float proj = dot(point, axis);
		minProj = std::min(minProj, proj);
		maxProj = std::max(maxProj, proj);
	}
	return { minProj, maxProj };
}
bool projectionsOverlap(const std::pair<float, float>& proj1, const std::pair<float, float>& proj2) {
	return !(proj1.second < proj2.first || proj2.second < proj1.first);
}

box calculate_overlap_area(const vec2& center1, const vec2& halfBB1, const vec2& center2, const vec2& halfBB2) {
	vec2 overlapLeftTop = { std::max(center1.x - halfBB1.x, center2.x - halfBB2.x), std::max(center1.y - halfBB1.y, center2.y - halfBB2.y) };
	vec2 overlapRightBottom = { std::min(center1.x + halfBB1.x, center2.x + halfBB2.x), std::min(center1.y + halfBB1.y, center2.y + halfBB2.y) };

	vec2 overlapCenter = (overlapLeftTop + overlapRightBottom) / 2.0f;
	vec2 overlapScale = overlapRightBottom - overlapLeftTop;
	if (overlapScale.x < 0 || overlapScale.y < 0) {
		return { {0, 0}, {0, 0} };
	}

	return { overlapCenter, overlapScale };
}

bool isPointInBox(const vec2& point, const box& bbox) {
	float left = bbox.center.x - bbox.scale.x;
	float right = bbox.center.x + bbox.scale.x;
	float bottom = bbox.center.y - bbox.scale.y;
	float top = bbox.center.y + bbox.scale.y;

	return point.x >= left && point.x <= right && point.y >= bottom && point.y <= top;
}