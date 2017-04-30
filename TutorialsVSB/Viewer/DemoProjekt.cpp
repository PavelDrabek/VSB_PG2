#include "DemoProjekt.h"

#include <time.h>
#include <chrono>
#include <math.h>
#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao_CubeV3C4N3T2.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_Cube.h"
#include "entity_OBJ.h"
#include "entity_Particles.h"

using namespace std::chrono;

void DemoProjekt::loadShader(ShaderProgram *sp, int index)
{
	if (index < m_sceneData->shaderPrograms.size()) {
		m_sceneData->shaderPrograms[index] = sp;
	}
	else {
		m_sceneData->shaderPrograms.push_back(sp);
	}
}


void DemoProjekt::initShaders()
{
	reloadShaders();

	//addResPath("shaders/");
	//initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag");
	//initShaderProgram("adsOBJ_v3_n3_t3_displacement.vert", "ads_v3_n3_t3_norm_depth.frag", 0, "adsOBJ_v3_n3_t3_displacement.cont", "adsOBJ_v3_n3_t3_displacement_noise.eval");
	//initShaderProgram("adsOBJ_v3_n3_t3_displacement.vert", "ads_v3_n3_t3_norm_depth_nophong.frag", 0, "adsOBJ_v3_n3_t3_displacement.cont", "adsOBJ_v3_n3_t3_displacement_lava.eval");

	resetResPath();
}

void DemoProjekt::reloadShaders()
{
	printf("reloading shaders\n");
	
	addResPath("shaders/");
	ShaderProgram *sp;
	sp = initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag");
	loadShader(sp, 0);
	sp = initShaderProgram("adsOBJ_v3_n3_t3_displacement.vert", "ads_v3_n3_t3_heart.frag", 0, "adsOBJ_v3_n3_t3_displacement.cont", "adsOBJ_v3_n3_t3_displacement_noise.eval");
	loadShader(sp, 1);
	sp = initShaderProgram("adsOBJ_v3_n3_t3_displacement_lava.vert", "ads_v3_n3_t3_lava.frag", 0, "adsOBJ_v3_n3_t3_displacement.cont", "adsOBJ_v3_n3_t3_displacement_lava.eval");
	loadShader(sp, 2);
	sp = initShaderProgram("particle.vert", "particle.frag");
	loadShader(sp, 3);

	resetResPath();
}

void DemoProjekt::initModels()
{
	ObjLoader objL;
	Model* m;

	addResPath("models/");

	//m = objL.loadModel(getResFile("basic/sphereFix2.obj"));
	m = objL.loadModel(getResFile("Heart2.obj"));
	m_sceneData->models.push_back(m);

	m = objL.loadModel(getResFile("basic/plane.obj"));
	m_sceneData->models.push_back(m);


	resetResPath();
}

void DemoProjekt::initVAOs()
{
	
	VAO_SceneOrigin* vao0 = new VAO_SceneOrigin();
	vao0->init();
	m_sceneData->vaos.push_back(vao0);

	VAO_GridXY* vao1 = new VAO_GridXY();
	vao1->init();
	m_sceneData->vaos.push_back(vao1);

	VAO_CubeV3C4N3T2* vao2 = new VAO_CubeV3C4N3T2();
	vao2->init();
	m_sceneData->vaos.push_back(vao2);

	VAO* vaoObj = new VAO();
	vaoObj->createFromModelWithTBN(m_sceneData->models[0]);
	m_sceneData->vaos.push_back(vaoObj);

	vaoObj = new VAO();
	vaoObj->createFromModelWithTBN(m_sceneData->models[1]);
	m_sceneData->vaos.push_back(vaoObj);

	VAO_Particle* vaoParticles = new VAO_Particle();
	vaoParticles->init();
	m_sceneData->vaos.push_back(vaoParticles);

}

void DemoProjekt::initTextures()
{
	addResPath("textures/");

	// buffer for screen capture
	buffer = (GLubyte*)malloc(3 * texWidth * texHeight);
	if (buffer == NULL) exit(1);

	//Load sprite textures
	GLuint texID = createTexture("HumanHeart-color.jpg", GL_CLAMP_TO_EDGE, GL_LINEAR);
	m_sceneData->textures.push_back(texID);

	texID = createTexture("stonewallNormal.bmp", GL_CLAMP_TO_EDGE, GL_LINEAR);
	m_sceneData->textures.push_back(texID);

	texID = createTexture("HumanHeart-bump.jpg", GL_CLAMP_TO_EDGE, GL_LINEAR);
	m_sceneData->textures.push_back(texID);

	texID = createTexture("lava_diffuse.jpg", GL_REPEAT, GL_LINEAR);
	m_sceneData->textures.push_back(texID);

	texID = createTexture("lava_normal.jpg", GL_REPEAT, GL_LINEAR);
	m_sceneData->textures.push_back(texID);

	texID = createTexture("lava_bump.jpg", GL_REPEAT, GL_LINEAR);
	m_sceneData->textures.push_back(texID);

	texID = createTexture("smoketex2.jpg", GL_CLAMP_TO_EDGE, GL_LINEAR);
	m_sceneData->textures.push_back(texID);

	resetResPath();
}

GLuint DemoProjekt::createTexture(const char *resourceFileName, int wrap, int filter)
{
	GLuint texID;
	FIBITMAP *image = ImageManager::GenericLoader(getResFile(resourceFileName), 0);

	//TODO Create Texture:
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	int bpp = FreeImage_GetBPP(image);
	int imgWidth = FreeImage_GetWidth(image);
	int imgHeight = FreeImage_GetHeight(image);
	switch (bpp)
	{
	case 8:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RED, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));
		break;
	case 24:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));
		break;
	case 32:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));
		break;
	default:
		printf("ERROR: Unknown BPP of image %s", resourceFileName);
		break;
	}

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, 1.5f);

	FreeImage_Unload(image);
	return texID;
}

void DemoProjekt::initMaterials()
{
	Material *m = new Material();

	m->setName("Black_opaque");
	m->m_diffuse[0] = 0.0f;
	m->m_diffuse[1] = 0.0f;
	m->m_diffuse[2] = 0.0f;
	m->m_diffuse[3] = 1.0f;
	m->m_transparency = 0.0f;

	m->m_diffuseTextureGL = m_sceneData->textures[0];
	m_sceneData->materials.push_back(m);

	m = new Material();
	m->setName("Stone");
	m->m_ambient[0] = m->m_ambient[1] = m->m_ambient[2] = 1.0f;	 m->m_ambient[3] = 1.0f;
	m->m_diffuse[0] = 0.8f; m->m_diffuse[1] = 1.0; m->m_diffuse[2] = 0.8f;	 m->m_diffuse[3] = 1.0f;
	m->m_specular[0] = m->m_specular[1] = m->m_specular[2] = 0.2f; m->m_specular[3] = 1.0f;
	m->m_transparency = 0.0f;
	m->m_diffuseTextureGL = m_sceneData->textures[0];
	m->m_normalTextureGL = m_sceneData->textures[1];
	m->m_depthTextureGL = m_sceneData->textures[2];
	m->height = 0.3f;
	m_sceneData->materials.push_back(m);

	m = new Material();
	m->setName("Lava");
	m->m_ambient[0] = m->m_ambient[1] = m->m_ambient[2] = 1.0f;	 m->m_ambient[3] = 1.0f;
	m->m_diffuse[0] = 0.8f; m->m_diffuse[1] = 1.0; m->m_diffuse[2] = 0.8f;	 m->m_diffuse[3] = 1.0f;
	m->m_specular[0] = m->m_specular[1] = m->m_specular[2] = 0.2f; m->m_specular[3] = 1.0f;
	m->m_transparency = 0.0f;
	m->m_diffuseTextureGL = m_sceneData->textures[3];
	m->m_normalTextureGL = m_sceneData->textures[4];
	m->m_depthTextureGL = m_sceneData->textures[5];
	m->height = 0.1f;
	m_sceneData->materials.push_back(m);

	m = new Material();
	m->setName("Smoke");
	m->m_ambient[0] = m->m_ambient[1] = m->m_ambient[2] = 1.0f;	 m->m_ambient[3] = 1.0f;
	m->m_diffuse[0] = 0.8f; m->m_diffuse[1] = 1.0; m->m_diffuse[2] = 0.8f;	 m->m_diffuse[3] = 1.0f;
	m->m_specular[0] = m->m_specular[1] = m->m_specular[2] = 0.2f; m->m_specular[3] = 1.0f;
	m->m_transparency = 0.0f;
	m->m_diffuseTextureGL = m_sceneData->textures[6];
	m_sceneData->materials.push_back(m);


	m = new Material();
	m->setName("White_transparent");
	m->m_ambient[0] = m->m_ambient[1] = m->m_ambient[2] = 1.0f;	 m->m_ambient[3] = 1.0f;
	m->m_diffuse[0] = 0.8f; m->m_diffuse[1] = 1.0; m->m_diffuse[2] = 0.8f;	 m->m_diffuse[3] = 1.0f;
	m->m_specular[0] = m->m_specular[1] = m->m_specular[2] = 0.2f; m->m_specular[3] = 1.0f;
	m->m_transparency = 0.5f;
	m->m_diffuseTextureGL = m_sceneData->textures[0];
	m_sceneData->materials.push_back(m);
}

void DemoProjekt::initInfoEntities()
{
	Entity_SceneOrigin *e0 = new Entity_SceneOrigin(m_sceneData->vaos[0]);
	e0->init();
	m_sceneData->infoEntities.push_back(e0);

	Entity_GridXY *e1 = new Entity_GridXY(m_sceneData->vaos[1]);
	e1->init();
	m_sceneData->infoEntities.push_back(e1);
}

void DemoProjekt::initSceneEntities()
{
	/*
	Entity_Cube *e = new Entity_Cube(m_sceneData->vaos[2]);
	e->m_material = m_sceneData->materials[0];
	e->init();
	m_sceneData->sceneEntities.push_back(e);
	*/

	Entity_OBJ *obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[3]);
	obj->setPosition(0, 0, 1);
	obj->setOrientation(90, 0, 180);
	obj->m_material = m_sceneData->materials[1];
	obj->setScale(0.01f, 0.01f, 0.01f);
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);

	obj = new Entity_OBJ(m_sceneData->models[1], m_sceneData->vaos[4]);
	obj->setPosition(0, 0, 0);
	obj->setOrientation(0, 0, 90);
	obj->setScale(10, 10, 1);
	obj->m_material = m_sceneData->materials[2];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);

	Entity_Particles* particle = new Entity_Particles(((VAO_Particle*)m_sceneData->vaos[5]));
	particle->setPosition(0, 0, 0.8f);
	particle->m_material = m_sceneData->materials[3];
	particle->init();
	m_sceneData->sceneEntities.push_back(particle);

}

void DemoProjekt::drawSphere()
{
	SceneSetting *ss = SceneSetting::GetInstance();

	ss->m_activeShader = m_sceneData->shaderPrograms[1];
	ss->m_activeShader->enable();
	Light::setShaderUniform(m_sceneData->lights.at(0), ss->m_activeShader, "light");

	int uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "CameraPosition");
	glUniform3fv(uniform, 1, (float*)&ss->m_activeCamera->getPosition()[0]);
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "time");
	glUniform1fv(uniform, 1, &timeSec);

	Entity_OBJ *e = static_cast<Entity_OBJ*>(m_sceneData->sceneEntities[0]);
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "MMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, (float*)&e->m_modelMatrix[0]);
	Material::setShaderUniform(e->m_material, ss->m_activeShader, "material");

	glBindVertexArray(e->m_vao->m_object);
	glPatchParameteri(GL_PATCH_VERTICES, 3);

	for (unsigned int i = 0; i < e->m_vao->m_eai->size(); i++)
	{
		glDrawArrays(GL_PATCHES,
			e->m_vao->m_eai->at(i).m_startIndex,
			e->m_vao->m_eai->at(i).m_noIndices);
	}
	glBindVertexArray(0);
}

void DemoProjekt::drawParticles()
{
	SceneSetting *ss = SceneSetting::GetInstance();
	ss->m_activeShader = m_sceneData->shaderPrograms[3];

	Entity_Particles* particles = (Entity_Particles*)m_sceneData->sceneEntities[2];
	particles->update();

	// Use additive blending to give it a 'glow' effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	ss->m_activeShader->enable();

	particles->draw();

	// Don't forget to reset to default blending mode
	ss->m_activeShader->disable();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void DemoProjekt::drawLava()
{
	SceneSetting *ss = SceneSetting::GetInstance();

	ss->m_activeShader = m_sceneData->shaderPrograms[2];
	ss->m_activeShader->enable();
	Light::setShaderUniform(m_sceneData->lights.at(0), ss->m_activeShader, "light");

	int uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "CameraPosition");
	glUniform3fv(uniform, 1, (float*)&ss->m_activeCamera->getPosition()[0]);
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "time");
	glUniform1fv(uniform, 1, &timeSec);

	Entity_OBJ *e = static_cast<Entity_OBJ*>(m_sceneData->sceneEntities[1]);
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "MMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, (float*)&e->m_modelMatrix[0]);
	Material::setShaderUniform(e->m_material, ss->m_activeShader, "material");

	glBindVertexArray(e->m_vao->m_object);
	glPatchParameteri(GL_PATCH_VERTICES, 3);

	for (unsigned int i = 0; i < e->m_vao->m_eai->size(); i++)
	{
		glDrawArrays(GL_PATCHES,
			e->m_vao->m_eai->at(i).m_startIndex,
			e->m_vao->m_eai->at(i).m_noIndices);
	}
	glBindVertexArray(0);
}

void DemoProjekt::render()
{
	SceneSetting *ss = SceneSetting::GetInstance();
	timeSec += 0.05f;

#pragma region Draw Info Entities

	ss->m_activeShader = m_sceneData->shaderPrograms[0];
	ss->m_activeShader->enable();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	int uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "MVPMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewProjectionMatrix());

	for (unsigned int i = 0; i < m_sceneData->infoEntities.size(); i++)
		m_sceneData->infoEntities[i]->draw();

#pragma endregion

#pragma region Draw Scene Entities

	drawSphere();
	drawLava();
	drawParticles();

	ss->m_activeShader->disable();

	//particles->update();
	//particles->draw();

	/*
	glFlush();
	glReadBuffer(GL_BACK);
	glReadPixels(200, 200, texWidth, texHeight, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	glDrawBuffer(GL_BACK);
	glRasterPos3f(0, 0, 0);
	glDrawPixels(texWidth, texHeight, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	glReadBuffer(GL_FRONT);
	*/

#pragma endregion

}
