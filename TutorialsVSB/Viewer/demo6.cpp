#include "demo6.h"

#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao_CubeV3C4N3T2.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_Cube.h"

void Demo6::initShaders()
{
	addResPath("shaders/");
	initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag");
	
	//TODO - update previous ADS shader to accept texture:
	initShaderProgram("ads_v3_c4_n3_t2.vert", "ads_v3_c4_n3_t2.frag");
	resetResPath();
}

void Demo6::initVAOs()
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
}

void Demo6::initTextures()
{
	addResPath("textures/");
	//Load sprite textures
	GLuint texID;
	FIBITMAP *image = ImageManager::GenericLoader(getResFile("skala.bmp"), 0);

	//TODO Create Texture:
	buffer = (GLubyte*)malloc(3 * texWidth * texHeight);
	if (buffer == NULL) exit(1);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,16);
	glTexEnvf(GL_TEXTURE_FILTER_CONTROL,GL_TEXTURE_LOD_BIAS, 1.5f);

	FreeImage_Unload(image);
	m_sceneData->textures.push_back(texID);

	resetResPath();
}

void Demo6::initMaterials()
{
	Material *m = new Material();

	m->setName("White_opaque");
	m->m_diffuse[0] = 0.0f;
	m->m_diffuse[1] = 0.0f;
	m->m_diffuse[2] = 0.0f;
	m->m_diffuse[3] = 1.0f;
	m->m_transparency = 0.0f;

	m->m_diffuseTextureGL = m_sceneData->textures[0];
	m_sceneData->materials.push_back(m);
}

void Demo6::initInfoEntities()
{
	Entity_SceneOrigin *e0 = new Entity_SceneOrigin(m_sceneData->vaos[0]);
	e0->init();
	m_sceneData->infoEntities.push_back(e0);

	Entity_GridXY *e1 = new Entity_GridXY(m_sceneData->vaos[1]);
	e1->init();
	m_sceneData->infoEntities.push_back(e1);
}

void Demo6::initSceneEntities()
{
	Entity_Cube *e = new Entity_Cube(m_sceneData->vaos[2]);
	e->m_material = m_sceneData->materials[0];
	e->init();
	m_sceneData->sceneEntities.push_back(e);
}

void Demo6::render()
{
	Entity *e = nullptr;
	SceneSetting *ss = SceneSetting::GetInstance();

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

	ss->m_activeShader = m_sceneData->shaderPrograms[1];
	ss->m_activeShader->enable();

	Light::setShaderUniform(m_sceneData->lights.at(0), ss->m_activeShader, "light");

	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());

	e = m_sceneData->sceneEntities[0];

	glm::mat4 tmp = ss->m_activeCamera->getVM() * e->m_modelMatrix;
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "MVMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, (float*)&tmp[0]);
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "NormalMatrix");
	glUniformMatrix3fv(uniform, 1, GL_FALSE, (float*)&(glm::inverseTranspose(glm::mat3(tmp)))[0]);

	Material::setShaderUniform(e->m_material, ss->m_activeShader, "material");

	//TODO: Active Texture Unit and Bind Texture

	e->draw();
	
	ss->m_activeShader->disable();

	glFlush();
	glReadBuffer(GL_BACK);
	glReadPixels(200, 200, texWidth, texHeight, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	glDrawBuffer(GL_BACK);
	glRasterPos3f(0, 0, 0);
	glDrawPixels(texWidth, texHeight, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	glReadBuffer(GL_FRONT);


#pragma endregion

}
