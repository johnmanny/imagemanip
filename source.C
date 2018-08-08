/*
	Author: John Nemeth
	Description: implementation file for source.C
	Sources: Class material
*/

#include <source.h>
#include <iostream>
#include <logging.h>
#include <stdio.h>

using std::cout;
using std::endl;

// primary constructor
Source::Source() {
	updated = false;
	pipeCircular = false;
	isInput = false;
}

/* updates image data:
	essentially a recursive method to check if current object
	is updated and to update all prior input objects
	if they're not updated before updating current object
*/
void Source::sUpdate() {
	if (!updated) {
		Logger::LogEvent("about to update", SourceName());
		if (pipeCircular) {
			DataFlowException e(SourceName(), ": ---Error! circular data pipeline found!");	
			throw e;
		}
		if (retSourceIn1()) {
			if (retSourceIn1() != this) 
				retSourceIn1()->Update();
			else {
				DataFlowException e(SourceName(), ": ---Error! input1 set to ittself!");
				throw e;
			}
		}
		if (retSourceIn2()) {
			if (retSourceIn2() != this) 
				retSourceIn2()->Update();
			else {
				DataFlowException e(SourceName(), ": ---Error! input2 set to ittself!");
				throw e;
			}
		}
		Execute();
	}
}

// checks whether newIn exists somewhere before in pipeline
bool Source::sCircPipeline(Image * newIn) {
	Logger::LogEvent("entering scircpipeline inside source", SourceName()); 
	if ((Image*)this == newIn){
		Logger::LogEvent("---Warning! Found circular data pipeline! causes exception!", SourceName()); 
		return true;
	}
	bool checkIn1 = false, checkIn2 = false;
	if (retSourceIn1())
		checkIn1 = const_cast<Image*>(retSourceIn1())->circPipeline(newIn);
	if (retSourceIn2())
		checkIn2 = const_cast<Image*>(retSourceIn2())->circPipeline(newIn);
	
	if (checkIn1 || checkIn2)
		return true;
	else
		return false;
}

// returns pointer to instance of image & sets isinput flag
Image * Source::GetOutput() {
	// assume every get output call refers to a SetInput call somewhere
	isInput = true;
	return this;
}
