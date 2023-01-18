

#include <FreeImage.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>


uint8_t* fi_load( const char* filename, unsigned int* size, int* width, int* height )
{
  if ( filename == NULL )
    return NULL;
  
  //check the file signature and deduce its format
  FREE_IMAGE_FORMAT fif = FreeImage_GetFileType( filename, 0);
  //if still unknown, try to guess the file format from the file extension
  if(fif == FIF_UNKNOWN) 
    fif = FreeImage_GetFIFFromFilename( filename );
  //if still unkown, return failure^M
  if(fif == FIF_UNKNOWN)
    return NULL;

  //check that the plugin has reading capabilities and load the file
  if(FreeImage_FIFSupportsReading(fif) == false)
    return NULL;
  
  FIBITMAP* odib = FreeImage_Load(fif, filename, 0 );
  
  //if the image failed to load, return failure
  if( odib == NULL )
    return NULL;
  
  unsigned char* data = NULL;
  FIBITMAP*      dib  = FreeImage_ConvertTo32Bits(odib);
  FreeImage_Unload(odib);
 
  *width    = FreeImage_GetWidth(dib);
  *height   = FreeImage_GetHeight(dib);
  *size     = 4*(*width)*(*height);
  data      = malloc( *size );
  
  
  BYTE* pixels = (BYTE*)FreeImage_GetBits(dib);
  
  //FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).
  int j = 0;
  for( j= 0; j< ((*width)*(*height)); j++)
  {
    data[j*4+0]= pixels[j*4+2];
    data[j*4+1]= pixels[j*4+1];
    data[j*4+2]= pixels[j*4+0];
    data[j*4+3]= pixels[j*4+3];
  }

  FreeImage_Unload(dib);
  
  return data;  
}


