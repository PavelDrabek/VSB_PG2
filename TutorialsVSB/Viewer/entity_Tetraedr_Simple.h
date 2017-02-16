////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	entity_Tetraedr_Simple.h
//
// summary:	Declaration and implementation of Entity_Tetraedr class
// author:	Pavel Drabek
// 
// Copyright © 2017 Pavel Drabek. All Rights Reserved.
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __ENTITY_TETRAEDR_SIMPLE_H_
#define __ENTITY_TETRAEDR_SIMPLE_H_

#include "stdafx.h"
#include "entity_VAO.h"

class Entity_Tetraedr_Simple : public Entity_VAO
{
public:
	Entity_Tetraedr_Simple(VAO* vao = nullptr) : Entity_VAO(vao) {}
	~Entity_Tetraedr_Simple(void) {}

	void draw(const unsigned int eid = 0);
};

inline void Entity_Tetraedr_Simple::draw(const unsigned int eid)
{
	if (!m_isInitialized) return;

	glBindVertexArray(m_vao->m_object);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

#endif