#include "texture.h"


texture::texture(void)
{
	_id = 0;
	_width = 0;
	_height=  0;
	_error = NO_BITMAP_ERROR;
	_passed_var = false;
}

texture::texture(string file_path, GLfloat tex_param)
{
	_id = 0;
	_width = 0;
	_height=  0;
	_error = NO_BITMAP_ERROR;
	_passed_var = false;

	cout << "Image name: " << file_path << "\n";
	ifstream reader(file_path, ios::in | ios::binary);
	if (!reader.is_open())
	{
		_error = FILE_NOT_FOUND;
		return;
	}
	if (reader.get() != 'B' || reader.get() != 'M')
	{
		_error = NOT_BITMAP;
		return;
	}

	reader.ignore(8);
	char offset_b[4];
	reader.read(offset_b, 4);
	unsigned int offset_i = *(unsigned int*)offset_b;
	

	reader.ignore(4);
	char width_b[4];
	reader.read(width_b, 4);
	_width = *(int*)width_b;
	cout << "Width: " << _width << "\n";

	char height_b[4];
	reader.read(height_b, 4);
	_height = *(int*)height_b;
	cout << "Height: " << _height << "\n";

	reader.ignore(2);
	char bits_b[2];
	reader.read(bits_b, 2);
	unsigned short bits_i = (*(unsigned short*)bits_b) * 0.125;
	cout << "Number of Bits: " << bits_i << "\n";


	if (bits_i != 3 && bits_i != 4)
	{
		cout << "Dead!\n";
		_width = 0;
		_height = 0;
		_error = WRONG_BITS;
		return;
	}

	reader.seekg(offset_i);

	char *pixel_data = new char[_width * _height * bits_i];

	reader.read(pixel_data, _width * _height * bits_i);

	reader.close();

				

	if (bits_i == 3)
	{
		char *pixel_data_final = new char[_width *_height * 4];
		char *swap = pixel_data_final;
		pixel_data_final = pixel_data;
		pixel_data = swap;
		for (unsigned int i = 0; i < _width * _height; i++)
		{
			pixel_data[(i * 3)] = pixel_data_final[(i * 3)];
			pixel_data[(i * 3) + 1] = pixel_data_final[(i * 3) + 1];
			pixel_data[(i * 3) + 2] = pixel_data_final[(i * 3) + 2];

			if (pixel_data[(i * 3)] == 255)
				pixel_data[(i * 3) + 3] = 0;
			else
				pixel_data[(i * 3) + 3] = 255;
		}
		delete[] pixel_data_final;
	}

	glGenTextures(1, &_id);
	cout << "Texture ID: " << _id << "\n";
	glBindTexture(GL_TEXTURE_2D, _id);
	
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);  
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tex_param);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tex_param);

	if (bits_i == 3)
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, _width, _height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixel_data);
	else if (bits_i == 4)
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, _width, _height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixel_data);
	delete[] pixel_data;
}

texture &texture::operator=(texture& passed)
{
	if (this != &passed)
	{
		this->_id = passed._id;
		this->_width = passed._width;
		this->_height = passed._height;
		this->_error = passed._error;
		passed._passed_var = true;
	}
	return *this;
}

texture::~texture(void)
{
	if (_id && !_passed_var)
		glDeleteTextures(1, &_id);
}
