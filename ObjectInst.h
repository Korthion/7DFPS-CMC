#pragma once
#include "__PreCompile.h"
class Model;


class ObjectInst
{
private:
	Model* _object;
	
public:
	vertex position;
	float rot_x;
	float rot_y;
	float tint[4];

	ObjectInst(void);
	ObjectInst(Model* object);

	void draw(void) const;

	inline void setTint(const float& r, const float& g, const float& b, const float& a) { tint[0] = r; tint[1] = g; tint[2] = b; tint[3] = a; };
	inline void setPosition(const float& x, const float& y, const float& z) { position.x = x; position.y = y; position.z = z; };

	~ObjectInst(void);
};

