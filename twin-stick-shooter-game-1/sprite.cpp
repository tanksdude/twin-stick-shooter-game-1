#include "sprite.h"
#include "texture-manager.h"
#include "constants.h"
#include <iostream>

Sprite::Sprite(const char* filename, int horzCount, int vertCount) : Rect(0,0,1,1) {
	this->horzCount = horzCount;
	this->vertCount = vertCount;

    std::cout << "Loading " << filename << std::endl;
    
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    
    int wi, hi, c;
    
    unsigned char * test = SOIL_load_image(filename, &wi, &hi, &c, SOIL_LOAD_AUTO);
    
    texture_id = SOIL_load_OGL_texture (
                                        filename,
                                        SOIL_LOAD_AUTO,
                                        SOIL_CREATE_NEW_ID,
                                        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB 
                                        );
    
    if(0 == texture_id){
        std::cout <<"SOIL loading error: " << SOIL_last_result() << std::endl;
    }
    
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texwidth);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texheight);
    
    int width, height;
    FILE *image;
    int size, i = 0;
    unsigned char *data;
    
    image =fopen(filename,"rb");  // open JPEG image file
    if(!image){
        printf("Unable to open image \n");
    }
    fseek(image,  0,  SEEK_END);
    size = ftell(image);
    fseek(image,  0,  SEEK_SET);
    data = (unsigned char *)malloc(size);
    fread(data, 1, size, image);
    /* verify valid JPEG header */
    if(data[i] == 0xFF && data[i + 1] == 0xD8 && data[i + 2] == 0xFF && data[i + 3] == 0xE0) {
        i += 4;
        /* Check for null terminated JFIF */
        if(data[i + 2] == 'J' && data[i + 3] == 'F' && data[i + 4] == 'I' && data[i + 5] == 'F' && data[i + 6] == 0x00) {
            while(i < size) {
                i++;
                if(data[i] == 0xFF){
                    if(data[i+1] == 0xC0) {
                        height = data[i + 5]*256 + data[i + 6];
                        width = data[i + 7]*256 + data[i + 8];
                        break;
                    }
                }
            }
        }
    }

    fclose(image);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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
