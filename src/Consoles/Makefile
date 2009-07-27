
CUR_DEPTH	= ../../
CXXFLAGS	= -O6 -pipe -DDEBUG_LEVEL=1  
CPPFLAGS	= -I$(CUR_DEPTH)/include  
OBJS		= XVWindowX.o XVRemoteWindowX.o

all: .depend $(OBJS)
	ar rv libXVCons.a $(OBJS)

clean: ;	rm -f *.o
	rm -f libXVCons.a .depend

install: all
	install -c libXVCons.a $(CUR_DEPTH)/lib

.depend: $(OBJS:.o=.cc)
	$(CC) -M $(CPPFLAGS) $(OBJS:.o=.cc) > .depend 

include .depend
