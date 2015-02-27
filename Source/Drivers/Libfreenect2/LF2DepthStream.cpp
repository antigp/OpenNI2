#include "LF2DepthStream.h"
#include "D2S.h.h"
#include "S2D.h.h"

using namespace LF2;

int
LF2DepthStream::BuildFrame(libfreenect2::Frame* frame_in,OniFrame* frame_out)
{
  // 1 DepthPixel is 2 byte
  frame_out->dataSize = (frame_in->width + 128) * (frame_in->height + 56) * 2;

  frame_out->data = xnOSMalloc (frame_out->dataSize);

  const float* const in_array = (float*) frame_in->data;
  uint16_t* const out_array = (uint16_t*) frame_out->data;
  unsigned int t = 0;
  unsigned int z = 0;

    
  for (unsigned int y = 0; y < frame_in->height + 56; y++)
	{
      for (unsigned int x = 0; x < frame_in->width + 128; x++)
		{
            if ((x >= frame_in->width) || (y >= frame_in->height)) {
                out_array[z] = 0;
                ++z;
            }
            else{
              out_array[z] = in_array[t];
              ++z;
              ++t;
            }
        }
    }


  frame_out->frameIndex = frame_in->sequence;
  frame_out->sensorType = ONI_SENSOR_DEPTH;

  frame_out->videoMode.pixelFormat = ONI_PIXEL_FORMAT_DEPTH_1_MM;
  frame_out->videoMode.resolutionX = frame_in->width+128;
  frame_out->videoMode.resolutionY = frame_in->height + 56;
  frame_out->videoMode.fps = 30;

  frame_out->width = frame_in->width + 128;
  frame_out->height = frame_in->height + 56;

  frame_out->cropOriginX = frame_out->cropOriginY = 0;
  frame_out->croppingEnabled = FALSE;

  frame_out->sensorType = ONI_SENSOR_DEPTH;
  frame_out->stride = (frame_in->width + 128)*sizeof(OniDepthPixel);
  frame_out->timestamp = frame_in->sequence*33369;

  return 1;
}

OniStatus 
LF2DepthStream::getProperty(int propertyId, void* data, int* pDataSize)
{
  OniStatus status = ONI_STATUS_NOT_SUPPORTED;

  switch (propertyId)
    {
    case ONI_STREAM_PROPERTY_HORIZONTAL_FOV:
      {
        float* val = (float*)data;
        XnDouble tmp;
        tmp =  LF2_DEPTH_HORIZONTAL_FOV * xnl::Math::DTR;
        *val = (float)tmp;
                    std::cout << "LF2_DEPTH_HORIZONTAL_FOV: " <<  tmp << std::endl;
        status = ONI_STATUS_OK;
        break;
      }
    case ONI_STREAM_PROPERTY_VERTICAL_FOV:
      {
        float* val = (float*)data;
        XnDouble tmp;
        tmp =  LF2_DEPTH_VERTICAL_FOV * xnl::Math::DTR;
        *val = (float)tmp;
          std::cout << "LF2_DEPTH_VERTICAL_FOV: " <<  tmp << std::endl;
        status = ONI_STATUS_OK;
        break;
      }		
	case ONI_STREAM_PROPERTY_MAX_VALUE:
      if (*pDataSize != sizeof(int))
		{
          return ONI_STATUS_BAD_PARAMETER;
		}

      *(int*)data = 10000;
      return ONI_STATUS_OK;
	case ONI_STREAM_PROPERTY_MIN_VALUE:
      if (*pDataSize != sizeof(int))
		{
          return ONI_STATUS_BAD_PARAMETER;
		}

      *(int*)data = 0;
      return ONI_STATUS_OK;
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
            ((OniVideoMode*)data)->resolutionX = 640;
            ((OniVideoMode*)data)->resolutionY = 480;
        }
        break;
    }
    case XN_STREAM_PROPERTY_ZERO_PLANE_DISTANCE:
            *(int*)data = 120;
        return ONI_STATUS_OK;
    case XN_STREAM_PROPERTY_ZERO_PLANE_PIXEL_SIZE:
            *(float*)data = 0.10520000010728836;
    return ONI_STATUS_OK;
        case XN_STREAM_PROPERTY_EMITTER_DCMOS_DISTANCE:
             *(float*)data = 7.5;
            return ONI_STATUS_OK;
        case XN_STREAM_PROPERTY_MAX_SHIFT:
            *(int*)data = 2047;
            return ONI_STATUS_OK;
        case XN_STREAM_PROPERTY_CONST_SHIFT:
            *(int*)data = 200;
            return ONI_STATUS_OK;
        case XN_STREAM_PROPERTY_PARAM_COEFF:
            *(int*)data = 4;
            return ONI_STATUS_OK;
        case XN_STREAM_PROPERTY_SHIFT_SCALE:
            *(int*)data = 10;
            return ONI_STATUS_OK;
        case XN_STREAM_PROPERTY_S2D_TABLE:
		{
			*pDataSize = sizeof(S2D);
			xnOSMemCopy(data, S2D, sizeof(S2D));
			status = ONI_STATUS_OK;
			break;
		}
        case XN_STREAM_PROPERTY_D2S_TABLE:
		{
			*pDataSize = sizeof(D2S);
			xnOSMemCopy(data, D2S, sizeof(D2S));
			status = ONI_STATUS_OK;
			break;
		}
    default:
      return LF2Stream::getProperty(propertyId, data, pDataSize);
    }
    
    
  return status;
}

OniStatus LF2DepthStream::GetIntProperty(const XnChar* strName, XnUInt64& nValue) const
{
    nValue = 2047;
    return ONI_STATUS_OK;
}
OniStatus LF2DepthStream::GetGeneralProperty(const XnChar* strName, XnUInt32 nBufferSize, void* pBuffer) const
{
    
    return ONI_STATUS_OK;
}
OniStatus LF2DepthStream::GetStringProperty(const XnChar* strName, XnChar* csValue, XnUInt32 nBufSize) const
{
    return ONI_STATUS_OK;
}
OniStatus LF2DepthStream::GetRealProperty(const XnChar* strName, XnDouble& dValue) const
{
    dValue = 1.0;
    return ONI_STATUS_OK;
}