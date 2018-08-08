/*
	Author: John Nemeth
	Description: implementation file for PNMwriter.h
	Sources: Class material
*/

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <PNMwriter.h>
#include <logging.h>

using std::ofstream;

void PNMwriter::Write(char * filename) {
	Logger::LogEvent("PNMwriter: Entering Write");
	if (!filename) {
		DataFlowException e(SinkName(), ": ---Error! No filename passed as 2nd argument!");
		throw e;
	}
	int width = input1->getWidth();
	int height = input1->getHeight();
	int numPixels = width * height;
	int maxVal = input1->getMaxColor();
	const char * magicNum = input1->getMagicNum();

	ofstream output;
	output.open(filename);

	const Pixel * pBuff = input1->getPBuffer();

	output << magicNum << "\n" << width << " " << height << "\n" << maxVal << "\n";
	char * buffPtr = (char*)pBuff;
	for (int i = 0; i < (numPixels * 3); i++) {
		output.put(*buffPtr);
		buffPtr++;
	}
	output.close();
	Logger::LogEvent("PNMwriter: exiting write");
}
