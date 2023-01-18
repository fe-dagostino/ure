
#ifndef IMAGES_UTILS_H
#define IMAGES_UTILS_H

#ifdef _USE_STB
extern "C"
{
uint8_t* stb_load( const char* filename, unsigned int* size, int* width, int* height );
}
#endif

#ifdef _USE_FREEIMAGE
extern "C"
{
/**
 * Used to load an image using freeimage library.
 * returned buffer must be released with free() function.
 */
uint8_t* fi_load( const char* filename, unsigned int* size, int* width, int* height );
}
#endif 

#ifdef _USE_AVCPP
bool     av_init();
uint8_t* av_load( const char* filename, unsigned int* size, int* width, int* height );
bool     av_final();
#endif 

#ifdef _USE_DEVIL
extern "C"
{
bool     il_init();
uint8_t* il_load( const char* filename, unsigned int* size, int* width, int* height );
bool     il_final();
}
#endif 

#endif //IMAGES_UTILS_H

