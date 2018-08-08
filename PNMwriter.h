/*
	Author: John Nemeth
	Description: header file for PNMwriter
	Sources: Class material
*/
#ifndef PNMWRITER_H
#define PNMWRITER_H

#include <sink.h>

class PNMwriter : public Sink
{
	public:
		// derived function 
		virtual const char *SinkName() { return "PNMwriter"; };

		virtual void Write(char *);
};
#endif
