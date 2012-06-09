#pragma once
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

enum bitmap_load_error_t
{
	NO_BITMAP_ERROR,
	FILE_NOT_FOUND,
	NOT_BITMAP,
	WRONG_BITS,
	BAD_DATA,
	SUDDEN_END
};

class texture
{
private:
	unsigned int _id;
	int _width;
	int _height;
	bitmap_load_error_t _error;
	bool _passed_var;

public:
	texture(void);
	texture(string file_path, GLfloat tex_param);

	inline const unsigned int& getId(void) const	{ return _id; };
	inline const int& getWidth(void) const			{ return _width; };
	inline const int& getHeight(void) const			{ return _height; };
	inline bool hasError(void) const				{ return _error != NO_BITMAP_ERROR; };
	inline bitmap_load_error_t getError(void) const { return _error; };

	texture &operator=(texture& passed);

	~texture(void);
};

