#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include "config.h"
#include "XVWindowX.h"
#include <XVImageScalar.h>
#include "XVAffineWarp.h"
#include <XVVideo.h>
#include "XVImageBase.h"
#include <XVV4L2.h>
#include <XVImageSeq.h>


static  XVV4L2<XVImageRGB<XV_RGB> >         *grabber;

void sighndl(int ws) {
  cerr << "SIGINT called" << endl;
  if(grabber) delete grabber;
   exit(0);
}


int main (int argc, char **argv) {

   int index=0;
   signal(SIGINT, sighndl);
   signal(SIGSEGV, sighndl);
   const int rate=10;
   struct timeval time1, time2;
   XVSize  size(640,480);
   XVWindowX<XV_RGB>      window1(size.Width(),size.Height());
   XVImageScalar<u_char> image(size.Width(),size.Height());
   XVImageRGB<XV_RGB> image_col(size.Width(),size.Height());
   grabber = new XVV4L2<XVImageRGB<XV_RGB > >(window1);

   window1.map();
  //grabber -> set_params ("I1N0B2");
  if (!grabber) {
    cerr<<"Error: no framegrabber found!"<<endl;
    exit(1);
  }
  grabber->initiate_acquire(index);
  for (;;) {
    //Start timing
    gettimeofday (&time1, NULL);
    for (int i=0; i<rate; i++) {


      grabber->initiate_acquire(index^1);
      grabber->wait_for_completion(index);
      memcpy(image.lock(),grabber->frame(index).data(),image.Width()*
                                                image.Height());
      image.unlock();
      window1.CopySubImage(ScalartoRGB(image,image_col));     
 
      window1.swap_buffers();
      window1.flush();
      index^=1;
    }
    //Produce timing results
    gettimeofday (&time2, NULL);
    cout<<"Rate: "<<rate/(time2.tv_sec-time1.tv_sec+
    (time2.tv_usec-time1.tv_usec)*1e-6) << " [Hz]" << endl;

  }
  return 0;
}

