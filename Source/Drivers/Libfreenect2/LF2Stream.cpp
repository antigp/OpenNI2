#include "LF2Stream.h"
#include "Driver/OniDriverAPI.h"
#include <iostream>

using namespace LF2;
using namespace oni::driver;
using namespace libfreenect2;

bool
LF2Stream::onNewFrame (Frame::Type type,Frame* frame)
{    
  if ((m_videoMode.pixelFormat == ONI_PIXEL_FORMAT_DEPTH_1_MM && type != Frame::Depth) || (m_videoMode.pixelFormat != ONI_PIXEL_FORMAT_DEPTH_1_MM && type != Frame::Ir))
    {
      return false;      
    }
  
  OniFrame* pFrame = getServices ().acquireFrame ();
  BuildFrame (frame,pFrame);
  raiseNewFrame (pFrame);
  getServices ().releaseFrame (pFrame);
  return true;
}

OniStatus 
LF2Stream::getProperty(int propertyId, void* data, int* pDataSize)
{
  OniStatus status = ONI_STATUS_NOT_SUPPORTED;

  switch (propertyId)
    {
    case ONI_STREAM_PROPERTY_CROPPING:
      {
        if (*pDataSize != sizeof(OniCropping))
          {
            printf("Unexpected size: %d != %ld\n", *pDataSize, sizeof(OniCropping));
            status = ONI_STATUS_ERROR;
          }
        ((OniCropping*) data)->enabled = false;
        ((OniCropping*) data)->originX = 0;
        ((OniCropping*) data)->originY = 0;
        ((OniCropping*) data)->width = m_videoMode.resolutionX;
        ((OniCropping*) data)->height = m_videoMode.resolutionY;
        status = ONI_STATUS_OK;        
        break;      
      }      
    case ONI_STREAM_PROPERTY_VIDEO_MODE:
      {
        if (*pDataSize != sizeof(OniVideoMode))
          {
            printf("Unexpected size: %d != %ld\n", *pDataSize, sizeof(OniVideoMode));
            status = ONI_STATUS_ERROR;
          }
        else
          {
            status = GetVideoMode((OniVideoMode*)data);
            std::cout << "video mode width: " <<  ((OniVideoMode*)data)->resolutionX << std::endl;
          }
        break;
      }
    default:
      status = ONI_STATUS_NOT_SUPPORTED;
      break;      
    }

  return status;  
}
