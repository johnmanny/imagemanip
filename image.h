/*	Author: John Nemeth
	Description: header file for image/pixel struct
	Sources: class material
*/
#ifndef IMAGE_H
#define IMAGE_H

#define CHAR_BUFFER 128

struct Pixel
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

class Image
{
	public:
		// constructor/destructors
		Image();
		Image(const Image &);		// copy constructor
		Image(int , int);		// parametrized constructor 
		virtual ~Image();		// virtual destructor

		// mutators
		void setWidth(int);
		void setHeight(int);
		void setMaxColor(int);
		void setMagicNum(char *);
		void setPBuffer(Pixel *);

		// accessors
		int getWidth() const;
		int getHeight() const;
		int getMaxColor() const;
		const char * getMagicNum() const;
		const Pixel * getPBuffer() const;

		// update/error check functions
		void Update()const { const_cast<Image*>(this)->sUpdate(); };	// update func for image
		bool circPipeline(Image *);					// returns virtual circular pipeline check
		virtual void sUpdate();						// virtual update function implemented for souce's update
		virtual bool sCircPipeline(Image *);

	private:
		int width;
		int height;
		int maxColorVal;	// holds color depth
		char *magicNum;		// holds type of pnm
		Pixel *pBuffer;		// holds pixel RGB values
};
#endif
