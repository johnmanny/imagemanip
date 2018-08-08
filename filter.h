/*
	Author: John Nemeth
	Description: header file for image manipulator classes
*/
#ifndef FILTER_H
#define FILTER_H

#include <source.h>
#include <sink.h>

// color source ////////////////////////////////////////////////////
class Color: public Source
{
	public:
		Color(int, int, int, int, int);		//height, width, r, g, b
		virtual const char *SourceName() { return "Color"; };
		virtual void Execute() {};
		virtual const Image * retSourceIn1() { return NULL; };
		virtual const Image * retSourceIn2() { return NULL; };
};

// checksum sink ///////////////////////////////////////////////////
class CheckSum: public Sink
{
	public: 
		void OutputCheckSum(const char *);
		virtual void SetInput(Image * anInput) { input1 = anInput; };
		virtual const char *SinkName() { return "CheckSum"; };
};

// filter source/sink //////////////////////////////////////////////
class Filter : public Source, public Sink
{
	public:
		// utility functions (reduces clutter)
		void checkInputs();
		void checkInput1();
		void checkEqualDims();
		void setImage(int, int, int, char *, Pixel *);
		char * copyMagicNum(const char *);

		// virtual functions (moves data between source/sink)
		virtual const char *FilterName() = 0;
		virtual const char *SourceName() { return FilterName(); };
		virtual const char *SinkName() { return FilterName(); };
		virtual const Image * retSourceIn1() { return input1; };
		virtual const Image * retSourceIn2() { return input2; };
		virtual void setIsUpdated(bool v) { updated = v; };
		virtual bool getIsInput() { return isInput; };
		virtual void setIsCircular(bool v) { pipeCircular = v; };
		virtual Image * getOutFrmSource() { return GetOutput(); };
};

// filters /////////////////////////////////////////////////////////
class Shrinker : public Filter
{
	public:
		virtual void Execute();
		virtual const char *FilterName() { return "Shrinker"; };
};

class LRCombine : public Filter
{
	public:
		virtual void Execute();
		virtual const char *FilterName() { return "LRCombine"; };
};

class TBCombine : public Filter
{
	public:
		virtual void Execute();
		virtual const char *FilterName() { return "TBCombine"; };
};

class Blender : public Filter
{
	public:
		Blender();
		Blender(double);

		void SetFactor(double);
		virtual const char *FilterName() { return "Blender"; };
		virtual void Execute();
	private:
		double factor;
};

class Mirror : public Filter
{
	public:
		virtual const char *FilterName() { return "Mirror"; };
		virtual void Execute();
};

class Rotate : public Filter
{
	public:
		virtual const char *FilterName() { return "Rotate"; };
		virtual void Execute();
};

class Subtract: public Filter
{
	public:
		virtual const char *FilterName() { return "Subtract"; };
		virtual void Execute();
};

class Grayscale: public Filter
{
	public:
		virtual const char *FilterName() { return "Grayscale"; };
		virtual void Execute();
};

class Blur: public Filter
{
		virtual const char *FilterName() { return "Blur"; };
		virtual void Execute();

};
#endif
