#ifndef LF2DEPTH_STREAM_H
#define LF2DEPTH_STREAM_H

#include "LF2Stream.h"
#include <XnOS.h>
#include "LF2Common.h"
#include <XnMath.h>
#include <iostream>
#include "XnHash.h"
#include "XnEvent.h"
#include "XnPlatform.h"
#include "PS1080.h"

namespace LF2 {
  class LF2DepthStream :
    public LF2Stream
  {
  public:
    LF2DepthStream (libfreenect2::Freenect2Device * f2dev) : LF2Stream (f2dev) { 
      m_videoMode.pixelFormat = ONI_PIXEL_FORMAT_DEPTH_1_MM;
      m_videoMode.resolutionX = 640;
      m_videoMode.resolutionY = 480;
      m_videoMode.fps = 30;
    }
    
    ~LF2DepthStream ()
    {
      stop ();
    }

    // function for OpenNI2
    OniStatus
    start ()
    {
      m_f2dev->setIrAndDepthFrameListener (this);
      m_f2dev->start();
      return ONI_STATUS_OK;
    }

    void
    stop ()
    {
      m_f2dev->setIrAndDepthFrameListener (NULL);
    }

	OniStatus
    getProperty(int propertyId, void* data, int* pDataSize);

  protected:
    int
    BuildFrame (libfreenect2::Frame*,OniFrame*);
      OniStatus
      GetRealProperty(const XnChar* strName, XnDouble& dValue) const;
      OniStatus GetStringProperty(const XnChar* strName, XnChar* csValue, XnUInt32 nBufSize) const;
      OniStatus GetGeneralProperty(const XnChar* strName, XnUInt32 nBufferSize, void* pBuffer) const;
      OniStatus GetIntProperty(const XnChar* strName, XnUInt64& nValue) const;
  };
}

#endif // LF2DEPTH_STREAM_H
