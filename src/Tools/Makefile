CUR_DEPTH	= ../../
CXXFLAGS	= -O3 -pipe -DDEBUG_LEVEL=0 -DHAVE_CONFIG_H 
CPPFLAGS	= -I$(CUR_DEPTH)/include  -I$(IPPROOT)/include `pkg-config --cflags x11`

OBJS		= XVMatrix.o XVAffineWarp.o XVOmniWarper.o XVGeometry.o \
		  XVLookupTable.o 

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

$(SDBIN): ;	$(CXX) -o $(SDBIN) $(CPPFLAGS) $(SDSRC)   `pkg-config --libs x11`;
	  	install -c $(SDBIN) $(CUR_DEPTH)/bin
