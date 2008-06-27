/*                                                      -*-c++-*-
    Copyright (C) 2000 Gregory D. Hager and Darius Burschka (JHU
    Lab for Computational Interaction with Physical Systems (CIPS))

Permission is granted to any individual or institution to use, copy,
modify, and distribute this software, provided that this complete
copyright and permission notice is maintained, intact, in all copies and
supporting documentation.  Authors of papers that describe software
systems using this software package are asked to acknowledge such use by
a brief statement in the paper.

We provide this software "as is" without express or implied warranty.
*/

/**----------------------------USAGE-------------------------------------
 *             One argument should be provided for each flag.
 *             If no flags are specified, default values will be used.
 * -help me    print these "USAGE" comments to the screen       
 * -dev        use a specific device type for video input
 *             [Bt8x8]   (Default = Bt8x8)
 * -mpeg       use an mpeg file for video input (Default = testmpeg.mpeg)
 * -color      convert from color image to grayscale image using
 *             [I]ntensity [R]edband [G]reenband [B]lueband
 *             (Default = Intensity)
 * -output     produce output into .ppm and .pgm files or onto the screen
 *             [W]indow [F]iles [B]oth [N]one     (Default = Window)
 * -height     height of the resulting image (Default = 400)
 * -width      width of the resulting image (Default = 400)
 * -centerx    x-coordinate of the origin for warping (Default = 260)
 * -centery    y-coordinate of the origin for warping (Default = 220)
 * -widthsamp  scaling coefficient in x-direction (Default = 0.7)
 * -heightsamp scaling coefficient in y-direction (Default = 0.7)
 * -angle      angle of rotation of the image in degrees(Default = 0)
 * -spin       increment for the angle of rotation in degrees(Default = 0)
 * -sheer      sheer coefficient (Default = 0)
 * -rate       number of frames shown between each timing (Default = 100)
 *---------------------------------------------------------------------*/

#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include "config.h"
#include "XVWindowX.h"
#include <XVImageScalar.h>
#include "XVAffineWarp.h"
// Hard-coded path due to windows confusion
//#include "../../Devices/Mpeg.h"
#ifdef HAVE_BTTV
#include "XVBt8x8.h"
#include "XVV4L2.h"
#endif
//#include "VideoManager.h"
#include <XVVideo.h>
#include <XVMpeg.h>
#include "XVImageBase.h"
#include <XVImageIO.h>

static  XVWindowX<XV_RGB>      *window;
static  XVVideo<XVImageRGB<XV_RGB> >         *grabber;

void help();

int main (int argc, char **argv) {

  int framenum=0, next_fr=0, n_buffers, i, j, rate=100,
    height=480, width=640, xloc=width/2, yloc=height/2, output=1, depth;
  struct timeval time1, time2; 
  RGB_Projection in_color = XV_RGB_INTENSITY;
  float delta_angle=0, angle=0, sheer=0, ws=1, hs=1;
  char *title = "movie1.mpg", *name;
  XVImageRGB<XV_RGB> new_color_im;
  XVImageScalar<float> new_gray_im (640, 480), targ(width, height);

  //Rudimentary argument parsing 
  for (i=1; i<argc; i +=2) {
    if ( i+1 == argc) 
      cerr<<"Warning: one argument should be included for every flag!"<<endl;
    if (strcmp(argv[i], "-help") == 0) help();
    else if (strcmp(argv[i], "-dev") == 0) {
#ifdef HAVE_BTTV
      if (strcmp(argv[i+1], "Bt8x8") == 0) {
        grabber = new XVBt8x8<XVImageRGB<XV_RGB> >();
        grabber -> set_params ("B2I2N0");
      }
      else {
        cout<<"Warning: unknown device "<<(argv[i+1])
            <<" encountered and ignored!"<<endl; 
      }
#endif
    }
    else if (strcmp(argv[i], "-mpeg") == 0) {
	title = argv[i+1];
	//grabber = new VideoManager<Mpeg<XV_RGB> >(title);
	grabber = new XVMpeg<XVImageRGB<XV_RGB> > (title);
    }
    else if (strcmp(argv[i], "-color") == 0) 
      switch (argv[i+1][0]) {
      case 'I': in_color = XV_RGB_INTENSITY;
	cout<<"Color images will be converted to grayscale"
            <<" images using XV_RGB_INTENSITY representation"<<endl;  
        break;
      case 'R': in_color = XV_REDBAND;
	cout<<"Color images will be converted to grayscale"
            <<" images using XV_REDBAND representation"<<endl;  
        break;
      case 'G': in_color = XV_GREENBAND;
	cout<<"Color images will be converted to grayscale"
            <<" images using XV_GREENBAND representation"<<endl;  
        break;
      case 'B': in_color = XV_BLUEBAND;
	cout<<"Color images will be converted to grayscale"
            <<" images using XV_BLUEBAND representation"<<endl;  
        break;
      default: cerr<<"Warning: unknown color mode, using default!"<<endl;
      }
    else if (strcmp(argv[i], "-output") == 0) {
      switch (argv[i+1][0]) {
      case 'F': 
        output = 2;
	break;
      case 'W': 
        output = 1;
	break;
      case 'B': 
        output = 3;
	break;
      case 'N':
        output = 0;
        break;
      default: cerr<<"Warning: unknown ooutput mode, using default!"<<endl;
        output = 1;
      }
    }
    else if (strcmp(argv[i], "-height") == 0) {
      height = atoi(argv[i+1]);
      //yloc = height/2;
    }
    else if (strcmp(argv[i], "-width") == 0) {
      width = atoi(argv[i+1]);
      //xloc = width/2;
    }
    else if (strcmp(argv[i], "-centerx") == 0) {
      xloc = atoi(argv[i+1]);
    }
    else if (strcmp(argv[i], "-centery") == 0) {
      yloc = atoi(argv[i+1]);
    }
    else if (strcmp(argv[i], "-widthsamp") == 0) {
      ws = atof(argv[i+1]);
    }
    else if (strcmp(argv[i], "-heightsamp") == 0) {
      hs = atof(argv[i+1]);
    }
    else if (strcmp(argv[i], "-spin") == 0) {
      delta_angle = atof(argv[i+1]);
    }
    else if (strcmp(argv[i], "-sheer") == 0) {
      sheer = atof(argv[i+1]);
    }
    else if (strcmp(argv[i], "-angle") == 0) {
      angle = atof(argv[i+1]);
    }
    else if (strcmp(argv[i], "-rate") == 0) {
      rate = atoi(argv[i+1]);
    }
    else {
      cerr<<"Error: unknown flag!"<<endl;
      help();
      exit(1);
    }
  } //end rudimentary parsing

#ifdef HAVE_BTTV
  if (!grabber) {
    grabber = new XVV4L2<XVImageRGB<XV_RGB> >(XVSize(640,480));
    grabber -> set_params ("I2N0B2");
  }
#endif
  if (!grabber) {
    cerr<<"Error: no framegrabber found!"<<endl;
    exit(1);
  }
  n_buffers=grabber->buffer_count();

  //Set up the window
  if (output == 1 || output == 3) {
    window = new XVWindowX<XV_RGB> (grabber->frame(0), 0, 0);
    window -> map();
  }  
  for (;;) {
    //Start timing
    gettimeofday (&time1, NULL);
    for (i=0; i<rate; i++) {

      new_color_im = grabber->next_frame_continuous();

      if (output>1) {
        XVWritePPM(new_color_im, "image.ppm");
        RGBtoScalar(new_color_im, new_gray_im, in_color);
        XVWritePGM(new_gray_im, "image.pgm");
      }
 

      if (output == 1 || output ==3) 
        window -> CopySubImage(new_color_im);     
 
      //Swap and flush
      if (output == 1 || output ==3) {
        window -> swap_buffers();
        window -> flush();
      }
    }
    //Produce timing results
    gettimeofday (&time2, NULL);
    cout<<"Rate: "<<rate/(time2.tv_sec-time1.tv_sec+
    (time2.tv_usec-time1.tv_usec)*1e-6) << " [Hz]" << endl;

  }
  return 0;
}

void help(){
  cout<<"*---------------------------USAGE------------------------------------"<<endl
      <<"*             One argument should be provided for each flag."<<endl
      <<"*             If no flags are specified, default values will be used."<<endl
      <<"* -help me    print these USAGE comments to the screen"<<endl
      <<"* -dev        use a specific device type for video input"<<endl
      <<"*             [Bt8x8]   (Default = Bt8x8)"<<endl
      <<"* -mpeg       use an mpeg file for video input (Default = testmpeg.mpeg)"<<endl
      <<"* -color      convert from color image to grayscale image using"<<endl
      <<"*             [I]ntensity [R]edband [G]reenband [B]lueband"<<endl
      <<"*             (Default = Intensity)"<<endl
      <<"* -output     produce output into .ppm and .pgm files or onto the screen"<<endl
      <<"*             [W]indow [F]iles [B]oth [N]one     (Default = Window)"<<endl
      <<"* -height     height of the resulting image (Default = 400)"<<endl
      <<"* -width      width of the resulting image (Default = 400)"<<endl
      <<"* -centerx    x-coordinate of the origin for warping (Default = 260)"<<endl
      <<"* -centery    y-coordinate of the origin for warping (Default = 220)"<<endl
      <<"* -widthsamp  scaling coefficient in x-direction (Default = 0.7)"<<endl
      <<"* -heightsamp scaling coefficient in y-direction (Default = 0.7)"<<endl
      <<"* -angle      angle of rotation of the image in degrees(Default = 0)"<<endl
      <<"* -spin       increment for the angle of rotation in degrees(Default = 0)"<<endl
      <<"* -sheer      sheer coefficient (Default = 0)"<<endl
      <<"* -rate       number of frames shown between each timing (Default = 100)"<<endl
      <<"*--------------------------------------------------------------------"<<endl;
}





