#pragma once
#include <vector>

#include "tiny_ecs.hpp"
#include "components.hpp"

class ECSRegistry
{
	// Callbacks to remove a particular or all entities in the system
	std::vector<ContainerInterface*> registry_list;

public:
	// Manually created list of all components this game has
	// TODO: A1 add a LightUp component
	ComponentContainer<DeathTimer> deathTimers;
	ComponentContainer<Motion> motions;
	ComponentContainer<Collision> collisions;
	ComponentContainer<Player> players;
	ComponentContainer<Mesh*> meshPtrs;
	ComponentContainer<RenderRequest> renderRequests;
	ComponentContainer<ScreenState> screenStates;
	ComponentContainer<Minion> minions;
	ComponentContainer<Eatable> eatables;
	ComponentContainer<DebugComponent> debugComponents;
	ComponentContainer<vec3> colors;
	ComponentContainer<Background> backgrounds;
	ComponentContainer<LightSource> lightSources;
	ComponentContainer<FpsCounter> fpsCounters;
	ComponentContainer<ScoreCounter> scoreCounters;
	ComponentContainer<Roamer> roamers;
	ComponentContainer<HelpScreen> helpScreens;
	ComponentContainer<Bullet> bullets;
	ComponentContainer<Shooter> shooters;
	ComponentContainer<PowerUp> powerUps;
	ComponentContainer<EnemyBullet> enemyBullets;
	ComponentContainer<Mesh_collision> mesh_collision;
	ComponentContainer<Panel> panel;
	ComponentContainer<CutScene> cutScenes;
	ComponentContainer<Charger> chargers;
	ComponentContainer<Giant> giants;
	ComponentContainer<Healer> healers;
	ComponentContainer<Tank> tanks;
	ComponentContainer<Sniper> snipers;
	ComponentContainer<Protection> protections;
	ComponentContainer<ParticleEmitter> particleEmitters;
	ComponentContainer<EmitterTimer> emitterTimers;
	ComponentContainer<Cleaner> cleaners;
	ComponentContainer<SplitShooter> splitShooters;
	ComponentContainer<Loot> loots;
	ComponentContainer<Boss> boss;
	ComponentContainer<Entity> Entity_to_Bullet_Mesh_Entity;
	ComponentContainer<Shield>shields;
	ComponentContainer<Entity> Entity_Mesh_Entity;
	ComponentContainer<Mesh_entity> Mesh_entity;
	ComponentContainer<Cursor> cursor;
	ComponentContainer<ToolTip> tooltip;
  
	int score = 0;
	bool is_pause = false;
	bool is_winning = false;
	bool is_dead = false;
	bool boss_spawned = false;
	bool is_minimized = false;

	// constructor that adds all containers for looping over them
	// IMPORTANT: Don't forget to add any newly added containers!
	ECSRegistry()
	{
		registry_list.push_back(&deathTimers);
		registry_list.push_back(&motions);
		registry_list.push_back(&collisions);
		registry_list.push_back(&players);
		registry_list.push_back(&meshPtrs);
		registry_list.push_back(&renderRequests);
		registry_list.push_back(&screenStates);
		registry_list.push_back(&minions);
		registry_list.push_back(&debugComponents);
		registry_list.push_back(&colors);
		registry_list.push_back(&backgrounds);
		registry_list.push_back(&lightSources);
		registry_list.push_back(&fpsCounters);
		registry_list.push_back(&bullets);
		registry_list.push_back(&shooters);
		registry_list.push_back(&powerUps);
		registry_list.push_back(&enemyBullets);
		registry_list.push_back(&scoreCounters);
		registry_list.push_back(&roamers);
		registry_list.push_back(&helpScreens);
		registry_list.push_back(&panel);
		registry_list.push_back(&cutScenes);
		registry_list.push_back(&chargers);
		registry_list.push_back(&healers);
		registry_list.push_back(&giants);
		registry_list.push_back(&tanks);
		registry_list.push_back(&snipers);
		registry_list.push_back(&cleaners);
		registry_list.push_back(&splitShooters);
		registry_list.push_back(&loots);
		registry_list.push_back(&boss);
		registry_list.push_back(&protections);
		registry_list.push_back(&Mesh_entity);
		registry_list.push_back(&particleEmitters);
		registry_list.push_back(&emitterTimers);
	}

	void clear_all_components() {
		for (ContainerInterface* reg : registry_list)
			reg->clear();
	}

	void list_all_components() {
		printf("Debug info on all registry entries:\n");
		for (ContainerInterface* reg : registry_list)
			if (reg->size() > 0)
				printf("%4d components of type %s\n", (int)reg->size(), typeid(*reg).name());
	}

	void list_all_components_of(Entity e) {
		printf("Debug info on components of entity %u:\n", (unsigned int)e);
		for (ContainerInterface* reg : registry_list)
			if (reg->has(e))
				printf("type %s\n", typeid(*reg).name());
	}

	void remove_all_components_of(Entity e) {
		for (ContainerInterface* reg : registry_list)
			reg->remove(e);
	}
};

extern ECSRegistry registry;