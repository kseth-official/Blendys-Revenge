#include "particle_system.hpp"

constexpr int NUMBER_OF_PARTICLES_PER_INSTANCE = 100;
constexpr int NUMBER_OF_VERTICES_IN_SQUARE = 4;
constexpr float THREE_SIXTY_DEGREES = 360.f;

// DEFAULT VALUES
const vec2& DEFAULT_EMITTER_POSITION = vec2{ window_width_px / 2, window_height_px / 2 };
const float& DEFAULT_BASE_LIFETIME = (float)10.f;
const float DEFAULT_CENTRE_OFFSET = 30.f;
const float& DEFAULT_VELOCITY_MAGNITUDE = 0.05f;
const vec3& DEFAULT_START_COLOR = vec3{ 1.0,0.0,0.0 };
const vec3& DEFAULT_END_COLOR = vec3{ 1.0,1.0,1.0 };

Emitter::Emitter()
{

}

Emitter::~Emitter()
{
	glDeleteBuffers(1, &this->vertexBuffer);
	glDeleteBuffers(1, &this->positionBuffer);

	gl_has_errors();
}

void Emitter::update(const float& dt)
{
	for (uint i = 0; i < this->particles.size(); ++i)
	{
		// subtract from the particles lifetime
		this->particles[i].lifetime -= dt;

		auto& current_angle = this->particles[i].angle;
		float velocity_magnitude = this->particles[i].velocity_magnitude; // The magnitude of the velocity vector of the particles
		float x_velocity = velocity_magnitude * cos(glm::radians(current_angle));
		float y_velocity = velocity_magnitude * sin(glm::radians(current_angle));

		// move the particle outwards depending on the delta time
		this->particles[i].position += dt * vec3(x_velocity, y_velocity, 0.0f);

		// update the position buffer
		this->positions[i * 4 + 0] = this->particles[i].position[0];
		this->positions[i * 4 + 1] = this->particles[i].position[1];
		this->positions[i * 4 + 2] = this->particles[i].position[2];
		this->positions[i * 4 + 3] = this->particles[i].lifetime;
	}
}

void Emitter::draw()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(4);

	gl_has_errors();

	// update the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);

	gl_has_errors();
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->particles.size() * 4 * sizeof(float), this->positions.data()); // throws INVALID_VALUE

	gl_has_errors();

	// vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	gl_has_errors();

	// position buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	glVertexAttribDivisor(4, 1);

	gl_has_errors();

	// draw triangles
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, this->particles.size());

	gl_has_errors();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(4);

	gl_has_errors();
}

void Emitter::seed_random_number_generator()
{
	auto time_since_beginning = glfwGetTime();
	auto new_seed = time_since_beginning * 10000000;
	rng.seed(new_seed);
}

float Emitter::get_base_lifetime() const
{
	return this->initial_lifetime;
}

void Emitter::set_particle_attributes(
	unsigned int i,
	const vec2& emitter_position = DEFAULT_EMITTER_POSITION,
	const float& base_lifetime = DEFAULT_BASE_LIFETIME,
	const float centre_offset = DEFAULT_CENTRE_OFFSET,
	const float& velocity_magnitude = DEFAULT_VELOCITY_MAGNITUDE,
	const vec3& start_color = DEFAULT_START_COLOR,
	const vec3& end_color = DEFAULT_END_COLOR
	)
{
	seed_random_number_generator();

	const float lifetime = base_lifetime;
	const float random_angle = uniform_dist(rng) * THREE_SIXTY_DEGREES;
	const float random_position_x = emitter_position.x + (uniform_dist(rng) - 0.5f) * centre_offset; 
	const float random_position_y = emitter_position.y + (uniform_dist(rng) - 0.5f) * centre_offset;

	this->particles[i].position = vec3(random_position_x, random_position_y, 1.0);
	this->particles[i].lifetime = lifetime;
	this->particles[i].angle = random_angle;
	this->particles[i].start_color = start_color;
	this->particles[i].end_color = end_color;
	this->particles[i].velocity_magnitude = velocity_magnitude;
}

void Emitter::create_square_vertices(std::vector<float>& out_vertices)
{
	// Bottom Left
	out_vertices.push_back(0.0f);
	out_vertices.push_back(0.0f);
	out_vertices.push_back(0.0f);

	// Bottom Right
	out_vertices.push_back(1.0f);
	out_vertices.push_back(0.0f);
	out_vertices.push_back(0.0f);

	// Top Left
	out_vertices.push_back(0.0f);
	out_vertices.push_back(1.0f);
	out_vertices.push_back(0.0f);

	// Top Right
	out_vertices.push_back(1.0f);
	out_vertices.push_back(1.0f);
	out_vertices.push_back(0.0f);
}

void Emitter::init(
	const vec2& emitter_position,
	const float& base_lifetime,
	const float& centre_offset,
	const float& outward_velocity_magnitude
)
{
	this->initial_lifetime = base_lifetime;

	// create number_of_particles particles
	this->particles.resize(NUMBER_OF_PARTICLES_PER_INSTANCE);
	for (uint i = 0; i < this->particles.size(); ++i)
	{
		set_particle_attributes(i, emitter_position, base_lifetime, centre_offset, outward_velocity_magnitude);
	}

	// create a vertex and position buffer
	glGenBuffers(1, &this->vertexBuffer);
	glGenBuffers(1, &this->positionBuffer);

	gl_has_errors();

	std::vector<float> out_vertices;
	create_square_vertices(out_vertices);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(float), out_vertices.data(), GL_STATIC_DRAW);

	gl_has_errors();

	// 4 * to represent 4 positions for each vertex of a square
	positions.resize(NUMBER_OF_PARTICLES_PER_INSTANCE * NUMBER_OF_VERTICES_IN_SQUARE);

	// fill the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->particles.size() * 4 * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);

	gl_has_errors();
}