#include "sprite.h"
#include "texture-manager.h"
#include "constants.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/* from ImGui: https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#example-for-opengl-users */
static bool LoadTextureFromMemory(const void* data, size_t data_size, GLuint* out_texture, int* out_width, int* out_height) {
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load_from_memory((const unsigned char*)data, (int)data_size, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}
static bool LoadTextureFromFile(const char* file_name, GLuint* out_texture, int* out_width, int* out_height) {
	FILE* f = fopen(file_name, "rb");
	if (f == NULL)
		return false;
	fseek(f, 0, SEEK_END);
	size_t file_size = (size_t)ftell(f);
	if (file_size == -1)
		return false;
	fseek(f, 0, SEEK_SET);
	void* file_data = malloc(file_size);
	fread(file_data, 1, file_size, f);
	fclose(f);
	bool ret = LoadTextureFromMemory(file_data, file_size, out_texture, out_width, out_height);
	free(file_data);
	return ret;
}
/* end ImGui */

Sprite::Sprite(const char* filename, int horzCount, int vertCount) : Rect(0,0,1,1) {
	this->horzCount = horzCount;
	this->vertCount = vertCount;

	std::cout << "Loading " << filename << std::endl;

	LoadTextureFromFile(filename, &texture_id, &texwidth, &texheight);
}

int Sprite::getFrameXIndex(int index) const {
	return index % horzCount;
}

int Sprite::getFrameYIndex(int index) const {
	return index / horzCount;
}

void Sprite::draw(float x, float y, float w, float h, float radians, float alpha, int frameNum, float z) const {
	int frameX = getFrameXIndex(frameNum);
	int frameY = getFrameYIndex(frameNum);
	//rotate
	glPushMatrix();
	glTranslatef(x + w/2, y + h/2, z);
	glRotatef(radians * (180/PI), 0, 0, 1); //rotate angle is in degrees for some reason
	glTranslatef(-(x + w/2), -(y + h/2), -z);

	glBindTexture( GL_TEXTURE_2D, texture_id );
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	glColor4f(1, 1, 1, alpha);
	glTexCoord2f(float(frameX) / float(horzCount), float(frameY) / float(vertCount));
	glVertex3f(x, y + h, z);

	glTexCoord2f(float(frameX) / float(horzCount), float(frameY + 1) / float(vertCount));
	glVertex3f(x, y, z);

	glTexCoord2f(float(frameX + 1) / float(horzCount), float(frameY + 1) / float(vertCount));
	glVertex3f(x+w, y, z);

	glTexCoord2f(float(frameX + 1) / float(horzCount), float(frameY) / float(vertCount));
	glVertex3f(x+w, y + h, z);

	glEnd();

	glDisable(GL_TEXTURE_2D);

	//finish rotating
	glPopMatrix();
}

SpriteHolder::SpriteHolder(const char* name, int delayBetweenFrames, int loopMax) {
	this->index = 0;
	this->loopCount = 0;
	this->loopMax = loopMax;
	this->delayBetweenFrames = delayBetweenFrames;
	this->cycle = 0; //not a great variable name, but whatever
	this->name = name;

	sprite = TextureManager::getLoneSprite(name);
}

std::string SpriteHolder::getName() const {
	return name;
}

bool SpriteHolder::isDone() const {
	if (loopMax < 0) {
		return false;
	}
	return (loopCount >= loopMax);
}

void SpriteHolder::frameAdvance() {
	cycle++;
	if (cycle >= delayBetweenFrames) {
		cycle -= delayBetweenFrames;
		index++;
	}
	if (index >= sprite->horzCount * sprite->vertCount) {
		index -= sprite->horzCount * sprite->vertCount;
		loopCount++;
	}
}

void SpriteHolder::draw(float x, float y, float w, float h, float radians, float alpha, float z) const {
	sprite->draw(x, y, w, h, radians, alpha, index, z);
}

void SpriteHolder::draw(float x, float y, float w, float h, float radians, float alpha, int frameNum, float z) const {
	sprite->draw(x, y, w, h, radians, alpha, frameNum, z);
}

SpriteHolder::~SpriteHolder() {

}

SpriteHolder::SpriteHolder(const SpriteHolder& other) {
	this->index = other.index;
	this->loopCount = other.loopCount;
	this->loopMax = other.loopMax;
	this->cycle = other.cycle;
	this->delayBetweenFrames = other.delayBetweenFrames;

	this->sprite = other.sprite; //okay to share because they aren't deleted and they don't store anything beyond their size
}
