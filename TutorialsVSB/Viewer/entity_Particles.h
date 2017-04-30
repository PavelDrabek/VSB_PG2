////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	entity_Particles.h
//
// summary:	Declaration and implementation of Entity_Particles class
// author:	Petr Gajdoš
// 
// Copyright © 2014 Petr Gajdoš. All Rights Reserved.
/////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __Entity_Particles_H_
#define __Entity_Particles_H_

#include "stdafx.h"

#include <glm.hpp>

#include "entity_VAO.h"
#include "vao_particle.h"

struct Particle {
	glm::vec3 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;

	Particle()
		: Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

class Entity_Particles : public Entity_VAO
{
protected:
	VAO_Particle* vao_particle;
	
	GLuint lastUsedParticle = 0;

	GLuint nr_particles = 500;
	std::vector<Particle> particles;

public:
	Entity_Particles(VAO* vao) : Entity_VAO(vao) {
		vao_particle = (VAO_Particle*)vao;

		// Create this->amount default particle instances
		for (GLuint i = 0; i < nr_particles; ++i)
			this->particles.push_back(Particle());

	}
	virtual ~Entity_Particles(void) {}

	virtual void init();
	virtual void update();
	virtual void draw(const unsigned int eid = 0);
	
	GLuint FirstUnusedParticle();
	void RespawnParticle(Particle &particle, glm::vec3 velocity, glm::vec3 offset);
};

inline void Entity_Particles::init()
{
	if ((m_vao) && (vao_particle))
	{
		m_isInitialized = true;
	}
}

GLuint Entity_Particles::FirstUnusedParticle()
{
	// Search from last used particle, this will usually return almost instantly
	for (GLuint i = lastUsedParticle; i < nr_particles; ++i) {
		if (particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Otherwise, do a linear search
	for (GLuint i = 0; i < lastUsedParticle; ++i) {
		if (particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Override first particle if all others are alive
	//lastUsedParticle = 0;
	return -1;
}

void Entity_Particles::RespawnParticle(Particle &particle, glm::vec3 velocity, glm::vec3 offset)
{
	GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat rColor = 0.5f + ((rand() % 100) / 100.0f);
	particle.Position = offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 0.5f);
	particle.Life = 1.0f;
	particle.Velocity = velocity;
}

float getRandom()
{
	return (rand() % 200) * 0.01f - 1;
}

inline void Entity_Particles::update() {
	GLuint nr_new_particles = 1;
	GLfloat dt = 0.01f;
	// Add new particles
	for (GLuint i = 0; i < nr_new_particles; ++i)
	{
		int unusedParticle = FirstUnusedParticle();
		if (unusedParticle >= 0) {
			vec3 offset = vec3(getRandom(), getRandom(), getRandom() * 0.2f) * 0.5f;
			RespawnParticle(particles[unusedParticle], glm::vec3(0, 0, 2), offset);
		}
	}
	// Uupdate all particles
	for (GLuint i = 0; i < nr_particles; ++i)
	{
		Particle &p = particles[i];
		p.Life -= dt; // reduce life
		if (p.Life > 0.0f)
		{	// particle is alive, thus update
			p.Position += p.Velocity * dt;
			//p.Color.a -= dt * 2.5;
		}
	}
}

inline void Entity_Particles::draw(const unsigned int eid)
{
	if (!m_isInitialized) return;

	SceneSetting *ss = SceneSetting::GetInstance();
	glm::mat4 mvMatrix = ss->m_activeCamera->getVM() * m_modelMatrix;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	// transparent
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);


	for (Particle particle : particles)
	{
		if (particle.Life > 0.0f)
		{
			int uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
			glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
			uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
			glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());
			uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "MVMatrix");
			glUniformMatrix4fv(uniform, 1, GL_FALSE, (float*)&mvMatrix[0]);
			uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "offset");
			glUniform3fv(uniform, 1, (float*)&particle.Position[0]);
			uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "life");
			glUniform1fv(uniform, 1, &particle.Life);

			if ((uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "sprite")) >= 0) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_material->m_diffuseTextureGL);
				glUniform1i(uniform, 0);			//Texture unit is set = sampler will access GL_TEXTURE0 = texture unit 0. DO NOT SET the diffuse texture diffuseTextureGL!!! 
			}

			glBindVertexArray(vao_particle->m_object);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			glBindVertexArray(0);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);


}

#endif