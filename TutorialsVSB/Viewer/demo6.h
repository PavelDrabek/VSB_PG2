#ifndef __DEMO6_H_
#define __DEMO6_H_

#include <sceneInitializer.h>

class Demo6 : public SceneInitializer
{
private:
	void initShaders();
	void initModels();
	void initTextures();
	void initMaterials();
	void initVAOs();
	void initInfoEntities();
	void initSceneEntities();

	int texWidth = 200;
	int texHeight = 200;
	GLubyte* buffer;
public:
	Demo6(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif