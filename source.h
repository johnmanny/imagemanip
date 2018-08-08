/*
	Author: John Nemeth
	Description: header file for source.c
	Sources: class material
*/
#ifndef SOURCE_H
#define SOURCE_H

#include <image.h>

class Source : public Image
{
	public:
		// constructor
		Source();

		// virtual functions
		virtual Image * GetOutput();
		virtual void sUpdate();
		virtual bool sCircPipeline(Image *);
	protected:
		// pure virtual functions
		virtual void Execute(void) = 0;
		virtual const char *SourceName() = 0;
		virtual const Image * retSourceIn1() = 0;
		virtual const Image * retSourceIn2() = 0;
		
		// flags for error checking
		bool updated;
		bool pipeCircular;
		bool isInput;
};
#endif
