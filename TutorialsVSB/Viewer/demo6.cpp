#include "demo6.h"

#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao_CubeV3C4N3T2.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_Cube.h"
#include "entity_OBJ.h"

void Demo6::initShaders()
{
	addResPath("shaders/");
	initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag");
	
	//TODO - update previous ADS shader to accept texture:
	initShaderProgram("ads_v3_c4_n3_t2.vert", "ads_v3_c4_n3_t2.frag");
	initShaderProgram("phong_v3_n3_t3_transparent.vert", "phong_v3_n3_t3_transparent.frag", 0);

	resetResPath();
}

void Demo6::initModels()
{
	ObjLoader objL;
	Model* m;

	addResPath("models/");

	m = objL.loadModel(getResFile("vase/vase.obj"));
	m_sceneData->models.push_back(m);

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

	VAO* vao3 = new VAO();
	vao3->createFromModel(m_sceneData->models[0]);
	m_sceneData->vaos.push_back(vao3);
}

void Demo6::initTextures()
{
	addResPath("textures/");
	//Load sprite textures
	GLuint texID;
	FIBITMAP *image = ImageManager::GenericLoader(getResFile("skala.bmp"), 0);

	buffer = (GLubyte*)malloc(3 * texWidth * texHeight);
	if (buffer == NULL) exit(1);
	
	//TODO Create Texture:
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
		printf("ERROR: Unknown BPP of image");
		break;
	}

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

	m->setName("Black_opaque");
	m->m_diffuse[0] = 0.0f;
	m->m_diffuse[1] = 0.0f;
	m->m_diffuse[2] = 0.0f;
	m->m_diffuse[3] = 1.0f;
	m->m_transparency = 0.0f;

	m->m_diffuseTextureGL = m_sceneData->textures[0];
	m_sceneData->materials.push_back(m);

	m = new Material();
	m->setName("White_opaque");
	m->m_ambient[0] = m->m_ambient[1] = m->m_ambient[2] = 1.0f;	 m->m_ambient[3] = 1.0f;
	m->m_diffuse[0] = 0.8f; m->m_diffuse[1] = 1.0; m->m_diffuse[2] = 0.8f;	 m->m_diffuse[3] = 1.0f;
	m->m_specular[0] = m->m_specular[1] = m->m_specular[2] = 0.2f; m->m_specular[3] = 1.0f;
	m->m_transparency = 0.0f;
	m->m_diffuseTextureGL = m_sceneData->textures[0];
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

	Entity_OBJ *obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[3]);
	obj->setPosition(0, 0, 0);
	obj->setOrientation(0, 0, 90);
	obj->m_material = m_sceneData->materials[2];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);

	obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[3]);
	obj->setPosition(0, 1.5f, 0);
	obj->setOrientation(0, 0, 90);
	obj->m_material = m_sceneData->materials[1];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);

	obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[3]);
	obj->setPosition(0, 3.0f, 0);
	obj->setOrientation(0, 0, 90);
	obj->m_material = m_sceneData->materials[2];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);
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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[0]);

	for (unsigned int i = 0; i < m_sceneData->infoEntities.size(); i++)
		m_sceneData->infoEntities[i]->draw();

#pragma endregion

#pragma region Draw Scene Entities

	// not transparent
	//glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	ss->m_activeShader = m_sceneData->shaderPrograms[2];
	ss->m_activeShader->enable();
	Light::setShaderUniform(m_sceneData->lights.at(0), ss->m_activeShader, "light");

	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());

	for (Entity **e = m_sceneData->sceneEntities.front(); e <= m_sceneData->sceneEntities.back(); e++)
	{
		if ((*e)->m_material->m_transparency <= 0) {
			Material::setShaderUniform((*e)->m_material, ss->m_activeShader, "material");
			(*e)->draw();
		}
	}

	// transparent
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	glDisable(GL_CULL_FACE);

	ss->m_activeShader = m_sceneData->shaderPrograms[2];
	ss->m_activeShader->enable();
	Light::setShaderUniform(m_sceneData->lights.at(0), ss->m_activeShader, "light");

	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());

	for (Entity **e = m_sceneData->sceneEntities.front(); e <= m_sceneData->sceneEntities.back(); e++)
	{
		if ((*e)->m_material->m_transparency > 0) {
			Material::setShaderUniform((*e)->m_material, ss->m_activeShader, "material");
			(*e)->draw();
		}
	}
	glDepthMask(GL_TRUE);

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
