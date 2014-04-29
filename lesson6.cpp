#ifdef linux
#include <GL/glut.h>
#include <stdlib.h>
#endif

#include <stdio.h>
#include <math.h>

/* ASCII code for the escape key */
#define ESCAPE 27

int window;             // The number of our GLUT window

//Animation Variables
float xrot, yrot, zrot; // variables for x, y, and z rotation
int anim = 0;           // animation toggle variable
int ms_delay = 33;      // minimum delay between frames in milliseconds
                        // equivalent to ~30 frames per second
float rpm = 15;         // base rotation speed

GLuint texture[1];      // storage for textures (only one for now)

/* Image type - contains height, width, and data */
struct Image 
{
      unsigned long sizeX;
      unsigned long sizeY;
      GLubyte *data;
};



// getint and getshort are helper functions to load larger data types
// in Big Endian CPUs such as those in older Mac (PowerPC) or Solaris
// (SPARC) workstations. They are needed because BMP files are
// designed for Little Endian CPUs like the Intel x86 series.
//
// Originally from the Xv bmp loader.

// Ensure that Little Endian ints are read into memory correctly on
// Big Endian platforms
static unsigned int getint(FILE *fp)
{
   unsigned int c, c1, c2, c3;

   // get 4 bytes
   c  = ((unsigned int)getc(fp));  
   c1 = ((unsigned int)getc(fp)) << 8;  
   c2 = ((unsigned int)getc(fp)) << 16;  
   c3 = ((unsigned int)getc(fp)) << 24;  

   return c | c1 | c2 | c3;
}


// Ensure that Little Endian shorts are read into memory correctly on
// Big Endian platforms
static unsigned short getshort(FILE* fp)
{
   unsigned short c, c1;

   //get 2 bytes
   c  = ((unsigned short)getc(fp));  
   c1 = ((unsigned short)getc(fp)) << 8;  

   return c | c1;
}

// quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.  
// See http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt for more info.

bool ImageLoad(char *filename, Image *image) 
{
   FILE *file;
   unsigned long size;          // size of the image in bytes.
   size_t i,j,k, linediff;		     // standard counter.
   unsigned short int planes;   // number of planes in image (must be 1) 
   unsigned short int bpp;      // number of bits per pixel (must be 24)
   char temp;                   // temporary storage for bgr-rgb conversion.

   // make sure the file is there.
   if ((file = fopen(filename, "rb"))==NULL) 
   {
      printf("File Not Found : %s\n",filename);
      return false;
   }

   // seek through the bmp header, up to the width/height:
   fseek(file, 18, SEEK_CUR);

   // read the width
   image->sizeX = getint (file);
   printf("Width of %s: %lu\n", filename, image->sizeX);

   // read the height 
   image->sizeY = getint (file);
   printf("Height of %s: %lu\n", filename, image->sizeY);

   // calculate the size (assuming 24 bits or 3 bytes per pixel).
   // BMP lines are padded to the nearest double word boundary.
   // fortunat
   size = 4.0*ceil(image->sizeX*24.0/32.0) * image->sizeY ;

   // read the planes
   planes = getshort(file);
   if (planes != 1) 
   {
      printf("Planes from %s is not 1: %u\n", filename, planes);
      return false;
   }

   // read the bpp
   bpp = getshort(file);
   if (bpp != 24) 
   {
      printf("Bpp from %s is not 24: %u\n", filename, bpp);
      return 0;
   }

   // seek past the rest of the bitmap header.
   fseek(file, 24, SEEK_CUR);

   // allocate space for the data. 
   image->data = new GLubyte[size];
   if (image->data == NULL) 
   {
      printf("Error allocating memory for color-corrected image data");
      return false;
   }

   // read the data
   i = fread(image->data, size, 1, file);
   if (i != 1) 
   {
      printf("Error reading image data from %s.\n", filename);
      return false;
   }
	
	
   // reverse all of the colors. (bgr -> rgb)
   //calculate distance to 4 byte boundary for each line.
   //if this distance is not 0, then there will be a color reversal error
   //unless we correct for the distance on each line.
   linediff = 4.0*ceil(image->sizeX*24.0/32.0) - image->sizeX*3.0;
   k = 0;
   for (j=0;j<image->sizeY;j++)
   {
		
      for (i=0;i<image->sizeX;i++) 
      {
	 temp = image->data[k];
	 image->data[k] = image->data[k+2];
	 image->data[k+2] = temp;
	 k+=3;
      }
      k+= linediff;

   }
		

   // we're done.
   return true;
}


// Load Bitmaps And Convert To Textures
void LoadGLTextures() {	
   // Load Texture
   Image *image1;

   // allocate space for texture
   image1 = new Image();
   if (image1 == NULL)
   {
      printf("Error allocating space for image");
      exit(0);
   }

   //load picture from file
   if (!ImageLoad("rink.bmp", image1))
   {
      exit(1);
   }

   // Create Texture Name and Bind it as current
   glGenTextures(1, &texture[0]);
   glBindTexture(GL_TEXTURE_2D, texture[0]);   // 2d texture (x and y size)
	
   //Set Texture Parameters
   // scale linearly when image bigger than texture
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
   // scale linearly when image smaller than texture
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 

   glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
   // Load texture into OpenGL RC
   glTexImage2D(GL_TEXTURE_2D,     // 2D texture
		0,                 // level of detail 0 (normal)
		3,	                // 3 color components
		image1->sizeX,     // x size from image
		image1->sizeY,     // y size from image
		0,	                // border 0 (normal)
		GL_RGB,            // rgb color data order
		GL_UNSIGNED_BYTE,  // color component types
		image1->data       // image data itself
      );

};

// A general OpenGL initialization
// Sets initial parameters and assumes no defaults.
void InitGL(int Width, int Height)	 
{
   LoadGLTextures();	                // Load The Texture(s) 
   glEnable(GL_TEXTURE_2D);              // Enable Texture Mapping
   glClearColor(0.0f, 0.0f, 1.0f, 0.0f); // Clear The Background Color To Blue 
   glClearDepth(1.0);                    // Enables Clearing Of The Depth Buffer
   glDepthFunc(GL_LESS);                 // The Type Of Depth Test To Do
   glEnable(GL_DEPTH_TEST);	        // Enables Depth Testing
   glShadeModel(GL_SMOOTH);              // Enables Smooth Color Shading
  
   glMatrixMode(GL_MODELVIEW);    
   glLoadIdentity();                     // Reset view to origin.
   glTranslatef(0.0f,0.0f,-5.0f);        // Move modelspace 5 units away from view
}

/* The function called when our window is resized (which shouldn't
   happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
   GLfloat aspect;
   if (Height==0)		     // Avoid Divide By Zero If Window Too Small
      Height=1;
  
   // Reset Current Viewport and Perspective Transformation
   glViewport(0, 0, Width, Height);   
  
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();                     // Reset The Projection Matrix
   aspect = (GLfloat)Width/Height;       // Calculate Aspect Ratio of the Window
   gluPerspective(45.0f,aspect,0.1f,100.0f); // Set Projection matrix
   glMatrixMode(GL_MODELVIEW);
}


/* The main drawing function. */
void DrawGLScene(void)
{
   //Rotations     1 minute    1 second   360 degrees     degrees
   //---------  * ---------- * -------- * ------------ =  -------
   //1 minute     60 seconds   1000 ms      Rotation        ms
   float baseRot = rpm/60/1000*360*ms_delay;
  
   // Clear The Screen And The Depth Buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
   //Preserve view matrix before modelling transforms
   glPushMatrix();               
  
   glRotatef(xrot,1.0f,0.0f,0.0f);   // Rotate On The X Axis
   glRotatef(yrot,0.0f,1.0f,0.0f);   // Rotate On The Y Axis
   glRotatef(zrot,0.0f,0.0f,1.0f);   // Rotate On The Z Axis

   glBindTexture(GL_TEXTURE_2D, texture[0]);   // choose the texture to use.

   glBegin(GL_QUADS);   // begin drawing a cube
   //(note that the texture corners match quad corners)

   // Front Face 
   glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
   glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
   glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
   glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	

   // Back Face
   glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
   glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
   glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
   glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);

   // Top Face
   glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
   glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
   glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
   glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
     
   // Bottom Face
   glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
   glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
   glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
   glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
     
   // Right face
   glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
   glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
   glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
   glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
     
   // Left Face
   glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
   glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
   glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
   glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
     
   glEnd(); // done with the Cube

   xrot+=baseRot; // Increase X Axis Rotation	
   yrot+=baseRot; // Increase Y Axis Rotation
   zrot+=baseRot; // Increase Z Axis Rotation
   
   glPopMatrix();

   // we are double buffering, so swap the buffers to display what we drew..
   glutSwapBuffers();
}

// This function is called when a timer set by glutTimerFunc runs
// out. The timer delays the swapping of frames... without this you
// use glutIdleFunc and the frames are swapped as fast as possible and
// the rotation speed varies widely for different machines.

void animate(int value) 
{
   glutTimerFunc(ms_delay,animate,1);
   if (anim)
      DrawGLScene();
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y) 
{
   switch (key)
   {
      case 'a':
      case 'A':
	 // The letter "a" toggles animation
	 anim = !anim;
	 break;
      case ESCAPE:
	 // shut down our window 
	 glutDestroyWindow(window);
	 exit(0);
   }
}


// Initialize GLUT state - glut will take any command line arguments
// that pertain to it or X Windows - look at its documentation at
// http://reality.sgi.com/mjk/spec3/spec3.html */
int main(int argc, char **argv) 
{  
   glutInit(&argc, argv);  

   // Select type of Display mode:   
   //     Double buffer 
   //     RGBA color
   //     Alpha components supported 
   //     Depth buffer  

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH);  

   // get a 640 x 480 window
   glutInitWindowSize(640, 480);  

   // the window starts at the upper left corner of the screen
   glutInitWindowPosition(0, 0);  

   // Open a window
   window = glutCreateWindow("Awesome Title, YAY!");  

   // Register the function to do all our OpenGL drawing.
   glutDisplayFunc(&DrawGLScene);  

   // Go fullscreen.  This is the soonest possible.
   //glutFullScreen();

   // Even if there are no input events, redraw our gl scene.
   // glutTimerFunc(ms_delay, &animate,1 );

   // Register event callbacks
   glutReshapeFunc(&ReSizeGLScene);
   glutKeyboardFunc(&keyPressed);
   InitGL(800, 600);

   // Start Event Processing Engine
   glutMainLoop();  

   return true;
}

