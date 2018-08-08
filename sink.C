/*
	Author: John Nemeth
	Description: implementation file for sink.h
	Sources: Class material
*/

#include <sink.h>
#include <stdlib.h>

Sink::Sink() {
	input1 = NULL;
	input2 = NULL;
}

void Sink::SetInput(Image * anInput) {
	if (!anInput) 
		Logger::LogEvent("---Warning! setinput input is null and will cause exception!", SinkName());
	if (getIsInput())
		setIsCircular(anInput->circPipeline(getOutFrmSource()));
	input1 = anInput;
	setIsUpdated(false);
}

void Sink::SetInput2(Image * anInput) {
	if (!anInput) 
		Logger::LogEvent("---Warning! setinput2 input is null and will cause exception!", SinkName());
	if (getIsInput())
		setIsCircular(anInput->circPipeline(getOutFrmSource()));
	input2 = anInput;
	setIsUpdated(false);
}
