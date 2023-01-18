

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

uint8_t* stb_load( const char* filename, unsigned int* size, int* width, int* height )
{
  if ( filename == NULL )
    return NULL;
  
  int comp = 0;

  uint8_t* data = stbi_load( filename, width, height, &comp, 4);

  *size    = 4*(*width)*(*height);

  return data;  
}


