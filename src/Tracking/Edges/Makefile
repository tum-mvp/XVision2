CUR_DEPTH	= ../../..
CXXFLAGS        = -O6 -pipe -DDEBUG_LEVEL=1 -mtune=cortex-a8 -march=armv7-a -ftree-vectorize -mfpu=neon -mfloat-abi=softfp -ffast-math -fomit-frame-pointer -funroll-loops -DHAVE_CONFIG_H
CPPFLAGS	= -I$(CUR_DEPTH)/include
OBJS		= XVPattern.o

all: .depend $(OBJS)

clean: ;	rm -f *.o
	        rm -f .depend

.depend: $(OBJS:.o=.cc)
	$(CC) -M $(CPPFLAGS) $(OBJS:.o=.cc) > .depend 

include .depend

