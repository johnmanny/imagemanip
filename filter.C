/*
	Author: John Nemeth
	Description: Implementation file for filter.h
	Sources: Class material
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <filter.h>

using std::ofstream;
using std::endl;

// color source /////////////////////////////////////////////////////////////
Color::Color(int w, int h, int r, int g, int b) {
	Logger::LogEvent("entering color constructor", SourceName());
	if ((w <= 0) || (w > 10000)) {
		DataFlowException e(SourceName(), "---Error! Width must be > 0 or <= 10000");
		throw e;
	}
	if ((h <= 0) || (h > 10000)) {
		DataFlowException e(SourceName(), "---Error! Height must be > 0 or <= 10000");
		throw e;
	}
	if ((r < 0) || (r > 255)) {
		Logger::LogEvent("NOTE: red value outside acceptable bounds. taking modulo 256");
		r = r % 256;
	}
	if ((g < 0) || (g > 255)) {
		Logger::LogEvent("NOTE: green value outside acceptable bounds. taking modulo 256");
		g = g % 256;
	}
	if ((b < 0) || (b > 255)) {
		Logger::LogEvent("NOTE: blue value outside acceptable bounds. taking modulo 256");
		b = b % 256;
	}

	int numPixels = h * w;
	char * newMagic = new char[strlen("P6") + 1];
	Pixel * newBuff = new Pixel[numPixels];
	strcpy(newMagic, "P6");
	for (int i = 0; i < numPixels; i++) {
		newBuff[i].red = r;
		newBuff[i].green = g;
		newBuff[i].blue = b;
	}

	this->setPBuffer(newBuff);
	this->setHeight(h);
	this->setWidth(w);
	this->setMaxColor(255);
	this->setMagicNum(newMagic);
	this->updated = true;
	Logger::LogEvent("exiting color constructor", SourceName());
}

// checkSum sink ////////////////////////////////////////////////////////////
void CheckSum::OutputCheckSum(const char * file) {
	Logger::LogEvent("entering OutputCheckSum", SinkName());
	if (!file) {
		DataFlowException e(SinkName(), "---Error! No filename passed as 2nd argument!");
		throw e;
	}
	if (!input1) {
		DataFlowException e(SinkName(), "---Error! input1 is null (only input1 is valid)");
		throw e;
	}
	ofstream output;
	output.open(file);
	int red = 0, green = 0, blue = 0;
	int numPixels = input1->getHeight() * input1->getWidth();
	const Pixel * buff = input1->getPBuffer();
	for (int i = 0; i < numPixels; i++) {
		red += buff[i].red;
		green += buff[i].green;
		blue += buff[i].blue;
	}
	
	output << "CHECKSUM: " << red % 256 << ", " << green % 256 << ", " << blue % 256 << endl;
	output.close();
}

// utility functions /////////////////////////////////////////////////////////
// checks for not null inputs & inputs set to themselves during update
void Filter::checkInputs() {
	if (!input1) {
		DataFlowException e(SinkName(), ": ---Error! input1 is null! (input2 -could- also be null)");
		throw e;
	}
	if (!input2) {
		DataFlowException e(SinkName(), ": ---Error! input2 is null! (but input1 valid)");
		throw e;
	}
}

// checks only first input during update 
void Filter::checkInput1() {
	if (!input1) {
		DataFlowException e(SinkName(), ": ---Error! input1 is null! (the only input)");
		throw e;
	}
}

// checks for equal dimensions during update
void Filter::checkEqualDims() {
	if ((input1->getHeight() != input2->getHeight()) || (input1->getWidth() != input2->getWidth())) {
		char msg[CHAR_BUFFER];
		sprintf(msg, "%s: Widths & heights must match - w1 2: %d %d h1 2: %d %d", SinkName(), input1->getWidth(), input2->getWidth(), input1->getHeight(), input2->getHeight());
		DataFlowException e(SinkName(), msg);
		throw e;
	}
}

// sets image values and updated status of filter instance
void Filter::setImage(int nW, int nH, int nM, char * nMN, Pixel * nPB) {
	Logger::LogEvent("entering setImage", SinkName());
	if ((nW < 0) || (nH < 0)) {
		DataFlowException e(SinkName(), "---Error! Width or height must be >= 0!");
		throw e;
	} 
	this->setWidth(nW);
	this->setHeight(nH);
	this->setMaxColor(nM);
	this->setMagicNum(nMN);
	this->setPBuffer(nPB);
	this->updated = true;
}

// function to copy magic num. reduces clutter
char * Filter::copyMagicNum(const char * oldMagicNum) {
	char * newMagicNum = new char[strlen(oldMagicNum) + 1];
	strcpy(newMagicNum, oldMagicNum);
	return newMagicNum;
}

// shrinker filter /////////////////////////////////////////////////////////////
void Shrinker::Execute() {
	Logger::LogEvent("entering Execute", SinkName());
	checkInput1();

	int newWidth = input1->getWidth() / 2, newHeight = input1->getHeight() / 2;
	int newBIndex = 0, in1Index = 0, in1WidthMult = input1->getWidth() * 2;
	const Pixel * in1Buff = input1->getPBuffer();
	Pixel * newBuff = new Pixel[newWidth * newHeight];
	for (int i = 0; i < newHeight; i++) {
		in1Index = i * in1WidthMult;
		for (int j = 0; j < newWidth; j++) {
			newBuff[newBIndex] = in1Buff[in1Index + (j * 2)];
			newBIndex++;
		}
	}

	setImage(newWidth, newHeight, input1->getMaxColor(), copyMagicNum(input1->getMagicNum()), newBuff);
	Logger::LogEvent("exiting Execute", SinkName());
}

// lrcombine filter ///////////////////////////////////////////////////////////
void LRCombine::Execute() {
	Logger::LogEvent("entering Execute", SinkName());
	checkInputs();
	if(input1->getHeight() != input2->getHeight()) {
		char msg[CHAR_BUFFER];
		sprintf(msg, "%s: ---Error! image heights not equal! h 1 - 2: %d - %d", SinkName(), input1->getHeight(), input2->getHeight());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	int in1Width = input1->getWidth(), in2Width = input2->getWidth();
	int newHeight = input1->getHeight();
	const Pixel * in1Buff = input1->getPBuffer();
	const Pixel * in2Buff = input2->getPBuffer();
	Pixel * newPBuff = new Pixel[(in1Width + in2Width) * newHeight];
	int in1Ind = 0, in2Ind = 0, newBuffInd = 0, j;
	for (int i = 0; i < newHeight; i++) {
		for (j = 0; j < in1Width; j++) {
			newPBuff[newBuffInd] = in1Buff[in1Ind];
			newBuffInd++;
			in1Ind++;
		}
		for (j = 0; j < in2Width; j++) {
			newPBuff[newBuffInd] = in2Buff[in2Ind];
			newBuffInd++;
			in2Ind++;
		}
	}

	setImage((in1Width + in2Width), newHeight, input1->getMaxColor(), copyMagicNum(input1->getMagicNum()), newPBuff);
	Logger::LogEvent("exiting Execute", SinkName());
}

// tbcombine filter /////////////////////////////////////////////////////////
void TBCombine::Execute() {
	Logger::LogEvent("entering Execute", SinkName());
	checkInputs();
	if (input1->getWidth() != input2->getWidth()) {
		char msg[CHAR_BUFFER];
		sprintf(msg, "%s: Widths must match - w1: %d w2: %d", SinkName(), input1->getWidth(), input2->getWidth());
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	int topPixels = input1->getWidth() * input1->getHeight();
	int botPixels = input2->getWidth() * input2->getHeight();
	const Pixel * in1Buff = input1->getPBuffer();
	const Pixel * in2Buff = input2->getPBuffer();
	Pixel * newPBuff = new Pixel[topPixels + botPixels];
	int outIndex = 0, i;
	for (i = 0; i < topPixels; i++) {
		newPBuff[outIndex] = in1Buff[i];
		outIndex++;
	}
	for (i = 0; i < botPixels; i++) {
		newPBuff[outIndex] = in2Buff[i];
		outIndex++;
	}
	
	setImage(input1->getWidth(), (input1->getHeight() + input2->getHeight()), input1->getMaxColor(), copyMagicNum(input1->getMagicNum()), newPBuff);
	Logger::LogEvent("exiting Execute", SinkName());
}


// blender class methods //////////////////////////////////////////////////////
Blender::Blender() {
	factor = 0.0;
	Logger::LogEvent("in default constructor - factor = 0.0", SinkName());
}

Blender::Blender(double aFactor) {
	factor = aFactor;
}

void Blender::SetFactor(double aFactor) {
	factor = aFactor;
}

void Blender::Execute() {
	Logger::LogEvent("entering Execute", SinkName());
	checkInputs();
	checkEqualDims();
	if ((factor < 0.0) || (factor > 1.0)) {
		char msg[CHAR_BUFFER];
		sprintf(msg, "%s: factor must be > 0.0 and < 1.0 - passed factor: %f", SinkName(), factor);
		DataFlowException e(SinkName(), msg);
		throw e;
	}

	double rFactor = 1 - factor;
	int numPixels = input1->getHeight() * input1->getWidth();
	const Pixel * oldBuff1 = input1->getPBuffer();
	const Pixel * oldBuff2 = input2->getPBuffer();
	Pixel * newBuff = new Pixel[numPixels];
	for (int i = 0; i < numPixels; i++) {
		newBuff[i].red = (factor * oldBuff1[i].red) + (rFactor * oldBuff2[i].red);
		newBuff[i].green = (factor * oldBuff1[i].green) + (rFactor * oldBuff2[i].green);
		newBuff[i].blue = (factor * oldBuff1[i].blue) + (rFactor * oldBuff2[i].blue);
	}

	setImage(input1->getWidth(), input1->getHeight(), input1->getMaxColor(), copyMagicNum(input1->getMagicNum()), newBuff);
	Logger::LogEvent("exiting Execute", SinkName());
}

// mirror class /////////////////////////////////////////////////////////////
void Mirror::Execute() {
	Logger::LogEvent("entering execute", SinkName());
	checkInput1();

	int aHeight = input1->getHeight();
	int aWidth = input1->getWidth();
	int newIndex, oldIndex;
	const Pixel * oldBuff = input1->getPBuffer();
	Pixel * newBuff = new Pixel[aHeight * aWidth];
	for (int i = 0; i < aHeight; i++) {
		newIndex = i * aWidth;
		oldIndex = newIndex + (aWidth - 1);
		for (int j = 0; j < aWidth; j++) {
			newBuff[newIndex + j] = oldBuff[oldIndex - j];
		}
	}

	setImage(aWidth, aHeight, input1->getMaxColor(), copyMagicNum(input1->getMagicNum()), newBuff);
	Logger::LogEvent("exiting execute", SinkName());
}

// rotate class //////////////////////////////////////////////////////////////
// this execute method uses pointers as indices in the for loops
void Rotate::Execute() {
	Logger::LogEvent("entering execute", SinkName());
	checkInput1();

	int aHeight = input1->getWidth();
	int aWidth = input1->getHeight();
	int numPixels = aHeight * aWidth;
	const Pixel * oldBuff = input1->getPBuffer();
	Pixel * newBuff = new Pixel[aHeight * aWidth];
	Pixel * newPtr = newBuff;
	oldBuff = oldBuff + (numPixels - (aHeight + 1));
	for (int i = 0; i < aHeight; i++) {
		oldBuff++;
		for (int j = 0; j < aWidth; j++) {
			*newPtr = *oldBuff;
			newPtr++;
			oldBuff = oldBuff - aHeight;
		}
		oldBuff = oldBuff + numPixels;
	}

	setImage(aWidth, aHeight, input1->getMaxColor(), copyMagicNum(input1->getMagicNum()), newBuff);
	Logger::LogEvent("exiting execute", SinkName());
}

// subtract class ///////////////////////////////////////////////////////////
void Subtract::Execute() {
	Logger::LogEvent("entering execute", SinkName());
	checkInputs();
	checkEqualDims();

	int numPixels = input1->getHeight() * input1->getWidth();
	const Pixel * in1 = input1->getPBuffer();
	const Pixel * in2 = input2->getPBuffer();
	Pixel * newBuff = new Pixel[numPixels];
	for (int i = 0; i < numPixels; i++) {
		if (in1[i].red > in2[i].red)
			newBuff[i].red = in1[i].red - in2[i].red;
		else
			newBuff[i].red = 0;
		if (in1[i].green > in2[i].green)
			newBuff[i].green = in1[i].green - in2[i].green;
		else
			newBuff[i].green = 0;
		if (in1[i].blue > in2[i].blue)
			newBuff[i].blue = in1[i].blue - in2[i].blue;
		else
			newBuff[i].blue = 0;
	}

	setImage(input1->getWidth(), input1->getHeight(), input1->getMaxColor(), copyMagicNum(input1->getMagicNum()), newBuff);
	Logger::LogEvent("exiting execute", SinkName());
}

// grayscale class ///////////////////////////////////////////////////////////
void Grayscale::Execute() {
	Logger::LogEvent("entering execute", SinkName());
	checkInput1();

	int numPixels = input1->getHeight() * input1->getWidth();
	char gray;
	const Pixel * in1 = input1->getPBuffer();
	Pixel * newBuff = new Pixel[numPixels];
	for (int i = 0; i < numPixels; i++) {
		gray = (in1[i].red / 5) + (in1[i].green / 2) + (in1[i].blue / 4);
		newBuff[i].red = gray;
		newBuff[i].green = gray;
		newBuff[i].blue = gray;
	}	

	setImage(input1->getWidth(), input1->getHeight(), input1->getMaxColor(), copyMagicNum(input1->getMagicNum()), newBuff);
	Logger::LogEvent("exiting execute", SinkName());
}

// blur class ////////////////////////////////////////////////////////////////
void Blur::Execute() {
	Logger::LogEvent("entering execute", SinkName());
	checkInput1();
	
	int aWidth = input1->getWidth(), aHeight = input1->getHeight(), newIndex = 0;
	const Pixel * in1 = input1->getPBuffer();
	Pixel * newBuff = new Pixel[(aHeight + 1) * (aWidth + 1)];

	/* 
	inside pixel has 8 neighbors
	if pixel is at j, then neighbors are(from top left to right and down):
	j-1-i, j-i, j-i+1, j-1, j+1, j-1+i, j+i, j+1+i
	*/
	for (int i = 0; i < aHeight; i++) {
		for (int j = 0; j < aWidth; j++) {
			if ((i == 0) || (j == 0) || (i == (aHeight - 1)) || (j == (aWidth - 1))) {
				newBuff[newIndex].red = in1[newIndex].red;
				newBuff[newIndex].green = in1[newIndex].green;
				newBuff[newIndex].blue = in1[newIndex].blue;
			}
			else {
				newBuff[newIndex].red = (in1[(newIndex - 1) - aWidth].red / 8) + (in1[newIndex - aWidth].red / 8) +
							(in1[(newIndex + 1) - aWidth].red / 8) + (in1[newIndex - 1].red / 8) +
							(in1[newIndex + 1].red / 8) + (in1[(newIndex - 1) + aWidth].red / 8) +
							(in1[newIndex + aWidth].red / 8) + (in1[(newIndex + 1) + aWidth].red / 8);
				newBuff[newIndex].green = (in1[(newIndex - 1) - aWidth].green / 8) + (in1[newIndex - aWidth].green / 8) +
							(in1[(newIndex + 1) - aWidth].green / 8) + (in1[newIndex - 1].green / 8) +
							(in1[newIndex + 1].green / 8) + (in1[(newIndex - 1) + aWidth].green / 8) +
							(in1[newIndex + aWidth].green / 8) + (in1[(newIndex + 1) + aWidth].green / 8);
				newBuff[newIndex].blue = (in1[(newIndex - 1) - aWidth].blue / 8) + (in1[newIndex - aWidth].blue / 8) +
							(in1[(newIndex + 1) - aWidth].blue / 8) + (in1[newIndex - 1].blue / 8) +
							(in1[newIndex + 1].blue / 8) + (in1[(newIndex - 1) + aWidth].blue / 8) +
							(in1[newIndex + aWidth].blue / 8) + (in1[(newIndex + 1) + aWidth].blue / 8);
			}	
			newIndex++;
		}
	}

	setImage(aWidth, aHeight, input1->getMaxColor(), copyMagicNum(input1->getMagicNum()), newBuff);
	Logger::LogEvent("exiting execute", SinkName());
}
