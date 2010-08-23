#ifndef _XVSTEREORECTIFY_H_
#define _XVSTEREORECTIFY_H_
#include "ippi.h"
#include "ippcv.h"
#include "XVMatrix.h"
#include "XVImageScalar.h"
#include "Stereo.h"
#include "camera_config.h"

#define MAX_STEREO_WIDTH   640
#define MAX_STEREO_HEIGHT  480

using namespace std;

typedef struct
{
  float coord[3];
} Stereo_3DPoint;

class XVStereoRectify
{
   private:
     XVMatrix		   R_l,R_r; // rotation between ideal and real
     double 		   coeffs_l[3][3];
     double 		   coeffs_r[3][3];
     float       	   B;

     XVImageScalar<float>  dispLeft, dispRight;
     Stereo_3DPoint	   *PointBuffer;
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
     bool		   calc_3Dpoints(int &num_points,
     					Stereo_3DPoint * &Points3D);
     void		   calc_rectification(Config &_config);
     XVStereoRectify(Config & _config);
     ~XVStereoRectify();
};
#endif
