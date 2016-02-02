#Wikipage detailing the content of each version and future work.

# GT Text Versions #

Follow the improvements of each version with the details
that helped its performance as well as future work devised.


## Version List ##

### Update 1.1.2 ###

  * Faster performance

Default GetPixel() [[1](http://msdn.microsoft.com/en-us/library/keze95wt(VS.80).aspx)] of MFC is very slow [[2](http://www.experts-exchange.com/Programming/System/Windows__Programming/MFC/Q_23756952.html)]
, so it has substituted
by a GetPixelFast() that access directly to memory to get the color and having checked  forehand the limit cases. (+ extra check while debugging)


  * New mode of region selection

The FillFlood() [[3](http://en.wikipedia.org/wiki/Flood_fill)] new mode works checking the color distance with the origin instead of the neighbor pixel used by the other modes.
This mode of getting the distance from the clicking pixel proves useful when regions have
a predominant color different from the background (draw back is it depends exactly where
you click, similar to MagicWand TM Photoshop). Feel free to use the other two modes that use gradient region properties when necessary.

  * Higher scalability for up to 70MP

An overall avoidance of the refreshing of the whole file but just when loading it the first time has made
the software possible to handle huge images, only limited by the memory and the intrinsic size of the image (12 bytes x (#)pixels is now ~= (O) the memory cost).

  * Compressed Ground Truth data definition

Information stored as runs (x,y,r) instead than single pixels (x,y)


### Update 1.2.1 ###

  * Improved pixel edition tool

The execution of pointer movements captures as being discrete made difficult a continuity of the pixel drawing. To solve this a drawing from last point captured and the new one has been made to smooth the pen drawing tool, noticing the difference in high loaded cpu moments.

### Update 1.2.2 ###

  * Improved the installation package

The installation now includes the pre-warper exe file that Windows versions use to check the system and old versions preventing the msi package from possible failures.
GTText\_1.2.2.exe file will then self extract and install all the software with better system check.

  * Faster Cut option

The cut tool has been speed up notably (One of the tools left to be speed from previous versions).

  * Minor visual smooth changes

Even less blinks

### Updates 1.2.3-6 ###

Developer releases.

### Release 1.3.0 ###

  * Fixed Bug:
Fixed bad Undo functioning when using the Cut tool (Incomplete state backup).

  * New functions:
Extracts the selected region into an Image of choice in black or using the original image colors.

  * Recognize the text
from all the color image or from the image formed out of the selected pixels using the tesseract-ocr engine [[4](http://code.google.com/p/tesseract-ocr/)].

  * New region editing option
to perform the selection from the whole image.

The selection algorithm now has an option to only stops checking once arrived to the limits of the image, having hence searched from all the image.

### Release 1.3.1 ###

  * Retry option when extracting text in which the image is scaled in every retry in order to permit read better the text to the extraction algorithm.

  * The extracted text it is showed in only some lines if text is too long, though it is copied to the clipboard in its full content when pressed continue.

### Release 1.4.2 ###

  * It is able to extract different languages adapting the program to the new characters set using tesseract engine.

  * It can extract a selected area by cropping the image and letting the engine work on the cropped image. This tool is very useful to get rid of misleading content that confuses the engine.


## References ##
[[1](http://msdn.microsoft.com/en-us/library/keze95wt(VS.80).aspx)] http://msdn.microsoft.com/en-us/library/keze95wt(VS.80).aspx

[[2](http://www.experts-exchange.com/Programming/System/Windows__Programming/MFC/Q_23756952.html)] http://www.experts-exchange.com/Programming/System/Windows__Programming/MFC/Q_23756952.html

[[3](http://en.wikipedia.org/wiki/Flood_fill)] http://en.wikipedia.org/wiki/Flood_fill

[[4](http://code.google.com/p/tesseract-ocr/)] http://code.google.com/p/tesseract-ocr/  tesseract-ocr