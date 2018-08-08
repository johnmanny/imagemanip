/*
	Author: John Nemeth
	Description: header file for sink.c
	Sources: Class material
*/
#ifndef SINK_H
#define SINK_H

#include <image.h>
#include <logging.h>

class Sink
{
	public:
		Sink();
		// base virtual function
		virtual const char *SinkName() = 0;
		// functions in filter - retuns info on source associated with this sink
		virtual void setIsUpdated(bool v)  { Logger::LogEvent("in sink setisupdated", SinkName()); };
		virtual bool getIsInput() 	   { Logger::LogEvent("in sink getisinput", SinkName()); };
		virtual void setIsCircular(bool v) { Logger::LogEvent("in sink setisreset", SinkName()); };
		virtual Image * getOutFrmSource()  { Logger::LogEvent("in sink getoutputfromsource", SinkName()); };

		virtual void SetInput(Image *);		// made virtual to avoid errors when using checksum
		void SetInput2(Image *);		// sets image pointer to input2
		
	protected:
		const Image * input1;
		const Image * input2;
};
#endif
