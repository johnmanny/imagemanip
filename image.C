/*	Author: John Nemeth
	Description: implementation file for image class
	Sources: class material
*/

#include <string.h>
#include <iostream>
#include <image.h>
#include <logging.h>

// constructor/destructors ///////////////////////////////////////////////////
Image::Image() {
	width = 0;
	height = 0;
	maxColorVal = 0;
	magicNum = NULL;
	pBuffer = NULL;
}

// parameterized constructor (used in halfsize, leftright & topdown combine)
Image::Image(int newWidth, int newHeight) {
	width = newWidth;
	height = newHeight;
	maxColorVal = 0;
	magicNum = new char[CHAR_BUFFER];
	pBuffer = new Pixel[width * height];
}

// copy constructor (essentially copies image)
Image::Image(const Image &anImage) {
	width = anImage.width;
	height = anImage.height;
	maxColorVal = anImage.maxColorVal;
	magicNum = new char[strlen(anImage.magicNum) + 1];
	strcpy(magicNum, anImage.magicNum);
	pBuffer= new Pixel[anImage.width * anImage.height];
	int numPixels = width * height;
	for (int i = 0; i < numPixels; i++) {
		pBuffer[i] = anImage.pBuffer[i];
	}
}

// destructor
Image::~Image() {
	if (magicNum)
		delete[] magicNum;
	if (pBuffer)
		delete[] pBuffer;
}

// mutators ///////////////////////////////////////////////////////////
void Image::setWidth(int w) {
	width = w;
}

void Image::setHeight(int h) {
	height = h;
}

void Image::setMaxColor(int m) {
	maxColorVal = m;
}

void Image::setMagicNum(char * aMagicNum) {
	if (magicNum)
		delete[] magicNum;
	magicNum = aMagicNum;
}

void Image::setPBuffer(Pixel * aPBuffer) {
	if (pBuffer)
		delete[] pBuffer;
	pBuffer = aPBuffer;
}

// accessors /////////////////////////////////////////////////////////
int Image::getWidth() const{
	return width;
}

int Image::getHeight() const{
	return height;
}

int Image::getMaxColor() const{
	return maxColorVal;
}

const char * Image::getMagicNum() const{
	return magicNum;
}

const Pixel * Image::getPBuffer() const{
	return pBuffer;
}

// virtual update function placeholder for source update function1
void Image::sUpdate() {
	Logger::LogEvent("Image: in supdate inside image.c, likely wanted virtual supdate");
}

// returns bool for whether data pipeline is circular
bool Image::circPipeline(Image * reset){
	return sCircPipeline(reset);
}

bool Image::sCircPipeline(Image * aImage) {
	Logger::LogEvent("Image: in sCircPipeline inside image.c");
}
