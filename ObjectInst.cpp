#include "obj.h"
#include "ObjectInst.h"

ObjectInst::ObjectInst(void)
{
	_object = NULL;
	position.x = 0;
	position.y = 0;
	position.z = 0;
	rot_x = 0;
	rot_y = 0;
	tint[0] = 0.8;
	tint[1] = 0.8;
	tint[2] = 0.8;
	tint[3] = 1.0;
}

ObjectInst::ObjectInst(Model* object)
{
	_object = object;
	position.x = 0;
	position.y = 0;
	position.z = 0;
	rot_x = 0;
	rot_y = 0;
	tint[0] = 0.8;
	tint[1] = 0.8;
	tint[2] = 0.8;
	tint[3] = 1.0;
}

void ObjectInst::draw() const
{
	glPushMatrix();
		glColor3f(tint[0], tint[1], tint[2]);
		glTranslatef(position.x, position.y, position.z);
		glRotatef(rot_x, 1, 0, 0);
		glRotatef(rot_y, 0, 1, 0);
		_object->Draw();
	glPopMatrix();
}

ObjectInst::~ObjectInst(void)
{
}
