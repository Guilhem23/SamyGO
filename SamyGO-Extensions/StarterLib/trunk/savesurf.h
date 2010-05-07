#include <SDL_image.h>
#include <png.h>
#include "jpeglib.h"
// #include <setjmp.h>

// /* import log */
// extern void loginfo(const char *, ...);
#define loginfo printf

static int png_colortype_from_surface(SDL_Surface *surface)
{
	int colortype = PNG_COLOR_MASK_COLOR; /* grayscale not supported */

	if (surface->format->palette)
		colortype |= PNG_COLOR_MASK_PALETTE;
	else if (surface->format->Amask)
		colortype |= PNG_COLOR_MASK_ALPHA;
		
	return colortype;
}

void png_user_warn(png_structp ctx, png_const_charp str)
{
	loginfo("libpng: warning: %s\n", str);
}

void png_user_error(png_structp ctx, png_const_charp str)
{
	loginfo("libpng: error: %s\n", str);
}



int png_save_surface(char *filename, SDL_Surface *surf)
{
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	int i, colortype;
	png_bytep *row_pointers;

	/* Opening output file */
	fp = fopen(filename, "wb");
	if (fp == NULL) {
		loginfo("fopen error");
		return -1;
	}

	/* Initializing png structures and callbacks */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
		NULL, png_user_error, png_user_warn);
	if (png_ptr == NULL) {
		loginfo("png_create_write_struct error!\n");
		return -1;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		loginfo("png_create_info_struct error!\n");
		exit(-1);
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		exit(-1);
	}

	png_init_io(png_ptr, fp);

	colortype = png_colortype_from_surface(surf);
	png_set_IHDR(png_ptr, info_ptr, surf->w, surf->h, 8, colortype,	PNG_INTERLACE_NONE, 
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	/* Writing the image */
	png_write_info(png_ptr, info_ptr);
	png_set_packing(png_ptr);

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep)*surf->h);
	for (i = 0; i < surf->h; i++)
		row_pointers[i] = (png_bytep)(Uint8 *)surf->pixels + i*surf->pitch;
	png_write_image(png_ptr, row_pointers);
	png_write_end(png_ptr, info_ptr);

	/* Cleaning out... */
	free(row_pointers);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);

	return 0;
}

static int jpeg_colortype_from_surface(SDL_Surface *surface)
{
	int colortype = JCS_UNKNOWN;

	if (surface->format->palette)
		colortype |= JCS_RGB;
	else if (surface->format->Amask)
		colortype |= JCS_YCbCr;

	//	JCS_UNKNOWN,		/* error/unspecified */
	//	JCS_GRAYSCALE,		/* monochrome */
	//	JCS_RGB,		/* red/green/blue */
	//	JCS_YCbCr,		/* Y/Cb/Cr (also known as YUV) */
	//	JCS_CMYK,		/* C/M/Y/K */
	//	JCS_YCCK		/* Y/Cb/Cr/K */
	
	// return JCS_YCCK; // green
	// return JCS_CMYK; // pictureokcolosnot
	return JCS_RGB;
}

static int jpeg_colors_from_surface(SDL_Surface *surface)
{
	return (surface->format->BitsPerPixel / 8 - 1);
}

int jpeg_save_surface(char *filename, SDL_Surface *surf){
	FILE *fp;
	int col_comps = jpeg_colors_from_surface(surf), i, j;

	/* Opening output file */
	fp = fopen(filename, "wb");
	if (fp == NULL) {
		loginfo("fopen error");
		return -1;
	}

	JSAMPLE *image_buffer = (JSAMPLE*) malloc(sizeof(JSAMPLE) * surf->w * surf->h * col_comps);
	if(surf == NULL || image_buffer == NULL)
		return -1;

	for(i = 0; i < surf->w * surf->h * 4; i += 4) {
		memcpy(image_buffer + (i / 4 * col_comps), surf->pixels + i, col_comps);
	}

	// initialise jpeg library
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	// write to file
	jpeg_stdio_dest(&cinfo, fp);

	// setup library
	cinfo.image_width = surf->w;
	cinfo.image_height = surf->h;
	cinfo.input_components = col_comps; // 1 gray, 3 RGB
	cinfo.in_color_space = jpeg_colortype_from_surface(surf);
	jpeg_set_defaults(&cinfo); // set defaults
	jpeg_set_quality(&cinfo, 75, TRUE);

	// start compressing
	int row_stride = surf->w * col_comps;
	JSAMPROW row_pointer[1]; // pointer to the current row data
	jpeg_start_compress(&cinfo, TRUE); // start compressing to jpeg

	while (cinfo.next_scanline < cinfo.image_height) {
		row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);

	// clean up
	fclose(fp);
	jpeg_destroy_compress(&cinfo);
	return 0;
}
