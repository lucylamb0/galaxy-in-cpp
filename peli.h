#include <windows.h>		// Header File For Windows
#include <windowsx.h>         //hy�dyllisi� windows juttuja
#include <mmsystem.h>
#include <math.h>			// Math Library Header File
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <vector>           //v�h�n c++
#include <string.h>  
#include "MilkshapeModel.h"
#include "Model.h"
#include "MS3DFile.h"

void DrawCharacter(float x,float y,float z);
GLuint LoadGLTexture( const char *filename );


//AUX_RGBImageRec *LoadBMP(const char *Filename)						// Loads A Bitmap Image
//{
//    FILE *File=NULL;												// File Handle
//
//    if (!Filename)													// Make Sure A Filename Was Given
//    {
//        return NULL;												// If Not Return NULL
//    }
//
//    File=fopen(Filename,"r");										// Check To See If The File Exists
//
//    if (File)														// Does The File Exist?
//    {
//        fclose(File);												// Close The Handle
//        return auxDIBImageLoad(Filename);							// Load The Bitmap And Return A Pointer
//    }
//
//    return NULL;													// If Load Failed Return NULL
//}
//
//
//GLuint LoadGLTexture( const char *filename )						// Load Bitmaps And Convert To Textures
//{
//    AUX_RGBImageRec *pImage;										// Create Storage Space For The Texture
//    GLuint texture = 0;												// Texture ID
//
//    pImage = LoadBMP( filename );									// Loads The Bitmap Specified By filename
//
//    // Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
//    if ( pImage != NULL && pImage->data != NULL )					// If Texture Image Exists
//    {
//        glGenTextures(1, &texture);									// Create The Texture
//
//        // Typical Texture Generation Using Data From The Bitmap
//        glBindTexture(GL_TEXTURE_2D, texture);
//        glTexImage2D(GL_TEXTURE_2D, 0, 3, pImage->sizeX, pImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);
//        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//
//        free(pImage->data);											// Free The Texture Image Memory
//        free(pImage);												// Free The Image Structure
//    }
//
//    return texture;													// Return The Status
//}
//
