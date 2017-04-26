#ifndef __Demo9_H_
#define __Demo9_H_

#include <sceneInitializer.h>

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

	void drawSphere();
	void drawLava();

	GLuint createTexture(const char *resourceFileName, int wrap, int filter);

	float timeSec = 0;
	int texWidth = 200;
	int texHeight = 200;
	GLubyte* buffer;
public:
	DemoProjekt(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif