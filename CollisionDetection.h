#pragma once
#include <vector>
#include "obj.h"
#include "ObjectInst.h"

class CollisionDetection
{
private:
	static std::vector<ObjectInst*> _objects;
public:
	static bool addObject(ObjectInst* object);
	static bool removeObject(ObjectInst* object);

	static bool checkCollision(const vertex& point);
	static void drawBoxes(void);
};

