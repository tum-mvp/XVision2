#include "config.h"
#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <XVRemoteWindowX.h>
#include "XVImageSeq.h"
#include "StereoVidere.h"

//#define REMOVE_FLOOR
#define VIDEO   //show grayscale image


typedef StereoVidere<XVImageRGB<XV_RGB>, XVImageScalar<u_short> > VID;
typedef XVRemoteWindowX<XV_RGB>                                   WIN;

static VID* vid;

void sighndl(int ws) {
  cerr << "SIGINT called" << endl;
  if(vid) delete vid;
  exit(0);
}

void sighndl2(int ws) {
  cerr << "SIGSEGV called" << endl;
  if(vid) delete vid;
  exit(0);
}

static void
calc_display(XVImageScalar<u_short> &disp_image,
                                XVImageRGB<XV_RGB> &im)
{
  im.resize(disp_image);
  int size=im.Width()*im.Height();
  int   shift=3;

  XVImageIterator<u_short>  rptr(disp_image);
  XV_RGB*  wptr=im.lock() ;
  unsigned char value;

  for(int count=0;count<size;++rptr,wptr++,count++)
  {
     value=(*rptr<60000)? *rptr>>shift : 0;
     //value=(*rptr<60000 && *rptr>0)? 128 : 0;
     wptr->setR(value);
     wptr->setB(value);
     wptr->setG(value);
  }
  im.unlock();
}

int main(int argc, char* argv[]) {
  signal(SIGINT, sighndl);
  signal(SIGSEGV, sighndl2);


  char * config = getenv("VIDERE_CONFIG");
  
  XVImageSeq<XVImageRGB<XV_RGB> > left_seq("left_image%03d.ppm",1,20);
  XVImageSeq<XVImageRGB<XV_RGB> > right_seq("right_image%03d.ppm",1,20);
  if(config) {
    vid = new VID(config);
  }
  else {
    vid = new VID();
  }
  
  XVImageScalar<u_short> *disp_image;
  XVImageScalar<u_char> image[2],**images;
  XVImageRGB<XV_RGB>     im;
  XVImageRGB<XV_RGB>     li,ri,dummy;
#ifdef VIDEO
  RGBtoScalar(li=left_seq.next_frame_continuous(),image[0]);
  RGBtoScalar(ri=right_seq.next_frame_continuous(),image[1]);
  vid->LoadImages(image[0],image[1],dummy,false);

  disp_image = vid->calc_stereo();
  XVWindowX<XV_RGB>   video(li.Width(),
                            li.Height(),
  			    disp_image->Width()+3,0);
  video.map();
#endif 
  WIN 			 win(*disp_image);
  win.map();
  struct timeval start_time, end_time;
  while(1) {
    gettimeofday(&start_time, NULL);
    
    for(int i = 0; i < 50; i++) {
      RGBtoScalar(li=left_seq.next_frame_continuous(),image[0]);
      RGBtoScalar(ri=right_seq.next_frame_continuous(),image[1]);
      vid->LoadImages(image[0],image[1],dummy,false);

      disp_image = vid->calc_stereo();
      if(!disp_image) continue;
      calc_display(*disp_image,im);
      win.CopySubImage(im);
      win.swap_buffers();
      win.flush();
      //sleep(2);
    }

    gettimeofday(&end_time, NULL);

    cout << 50 / ((end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) * 1e-6)
	 << " [Hz]"<< endl;
  }

  delete vid;
  return 1;
}

