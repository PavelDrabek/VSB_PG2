#ifndef __DEMO7_H_
#define __DEMO7_H_

#include <sceneInitializer.h>

class Demo7 : public SceneInitializer
{
private:
	void initShaders();
	void initModels();
	void initTextures();
	void initMaterials();
	void initVAOs();
	void initInfoEntities();
	void initSceneEntities();

	GLuint createTexture(const char *resourceFileName, int wrap, int filter);

	int texWidth = 200;
	int texHeight = 200;
	GLubyte* buffer;
public:
	Demo7(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif