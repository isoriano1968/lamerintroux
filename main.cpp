/*
 * Lamer Intro sgi 
 * 
 * Party invitation for the Classic Computing gathering taking place in September
 * from 8th till 11th 2022 in Lingen/Germany
 * 
 * No Blitter or Copper was harmed during this prod which is based on OpenGL and libmikmod.
 * All code done on an sgi Fuel with V10 gfx running IRIX 6.5.30 using the MIPSpro 7.4 compiler suite.
 * 
 * Hope to see your demos/intros/cracktros next year
 * 
 * Special thanks go to Julien Heyman (bidsomail)
 * 
 * Iggi 28th December 2021, Sundern Germany
 */
 

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <mikmod.h>

#include <GL/glut.h>
#include <GL/glu.h>	
//#include <GL/glx.h>

#define PI 3.1415927

#define MAXNUMBERRASTERS 50
#define TRANSITIONTIME 2.0
#define NB_STARS 250
#define NB_TRANSITIONS 8

#define STAR_XMIN -35
#define STAR_XMAX 35
#define STAR_YMIN -25
#define STAR_YMAX 15
#define STAR_ZMIN -50
#define STAR_ZMAX 0
#define STAR_V 15

#define TUNNEL_RADIUS       6
#define TUNNEL_MINZ         -10
#define TUNNEL_MAXZ         60
#define TUNNEL_CIRCUMPIECES 25
#define TUNNEL_DEPTH_PIECES 25

#define SINELOGO_SUBTILES_X   16
#define SINELOGO_SUBTILES_Y   16

#define BITMAP_SCROLLER_LETTER_SPACING 15 // Pixel
#define NBBITMAPSCROLLINGLETTERS 40 // Bitmap characters in font
#define NBTHREEDDIMENSIONALLETTERS 30
#define THREED_VERTICAL_SCROLLER_LETTER_SPACING 15
#define THREED_HORIZONTAL_SCROLLER_LETTER_SPACING 0.85
#define FONTFILE "newbaveuse.ttf"
#define FONTNAME "NewBaveuse"

#define PHRASE1 "*** LOOKS LIKE THE LAMER INTRO SGI IS STARTING TO GET SOME ... FLOW ...  *** REMEMBER *** ONLY AMIGA MAKES IT POSSIBLE"
#define PHRASE2 " HAHAHA ... LETS GET THIS STUFF DONE AND PORTABLE TO OTHER DEAD PLATFORMS ***"
#define PHRASE3 " AND BTW 2022 WE HAVE THE CLASSIC COMPUTING MEETING IN LINGEN/GERMANY FROM 8TH TILL 11TH OF SEPTEMBER "
#define PHRASE4 ""
#define PHRASE5 ""
#define PHRASE6 ""

// Enable and disable effects
bool showRasters = false;
bool showStarfield = false;
bool showTunnel = false;
bool showSineLogo = false;
bool showBackground = false;
bool bitmap_scroller = false;
bool transition = false;
bool rasterEnableBlend = true;
bool verticalRaster = false;
bool threeDimensionalHorizontalScroller = false;
bool threeDimensionalVerticalScroller = false;
bool scroller_twistX = false;
bool scroller_twistY = false;
bool scroller_isFat = false;


int demoEffectRuntime[8] = {0,5, 50, 60, 70, 80, 90, 999999};
bool demoEffectSequence[8];
char scrollerText[2048];
int demoPhase = 0;

int InitWidth = 800;
int InitHeight= 600;
int InitPos_X = 200;
int InitPos_Y = 200;
int RFPS = 40;
int frame=0, time, timebase=0;

unsigned int listBaseTextInfo;
unsigned int listBaseTextBigFat;
unsigned int listBaseTextBigThin;

GLfloat lightDiffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat lightPos[] = {0.0f, 0.0f, 3.0f, 1.0f};
GLfloat lightAmbient[] = {0.01f, 0.01f, 0.01f, 1.0f};
GLfloat lightSpecular[] = {0.5f, 0.5f, 0.5f, 1.0f};

//	Ascii codes    Sp  !   "   #   $   % & ' ( ) *   + ,   - . / 0   1   2   3   4   5   6   7   8   9   : ; < = > ?   @ A  B  C  D  E   F   G   H   I   J   K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z  
int startList[] = {622,651,661,676,702,0,0,0,0,0,952,0,968,0,0,0,723,747,759,783,805,830,854,878,903,928,0,0,0,0,0,985,0,0, 26,50,72,96, 117,138,162,186,195,213,239,259,289,315,339,363,389,413,436,459,482,507,546,572,598};
int widthList[] = { 28,  7, 12, 23, 19,0,0,0,0,0, 14,0,  7,0,0,0, 22,  9, 22, 19, 22, 22, 22, 22, 22, 22,0,0,0,0,0,20, 0,25,23,21,23,20,  20, 23, 23,  8, 17, 25, 19, 29, 25, 23, 23, 25, 23, 22, 22, 22, 24, 38, 25, 25, 22};

float demotimer = 0;

bool fullscreen = false;

float flashFadeFactor=0.0f;
float virtualFlashFadeFactor=0.0f;
float finalFadeFactor=0.0;
int flash_direction=1;

float start;

float _angle = 30.0f;
int _textureId;
int _rasterTextureID;
int _starTextureId;
int backGroundTexture;

float Zdistance = -6.0f;

float scroller_angleX = -30.0f;
float scroller_angleY = -42.0f;
float scroller_sineAmplitudeX = 0.0f;
float scroller_sineAmplitudeY = 0.0f;
float scroller_sineAmplitudeZ = 0.0f;
float scroller_movex = 0.0;
float scroller_movey = 3.0f;
float scroller_stop = -5.0f;
float scroller_speed;

float background_texture_shiftX = 0.0f;
float background_texture_shiftY = 0.0f;
float background_angle = 0.0f;

float backGroundTileFactor = 0.0;
float backGroundColorFactor = 0.0;

int _nbRasters = MAXNUMBERRASTERS / 3;

int window_width = InitWidth;
int window_height = InitHeight;

//XFontStruct theFontHandles[20];
int fontCounter = 0;

enum {	TEXTURE_RASTER_PLAIN,
		TEXTURE_RASTER_LINEAR,
		TEXTURE_BACKGROUND,
		TEXTURE_BITMAPFONT,
		TEXTURE_SINELOGO,
		TEXTURE_TROMBINE,
		TEXTURE_STAR,
		NB_TEXTURES};
		
typedef struct
{
	GLfloat rotate;
	GLfloat thickness;
	GLfloat width;
	GLfloat x;
	GLfloat y;
	GLfloat color_r;
	GLfloat color_g;
	GLfloat	color_b;
	GLfloat	color_a;
} Raster;

Raster rasters[MAXNUMBERRASTERS];

typedef struct
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat v;	
} Star;

Star stars[NB_STARS];

typedef struct
{
	GLubyte	*imageData;
	GLuint	bpp;
	GLuint	width;
	GLuint	height;
	GLuint	ID;
} TextureImage;

TextureImage textures[NB_TEXTURES];
TextureImage texture;

typedef struct
{
	GLfloat	x;
	GLfloat	y;
} SineLogoTile;

SineLogoTile sineLogoTiles[SINELOGO_SUBTILES_X+1][SINELOGO_SUBTILES_Y+1];

typedef struct 
{
	float x;
	float y;
	int letter_index;
} ScrollingLetter;

ScrollingLetter bitmapScrollingLetters[NBBITMAPSCROLLINGLETTERS];
ScrollingLetter threeDimensionalScrollingLetters[NBTHREEDDIMENSIONALLETTERS];

bool UpdateTexture(const TextureImage *texture)
{
	GLuint type = GL_RGBA;
	
	glBindTexture(GL_TEXTURE_2D, texture->ID);		
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	
	if (texture->bpp==24)									
	{
		type=GL_RGB;										
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture->width, texture->height, 0, type, GL_UNSIGNED_BYTE, texture->imageData);

	return true;
}

bool LoadTGATexture(TextureImage *texture, const char *filename)			
{    
	GLubyte	TGAheader[18];
	GLuint	bytesPerPixel;								
	GLuint	imageSize;									
	GLuint	temp;										
	GLuint	type=GL_RGBA;								

/* TGA FORMAT
typedef struct
{
    0 : byte  identsize;          // size in bytes of ID field that follows 18 byte header (0 usually)
    1 : byte  colourmaptype;      // type of colour map 0=none, 1=has palette
    2 : byte  imagetype;          // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

    3-4 : short colourmapstart;     // first colour map entry in palette
    5-6 : short colourmaplength;    // number of colours in palette
    7 : byte  colourmapbits;      // number of bits per palette entry 15,16,24,32

    8-9 : short xstart;             // image x origin
    10-11 : short ystart;             // image y origin
    12-13 : short width;              // image width in pixels
    14-15 : short height;             // image height in pixels
    16 byte : bits;               // image bits per pixel 8,16,24,32
    17 byte : descriptor;         // image descriptor bits (vh flip bits)
    
    // pixel data follows header
    
} TGA_HEADER
*/

	if (texture == NULL)
		return false;
	
	FILE *file = fopen(filename, "rb");						
	if(file==NULL)											
	{
		return false;									
	}

	if(	fread(TGAheader,1,sizeof(TGAheader),file)!=sizeof(TGAheader) )			
	{
		fclose(file);										
		return false;										
	}

	//skip potential additional ID field
	if (TGAheader[0] != 0)
		fread(TGAheader,TGAheader[0],1,file);

	//only RGB(A) supported for now.
	if (TGAheader[2] != 2)
	{
		fclose(file);										
		return false;										
	}

	texture->width  = TGAheader[13] * 256 + TGAheader[12];			
	texture->height = TGAheader[15] * 256 + TGAheader[14];			
    
 	if(	texture->width	<=0	||								
		texture->height	<=0	||							
		(TGAheader[16]!=24 && TGAheader[16]!=32))					
	{
		fclose(file);										
		return false;									
	}

	texture->bpp	= TGAheader[16];						
	bytesPerPixel	= texture->bpp/8;						
	imageSize		= texture->width*texture->height*bytesPerPixel;	

	texture->imageData=(GLubyte *)malloc(imageSize);	

	if(	texture->imageData==NULL ||							
		fread(texture->imageData, 1, imageSize, file)!=imageSize)	
	{
		if(texture->imageData!=NULL)					
			free(texture->imageData);					

		fclose(file);									
		return false;									
	}

	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)		
	{														
		temp=texture->imageData[i];							
		texture->imageData[i] = texture->imageData[i + 2];	
		texture->imageData[i + 2] = temp;					
	}

	fclose (file);											

	// Get a new Tex id and build a texture from the data
	glGenTextures(1, &(texture->ID));					
	UpdateTexture(texture);

	if(texture->imageData!=NULL)					
		free(texture->imageData);

	return true;											
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) 
	{
		case 27: //Escape key
			for (int n = 0; n < NB_TEXTURES; n++)
			{
				if (textures[n].ID != 0)
					glDeleteTextures(1, &textures[n].ID);
			}
			
			if (texture.imageData != NULL)
					delete [] texture.imageData;
			
			exit(0);
	}
}

void specialKeyPressed(int key, int x, int y) 
{
  switch (key) {    
	case GLUT_KEY_F1:
	fullscreen = !fullscreen;
	if (fullscreen) {
	    glutSetCursor(GLUT_CURSOR_NONE);
	    InitPos_X = glutGet((GLenum)GLUT_WINDOW_X);	/* Save parameters */
	    InitPos_Y = glutGet((GLenum)GLUT_WINDOW_Y);
	    InitWidth = glutGet((GLenum)GLUT_WINDOW_WIDTH);
	    InitHeight = glutGet((GLenum)GLUT_WINDOW_HEIGHT);
	    glutFullScreen();				/* Go to full screen */
	} else {
	    glutReshapeWindow(InitWidth, InitHeight);		/* Restore us */
	    glutPositionWindow(InitPos_X, InitPos_Y);
	}
	   break;
	default:
	   break;
	}
}

/*
unsigned int CreateBitmapFontInfo(char *fontName, int fontSize)
{
	XFontStruct hFont;
	
	unsigned int base;

	base = glGenLists(96);      // create storage for 96 characters

	** Still to be done with some UNIX ttf handler .. don't know how **
	
	return base;
}

unsigned int CreateBitmapFontBig(char *fontName, int fontSize, bool fat)
{
	XFontStruct hFont;         
	unsigned int base;

	base = glGenLists(96);      // create storage for 96 characters

	** Still to be done with some UNIX ttf handler .. don't know how **
	
	return base;
}

void ClearFont(unsigned int base)
{
	if (base != 0)
		glDeleteLists(base, 96);

	** No clue .. needs research **
}
*/

void OutputTextInfo (char* text, unsigned int x, unsigned int y, float color_r, float color_g, float color_b)
{
	glColor3f(color_r, color_g, color_b);
	glRasterPos2d(x, y);

	if ((listBaseTextInfo == 0) || (text == (char*)NULL))
		return;

	glPushAttrib(GL_LIST_BIT);
		glListBase(listBaseTextInfo - 32);
		glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}

void OutputTextBig (char* text, float color_r, float color_g, float color_b, bool fat)
{
	glColor3f(color_r, color_g, color_b);

	if ((listBaseTextBigFat == 0) || (listBaseTextBigThin == 0) || (text == (char*)NULL))
		return;

	glPushAttrib(GL_LIST_BIT);
		if (fat)
			glListBase(listBaseTextBigFat - 32);
		else
			glListBase(listBaseTextBigThin - 32);
		glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}

void initRendering() 
{
	int i;
	int total_width = 0;
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glFrontFace(GL_CCW);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		
	glDisable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	
		
	for (i=0; i < NB_STARS; i++)
	{
			stars[i].x = STAR_XMIN + (float)rand()*(STAR_XMAX-STAR_XMIN)/RAND_MAX;
			stars[i].y = STAR_YMIN + (float)rand()*(STAR_YMAX-STAR_YMIN)/RAND_MAX;
			stars[i].z = STAR_ZMIN + (float)rand()*(STAR_ZMAX-STAR_ZMIN)/RAND_MAX;
	}
	
	
	for (i = 0; i < NB_TRANSITIONS; i++)
	{
		demoEffectSequence[i] = false;
	}
	
	if (!LoadTGATexture(&textures[TEXTURE_STAR],"star.tga") || !LoadTGATexture(&textures[TEXTURE_RASTER_PLAIN],"raster_plain.tga") ||
		!LoadTGATexture(&textures[TEXTURE_SINELOGO],"sinelogo.tga") || !LoadTGATexture(&textures[TEXTURE_BACKGROUND],"background.tga") ||
		!LoadTGATexture(&textures[TEXTURE_BITMAPFONT],"fonte.tga")|| !LoadTGATexture(&textures[TEXTURE_RASTER_LINEAR],"raster.tga")||
		!LoadTGATexture(&textures[TEXTURE_TROMBINE],"trombine.tga"))
	{
			printf("Error loading assets!\n");
	} 
		else
	{
			
			
	}
	
	/*strcpy(scrollerText, PHRASE1);
	strcat(scrollerText, PHRASE2);
	strcat(scrollerText, PHRASE3);
	strcat(scrollerText, PHRASE4);*/
	
	
}

void EnterOrthoMode()
{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, InitWidth, 0, InitHeight, -100.0, 100.0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity(); 
}

void ExitOrthoMode()
{
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

}

void rotate(float x_in, float y_in, float &x_out, float &y_out, float angle)
{
	x_out = x_in*cos(angle) + y_in*sin(angle);
	y_out = x_in*sin(angle) - y_in*cos(angle);
}

void drawBackground()
{
	glBindTexture(GL_TEXTURE_2D,textures[backGroundTexture].ID);
	
	if (!rasterEnableBlend) 
		glDisable(GL_BLEND);
	else
		glEnable(GL_BLEND);
	
	glColor4f(0.0, 1.0, 0.0, 0.6);
	glPushMatrix();
	
	float x1,y1,x2,y2,x3,y3,x4,y4;
	rotate(-backGroundTileFactor/2, -backGroundTileFactor/2, x1, y1, background_angle);
	rotate(backGroundTileFactor/2, -backGroundTileFactor/2, x2, y2, background_angle);
	rotate(backGroundTileFactor/2, backGroundTileFactor/2, x3, y3, background_angle);
	rotate(-backGroundTileFactor/2, backGroundTileFactor/2, x4, y4, background_angle);
	
	glBegin(GL_QUADS);
	glTexCoord2f(background_texture_shiftX + x1,background_texture_shiftY + y1);	
	glVertex2d(0, 0);
	glTexCoord2f(background_texture_shiftX + x2,background_texture_shiftY + y2);
	glVertex2d(InitWidth, 0);
	glTexCoord2f(background_texture_shiftX + x3,background_texture_shiftY + y3);
	glVertex2d(InitWidth, InitHeight);
	glTexCoord2f(background_texture_shiftX + x4,background_texture_shiftY + y4);
	glVertex2d(0,InitHeight);
	glEnd();
	
	glPopMatrix();
	
}

void drawSinelogo()
{
	int i, j;
	
	glBindTexture(GL_TEXTURE_2D,textures[TEXTURE_SINELOGO].ID);
	
	glColor4f(1.0, 1.0, 1.0, 1.0);
	
	for (i=0; i < SINELOGO_SUBTILES_X ; i++)
	{
		for (j=0; j < SINELOGO_SUBTILES_Y ; j++)
		{
			glBegin(GL_QUADS);
			glTexCoord2f((float)(i)/SINELOGO_SUBTILES_X,-(float)(j)/SINELOGO_SUBTILES_Y);	
			glVertex2d(sineLogoTiles[i][j].x, sineLogoTiles[i][j].y);
			glTexCoord2f((float)(i+1)/SINELOGO_SUBTILES_X, -(float)(j)/SINELOGO_SUBTILES_Y);
			glVertex2d(sineLogoTiles[i+1][j].x, sineLogoTiles[i+1][j].y);
			glTexCoord2f((float)(i+1)/SINELOGO_SUBTILES_X, -(float)(j+1)/SINELOGO_SUBTILES_Y);
			glVertex2d(sineLogoTiles[i+1][j+1].x, sineLogoTiles[i+1][j+1].y);
			glTexCoord2f((float)(i)/SINELOGO_SUBTILES_X, -(float)(j+1)/SINELOGO_SUBTILES_Y);
			glVertex2d(sineLogoTiles[i][j+1].x, sineLogoTiles[i][j+1].y);
			glEnd();
		}
	}
}

void drawScene() {
	
	int i, j;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	// Starfield doey not work at all that shit!
	if (showStarfield)
	{
		glTranslatef(0.0, 0.0, Zdistance);
		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		
		glBindTexture(GL_TEXTURE_2D,textures[TEXTURE_STAR].ID);
		
		for (i=0; i < NB_STARS ; i++)
		{
			float fade = 0.75*(stars[i].z - STAR_ZMIN)/(STAR_ZMAX - STAR_ZMIN);
			glColor3f(fade, fade, fade);
			
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f,0.0f);	
				glVertex3f(stars[i].x, stars[i].y, stars[i].z);
				glTexCoord2f(1.0f,0.0f);
				glVertex3f(stars[i].x+1, stars[i].y, stars[i].z);
				glTexCoord2f(1.0f,1.0f);
				glVertex3f(stars[i].x+1, stars[i].y+1, stars[i].z);
				glTexCoord2f(0.0f,1.0f);
				glVertex3f(stars[i].x, stars[i].y+1, stars[i].z);
			glEnd();
			
		} 
		
		glPopAttrib();
		glPopMatrix();
		
		
	}
	
	EnterOrthoMode();
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	
	if (showBackground)
	{
		drawBackground();
	}
	
	if (showSineLogo)
	{
		drawSinelogo();
	}
	
	if (showRasters)
	{
		glBindTexture(GL_TEXTURE_2D,textures[_rasterTextureID].ID);
		
		if (!rasterEnableBlend) 
			glDisable(GL_BLEND);
		else
			glEnable(GL_BLEND);

		for (i=0; i < _nbRasters ; i++)
		{
			glColor4f(rasters[i].color_r, rasters[i].color_g, rasters[i].color_b, rasters[i].color_a);
			glPushMatrix();
			glTranslatef(0.0, rasters[i].y, 0.0);
			glRotatef(rasters[i].rotate,0.0,0.0,1.0);
			
			if (!verticalRaster)
			{
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f,0.0f);	
				glVertex2d((int)(rasters[i].x-0.5*rasters[i].width), (int)(-0.5*rasters[i].thickness));
				glTexCoord2f(16.0f,0.0f);
				glVertex2d((int)(rasters[i].x+0.5*rasters[i].width), (int)(-0.5*rasters[i].thickness));
				glTexCoord2f(16.0f,1.0f);
				glVertex2d((int)(rasters[i].x+0.5*rasters[i].width), (int)(0.5*rasters[i].thickness));
				glTexCoord2f(0.0f,1.0f);
				glVertex2d((int)(rasters[i].x-0.5*rasters[i].width), (int)(0.5*rasters[i].thickness));
				glEnd();
			}
			else
			{
				glBegin(GL_QUADS);
				glTexCoord2f(16.0f,0.0f);	
				glVertex2d((int)(rasters[i].x-0.5*rasters[i].width), (int)(-0.5*rasters[i].thickness));
				glTexCoord2f(16.0f,1.0f);
				glVertex2d((int)(rasters[i].x+0.5*rasters[i].width), (int)(-0.5*rasters[i].thickness));
				glTexCoord2f(0.0f,1.0f);
				glVertex2d((int)(rasters[i].x+0.5*rasters[i].width), (int)(0.5*rasters[i].thickness));
				glTexCoord2f(0.0f,0.0f);
				glVertex2d((int)(rasters[i].x-0.5*rasters[i].width), (int)(0.5*rasters[i].thickness));
				glEnd();			
			}
			glPopMatrix();
		}
	}
	
	if (bitmap_scroller)
	{
		glBindTexture(GL_TEXTURE_2D,textures[TEXTURE_BITMAPFONT].ID);

		for (i=0; i < NBBITMAPSCROLLINGLETTERS ; i++)
		{
			glColor4f(1.0, 1.0, 1.0, 1.0);
			
			float start_texture = (float)startList[scrollerText[bitmapScrollingLetters[i].letter_index] - 32]/1024;
			int width = widthList[scrollerText[bitmapScrollingLetters[i].letter_index] - 32];
			float stop_texture = (float)(startList[scrollerText[bitmapScrollingLetters[i].letter_index] - 32] + width)/1024;

			glPushMatrix();
			glTranslated((int)bitmapScrollingLetters[i].x,bitmapScrollingLetters[i].y,0);
			glBegin(GL_QUADS);
				glTexCoord2f(1.001*start_texture,0.99f);	
				glVertex2d(0, 0);
				glTexCoord2f(1.001*start_texture,0.01f);
				glVertex2d(0, 40);
				glTexCoord2f(0.999*stop_texture,0.01f);
				glVertex2d(width, 40);
				glTexCoord2f(0.999*stop_texture,0.99f);
				glVertex2d(width, 0);
			glEnd();
			glPopMatrix();
		}
	}
	glPopAttrib();
	//ExitOrthoMode();
	
	// The tunnel is a piece of shit! And it it even isn't a real tunnel just a faked half pipe to have the text scrolling within
	
	if(showTunnel)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		glEnable(GL_LIGHTING);	
		glShadeModel(GL_FLAT);					
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);	
				
		glTranslatef(0.0, 0.0, Zdistance);
		glRotatef(scroller_angleX, 1.0, 0.0, 0.0);
		glRotatef(scroller_angleY+40, 0.0, 1.0, 0.0);

		
		float P1x, P1y, P1z;
		float P2x, P2y, P2z;		
		float P3x, P3y, P3z;
		float P4x, P4y, P4z;

		for (int z=0; z<TUNNEL_DEPTH_PIECES; z++)
		{
			for (int k=0.5*TUNNEL_CIRCUMPIECES; k<TUNNEL_CIRCUMPIECES; k++)
			{
				float red = 0.2 + 0.8*sin(PI*(float)z/TUNNEL_DEPTH_PIECES)*cos(0.3*demotimer);
				float green = 0.3 + 0.7*sin(PI*(float)k/TUNNEL_CIRCUMPIECES)*sin(0.1*demotimer);
				float blue = 0.1 + 0.6*sin(0.5*demotimer)*sin(0.5*demotimer);
				glColor3f(red,green,blue);

				P1x = TUNNEL_RADIUS*(2.5+0.5*cos(2*demotimer+0.5*z))*cos((float)2*PI*k/TUNNEL_CIRCUMPIECES)+3*cos(demotimer)+4;
				P1y = TUNNEL_RADIUS*(2.5+0.5*cos(2*demotimer+0.5*z))*sin((float)2*PI*k/TUNNEL_CIRCUMPIECES)+3*sin(demotimer)+6;
				P1z = TUNNEL_MINZ + (float)z*(TUNNEL_MAXZ-TUNNEL_MINZ)/TUNNEL_DEPTH_PIECES;
				
				P2x = TUNNEL_RADIUS*(2.5+0.5*cos(2*demotimer+0.5*(z+1)))*cos((float)2*PI*k/TUNNEL_CIRCUMPIECES)+3*cos(demotimer)+4;
				P2y = TUNNEL_RADIUS*(2.5+0.5*cos(2*demotimer+0.5*(z+1)))*sin((float)2*PI*k/TUNNEL_CIRCUMPIECES)+3*sin(demotimer)+6;
				P2z = TUNNEL_MINZ + (float)(z+1)*(TUNNEL_MAXZ-TUNNEL_MINZ)/TUNNEL_DEPTH_PIECES;
				
				P3x = TUNNEL_RADIUS*(2.5+0.5*cos(2*demotimer+0.5*(z+1)))*cos((float)2*PI*(k+1)/TUNNEL_CIRCUMPIECES)+3*cos(demotimer)+4;
				P3y = TUNNEL_RADIUS*(2.5+0.5*cos(2*demotimer+0.5*(z+1)))*sin((float)2*PI*(k+1)/TUNNEL_CIRCUMPIECES)+3*sin(demotimer)+6;
				P3z = TUNNEL_MINZ + (float)(z+1)*(TUNNEL_MAXZ-TUNNEL_MINZ)/TUNNEL_DEPTH_PIECES;
				
				P4x = TUNNEL_RADIUS*(2.5+0.5*cos(2*demotimer+0.5*z))*cos((float)2*PI*(k+1)/TUNNEL_CIRCUMPIECES)+3*cos(demotimer)+4;
				P4y = TUNNEL_RADIUS*(2.5+0.5*cos(2*demotimer+0.5*z))*sin((float)2*PI*(k+1)/TUNNEL_CIRCUMPIECES)+3*sin(demotimer)+6;
				P4z = TUNNEL_MINZ + (float)z*(TUNNEL_MAXZ-TUNNEL_MINZ)/TUNNEL_DEPTH_PIECES;
				
				glBegin(GL_QUADS);
				//glTexCoord2f(0.0f,0.0f);	
				glVertex3f(P1x, P1y, P1z);
				//glTexCoord2f(1.0f,0.0f);
				glVertex3f(P2x, P2y, P2z);
				//glTexCoord2f(1.0f,1.0f);
				glVertex3f(P3x, P3y, P3z);
				//glTexCoord2f(0.0f,1.0f);
				glVertex3f(P4x, P4y, P4z);
				glEnd();			
				
			}
		}
		
		glPopAttrib();
	
	}
	
	if (threeDimensionalHorizontalScroller || threeDimensionalVerticalScroller)
	{
//		glEnable(GL_TEXTURE_2D);				
//		glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_BITMAPFONT].ID);
		
		glEnable(GL_LIGHTING);	
		
		glLoadIdentity();

		if (threeDimensionalHorizontalScroller)
			glTranslatef(0.0,scroller_movey,Zdistance);
		else if (threeDimensionalVerticalScroller)
			glTranslatef(scroller_movex,0.0,Zdistance);

		glRotatef(scroller_angleX,1.0,0.0,0.0);
		glRotatef(scroller_angleY,0.0,1.0,0.0);
		
		for (unsigned int k=0; k < NBTHREEDDIMENSIONALLETTERS; k++)
		{
			char temp[3];
			glPushMatrix();
			
			if (threeDimensionalHorizontalScroller)
			{
				if (scroller_twistX) glRotatef(180*demotimer+10*threeDimensionalScrollingLetters[k].x,1.0,0.0,0.0);
				if (scroller_twistY) glRotatef(demotimer+0.35*threeDimensionalScrollingLetters[k].x,0.0,1.0,0.0);
				glTranslatef(threeDimensionalScrollingLetters[k].x,-0.35 + scroller_sineAmplitudeY*sin(4*demotimer+0.35*threeDimensionalScrollingLetters[k].x),scroller_sineAmplitudeZ*sin(demotimer+0.35*threeDimensionalScrollingLetters[k].x));
			}
			else if (threeDimensionalVerticalScroller)
			{
				if (scroller_twistX) glRotatef(180*demotimer+10*threeDimensionalScrollingLetters[k].y,1.0,0.0,0.0);
				if (scroller_twistY) glRotatef(demotimer+0.35*threeDimensionalScrollingLetters[k].y,0.0,1.0,0.0);
				glTranslatef(scroller_sineAmplitudeX*sin(2*demotimer+0.2*threeDimensionalScrollingLetters[k].y), threeDimensionalScrollingLetters[k].y,scroller_sineAmplitudeZ*sin(demotimer+0.35*threeDimensionalScrollingLetters[k].y));
				glRotatef(45*sin(2*demotimer+0.2*threeDimensionalScrollingLetters[k].y),0.0,1.0,0.0);
			}

			sprintf(temp, "%c",scrollerText[threeDimensionalScrollingLetters[k].letter_index]);
			if (threeDimensionalHorizontalScroller) OutputTextBig(temp, 1.0,1.0,1.0, scroller_isFat);
			else if (threeDimensionalVerticalScroller) OutputTextBig(temp, 1.0,0.0,0.0, scroller_isFat);
			glPopMatrix();
		}
		
		glDisable(GL_LIGHTING);	
	}
	
	if (transition)
	{
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);				
			glDisable(GL_DEPTH_TEST);				
			EnterOrthoMode();
						
			glShadeModel(GL_FLAT);					
			glDisable(GL_CULL_FACE);	
			glDisable(GL_LIGHT0);
			glDisable(GL_COLOR_MATERIAL);
			
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			glDisable(GL_DEPTH_TEST);			
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			glColor4f(1.0f, 1.0f, 1.0f, flashFadeFactor); 	
			
			glBegin(GL_QUADS);
			glVertex2d(0, 0);
			glVertex2d(InitWidth-1, 0);
			glVertex2d(InitWidth-1, InitHeight-1);
			glVertex2d(0,InitHeight-1);
			glEnd();
			
			glPopAttrib();
			ExitOrthoMode();
			
	}
	
	glutSwapBuffers();

}

int getTimer()
{
	return glutGet(GLUT_ELAPSED_TIME);
}

void update()
{
	char temp[20];
	int fps = 100;
	int i, j;
	
	// Compute frames per second

	frame++;
	
	time = getTimer();

	if (time - timebase > 1000)
	{
		fps = frame * 1000.0/(time - timebase);
		timebase = time;
		frame = 0;
		sprintf(temp, "FPS: %d", fps);
		glutSetWindowTitle(temp);
	}

	demotimer = (float)getTimer()/1000;
		
	// Manages the demo effects during each phase
	if (demotimer > demoEffectRuntime[6] && demoEffectSequence[6] == false)
		 {
			threeDimensionalHorizontalScroller = true;
			backGroundTexture = TEXTURE_TROMBINE;
			backGroundColorFactor = 0.5;
			showRasters = false;
			showStarfield = false;
			showTunnel = true;
			showBackground = false;
			showSineLogo = false;
			bitmap_scroller = false;
			scroller_movey = 0.0f;
			scroller_movex = 3.0f;
			scroller_stop = -5.0f;
			scroller_sineAmplitudeX = 1.0f;
			scroller_sineAmplitudeY = 0.0f;
			scroller_sineAmplitudeZ = 0.0f;
			scroller_twistX= false;
			scroller_twistY = false;
			scroller_angleX = 30.0f;
			scroller_angleY = 42.0f;
			scroller_speed = 6.0f;
			rasterEnableBlend = true;
			demoEffectSequence[6] = true;
		 }
	else if (demotimer > (demoEffectRuntime[6] - TRANSITIONTIME/2) && demoEffectSequence[6] == false)
		 {
			 transition = true;
		 }
	else if (demotimer > demoEffectRuntime[5] && demoEffectSequence[5] == false)
		 {
			backGroundTexture = TEXTURE_TROMBINE;
			backGroundColorFactor = 0.5;
			showRasters = false;
			showStarfield = false;
			showTunnel = false;
			showBackground = true;
			showSineLogo = true;
			threeDimensionalHorizontalScroller = false;
			threeDimensionalVerticalScroller = false;
			
			scroller_isFat = true;
			scroller_sineAmplitudeX = 1.0f;
			scroller_sineAmplitudeY = 0.0f;
			scroller_sineAmplitudeZ = 0.0f;
			scroller_twistX = false;
			scroller_twistY = false;
			scroller_angleX = 0.0f;
			scroller_angleY = 0.0f;
			scroller_speed = 5.0f;
			verticalRaster = false;

			demoPhase = 7;
			
			for (i=0; i < NBTHREEDDIMENSIONALLETTERS; i++)
			{
					threeDimensionalScrollingLetters[i].x = 0.0f;
					threeDimensionalScrollingLetters[i].y = -5 - THREED_VERTICAL_SCROLLER_LETTER_SPACING*i;
					threeDimensionalScrollingLetters[i].letter_index = i;
			}
			strcpy (scrollerText, PHRASE2);
			
			demoEffectSequence[5] = true;
		 }
	else if (demotimer > (demoEffectRuntime[5] - TRANSITIONTIME/2) && demoEffectSequence[5] == false)
		 {
			 transition = true;
		 }
	else if (demotimer > demoEffectRuntime[4] && demoEffectSequence[4] == false)
		 {
			backGroundTexture = TEXTURE_TROMBINE;
			backGroundColorFactor = 0.5;
			rasterEnableBlend = true;
			showRasters = true;
			showStarfield = false;
			showTunnel = false;
			showBackground = true;
			showSineLogo = true;
			_rasterTextureID = TEXTURE_RASTER_LINEAR;
			_nbRasters = MAXNUMBERRASTERS;
			bitmap_scroller = false;
			demoEffectSequence[4] = true;
			demoPhase = 6;
		 }
	else if (demotimer > (demoEffectRuntime[4] - TRANSITIONTIME/2) && demoEffectSequence[4] == false)
		 {
			 transition = true;
		 }
	else if (demotimer > demoEffectRuntime[3] && demoEffectSequence[3] == false) 
		{
			rasterEnableBlend = true;
			showRasters = true;
			verticalRaster = true;
			_rasterTextureID = TEXTURE_RASTER_LINEAR;
			_nbRasters = MAXNUMBERRASTERS;
			showStarfield = false;
			showTunnel = false;
			showBackground = true;
			showSineLogo = false;
			bitmap_scroller = false;
			demoEffectSequence[3] = true;
			demoPhase = 5;
		 }
	else if (demotimer > (demoEffectRuntime[3] - TRANSITIONTIME/2) && demoEffectSequence[3] == false)
		 {
			 transition = true;
		 }
	
	else if (demotimer > demoEffectRuntime[2] && demoEffectSequence[2] == false)
		 {
			rasterEnableBlend = true;
			showRasters = true;
			showStarfield = false;
			showTunnel = false;
			showBackground = false; 
			bitmap_scroller = false;
			showSineLogo = false;
			_rasterTextureID = TEXTURE_RASTER_LINEAR;
			_nbRasters = MAXNUMBERRASTERS / 2;
			demoEffectSequence[2] = true;
			demoPhase = 4;
		 }
	else if (demotimer > (demoEffectRuntime[2] - TRANSITIONTIME/2) && demoEffectSequence[2] == false)
		 {
			 transition = true;
		 }
	else if (demotimer > demoEffectRuntime[1] && demoEffectSequence[1] == false)
		 {
			strcpy(scrollerText, PHRASE1);
			strcat(scrollerText, PHRASE2);
			strcat(scrollerText, PHRASE3);
			strcat(scrollerText, PHRASE4);
	
			int total_width = 0;
			for (i=0; i < NBBITMAPSCROLLINGLETTERS; i++)
			{
				bitmapScrollingLetters[i].x = InitWidth+100+total_width;
				bitmapScrollingLetters[i].y = 0.0f;
				bitmapScrollingLetters[i].letter_index = i;
				total_width += widthList[scrollerText[i]-32] + BITMAP_SCROLLER_LETTER_SPACING;
			}
			_rasterTextureID = TEXTURE_RASTER_LINEAR;
			rasterEnableBlend = true;
			_nbRasters = MAXNUMBERRASTERS / 3;
			showRasters = true;
			showStarfield = false;
			showTunnel = false;
			showBackground = true; 
			bitmap_scroller = true;
			showSineLogo = false;
			demoEffectSequence[1] = true;
			demoPhase = 3;
		 }
	else if (demotimer > (demoEffectRuntime[1] - TRANSITIONTIME/2) && demoEffectSequence[1] == false)
		 {
			 transition = true;
		 }
	else if (demotimer > demoEffectRuntime[0] && demoEffectSequence[0] == false)
		 {
			for (i=0; i < NBTHREEDDIMENSIONALLETTERS; i++)
			{
					threeDimensionalScrollingLetters[i].x = 15 + THREED_HORIZONTAL_SCROLLER_LETTER_SPACING * i;
					threeDimensionalScrollingLetters[i].y = 0.0f;
					threeDimensionalScrollingLetters[i].letter_index = i;
			}
			
			rasterEnableBlend = true;
			threeDimensionalHorizontalScroller = false;
			threeDimensionalVerticalScroller = false;
			showRasters = true;
			showStarfield = true;
			showTunnel = false;
			showSineLogo = false;
			showBackground = false; 
			bitmap_scroller = false;
			_rasterTextureID = TEXTURE_RASTER_PLAIN;
			backGroundTexture = TEXTURE_BACKGROUND;
			backGroundTileFactor = 8.0;
			backGroundColorFactor = 0.1;
			strcpy(scrollerText, PHRASE1);
			demoEffectSequence[0] = true;
			demoPhase= 2;
		 }
		 	 
	if (transition)
	{
		// Ok, we will gradually move from alpha=0 to a max alpha, which can be greater
		// than one (meaning that after alpha reaches one we will stay white for a bit more
		// time). Need to increment alpha taking into account the round trip (going from 0 to
		// max alpha, THEN back to 0. Hence the x2 factor in the value in the formula.
		virtualFlashFadeFactor += flash_direction*2.4*0.02/TRANSITIONTIME;
		
		if (virtualFlashFadeFactor > 1.2) /* max alpha */
		{
			//max alpha (time...) reached, clamp to alpha=1 and reverse flash direction
			flashFadeFactor = 1.0;
			flash_direction = -1;
		}
		else if (virtualFlashFadeFactor > 1.0) 
		{
			// just clamp alpha to 1, we have not reached the peak (=max time...) yet
			flashFadeFactor = 1.0;
		}
		else if (virtualFlashFadeFactor < 0.0) 
		{
			// clamp to 0, reset direction and stop flashing mode.
			flashFadeFactor = 0.0;
			flash_direction = 1;
			transition = false;
		}
		else
			// just update new alpha factor for fadein/fadeout
			flashFadeFactor = virtualFlashFadeFactor;
	}
	
	switch(demoPhase)
	{
	case 0:
	case 1:
	case 2: // simple horizontal rasters; unique shade of blue color

		for (i=0; i < NB_STARS ; i++)
		{
			stars[i].x += sin(0.6*demotimer)*STAR_V*0.02;
			if (stars[i].x > STAR_XMAX) stars[i].x = STAR_XMIN;
			if (stars[i].x < STAR_XMIN) stars[i].x = STAR_XMAX;

			stars[i].z += 1.5*STAR_V*0.02;
			if (stars[i].z > STAR_ZMAX) stars[i].z = STAR_ZMIN;

			stars[i].y += cos(0.5*demotimer)*STAR_V*0.02;
			if (stars[i].y > STAR_YMAX) stars[i].y = STAR_YMIN;
			if (stars[i].y < STAR_YMIN) stars[i].y = STAR_YMAX;
		}

		for (i=0; i < _nbRasters ; i++)
		{
			rasters[i].thickness = 32;
			rasters[i].width = InitWidth;
			rasters[i].rotate = 0;
			rasters[i].y = (InitHeight-128)/2 + (InitHeight-128)/2 * sin(2*(demotimer+0.05*i));
			rasters[i].x = InitWidth/2;
			rasters[i].color_r = 0.0f;
			rasters[i].color_g = 0.0f;
			rasters[i].color_b = 0.5+0.5*(float)i/_nbRasters;
			rasters[i].color_a = 1.0f;
		}
		break;
	case 3: // more colorful !
		background_angle += 0.05*0.01;

		//move bitmap letters
		for (i=0; i < NBBITMAPSCROLLINGLETTERS; i++)
		{
			bitmapScrollingLetters[i].x -= 300*0.009;
			bitmapScrollingLetters[i].y = (InitHeight/6-30)+(InitHeight/6-30)*sin(5*demotimer+0.004*bitmapScrollingLetters[i].x);
			if (bitmapScrollingLetters[i].x < -100) 
			{
				//find the coordinates of the letter to the far right, and move the letter after that
				int max_x = 0, max_index = 0;
				for (int j=0; j < NBBITMAPSCROLLINGLETTERS; j++)
				{
					max_x = bitmapScrollingLetters[j].x > max_x ? bitmapScrollingLetters[j].x : max_x;
					max_index = bitmapScrollingLetters[j].x == max_x ? j : max_index;
				}
				
				int previous_index = (i > 0) ? i-1: NBBITMAPSCROLLINGLETTERS-1;
				bitmapScrollingLetters[i].x = bitmapScrollingLetters[previous_index].x + widthList[scrollerText[bitmapScrollingLetters[previous_index].letter_index]-32] + BITMAP_SCROLLER_LETTER_SPACING;
			
				// current letter has disappeared from the screen : pick the next letter to be scrolled
				bitmapScrollingLetters[i].letter_index += NBBITMAPSCROLLINGLETTERS;
				int delta = strlen(scrollerText) - bitmapScrollingLetters[i].letter_index;
				if (delta<=0) bitmapScrollingLetters[i].letter_index = -delta;
			}
		}
		
		// move rasters
		for (i=0; i < _nbRasters ; i++)
		{
			rasters[i].thickness = 32;
			rasters[i].width = InitWidth;
			rasters[i].rotate = 0;
			rasters[i].y = 2*InitHeight/3 + InitHeight/3 * sin(2*(demotimer+0.05*i));
			rasters[i].x = InitWidth/2;
			rasters[i].color_r = cos(3*demotimer)+(1-cos(3*demotimer))*(float)i/_nbRasters;
			rasters[i].color_g = 1-(float)i/_nbRasters;
			rasters[i].color_b = sin(2*demotimer)+(1-sin(2*demotimer))*(float)i/_nbRasters;
			rasters[i].color_a = 1.0f;
		}
		break;
	case 4: // let's twist a bit!
		background_angle += 0.05*0.1;
		
		//move rasters
		for (i=0; i < _nbRasters ; i++)
		{
			rasters[i].thickness = 32;
			rasters[i].width = InitWidth;
			rasters[i].rotate = 20*sin(demotimer+0.025*i);
			rasters[i].y = InitHeight/2 + InitHeight/2 * sin(2*(demotimer+0.05*i));
			rasters[i].x = InitWidth/2;
			rasters[i].color_r = cos(3*demotimer)+(1-cos(3*demotimer))*(float)i/_nbRasters;
			rasters[i].color_g = 1-(float)i/_nbRasters;
			rasters[i].color_b = sin(2*demotimer)+(1-sin(2*demotimer))*(float)i/_nbRasters;
			rasters[i].color_a = 1.0f;
		}
		scroller_movey = 1.5*fabs(sin(3*demotimer));

		break;
	case 5: // final raster mayhem
		background_angle += 0.05*0.02;
	
		//move rasters
		for (i=0; i < _nbRasters ; i++)
		{
			rasters[i].thickness = 32;
			rasters[i].width = InitWidth/2 + InitWidth/4 * sin(0.25*sin(0.15*demotimer)*(demotimer+0.5*i));
			rasters[i].rotate = 70*sin(demotimer+0.05*i);
			rasters[i].y = InitHeight/2 + InitHeight/4 * sin(2*(demotimer+0.05*i));
			rasters[i].x = InitWidth/2;
			rasters[i].color_r = cos(3*demotimer)+(1-cos(3*demotimer))*(float)i/_nbRasters;
			rasters[i].color_g = 1-(float)i/_nbRasters;
			rasters[i].color_b = sin(2*demotimer)+(1-sin(2*demotimer))*(float)i/_nbRasters;
			rasters[i].color_a = 1.0f;
		}
		break;
	case 6: // change texture !
		background_angle += 0.05*0.02;
		
		for (i=0; i < _nbRasters ; i++)
		{
			rasters[i].thickness = InitHeight;
			rasters[i].width = 32;
			rasters[i].rotate = 0;
			rasters[i].y = InitHeight*(1 - (float)i/_nbRasters);
			rasters[i].x = InitWidth/2+ 1.5*(InitWidth/4)*sin(2*demotimer+0.075*i)*sin(0.6*demotimer+0.03*i);
			rasters[i].color_r = cos(3*demotimer)+(1-cos(3*demotimer))*(float)i/_nbRasters;
			rasters[i].color_g = 1-(float)i/_nbRasters;
			rasters[i].color_b = sin(2*demotimer)+(1-sin(2*demotimer))*(float)i/_nbRasters;
			rasters[i].color_a = 1.0f;
		}
		break;

	case 7: 

		background_angle = 360*sin(0.001*demotimer);
		backGroundTileFactor = 0.5+ 1.5*fabs(sin(0.5*demotimer));

		background_texture_shiftX += 0.5*0.02;
		//if (background_texture_shiftX > backGroundTileFactor) background_texture_shiftX = 0.0f;

		background_texture_shiftY += 1.5*0.02;
		//if (background_texture_shiftY > backGroundTileFactor) background_texture_shiftY = 0.0f;

		for (i=0; i <= SINELOGO_SUBTILES_X ; i++)
		{
			for (j=0; j <= SINELOGO_SUBTILES_Y ; j++)
			{
				sineLogoTiles[i][j].x = (InitWidth/3-128)+i*256/SINELOGO_SUBTILES_X+50*sin(3*demotimer+4.0*j/SINELOGO_SUBTILES_Y);
				sineLogoTiles[i][j].y = (InitHeight/2-128)+j*256/SINELOGO_SUBTILES_Y+50*sin(3*demotimer+4.0*i/SINELOGO_SUBTILES_X);
			}
		}
		break;

	case 8:
	case 9:
		break;

	default:
		break;
	}
	
	
	
	if (threeDimensionalHorizontalScroller)
	{
		for (i=0; i < NBTHREEDDIMENSIONALLETTERS; i++)
		{
			threeDimensionalScrollingLetters[i].x -= scroller_speed*0.02f;
			if (threeDimensionalScrollingLetters[i].x < scroller_stop) 
			{
				//find the coordinates of the letter to the far right, and move the letter after that
				
				int max_x = 0, max_index = 0;
				for (int j=0; j < NBTHREEDDIMENSIONALLETTERS; j++)
				{
					max_x = threeDimensionalScrollingLetters[j].x > max_x ? threeDimensionalScrollingLetters[j].x : max_x;
					max_index = threeDimensionalScrollingLetters[j].x == max_x ? j : max_index;
				}
				
				int previous_index = (i > 0) ? i-1: NBTHREEDDIMENSIONALLETTERS-1;
				threeDimensionalScrollingLetters[i].x = threeDimensionalScrollingLetters[previous_index].x + THREED_HORIZONTAL_SCROLLER_LETTER_SPACING;
				
				// current letter has disappeared from the screen : pick the next letter to be scrolled
				threeDimensionalScrollingLetters[i].letter_index += NBTHREEDDIMENSIONALLETTERS;
				int delta = strlen(scrollerText) - threeDimensionalScrollingLetters[i].letter_index;
				if (delta<=0) threeDimensionalScrollingLetters[i].letter_index = -delta;
			}
		}
	}

	// Move 3D Letters
	if (threeDimensionalVerticalScroller)
	{
		for (i=0; i < NBTHREEDDIMENSIONALLETTERS; i++)
		{
			threeDimensionalScrollingLetters[i].y += scroller_speed*0.02f;
			if (threeDimensionalScrollingLetters[i].y > scroller_stop) 
			{
				//find the coordinates of the letter to the far right, and move the letter after that
				
				int max_y = 0, max_index = 0;
				for (int j=0; j < NBTHREEDDIMENSIONALLETTERS; j++)
				{
					max_y = threeDimensionalScrollingLetters[j].y > max_y ? threeDimensionalScrollingLetters[j].y : max_y;
					max_index = threeDimensionalScrollingLetters[j].y == max_y ? j : max_index;
				}
				
				int previous_index = (i > 0) ? i-1: NBTHREEDDIMENSIONALLETTERS-1;
				threeDimensionalScrollingLetters[i].y = threeDimensionalScrollingLetters[previous_index].y - THREED_VERTICAL_SCROLLER_LETTER_SPACING;
				
				// current letter has disappeared from the screen : pick the next letter to be scrolled
				threeDimensionalScrollingLetters[i].letter_index += NBTHREEDDIMENSIONALLETTERS;
				int delta = strlen(scrollerText) - threeDimensionalScrollingLetters[i].letter_index;
				if (delta<=0) threeDimensionalScrollingLetters[i].letter_index = -delta;
			}
		}
	}
	glutPostRedisplay();
	
	MikMod_Update();
}

int main(int argc, char** argv) {

	MODULE *module;

  	/* initialize MikMod threads */
  	//MikMod_InitThreads();

  	/* register all the drivers */
  	MikMod_RegisterAllDrivers();

  	/* register all the module loaders */
  	MikMod_RegisterAllLoaders();

  	/* init the library */
  	md_mode |= DMODE_SOFT_MUSIC ;
  	if (MikMod_Init(""))
  	{
    	 	fprintf(stderr, "Could not initialize sound, reason: %s\n", MikMod_strerror(MikMod_errno));
    		return 2;
  	}

  	/* load module */
  	module = Player_Load(argv[1], 64, 0);
  	if (module)
  	{
    		/* start module */
    		printf("Playing %s (%d chn)\n", module->songname, (int)module->numchn);
    		module->wrap = 1;
    		Player_Start(module);	 
      	}
  	 else
  	{
    		fprintf(stderr, "Could not load module, reason: %s\n", MikMod_strerror(MikMod_errno));
  	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(InitPos_X, InitPos_Y);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("LamerIntroSGI");
	if (argc > 2 && !strcmp(argv[2], "-f"))
		{
			glutFullScreen();
			glutSetCursor(GLUT_CURSOR_NONE);
		}
	initRendering();
	
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(specialKeyPressed);
	glutIdleFunc(update);
	
	glutMainLoop();

	Player_Stop();
    Player_Free(module);
	MikMod_Exit();
	
	return 0;
}









