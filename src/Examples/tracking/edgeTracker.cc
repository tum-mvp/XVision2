#include <config.h>
#include <XVMpeg.h>
#include <XVImageSeq.h>
#include <XVVideo.h>
#include <XVMpeg.h>
//#include <XVDig1394.h>
#include <XVEdgeFeature.h>
#include <XVTracker.h>
#include <XVWindowX.h>

static u_short preferredHue = 0;
static u_short range = 2;

int main(int argc, char * argv[]){

  typedef XVVideo<XVImageRGB<XV_RGB> > VID;
  //typedef XVMpeg<XVImageRGB<XV_RGB> >  MPG;
  typedef XVMpeg<XVImageRGB<XV_RGB> >  MPG;
  typedef XVInteractWindowX<XV_RGB>    WIN;

  typedef XVEdgeFeature<int, XVEdge<int> > EDGE;

  VID * vid = new MPG(argv[1]);

  WIN win(vid->frame(0), 0, 400);
  win.map();

  XVEdge<int> edge;

  EDGE feat(edge, 40); 

  XVDisplayTracker<VID, WIN, EDGE > tracker(*vid, win, feat);

  XVLineState bs = tracker.init();

  XVWindowX<XV_RGB> win2(vid->frame(0), 600, 0);
  win2.map();
  
  while(1){
    try {
      tracker.nextState();
      XV2Vec<double> ends[2];
      bs = feat.diffState();
      
      win2.CopySubImage((XVImageRGB<XV_RGB>)(feat.warpedImage()));
      win2.swap_buffers();
      win2.flush();

    }catch(XVTrackerException te){ cout << te.what() << endl; };
  }
  delete vid;
};
