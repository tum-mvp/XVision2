CUR_DEPTH	= ../../../
CPPFLAGS	=  -g -I/opt/local/include\
                  -I/lib/modules/`uname -r`/source/drivers/ieee1394\
                  -I$(CUR_DEPTH)/include -g -DSCREEN_DEPTH=32
OBJS		=  colorTracker.o ssdTracker.o edgeTracker.o
PROGS           =  colorTracker ssdTracker edgeTracker
LIBS		= -L$(CUR_DEPTH)/lib -lXVTrack -lXVDevs -lXVCons \
		    -L/opt/local/lib\
                   -lXVSeg -lXVTools -lXVImages -lmpeg  \
		   -lXext -lX11  -lpthread   

# if you want to set the screen depth explicitly,
# do it here.  Otherwise, the depth of the screen 
# used to compile XVision2 (the screen you type "make" in)
# will be determined and used.
SD = $(shell $(CUR_DEPTH)/bin/screendepth)

all: .depend $(OBJS)
	@for i in $(PROGS); do ($(CXX)  -o $$i $$i.o $(LIBS) -lm); done

clean: ;	rm -f *.o
		rm -f $(PROGS) .depend

.depend: $(OBJS:.o=.cc)
	$(CC) -M  $(CPPFLAGS) $(OBJS:.o=.cc) > .depend

install: all
	install -c $(PROGS) $(CUR_DEPTH)/bin

include .depend
