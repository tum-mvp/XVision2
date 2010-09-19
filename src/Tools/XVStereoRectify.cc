#include "config.h"
#include <string.h>
#include <float.h>
#include "XVImageIO.h"
#include "XVStereoRectify.h"

#define Sqr(a) ((a)*(a))

XVColVector 
XVStereoRectify::cross(XVColVector &v1,XVColVector &v2)
{
   XVColVector res(3);

   res[0]=v1[1]*v2[2]-v1[2]*v2[1];
   res[1]=v1[2]*v2[0]-v1[0]*v2[2];
   res[2]=v1[0]*v2[1]-v1[1]*v2[0];
   return res;
}


XVImageScalar<float>
XVStereoRectify::calc_disparity(XVImageScalar<u_char> &image_l,
		                XVImageScalar<u_char> &image_r, bool left_image)
{
       int width=config.width;
       int height=config.height;
       IppiSize roi={temp_image1.Width(),temp_image1.Height()};
       IppiRect roi1={0,0,temp_image1.Width(),temp_image1.Height()};
       bzero(temp_image1.lock(),temp_image1.Width()*temp_image1.Height());
       temp_image1.unlock();
       ippiUndistortRadial_8u_C1R((Ipp8u*)image_l.data(), image_l.Width(),
				 temp_image1.lock(),temp_image1.Width(),
				 roi,config.camera_params[0].f[0], 
				 config.camera_params[0].f[1],
				 config.camera_params[0].C[0],
				 config.camera_params[0].C[1],
				 config.camera_params[0].kappa[0],
				config.camera_params[0].kappa[1], DistortBuffer);

       temp_image1.unlock();
       u_char min_vl,max_vl;
       u_char *ptr;
       min_vl=255,max_vl=0;
       ptr=temp_image1.lock();
       for(int i=0;i<temp_image1.Height();i++)
       {
         if(min_vl>ptr[i*width+i]) min_vl=ptr[i*width+i];
         if(max_vl<ptr[i*width+i]) max_vl=ptr[i*width+i];
       }
      
       temp_image1.unlock();
       bzero(temp_image2.lock(),temp_image2.Width()*temp_image2.Height());
       temp_image2.unlock();
       ippiWarpPerspective_8u_C1R((Ipp8u*)temp_image1.data(),
     				 roi,temp_image1.Width(),roi1,
				 (Ipp8u*)temp_image2.lock(),
				 temp_image2.Width(),roi1,coeffs_l,
				 IPPI_INTER_NN);
       temp_image2.unlock();
       IppiSize dst_roi={MAX_STEREO_WIDTH,MAX_STEREO_HEIGHT};
       IppiRect roi_rect={0,0,temp_image2.Width(),temp_image2.Height()};
       //XVWritePGM(temp_image2,"im_left.pgm");
       roi_rect.x=config.offset*width/MAX_STEREO_WIDTH;
       roi_rect.x=config.offset;
       ippiResize_8u_C1R((const Ipp8u*)temp_image2.data(),roi,
                                 temp_image2.Width(), roi_rect,
				 (Ipp8u *)gray_image_l.lock(),
				 gray_image_l.Width(),
				 dst_roi,(float)MAX_STEREO_WIDTH/width,
				 (float)MAX_STEREO_HEIGHT/height,IPPI_INTER_NN);
       gray_image_l.unlock();
       //XVWritePGM(gray_image_l,"im_left.pgm");
       roi_rect.x=0;
       bzero(temp_image1.lock(),temp_image1.Width()*temp_image1.Height());
       temp_image1.unlock();
       ippiUndistortRadial_8u_C1R((Ipp8u*)image_r.data(), image_r.Width(),
				 temp_image1.lock(),temp_image1.Width(),
				 roi,config.camera_params[1].f[0], 
				 config.camera_params[1].f[1],
				 config.camera_params[1].C[0],
				 config.camera_params[1].C[1],
				 config.camera_params[1].kappa[0],
			 	 config.camera_params[1].kappa[1], DistortBuffer);

       temp_image1.unlock();
       u_char min_vr,max_vr;
       min_vr=255,max_vr=0;
       ptr=temp_image1.lock();
       for(int i=0;i<temp_image1.Height();i++)
       {
         if(min_vr>ptr[i*width+i]) min_vr=ptr[i*width+i];
         if(max_vr<ptr[i*width+i]) max_vr=ptr[i*width+i];
       }
      
       temp_image1.unlock();
       //rectify rotation
       bzero(temp_image2.lock(),temp_image2.Width()*temp_image2.Height());
       temp_image2.unlock();
       ippiWarpPerspective_8u_C1R((Ipp8u*)temp_image1.data(),
      				 roi,temp_image1.Width(),roi1,
				 (Ipp8u*)temp_image2.lock(),
				 temp_image2.Width(),roi1,coeffs_r,
				 IPPI_INTER_NN);
       temp_image2.unlock();
       roi_rect.x=0;
       roi_rect.x=0;
       ippiResize_8u_C1R((const Ipp8u*)temp_image2.data(),roi,
                                 temp_image2.Width(), roi_rect,
				 (Ipp8u *)gray_image_r.lock(),
				 gray_image_r.Width(),
				 dst_roi,(float)MAX_STEREO_WIDTH/width,
				 (float)MAX_STEREO_HEIGHT/height,IPPI_INTER_NN);
       gray_image_r.unlock();
#ifdef NEVER
       if(min_vr < min_vl)
       {  
         ippiAddC_8u_C1IRSfs(min_vl-min_vr,(Ipp8u*)gray_image_r.lock(),
	        gray_image_r.Width(), dst_roi,1);
         gray_image_r.unlock();
       }
       else
       {
         ippiAddC_8u_C1IRSfs(min_vr-min_vl,(Ipp8u*)gray_image_l.lock(),
	        gray_image_l.Width(), dst_roi,1);
         gray_image_l.unlock();
       }
#endif
#ifdef OPENCV_STEREO
       cv::Mat img1(MAX_STEREO_HEIGHT,MAX_STEREO_WIDTH,CV_8U);
       cv::Mat img2(MAX_STEREO_HEIGHT,MAX_STEREO_WIDTH,CV_8U);
       memcpy(img1.data,gray_image_l.data(),
			gray_image_l.Width()*gray_image_l.Height());
       memcpy(img2.data,gray_image_r.data(),
			gray_image_r.Width()*gray_image_r.Height());
       sgbm(img1,img2,disp);
       short *rptr=(short*)disp.data;
       float *wptr=dispLeft.lock();
       for(int i=0;i<gray_image_l.Width()*gray_image_l.Height();i++)
           *wptr++=(float)*rptr++;
       dispLeft.unlock();
#else
       stereoUpload(gray_image_l.data(),gray_image_r.data());
       stereoProcess();
       stereoDownload(dispLeft.lock(),dispRight.lock());
       dispLeft.unlock(),dispRight.unlock();
#endif
       
       return left_image? dispLeft : dispRight;
}

bool
XVStereoRectify::calc_3Dpoints(int &num_points,Stereo_3DPoint* &Points3D)
{
  const float 		*r_ptr=dispLeft.data();
  Stereo_3DPoint	*point_ptr=PointBuffer;
  XVMatrix		CorrMat=R_l.t()*K_ideal.i()*B*K_ideal[0][0];

  num_points=MAX_STEREO_WIDTH*MAX_STEREO_HEIGHT;
  Points3D=PointBuffer;

  memset(PointBuffer,0,MAX_STEREO_WIDTH*MAX_STEREO_HEIGHT*sizeof(Stereo_3DPoint));
  XVColVector vec(3);

  for(int y=0;y<MAX_STEREO_HEIGHT;y++)
    for(int x=0;x<MAX_STEREO_WIDTH;x++,r_ptr++,point_ptr++)
    {
#ifndef OPENCV_STEREO
       if(*r_ptr==255 && *r_ptr==0) continue; //invalid pixel?
#endif
       vec[0]=x,vec[1]=y,vec[2]=1.0;
#ifndef OPENCV_STEREO
       vec=(CorrMat*vec)/(*r_ptr+config.offset);
#else
       vec=(CorrMat*vec)/(*r_ptr/16+config.offset);
#endif
       point_ptr->coord[0]=vec[0],
       point_ptr->coord[1]=vec[1],
       point_ptr->coord[2]=vec[2];
    }

  return true;
}

void 
XVStereoRectify::calc_rectification(Config &_config)
{
  int width=_config.width,height=_config.height;
  IppiSize roi={width,height};
  IppiRect roi_rect={0,0,width,height};


   XVMatrix ext(3,3);
   XVColVector T(3);
   ext[0][0]=_config.extrinsics[0],
   ext[0][1]=_config.extrinsics[1],
   ext[0][2]=_config.extrinsics[2],
   ext[1][0]=_config.extrinsics[4],
   ext[1][1]=_config.extrinsics[5],
   ext[1][2]=_config.extrinsics[6],
   ext[2][0]=_config.extrinsics[8],
   ext[2][1]=_config.extrinsics[9],
   ext[2][2]=_config.extrinsics[10];
   T[0]=_config.extrinsics[3],T[1]=_config.extrinsics[7],
   T[2]=_config.extrinsics[11];
   //T=ext.t()*T;
   
   XVColVector v1(3);
   XVColVector v2(3);
   B=sqrt(Sqr(T[0])+Sqr(T[1])+Sqr(T[2]));
   //calculate new Baseline alignment
   v1[0]=T[0]/B,v1[1]=T[1]/B,v1[2]=T[2]/B;
   v2[0]=1,v2[1]=0,v2[2]=0;
   v2=cross(v1,v2);
   float norm=sqrt(Sqr(v2[0])+Sqr(v2[1])+Sqr(v2[2]));
   float alph=asin(norm);
   if(norm>1e-7) v2/=norm;
   XVMatrix u_out(3,3);

   u_out[0][0]=v2[0]*v2[0],u_out[0][1]=v2[0]*v2[1],u_out[0][2]=v2[0]*v2[2];
   u_out[1][0]=v2[1]*v2[0],u_out[1][1]=v2[1]*v2[1],u_out[1][2]=v2[1]*v2[2];
   u_out[2][0]=v2[2]*v2[0],u_out[2][1]=v2[2]*v2[1],u_out[2][2]=v2[2]*v2[2];
   R_l[0][0]=1.0,R_l[0][1]=0.0,R_l[0][2]=0.0;
   R_l[1][0]=0.0,R_l[1][1]=1.0,R_l[1][2]=0.0;
   R_l[2][0]=0.0,R_l[2][1]=0.0,R_l[2][2]=1.0;
   //abusing R_r for the skew matrix
   R_r[0][0]=0.0,R_r[0][1]=-v2[2],R_r[0][2]=v2[1];
   R_r[1][0]=v2[2],R_r[1][1]=0,R_r[1][2]=-v2[0];
   R_r[2][0]=-v2[1],R_r[2][1]=v2[0],R_r[2][2]=0;
   R_l=u_out +(R_l-u_out)*cos(alph)+R_r*sin(alph);
   R_r=R_l*ext.t();
   // rectification matrices H
   XVMatrix K(3,3),H;
   // camera matrix
   K=0.0;
   K[0][0]=_config.camera_params[0].f[0];
   K[1][1]=_config.camera_params[0].f[0];
   K[2][2]=1.0;
   // ideal stereo camera
   K_ideal=K;
   K[0][2]=_config.camera_params[0].C[0];
   K[1][2]=_config.camera_params[0].C[1];
   K[1][1]=_config.camera_params[0].f[1];
   K_ideal[0][2]=width/2;
   K_ideal[1][2]=height/2;
   H=K.i();
   double quad[4][2];
   XVColVector coord(3);
   //project the four boundary corners
   coord[0]=0,coord[1]=0,coord[2]=1;
   coord=H*coord;
   coord[0]/=coord[2],coord[1]/=coord[2];coord[2]=1;
   coord=K_ideal*coord;
   quad[0][0]=coord[0],quad[0][1]=coord[1];
   coord[0]=_config.width,coord[1]=0,coord[2]=1;
   coord=H*coord;
   coord[0]/=coord[2],coord[1]/=coord[2];coord[2]=1;
   coord=K_ideal*coord;
   quad[1][0]=coord[0],quad[1][1]=coord[1];
   coord[0]=_config.width,coord[1]=_config.height,coord[2]=1;
   coord=H*coord;
   coord[0]/=coord[2],coord[1]/=coord[2];coord[2]=1;
   coord=K_ideal*coord;
   quad[2][0]=coord[0],quad[2][1]=coord[1];
   coord[0]=0,coord[1]=_config.height,coord[2]=1;
   coord=H*coord;
   coord[0]/=coord[2],coord[1]/=coord[2];coord[2]=1;
   coord=K_ideal*coord;
   quad[3][0]=coord[0],quad[3][1]=coord[1];
   ippiGetPerspectiveTransform(roi_rect,quad,coeffs_l);
   K[0][0]=_config.camera_params[1].f[0];
   K[1][1]=_config.camera_params[1].f[1];
   K[0][2]=_config.camera_params[1].C[0];
   K[1][2]=_config.camera_params[1].C[1];
   H=R_r*K.i();
   //project the four boundary corners
   coord[0]=0,coord[1]=0,coord[2]=1;
   coord=H*coord;
   coord[0]=coord[0]/coord[2],coord[1]=coord[1]/coord[2];coord[2]=1;
   coord=K_ideal*coord;
   quad[0][0]=coord[0],quad[0][1]=coord[1];
   coord[0]=_config.width,coord[1]=0,coord[2]=1;
   coord=H*coord;
   coord[0]=coord[0]/coord[2],coord[1]=coord[1]/coord[2];coord[2]=1;
   coord=K_ideal*coord;
   quad[1][0]=coord[0],quad[1][1]=coord[1];
   coord[0]=_config.width,coord[1]=_config.height,coord[2]=1;
   coord=H*coord;
   coord[0]=coord[0]/coord[2],coord[1]=coord[1]/coord[2];coord[2]=1;
   coord=K_ideal*coord;
   quad[2][0]=coord[0],quad[2][1]=coord[1];
   coord[0]=0,coord[1]=_config.height,coord[2]=1;
   coord=H*coord;
   coord[0]=coord[0]/coord[2],coord[1]=coord[1]/coord[2];coord[2]=1;
   coord=K_ideal*coord;
   quad[3][0]=coord[0],quad[3][1]=coord[1];
   ippiGetPerspectiveTransform(roi_rect,quad,coeffs_r);
}


XVStereoRectify::XVStereoRectify(Config & _config)
{
  
   config=_config;
   int width=config.width,height=config.height;
   IppiSize roi={width,height};
   int dist_buf_size;

#ifdef OPENCV_STEREO
   sgbm.preFilterCap = 63;
   sgbm.SADWindowSize = 3;
   sgbm.P1 = 8*sgbm.SADWindowSize*sgbm.SADWindowSize;
   sgbm.P2 = 32*sgbm.SADWindowSize*sgbm.SADWindowSize;
   sgbm.minDisparity = 0;
   sgbm.numberOfDisparities = 64;
   sgbm.uniquenessRatio = 10;
   sgbm.speckleWindowSize = 100;
   sgbm.speckleRange = 32;
   sgbm.disp12MaxDiff = 1;
   sgbm.fullDP = true;
#else
   stereoInit(MAX_STEREO_WIDTH,MAX_STEREO_HEIGHT);
#endif

   PointBuffer=new Stereo_3DPoint[MAX_STEREO_WIDTH*MAX_STEREO_HEIGHT];
   if(!PointBuffer) throw 1;
   R_l.resize(3,3), R_r.resize(3,3);

   dispLeft.resize(MAX_STEREO_WIDTH,MAX_STEREO_HEIGHT);
   dispRight.resize(MAX_STEREO_WIDTH,MAX_STEREO_HEIGHT);
   gray_image_l.resize(MAX_STEREO_WIDTH,MAX_STEREO_HEIGHT);
   gray_image_r.resize(MAX_STEREO_WIDTH,MAX_STEREO_HEIGHT);
   temp_image1.resize(width,height);
   temp_image2.resize(width,height);
   ippiUndistortGetSize(roi,&dist_buf_size);
   DistortBuffer=new Ipp8u[dist_buf_size];

   calc_rectification(_config);
}

XVStereoRectify::~XVStereoRectify()
{
  delete [] DistortBuffer;
  delete [] PointBuffer;
}
