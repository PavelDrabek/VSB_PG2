#ifndef __DEMO5_H_
#define __DEMO5_H_

#include <sceneInitializer.h>

class Demo5 : public SceneInitializer
{
private:
	void initShaders();
	void initModels();
	void initMaterials();
	void initVAOs();
	void initInfoEntities();
	void initSceneEntities();

public:
	Demo5(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif