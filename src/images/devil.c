

#include "IL/il.h"
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

bool il_init()
{
  // init DevIL. This needs to be done only once per application
  ilInit();
  
  return true;
}

uint8_t* il_load( const char* filename, unsigned int* size, int* width, int* height )
{
  if ( filename == NULL )
    return NULL;
  
  ILboolean success;
  unsigned int imageID;
 
  // generate an image name
  ilGenImages(1, &imageID); 
  // bind it
  ilBindImage(imageID); 
  // match image origin to OpenGL’s
  ilEnable(IL_ORIGIN_SET);
  ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
  // load  the image
  success = ilLoadImage(filename);
  // check to see if everything went OK
  if (!success) 
  {
    ilDeleteImages(1, &imageID); 
 
    return NULL;
  }

  /* Convert image to RGBA with unsigned byte data type */
  ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

  unsigned char* data = NULL;
  *width    = ilGetInteger(IL_IMAGE_WIDTH);
  *height   = ilGetInteger(IL_IMAGE_HEIGHT);  
  *size     = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL)*(*width)*(*height);
  data      = malloc( *size );
  
  memcpy( data, ilGetData(), *size );
  
  ilDeleteImages(1, &imageID); 
  
  return data;  
}

bool           il_final()
{
  return true;
}
