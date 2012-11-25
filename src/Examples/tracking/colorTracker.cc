#include <config.h>
#include <XVMpeg.h>
#include <XVImageSeq.h>
#include <XVMpeg.h>
#include <XVVideo.h>
#include <XVColorSeg.h>
#include <XVBlobFeature.h>
#include <XVTracker.h>
#include <XVWindowX.h>

static u_short preferredHue = 0;
static u_short range = 2;

int main(int argc, char * argv[]){

  typedef XVVideo<XVImageRGB<XV_RGB> > VID;
  //typedef XVMpeg<XVImageRGB<XV_RGB> >  MPG;
  typedef XVMpeg<XVImageRGB<XV_RGB> >  MPG;
  typedef XVInteractWindowX<XV_RGB>    WIN;

  typedef XVBlobFeature<XVImageRGB<XV_RGB>, XVImageScalar<u_short> > BLOB;

  VID * vid = new MPG(argv[1]);

  for(int i=0;i<15;i++)
    vid->next_frame_continuous();
  WIN win(vid->frame(0));
  win.map();

  XVHueRangeSeg<XV_RGB, u_short> seg;

  BLOB feat(seg); //, true);  // for resampling

  XVDisplayTracker<VID, WIN, BLOB> tracker(*vid, win, feat);

  XVBlobState bs = tracker.init();
  
  while(1){
    try {
      bs = tracker.nextState();
    }catch(XVTrackerException te){ cout << te.what() << endl; };
  }
  delete vid;
};
