#include "CollisionDetection.h"

std::vector<ObjectInst*> CollisionDetection::_objects = std::vector<ObjectInst*>();

bool CollisionDetection::addObject(ObjectInst* object)
{
	for(std::vector<ObjectInst*>::iterator it = _objects.begin(); it != _objects.end(); it++)
		if (*it == object)
			return true;
	_objects.push_back(object);
	return false;
}

bool CollisionDetection::removeObject(ObjectInst* object)
{
	for(std::vector<ObjectInst*>::iterator it = _objects.begin(); it != _objects.end(); it++)
		if (*it == object)
		{
			_objects.erase(it);
			return false;
		}
	return true;
}

bool CollisionDetection::checkCollision(const vertex& point)
{
	for(std::vector<ObjectInst*>::iterator it = _objects.begin(); it != _objects.end(); it++)
		if ((*it)->position.x < point.x &&
			(*it)->position.x + (*it)->size.x > point.x &&
			(*it)->position.z < point.z &&
			(*it)->position.z + (*it)->size.z > point.z)
				return true;
	return false;
	/* 
	 &&
			(*it)->size.x - (*it)->position.x > point.x)
		&&
			(*it)->size.z + (*it)->position.z < point.z &&
			(*it)->size.z - (*it)->position.z > point.z
			(*it)->size.y + (*it)->position.y < point.y &&
			(*it)->size.y - (*it)->position.y > point.y*/
}

void CollisionDetection::drawBoxes(void)
{
	for(std::vector<ObjectInst*>::iterator it = _objects.begin(); it != _objects.end(); it++)
	{
		glPushMatrix();
			glColor4f(0.4, 0.8, 0.4, 0.4);

			glTranslatef((*it)->position.x, 0, (*it)->position.z);
			glRotatef((*it)->rot_x, 1, 0, 0);
			glRotatef((*it)->rot_y, 0, 1, 0);
			glBegin(GL_TRIANGLES);
				glVertex3f(0,			  0,			 0);
				glVertex3f(0,			  0,			 (*it)->size.z);
				glVertex3f(0,			  (*it)->size.y, (*it)->size.z);

				glVertex3f(0,			  0,			 0);
				glVertex3f(0,			  (*it)->size.y, (*it)->size.z);
				glVertex3f(0,			  (*it)->size.y, 0);

				glVertex3f(0,			  0,			 (*it)->size.z);
				glVertex3f((*it)->size.x, 0,			 (*it)->size.z);
				glVertex3f((*it)->size.x, (*it)->size.y, (*it)->size.z);

				glVertex3f(0,			  0,			 (*it)->size.z);
				glVertex3f((*it)->size.x, (*it)->size.y, (*it)->size.z);
				glVertex3f(0,			  (*it)->size.y, (*it)->size.z);

				glVertex3f((*it)->size.x, 0,			 (*it)->size.z);
				glVertex3f((*it)->size.x, 0,			 0);
				glVertex3f((*it)->size.x, (*it)->size.y, 0);

				glVertex3f((*it)->size.x, 0,			 (*it)->size.z);
				glVertex3f((*it)->size.x, (*it)->size.y, (*it)->size.z);
				glVertex3f((*it)->size.x, (*it)->size.y, 0);

				glVertex3f((*it)->size.x, 0,			 0);
				glVertex3f(0,			  0,			 0);
				glVertex3f(0,			  (*it)->size.y, 0);

				glVertex3f((*it)->size.x, 0,			 0);
				glVertex3f(0,			  (*it)->size.y, 0);
				glVertex3f((*it)->size.x, (*it)->size.y, 0);
				
			glEnd();
		glPopMatrix();
	}
}