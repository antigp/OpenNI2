#include "XnLib.h"
#include "LF2Device.h"
#include "LF2DepthStream.h"
#include "LF2ImageStream.h"
#include "LF2IrStream.h"

using namespace LF2;
using namespace oni::driver;

StreamBase*
LF2Device::createStream (OniSensorType sensorType)
{
  if (sensorType == ONI_SENSOR_DEPTH)
    {
      LF2DepthStream* pDepth = XN_NEW(LF2DepthStream,m_f2dev);
      return pDepth;
    }
  else
    {
      printf("This type of stream is not implemented yet\n");
      abort ();
    }
  // else if (sensorType == ONI_SENSOR_COLOR)
  //   {
  //     LF2ImageStream* pImage = XN_NEW(LF2ImageStream);
  //     return pImage;
  //   }
  // else if (sensorType == ONI_SENSOR_IR)
  //   {
  //     LF2IrStream* pIr = XN_NEW(LF2IrStream);
  //     return pIr;
  //   }

  return NULL;
}

void
LF2Device::destroyStream (StreamBase* pStream)
{
  XN_DELETE (pStream);
}

OniBool LF2Device::isPropertySupported(int propertyId)
{
	return false;
}

OniBool LF2Device::isCommandSupported(int commandId)
{
	return ONI_STATUS_NOT_IMPLEMENTED;
}

OniStatus LF2Device::tryManualTrigger()
{
	return ONI_STATUS_NOT_IMPLEMENTED;
}

OniBool LF2Device::isImageRegistrationModeSupported(OniImageRegistrationMode mode)
{
	return (mode == ONI_IMAGE_REGISTRATION_DEPTH_TO_COLOR);
}