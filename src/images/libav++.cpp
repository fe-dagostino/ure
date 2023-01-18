

#include "avapplication.h"
#include "avimage.h"

using namespace libavcpp;

bool av_init()
{
  // init libav++. This needs to be done only once per application
  
  CAVApplication::initLibAVCPP();
  
  return true;
}

uint8_t* av_load( const char* filename, unsigned int* size, int* width, int* height )
{
  if ( filename == NULL )
    return NULL;
  
  CAVImage avImg;
  if ( avImg.load( filename, -1, -1, AV_PIX_FMT_RGBA ) != eAVSucceded )
    return NULL;
  
  unsigned char* _data = NULL;
  int            _size = 0;
  
  *width    = avImg.getWidth();
  *height   = avImg.getHeight();  
  
  _data     = avImg.detachData( 0, &_size );
  *size     = _size;
  
  return _data;  
}

bool           av_final()
{
  CAVApplication::deinitLibAVCPP();
  
  return true;
}
