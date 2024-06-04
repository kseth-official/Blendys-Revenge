// internal
#include "render_system.hpp"

#include <iostream>
#include <SDL.h>

#include "tiny_ecs_registry.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


void RenderSystem::handle_health_bar_rendering(const RenderRequest& render_request, GLuint program)
{
	GLuint health_bar_texture_id = texture_gl_handles[(GLuint)TEXTURE_ASSET_ID::FULL_HEALTH_BAR];

	GLuint health_bar_effect_id = effects[(GLuint)EFFECT_ASSET_ID::HEALTH_BAR];

	// Use the health bar shader program
	glUseProgram(health_bar_effect_id);
	gl_has_errors();
	
	GLint in_position_loc = glGetAttribLocation(program, "in_position");

	gl_has_errors();

	glEnableVertexAttribArray(in_position_loc);
	glVertexAttribPointer(in_position_loc, 2, GL_FLOAT, GL_FALSE,
		sizeof(vec2), (void*)0);
	gl_has_errors();

	// Set the health bar texture as the active texture on texture unit 2
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, health_bar_texture_id);

	// Set uniform locations
	GLint bar_color_loc = glGetUniformLocation(program, "barColor");
	GLint health_percentage_loc = glGetUniformLocation(program, "healthPercentage");


	if //(bar_color_loc != -1 && health_percentage_loc != -1)
		(render_request.used_effect == EFFECT_ASSET_ID::HEALTH_BAR)
	{
		// Set the bar color (e.g., red)
		glUniform3f(bar_color_loc, 1.0f, 0.0f, 0.0f);

		// Set the health percentage 
		glUniform1f(health_percentage_loc, 0.75f);
	}
	else
	{
		// Handle error: barColor or healthPercentage uniform not found
		std::cerr << "Error: barColor or healthPercentage uniform not found in health bar shader" << std::endl;
	}

	// Render the health bar geometry 
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
	gl_has_errors();

	// Disable vertex attribute array
	glDisableVertexAttribArray(in_position_loc);
	gl_has_errors();

}

void RenderSystem::handle_particle_rendering(const RenderRequest& render_request, const GLuint& program, const mat3& projection, const Transform& transform)
{
	auto& particle_emitter_registry = registry.particleEmitters;

	for (int i = 0; i < particle_emitter_registry.size(); i++)
	{
		// Redundantly calling set program again
		glUseProgram(program);

		GLint M_v_loc = glGetUniformLocation(program, "M_v");
		GLint M_p_loc = glGetUniformLocation(program, "M_p");
		GLint particleSize_loc = glGetUniformLocation(program, "particleSize");
		GLint startColor_loc = glGetUniformLocation(program, "startColor");
		GLint endColor_loc = glGetUniformLocation(program, "endColor");
		GLint lifetime_loc = glGetUniformLocation(program, "lifetime");
		//assert(M_v_loc > -1);
		assert(M_p_loc > -1);
		assert(particleSize_loc > -1);
		assert(startColor_loc > -1);
		assert(endColor_loc > -1);
		assert(lifetime_loc > -1);
		gl_has_errors();

		// Setting uniform values to the currently bound program
		glUniformMatrix3fv(M_v_loc, 1, GL_FALSE, (float*)&transform.mat);
		gl_has_errors();

		glUniformMatrix3fv(M_p_loc, 1, GL_FALSE, (float*)&projection);
		gl_has_errors();

		auto& particle_emitter_entity = particle_emitter_registry.entities[i];
		auto& particle_emitter_component = particle_emitter_registry.components[i];
		auto& emitter_instance = particle_emitter_component.emitter_instance;

		glUniform3fv(startColor_loc, 1, &particle_emitter_component.particle_start_color[0]);
		gl_has_errors();

		glUniform3fv(endColor_loc, 1, &particle_emitter_component.particle_end_color[0]);
		gl_has_errors();

		auto& emitter_timer_registry = registry.emitterTimers;
		auto& emitter_timer = emitter_timer_registry.get(particle_emitter_entity);
		
		gl_has_errors();

		glUniform1f(particleSize_loc, particle_emitter_component.particle_size);
		gl_has_errors();

		glUniform1f(lifetime_loc, emitter_instance.get_base_lifetime());
		gl_has_errors();

		emitter_instance.draw();

		gl_has_errors();
	}
}


const char* fontVertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>\n"
"out vec2 TexCoords; \n"
"\n"
"uniform mat4 projection; \n"
"uniform mat4 transform;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = projection * transform * vec4(vertex.xy, 0.0, 1.0); \n"
"    TexCoords = vertex.zw; \n"
"}\0";

const char* fontFragmentShaderSource =
"#version 330 core\n"
"in vec2 TexCoords; \n"
"out vec4 color; \n"
"\n"
"uniform sampler2D text; \n"
"uniform vec3 textColor; \n"
"\n"
"void main()\n"
"{\n"
"    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
"    color = vec4(textColor, 1.0) * sampled;\n"
"}\0";

bool RenderSystem::fontInit_internal(const std::string& font_filename, unsigned font_default_size)
{
	// font buffer setup
	glGenVertexArrays(1, &m_font_VAO);
	glGenBuffers(1, &m_font_VBO);

	// font vertex shader
	unsigned int font_vertexShader;
	font_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(font_vertexShader, 1, &fontVertexShaderSource, NULL);
	glCompileShader(font_vertexShader);

	gl_has_errors();

	// font fragement shader
	unsigned int font_fragmentShader;
	font_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(font_fragmentShader, 1, &fontFragmentShaderSource, NULL);
	glCompileShader(font_fragmentShader);

	gl_has_errors();

	// font shader program
	m_font_shaderProgram = glCreateProgram();
	glAttachShader(m_font_shaderProgram, font_vertexShader);
	glAttachShader(m_font_shaderProgram, font_fragmentShader);
	glLinkProgram(m_font_shaderProgram);

	gl_has_errors();

	// clean up shaders
	glDeleteShader(font_vertexShader);
	glDeleteShader(font_fragmentShader);

	gl_has_errors();

	// use our new shader
	glUseProgram(m_font_shaderProgram);

	gl_has_errors();

	// apply projection matrix for font
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(window_width_px), 0.0f, static_cast<float>(window_height_px));
	GLint project_location = glGetUniformLocation(m_font_shaderProgram, "projection");
	assert(project_location > -1);
	std::cout << "project_location: " << project_location << std::endl;
	glUniformMatrix4fv(project_location, 1, GL_FALSE, glm::value_ptr(projection));

	// init FreeType fonts
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return false;
	}

	FT_Face face;
	if (FT_New_Face(ft, font_filename.c_str(), 0, &face))
	{
		std::cerr << "ERROR::FREETYPE: Failed to load font: " << font_filename << std::endl;
		return false;
	}

	// extract a default size
	FT_Set_Pixel_Sizes(face, 0, font_default_size);

	// disable byte-alignment restriction in OpenGL
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// load each of the chars - note only first 128 ASCII chars
	for (unsigned char c = 0; c < 128; c++)
	{
		// load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cerr << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// std::cout << "texture: " << c << " = " << texture << std::endl;

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<unsigned int>(face->glyph->advance.x),
			c
		};
		m_ftCharacters.insert(std::pair<char, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	gl_has_errors();

	// clean up
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	gl_has_errors();

	// bind buffers
	glBindVertexArray(m_font_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_font_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	gl_has_errors();

	// release buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	gl_has_errors();

	// Resetting the program to default state
	glUseProgram(0);

	gl_has_errors();

	return true;
}

bool RenderSystem::initializeFonts()
{
	return fontInit_internal(FONT_FOLDER_PATH + Kenney_Future_Narrow + DOT_TTF, FONT_DEFAULT_SIZE);
}

void RenderSystem::renderText(const std::string& text, float x, float y, float scale, const glm::vec3& color,
                              const glm::mat4& trans)
{
	// activate the shaders!
	glUseProgram(m_font_shaderProgram);

	gl_has_errors();

	unsigned int textColor_location =
		glGetUniformLocation(
			m_font_shaderProgram,
			"textColor"
		);
	assert(textColor_location >= 0);
	glUniform3f(textColor_location, color.x, color.y, color.z);

	auto transform_location = glGetUniformLocation(
		m_font_shaderProgram,
		"transform"
	);
	assert(transform_location > -1);
	glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(trans));

	glBindVertexArray(m_font_VAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = m_ftCharacters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		// Set the active texture (without this texture may be overriden)
		glActiveTexture(GL_TEXTURE0);

		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		//std::cout << "binding texture: " << ch.character << " = " << ch.TextureID << std::endl;

		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, m_font_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)

		gl_has_errors();
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	gl_has_errors();
}

void RenderSystem::particles_step(const float& elapsed_ms)
{
	auto& particle_emitters = registry.particleEmitters;

	for (int i = 0; i < particle_emitters.size(); i++)
	{
		auto& emitter_instance = particle_emitters.components[i].emitter_instance;
		emitter_instance.update(elapsed_ms);
		gl_has_errors();
	}
}

void RenderSystem::handle_normal_map_uniform(Entity entity, const GLuint program)
{
	GLuint normal_map_id =
		texture_gl_handles[(GLuint)registry.renderRequests.get(entity).used_normal_map];

	// Enabling and binding normal map texture to slot 1
	glActiveTexture(GL_TEXTURE1);
	gl_has_errors();
	glBindTexture(GL_TEXTURE_2D, normal_map_id);
	gl_has_errors();

	// Getting uniform locations for glUniform* calls
	GLint normal_map_uloc = glGetUniformLocation(program, "normal_map");
	assert(normal_map_uloc >= 0);
	glUniform1i(normal_map_uloc, 1);
	gl_has_errors();
}

void RenderSystem::setUsesNormalMap(bool cond, const GLuint program)
{
	// Getting uniform locations for glUniform* calls
	GLint usesNormalMap_uloc = glGetUniformLocation(program, "usesNormalMap");
	glUniform1i(usesNormalMap_uloc, cond);
	gl_has_errors();
}

void RenderSystem::handle_textured_rendering(Entity entity, const GLuint program, const RenderRequest& render_request)
{
	// Redundantly ensuring program is as expected
	/*glUseProgram(program);
	gl_has_errors();*/

	GLint in_position_loc = glGetAttribLocation(program, "in_position");
	GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");
	gl_has_errors();
	assert(in_texcoord_loc >= 0);

	glEnableVertexAttribArray(in_position_loc);
	gl_has_errors();
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
	                      sizeof(TexturedVertex), (void *)0);
 	gl_has_errors();

	glEnableVertexAttribArray(in_texcoord_loc);
	glVertexAttribPointer(
		in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
		(void *)sizeof(
			vec3)); // note the stride to skip the preceeding vertex position
	gl_has_errors();

	assert(registry.renderRequests.has(entity));
	GLuint base_texture_id =
		texture_gl_handles[(GLuint)registry.renderRequests.get(entity).used_texture];

	// Enabling and binding base texture to slot 0
	glActiveTexture(GL_TEXTURE0);
	gl_has_errors();
	glBindTexture(GL_TEXTURE_2D, base_texture_id);
	gl_has_errors();

	// Handle normal map uniform if normal map exists for texture
	if (render_request.used_normal_map != TEXTURE_ASSET_ID::TEXTURE_COUNT)
	{
		handle_normal_map_uniform(entity, program);
	}

	handle_giant_uniform(entity, program);
}

void RenderSystem::handle_chicken_or_egg_effect_rendering(const RenderRequest& render_request, const GLuint program)
{
	GLint in_position_loc = glGetAttribLocation(program, "in_position");
	GLint in_color_loc = glGetAttribLocation(program, "in_color");
	gl_has_errors();

	glEnableVertexAttribArray(in_position_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
	                      sizeof(ColoredVertex), (void *)0);
	gl_has_errors();

	glEnableVertexAttribArray(in_color_loc);
	glVertexAttribPointer(in_color_loc, 3, GL_FLOAT, GL_FALSE,
	                      sizeof(ColoredVertex), (void *)sizeof(vec3));
	gl_has_errors();

	if (render_request.used_effect == EFFECT_ASSET_ID::CHICKEN)
	{
		// Light up?
		GLint light_up_uloc = glGetUniformLocation(program, "light_up");
		assert(light_up_uloc >= 0);

		// !!! TODO A1: set the light_up shader variable using glUniform1i,
		// similar to the glUniform1f call below. The 1f or 1i specified the type, here a single int.
		gl_has_errors();
	}
}

void RenderSystem::configure_base_uniforms(Entity entity, const mat3& projection, Transform transform, const GLuint program, GLsizei& out_num_indices, const RenderRequest& render_request)
{

	// Getting uniform locations for glUniform* calls
	GLint color_uloc = glGetUniformLocation(program, "fcolor");
	const vec3 color = registry.colors.has(entity) ? registry.colors.get(entity) : vec3(1);
	glUniform3fv(color_uloc, 1, (float *)&color);
	gl_has_errors();

	setUsesNormalMap(render_request.used_normal_map != TEXTURE_ASSET_ID::TEXTURE_COUNT, program);

	// Lighting Config
	const LightSource& directional_light_component = registry.lightSources.get(directional_light);
	const Motion& motion = registry.motions.get(directional_light);

	// Configuring lightPosition 
	GLint lightPosition_uloc = glGetUniformLocation(program, "lightPosition");
	glUniform3fv(lightPosition_uloc, 1, (float*)& vec3(motion.position, directional_light_component.z_depth));
	gl_has_errors();

	// Configuring lightColor
	GLint lightColor_uloc = glGetUniformLocation(program, "lightColor");
	glUniform3fv(lightColor_uloc, 1, (float*)&directional_light_component.light_color);
	gl_has_errors();

	// Configuring shinyness
	GLint shininess_uloc = glGetUniformLocation(program, "shininess");
	glUniform1f(shininess_uloc, (float) directional_light_component.shininess);
	gl_has_errors();

	// Configuring ambientIntensity
	GLint ambientIntensity_uloc = glGetUniformLocation(program, "ambientIntensity");
	glUniform1f(ambientIntensity_uloc, (float)directional_light_component.ambientIntensity);
	gl_has_errors();

	// Configuring eyePosition
	GLint cameraPosition_uloc = glGetUniformLocation(program, "cameraPosition");
	glUniform3fv(cameraPosition_uloc, 1, (float*)&directional_light_component.camera_position);
	gl_has_errors();

	// Get number of indices from index buffer, which has elements uint16_t
	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	gl_has_errors();

	out_num_indices = size / sizeof(uint16_t);
	// GLsizei num_triangles = out_num_indices / 3;

	GLint currProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);

	// Setting uniform values to the currently bound program
	GLuint transform_loc = glGetUniformLocation(currProgram, "transform");
	glUniformMatrix3fv(transform_loc, 1, GL_FALSE, (float *)&transform.mat);
	GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
	glUniformMatrix3fv(projection_loc, 1, GL_FALSE, (float *)&projection);
	gl_has_errors();
}

void RenderSystem::handle_giant_uniform(const Entity entity, const GLuint program)
{
	GLuint isGiant_uloc = glGetUniformLocation(program, "isGiant");
	assert(isGiant_uloc >= 0);

	if (registry.giants.has(entity))
	{
		glUniform1i(isGiant_uloc, 1);
	} else
	{
		glUniform1i(isGiant_uloc, 0);
		return;
	}

	GLuint time_uloc = glGetUniformLocation(program, "time");
	assert(time_uloc >= 0);

	// Setting Time
	glUniform1f(time_uloc, (float)(glfwGetTime() * 10.0f));
}

// TODO: A number of code smells in this function that need to be cleaned up
void RenderSystem::drawTexturedMesh(Entity entity,
									const mat3 &projection)
{
	Motion &motion = registry.motions.get(entity);
	// Transformation code, see Rendering and Transformation in the template
	// specification for more info Incrementally updates transformation matrix,
	// thus ORDER IS IMPORTANT
	Transform transform;
	transform.translate(motion.position);
	transform.rotate(motion.angle);
	transform.scale(motion.scale);

	assert(registry.renderRequests.has(entity));
	const RenderRequest &render_request = registry.renderRequests.get(entity);


	const GLuint used_effect_enum = (GLuint)render_request.used_effect;
	assert(used_effect_enum != (GLuint)EFFECT_ASSET_ID::EFFECT_COUNT);
	const GLuint program = (GLuint)effects[used_effect_enum];

	// Setting shaders
	glUseProgram(program);
	gl_has_errors();

	assert(render_request.used_geometry != GEOMETRY_BUFFER_ID::GEOMETRY_COUNT);
	const GLuint vbo = vertex_buffers[(GLuint)render_request.used_geometry];
	const GLuint ibo = index_buffers[(GLuint)render_request.used_geometry];

	// Setting vertex and index buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	gl_has_errors();

	// Input data location as in the vertex buffer
	if (render_request.used_effect == EFFECT_ASSET_ID::TEXTURED)
	{
		handle_textured_rendering(entity, program, render_request);
	}
	else if (render_request.used_effect == EFFECT_ASSET_ID::CHICKEN || render_request.used_effect == EFFECT_ASSET_ID::EGG)
	{
		handle_chicken_or_egg_effect_rendering(render_request, program);
	}
	else if (render_request.used_effect == EFFECT_ASSET_ID::HEALTH_BAR)
	{
		handle_health_bar_rendering(render_request, program);
	} else if (render_request.used_effect == EFFECT_ASSET_ID::PARTICLES)
	{
		handle_particle_rendering(render_request, program, projection, transform);
	}
	else
	{
		assert(false && "Type of render request not supported");
	}

	GLsizei out_num_indices;
	configure_base_uniforms(entity, projection, transform, program, out_num_indices, render_request);

	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, out_num_indices, GL_UNSIGNED_SHORT, nullptr);
	gl_has_errors();
}

// draw the intermediate texture to the screen, with some distortion to simulate
// wind
void RenderSystem::drawToScreen()
{
	// Setting shaders
	// get the wind texture, sprite mesh, and program
	glUseProgram(effects[(GLuint)EFFECT_ASSET_ID::WIND]);
	gl_has_errors();
	// Clearing backbuffer
	int w, h;
	glfwGetFramebufferSize(window, &w, &h); // Note, this will be 2x the resolution given to glfwCreateWindow on retina displays
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, w, h);
	glDepthRange(0, 10);
	glClearColor(1.f, 0, 0, 1.0);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gl_has_errors();
	// Enabling alpha channel for textures
	glDisable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	// Draw the screen texture on the quad geometry
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[(GLuint)GEOMETRY_BUFFER_ID::SCREEN_TRIANGLE]);
	glBindBuffer(
		GL_ELEMENT_ARRAY_BUFFER,
		index_buffers[(GLuint)GEOMETRY_BUFFER_ID::SCREEN_TRIANGLE]); // Note, GL_ELEMENT_ARRAY_BUFFER associates
																	 // indices to the bound GL_ARRAY_BUFFER
	gl_has_errors();
	const GLuint wind_program = effects[(GLuint)EFFECT_ASSET_ID::WIND];
	// Set clock
	GLuint time_uloc = glGetUniformLocation(wind_program, "time");
	GLuint dead_timer_uloc = glGetUniformLocation(wind_program, "darken_screen_factor");
	glUniform1f(time_uloc, (float)(glfwGetTime() * 10.0f));
	ScreenState &screen = registry.screenStates.get(screen_state_entity);
	glUniform1f(dead_timer_uloc, screen.darken_screen_factor);
	gl_has_errors();
	// Set the vertex position and vertex texture coordinates (both stored in the
	// same VBO)
	GLint in_position_loc = glGetAttribLocation(wind_program, "in_position");
	glEnableVertexAttribArray(in_position_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void *)0);
	gl_has_errors();

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	gl_has_errors();

	glBindTexture(GL_TEXTURE_2D, off_screen_render_buffer_color);
	gl_has_errors();
	// Draw
	glDrawElements(
		GL_TRIANGLES, 3, GL_UNSIGNED_SHORT,
		nullptr); // one triangle = 3 vertices; nullptr indicates that there is
				  // no offset from the bound index buffer
	gl_has_errors();
}

// Render our game world
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
void RenderSystem::draw()
{
	// Getting size of window
	int w, h;
	glfwGetFramebufferSize(window, &w, &h); // Note, this will be 2x the resolution given to glfwCreateWindow on retina displays

	// First render to the custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	gl_has_errors();
	// Clearing backbuffer
	glViewport(0, 0, w, h);
	glDepthRange(0.00001, 10);
	glClearColor(0.66, 0.73, 0.76 , 1.0);
	glClearDepth(10.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST); // native OpenGL does not work with a depth buffer
							  // and alpha blending, one would have to sort
							  // sprites back to front
	gl_has_errors();

	// Rebinding dummy_vao here
	glBindVertexArray(dummy_vao);
	gl_has_errors();

	mat3 projection_2D = createProjectionMatrix();
	// Draw all textured meshes that have a position and size component
	// Phase 1: Draw all entities except bullets


	for (Entity entity : registry.renderRequests.entities)
	{
		if (!registry.motions.has(entity) || registry.bullets.has(entity) || registry.powerUps.has(entity) || registry.helpScreens.has(entity) || registry.cursor.has(entity) || registry.panel.has(entity) || registry.cutScenes.has(entity))
			continue;

		drawTexturedMesh(entity, projection_2D);
	}

	// Phase 2: Draw only bullet entities
	for (Entity entity : registry.renderRequests.entities)
	{
		if (!registry.motions.has(entity) || !(registry.bullets.has(entity) || registry.powerUps.has(entity)))
			continue; // Skip non-bullet entities in this phase

		drawTexturedMesh(entity, projection_2D);
	}

	for (Entity entity : registry.renderRequests.entities)
	{
		if (!registry.motions.has(entity) || !(registry.tooltip.has(entity) || registry.helpScreens.has(entity) || registry.panel.has(entity)))
			continue; // Skip non-bullet entities in this phase

		drawTexturedMesh(entity, projection_2D);
	}
	for (Entity entity : registry.renderRequests.entities)
	{
		if (!registry.motions.has(entity) || !registry.cutScenes.has(entity))
			continue; // Skip non-bullet entities in this phase

		drawTexturedMesh(entity, projection_2D);
	}
	for (Entity entity : registry.renderRequests.entities) {
		if (!registry.cursor.has(entity))
			continue;
		drawTexturedMesh(entity, projection_2D);
	}

	debug_fps(projection_2D);
	display_score();
	display_text();

	// Rebinding dummy_vao here
	glBindVertexArray(dummy_vao);
	gl_has_errors();
	
	// Truely render to the screen
	drawToScreen();

	// flicker-free display with a double buffer
	glfwSwapBuffers(window);
	gl_has_errors();
}

mat3 RenderSystem::createProjectionMatrix()
{
	// Fake projection matrix, scales with respect to window coordinates
	float left = 0.f;
	float top = 0.f;

	gl_has_errors();
	float right = (float) window_width_px;
	float bottom = (float) window_height_px;

	float sx = 2.f / (right - left);
	float sy = 2.f / (top - bottom);
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	return {{sx, 0.f, 0.f}, {0.f, sy, 0.f}, {tx, ty, 1.f}};
}