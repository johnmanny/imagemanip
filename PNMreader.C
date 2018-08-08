/*
	Author: John Nemeth
	Description: implementation file for PNMreader.h
	Sources: class material
*/

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <PNMreader.h>
#include <logging.h>

using std::ifstream;

// primary constructor
PNMreader::PNMreader() {
	fileName = NULL;
}

// parameterized constructor
PNMreader::PNMreader(char * filename) {
	fileName = new char[CHAR_BUFFER];
	strcpy(fileName, filename);
}

// destructor
PNMreader::~PNMreader() {
	if (fileName)
		delete[] fileName;
}

// using pure virtual function
void PNMreader::Execute() {
	Logger::LogEvent("PNMreader: entering execute");
	if (!fileName) {
		DataFlowException e(SourceName(), ": ---Error! Filename not defined");
		throw e;
	}
	ifstream input;
	input.open(fileName);
	if (!input) {
		char msg[CHAR_BUFFER];
		sprintf(msg, ": ---Error! filename '%s' not found!", fileName);
		DataFlowException e(SourceName(), msg);
		throw e;
	}
	int aWidth, aHeight, maxVal, streamLength, curStreamPtrPos;
	char * aMagicNum;
	aMagicNum = new char[CHAR_BUFFER];

	// finding length of stream
	input.seekg(0, input.end);
	streamLength = input.tellg();
	input.seekg(0, input.beg);

	// getting char and integer inputs
	input.get(aMagicNum, 3, '\n');
	input >> aWidth >> aHeight >> maxVal;
	input.ignore();

	// get current pointer position in input stream
	curStreamPtrPos = input.tellg();

	// reading in image data from beginning of pixel buffer
	Pixel * aPBuffer = new Pixel[(aWidth * aHeight)];
	input.read((char*)aPBuffer, (streamLength - curStreamPtrPos));

	this->setMagicNum(aMagicNum);
	this->setWidth(aWidth);
	this->setHeight(aHeight);
	this->setMaxColor(maxVal);
	this->setPBuffer(aPBuffer);

	input.close();
	updated = true;
	Logger::LogEvent("PNMreader: exiting Execute");
}
