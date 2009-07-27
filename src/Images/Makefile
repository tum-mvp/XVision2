
CUR_DEPTH	= ../../
CXXFLAGS	= -O6 -pipe -DDEBUG_LEVEL=1 
CPPFLAGS	= -I$(CUR_DEPTH)/include 
OBJS		= XVPixel.o XVColorBase.o XVImageBase.o XVImageScalar.o \
		  XVImageRGB.o XVImageYUV.o XVImageHSV.o XVImageYUV422.o \
		XVImageYCbCr.o	XVMaskedImage.o

all: .depend $(OBJS)
	ar rv libXVImages.a $(OBJS)

clean: ;	rm -f *.o
	rm -f libXVImages.a .depend

install: all
	install -c libXVImages.a $(CUR_DEPTH)/lib

.depend: $(OBJS:.o=.cc)
	$(CC) -M $(CPPFLAGS) $(OBJS:.o=.cc) > .depend 

include .depend
