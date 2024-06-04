#pragma once

#include <random>
#include "common.hpp"

class Emitter
{
public:
	Emitter();
	~Emitter();

	void update(const float& dt);
	void draw();
	void seed_random_number_generator();
	float get_base_lifetime() const;
	
	void init(
		const vec2& emitter_position,
		const float& base_lifetime,
		const float& centre_offset,
		const float& outward_velocity_magnitude
	);

private:
	struct Particle
	{
		vec3 position;
		float angle;
		float lifetime;
		float velocity_magnitude;
		vec3 start_color;
		vec3 end_color;
	};

	std::vector<Particle> particles;
	std::vector<float> positions;

	GLuint vertexBuffer;
	GLuint positionBuffer;

	float initial_lifetime;

	std::default_random_engine rng;
	std::uniform_real_distribution<float> uniform_dist; // number between 0..1

	// Helper Functions
	void create_square_vertices(std::vector<float>& out_vertices);
	void set_particle_attributes(
		unsigned i,
		const vec2& emitter_position,
		const float& base_lifetime,
		const float centre_offset,
		const float& velocity_magnitude,
		const vec3& end_color, const vec3& start_color
	);
};