#ifndef __Demo9_H_
#define __Demo9_H_

#include <sceneInitializer.h>
//#include "entity_Particles.h"

class DemoProjekt : public SceneInitializer
{

private:
	void initShaders();
	void initModels();
	void initTextures();
	void initMaterials();
	void initVAOs();
	void initInfoEntities();
	void initSceneEntities();

	void loadShader(ShaderProgram *sp, int index);
	void drawSphere();
	void drawLava();
	void drawParticles();

	GLuint createTexture(const char *resourceFileName, int wrap, int filter);

	float timeSec = 0;
	int texWidth = 200;
	int texHeight = 200;
	GLubyte* buffer;

	//Entity_Particles* particles;

public:
	DemoProjekt(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void reloadShaders();
	void render();
};

#endif