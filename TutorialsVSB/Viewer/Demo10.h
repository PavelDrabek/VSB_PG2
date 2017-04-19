#ifndef __Demo10_H_
#define __Demo10_H_

#include <sceneInitializer.h>

class Demo10 : public SceneInitializer
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
	Demo10(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif