/*
	Author: John Nemeth
	Description: header file for pnm reader
	Sources: class material
*/
#ifndef PNMREADER_H
#define PNMREADER_H

#include <source.h>
#include <stdio.h>

// is concrete due to defining function Execute
class PNMreader : public Source
{
	public:
		// constructor/destructors
		PNMreader();
		PNMreader(char *);			// parameterized constructor
		virtual ~PNMreader();			// virtual destructor

		// perform primary function
		virtual const char *SourceName() { return "PNMreader"; };
		virtual const Image * retSourceIn1() { return NULL; };
		virtual const Image * retSourceIn2() { return NULL; };
		virtual void Execute();
	private:
		char * fileName;
};
#endif
