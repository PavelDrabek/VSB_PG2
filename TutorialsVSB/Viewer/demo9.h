#ifndef __Demo9_H_
#define __Demo9_H_

#include <sceneInitializer.h>

class Demo9 : public SceneInitializer
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
	Demo9(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif