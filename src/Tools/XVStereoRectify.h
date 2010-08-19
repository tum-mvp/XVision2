#ifndef _XVSTEREORECTIFY_H_
#define _XVSTEREORECTIFY_H_
#include "ippi.h"
#include "ippcv.h"
#include "XVMatrix.h"
#include "XVImageScalar.h"
#include "Stereo.h"
#include "camera_config.h"

class XVStereoRectify
{
   private:
     double 		   coeffs_l[3][3];
     double 		   coeffs_r[3][3];
     float       	   B;
     XVImageScalar<float>  dispLeft, dispRight;
     Ipp8u  		   *DistortBuffer;
     XVImageScalar<u_char> temp_image1,temp_image2;
     Config		   config;
     XVMatrix 	           K_ideal;
     XVColVector           cross(XVColVector &v1,XVColVector &v2);
   public:
     XVImageScalar<u_char> gray_image_l,gray_image_r;
     XVImageScalar<float>  calc_disparity(XVImageScalar<u_char> &image_l,
     					  XVImageScalar<u_char> &image_r,
					  bool left_image=true) ;
     XVStereoRectify(Config & config);
     ~XVStereoRectify();
};
#endif
