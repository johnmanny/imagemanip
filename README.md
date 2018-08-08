# imagemanip
## Notes:
Object design layout is in proj.pdf. It's worth noting proj.pdf were not
the instructions for the final revision of this code and it does not reflect
the full functionality.

The only work that is not original is the bulk of the code in 
logging.h and the exception code at the bottom of main.C. All other 
code, unless explicitly stated in a comment, is original and was 
written throughout the 2018 spring term of UO's C++ class. 

## Description:
This program features exception handling and a highly hierarchical, 
inheritance-based OOP design for inputing a PNM image file, performing 
manipulations, and outputting a PNM image file. 

## Instructions:
The makefile organizes directories before using the compiler. To view images as PNG,
there is a makefile target to create a PNG using "pnmtopng". To compile it all and
produce the output image (specified in the makefile to be "outimage.png"), as well
as revert the repo to its original state, you can use the makefile targets "all" and
"clean". 

```
make all
...
make clean
```


