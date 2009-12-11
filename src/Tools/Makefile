CUR_DEPTH	= ../../
CXXFLAGS	= -O6 -pipe -DDEBUG_LEVEL=1 -mtune=cortex-a8 -march=armv7-a -ftree-vectorize -mfpu=neon -mfloat-abi=softfp -ffast-math -fomit-frame-pointer -funroll-loops -DHAVE_CONFIG_H 
CPPFLAGS	= -I$(CUR_DEPTH)/include 
OBJS		= XVMatrix.o XVAffineWarp.o XVOmniWarper.o XVGeometry.o XVLookupTable.o

SDSRC           = sd.cc
SDBIN           = screendepth

all: .depend $(OBJS)
	ar rv libXVTools.a $(OBJS)

clean: ;	rm -f *.o
	rm -f libXVTools.a .depend $(SDBIN)

install: $(SDBIN) all 
	install -c libXVTools.a $(CUR_DEPTH)/lib

.depend: $(SDBIN) $(OBJS:.o=.cc)
	$(CC) -M $(CPPFLAGS) $(OBJS:.o=.cc) > .depend 

include .depend

$(SDBIN): ;	$(CXX) -o $(SDBIN) $(CPPFLAGS) $(SDSRC)  -lXxf86vm -lXxf86dga -lXext -lX11 -lpng ;
	  	install -c $(SDBIN) $(CUR_DEPTH)/bin
