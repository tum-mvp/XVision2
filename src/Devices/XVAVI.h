// *** BEGIN_XVISION2_COPYRIGHT_NOTICE ***
// *** END_XVISION2_COPYRIGHT_NOTICE ***

#ifndef _XVMPEG2_H_
#define _XVMPEG2_H_

#define DEF_MPEG_NUM   4

extern "C"{
#include <stdint.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

#include <stdio.h>

#include <XVVideo.h>

template <class IMTYPE>
class XVAVI : public XVVideo<IMTYPE> {
protected:
  using XVVideo<IMTYPE>::size ;
  using XVVideo<IMTYPE>::init_map ;
public:
  using XVVideo<IMTYPE>::frame ;

private:
  int		  fd;
  int		  rest_count;
  int		  index;
  int		  videoStream;
  AVCodecContext  *av_context;
  AVCodec	  *av_codec;
  AVFrame	  *av_frame;
  AVFormatContext *pFormatCtx;
  URLProtocol     file_protocol;
  AVPacket	  packet;
  int             bytesRemaining;
  uint8_t  	  *rawData;
   bool           fFirstTime;
  struct SwsContext *img_convert_ctx;
  AVFrame         *pFrameRGB;
  char *	  buffers[2];
public:

  XVAVI (const char *fname = "mpeg_file.mpg",
	  const char *parm_string="");
  ~XVAVI(){};
  virtual int open(const char * filename);
  void close();
  virtual int  initiate_acquire(int buffernum);
  virtual int  wait_for_completion(int buffernum);

  int set_params(char *params) {return 1;};
};

#endif
