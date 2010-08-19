#include "config.h"
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
       ippiUndistortRadial_8u_C1R((Ipp8u*)image_l.data(), image_l.Width(),
				 temp_image1.lock(),temp_image1.Width(),
				 roi,config.camera_params[0].f[0], 
				 config.camera_params[0].f[1],
				 config.camera_params[0].C[0],
				 config.camera_params[0].C[1],
				 config.camera_params[0].kappa[0],
				config.camera_params[0].kappa[1], DistortBuffer);

       temp_image1.unlock();
       ippiWarpPerspective_8u_C1R((Ipp8u*)temp_image1.data(),
      				 roi,temp_image1.Width(),roi1,
				 (Ipp8u*)temp_image2.lock(),
				 temp_image2.Width(),roi1,coeffs_l,
				 IPPI_INTER_NN);
       temp_image2.unlock();
       IppiSize dst_roi={640,480};
       IppiRect roi_rect={0,0,temp_image2.Width(),temp_image2.Height()};
       //XVWritePGM(gray_image_l,"im_left.pgm");
       ippiResize_8u_C1R((const Ipp8u*)temp_image2.data(),roi,
                                 temp_image2.Width(), roi_rect,
				 (Ipp8u *)gray_image_l.lock(),
				 gray_image_l.Width(),
				 dst_roi,640.0/width,480.0/height,IPPI_INTER_NN);
       gray_image_l.unlock();

       ippiUndistortRadial_8u_C1R((Ipp8u*)image_r.data(), image_r.Width(),
				 temp_image1.lock(),temp_image1.Width(),
				 roi,config.camera_params[1].f[0], 
				 config.camera_params[1].f[1],
				 config.camera_params[1].C[0],
				 config.camera_params[1].C[1],
				 config.camera_params[1].kappa[0],
			 	 config.camera_params[1].kappa[1], DistortBuffer);

       temp_image1.unlock();
       //rectify rotation
       ippiWarpPerspective_8u_C1R((Ipp8u*)temp_image1.data(),
      				 roi,temp_image1.Width(),roi1,
				 (Ipp8u*)temp_image2.lock(),
				 temp_image2.Width(),roi1,coeffs_r,
				 IPPI_INTER_NN);
       temp_image2.unlock();
       //XVWritePGM(gray_image_r,"im_right.pgm");
       ippiResize_8u_C1R((const Ipp8u*)temp_image2.data(),roi,
                                 temp_image2.Width(), roi_rect,
				 (Ipp8u *)gray_image_r.lock(),
				 gray_image_r.Width(),
				 dst_roi,640.0/width,480.0/height,IPPI_INTER_NN);
       gray_image_r.unlock();

       stereoUpload(gray_image_l.data(),gray_image_r.data());
       stereoProcess();
       stereoDownload(dispLeft.lock(),dispRight.lock());
       dispLeft.unlock(),dispRight.unlock();

       
       return left_image? dispLeft : dispRight;
}

XVStereoRectify::XVStereoRectify(Config & _config)
{
  
  config=_config;
  int width=config.width,height=config.height;
  XVMatrix R_l(3,3),R_r(3,3);
  IppiSize roi={width,height};
  int dist_buf_size;

   stereoInit(640,480);

   dispLeft.resize(640,480);
   dispRight.resize(640,480);
   gray_image_l.resize(640,480);
   gray_image_r.resize(640,480);
   temp_image1.resize(width,height);
   temp_image2.resize(width,height);
   ippiUndistortGetSize(roi,&dist_buf_size);
   DistortBuffer=new Ipp8u[dist_buf_size];


   XVMatrix ext(3,3);
   XVColVector T(3);
   ext[0][0]=config.extrinsics[0],
   ext[0][1]=config.extrinsics[1],
   ext[0][2]=config.extrinsics[2],
   ext[1][0]=config.extrinsics[4],
   ext[1][1]=config.extrinsics[5],
   ext[1][2]=config.extrinsics[6],
   ext[2][0]=config.extrinsics[8],
   ext[2][1]=config.extrinsics[9],
   ext[2][2]=config.extrinsics[10];
   T[0]=config.extrinsics[3],T[1]=config.extrinsics[7],
   T[2]=config.extrinsics[11];
   T=ext*T;
   
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
   //cerr << R_l << endl;
   R_r=R_l*ext;
   // rectification matrices H
   XVMatrix K(3,3),H;
   // camera matrix
   K=0.0;
   K[0][0]=config.camera_params[0].f[0];
   K[1][1]=config.camera_params[0].f[1];
   K[2][2]=1.0;
   // ideal stereo camera
   K_ideal=K;
   K[0][2]=config.camera_params[0].C[0];
   K[1][2]=config.camera_params[0].C[1];
   K_ideal[0][2]=width/2;
   K_ideal[1][2]=height/2;
   H=K_ideal*R_l*K.i();
   for(int i=0;i<3;i++)
      for(int j=0;j<3;j++)
	coeffs_l[i][j]=H[i][j];
   K[0][0]=config.camera_params[1].f[0];
   K[1][1]=config.camera_params[1].f[1];
   K[0][2]=config.camera_params[1].C[0];
   K[1][2]=config.camera_params[1].C[1];
   H=K_ideal*R_r*K.i();
   for(int i=0;i<3;i++)
      for(int j=0;j<3;j++)
	coeffs_r[i][j]=H[i][j];
}

XVStereoRectify::~XVStereoRectify()
{
  delete [] DistortBuffer;
}
