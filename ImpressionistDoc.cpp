// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "ImpBrush.h"

// Include individual brush headers here.
#include "PointBrush.h"
#include "LineBrush.h"
#include "CircleBrush.h"
#include "ScatteredPointBrush.h"
#include "ScatteredLineBrush.h"
#include "ScatteredCircleBrush.h"
#include "TriangleBrush.h"
#include "ScatteredTriangleBrush.h"


#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucBitmap = NULL;
	m_ucPainting = NULL;
	m_ucEdgeImage = NULL;
	m_ucAnotherImage = NULL;
	m_ucDissolveImage = NULL;
	m_ucBlurImage = NULL;

	// create one instance of each brush
	ImpBrush::c_nBrushCount = NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes = new ImpBrush*[ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS] = new PointBrush(this, "Points");

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]
		= new LineBrush(this, "Lines");
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]
		= new CircleBrush(this, "Circles");
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]
		= new ScatteredPointBrush(this, "Scattered Points");
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]
		= new ScatteredLineBrush(this, "Scattered Lines");
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]
		= new ScatteredCircleBrush(this, "Scattered Circles");
	ImpBrush::c_pBrushes[BRUSH_TRIANGLES]
		= new TriangleBrush(this, "Triangles");
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_TRIANGLES]
		= new ScatteredTriangleBrush(this, "Scattered Triangles");

	// make one of the brushes current
	m_pCurrentBrush = ImpBrush::c_pBrushes[0];
}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush = ImpBrush::c_pBrushes[type];
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getBrushSize()
{
	return m_pUI->getBrushSize();
}

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=readBMP(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight = height;
	m_nPaintHeight = height;

	// release old storage
	if (m_ucBitmap) delete[] m_ucBitmap;
	if (m_ucPainting) delete[] m_ucPainting;
	if (m_ucEdgeImage) delete[] m_ucEdgeImage;
	if (m_ucAnotherImage) delete[] m_ucAnotherImage;
	if (m_ucDissolveImage) delete[] m_ucDissolveImage;
	if (m_ucBlurImage) delete[] m_ucBlurImage;

	m_ucBitmap = data;

	// allocate space for draw view
	m_ucPainting = new unsigned char[width*height * 3];
	m_ucEdgeImage = new unsigned char[width*height * 3];
	m_ucAnotherImage = new unsigned char[width*height * 3];
	m_ucDissolveImage = new unsigned char[width*height * 3];
	m_ucBlurImage = new unsigned char[width*height * 3];
	memset(m_ucPainting, 0, width*height*3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	edgeView();
	blurView();
	anotherView();
	m_pUI->m_origView->setView(VIEW_TYPE::ORIGINAL_VIEW);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();


	return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) 
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}

int ImpressionistDoc::loadEdgeImage(char *iname)
{
	if (loadImage(iname)) {
		edgeView();
		m_pUI->m_origView->setView(VIEW_TYPE::EDGE_VIEW);
		return (1);
	}
}

int ImpressionistDoc::loadAnotherImage(char *iname)
{
	if (loadImage(iname)) {
		anotherView();
		return (1);
	}
}

int ImpressionistDoc::loadDissolveImage(char *iname)
{
	unsigned char*	data;
	int				width,
		height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}
	dissolveView(data, width, height);
	m_pUI->m_origView->setView(VIEW_TYPE::DISSOLVE_VIEW);
	return (1);
}

int ImpressionistDoc::loadBlurImage(char *iname)
{
	if (loadImage(iname)) {
		blurView();
		m_pUI->m_origView->setView(VIEW_TYPE::BLUR_VIEW);
		return (1);
	}
}


void ImpressionistDoc::edgeView()
{
	if (!m_ucBitmap) return;
	static const char gx[][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
	static const char gy[][3] = { { -1, -2, -1 }, { 0, 0, 0 }, { 1, 2, 1 } };
	unsigned char color[3][3];
	int gxSum = 0;
	int gySum = 0;

	for (int x = 0; x < m_nWidth; x++)	{
		for (int y = 0; y < m_nHeight; y++)	{
			gxSum = 0;
			gySum = 0;
			for (int i = 0; i < 3; i++)	{
				for (int j = 0; j < 3; j++)	{
					color[i][j] = (unsigned char)(GetOriginalPixel(x - 1 + j, y - 1 + i)[0] * 0.299 + 
						GetOriginalPixel(x - 1 + j, y - 1 + i)[1] * 0.587 + 
						GetOriginalPixel(x - 1 + j, y - 1 + i)[2] * 0.114
					);
					gxSum += color[i][j] * gx[i][j];
					gySum += color[i][j] * gy[i][j];
				}
			}
			unsigned char color = 0;
			if (sqrt(gxSum * gxSum + gySum * gySum) > m_pUI->getBrushEdgeThreshold()) {
				color = 0xFF;
			}
			
			m_ucEdgeImage[3 * (y * m_nWidth + x)] = color;
			m_ucEdgeImage[3 * (y * m_nWidth + x) + 1] = color;
			m_ucEdgeImage[3 * (y * m_nWidth + x) + 2] = color;

		}
	}
}

void ImpressionistDoc::anotherView()
{
}

void ImpressionistDoc::dissolveView( unsigned char* src, int width, int height)
{
	if (!m_ucBitmap || !src) return;
	memcpy(m_ucDissolveImage, m_ucBitmap, m_nWidth * m_nHeight * 3);
	int dissolveWidth = ( (width > m_nWidth) ? m_nWidth : width);
	int dissolveHeight = ( (height > m_nHeight) ? m_nHeight : height);
	/*
	for (int i = 0; i < dissolveWidth; i ++){
		int j = i % 2;
		for (; j < dissolveHeight - 1; j += 2){
			m_ucDissolveImage[3 * ((m_nHeight - j - 1) * m_nWidth + i)]		= src[3 * ((height - j - 1) * width + i)];
			m_ucDissolveImage[3 * ((m_nHeight - j - 1) * m_nWidth + i) + 1]	= src[3 * ((height - j - 1) * width + i) + 1];
			m_ucDissolveImage[3 * ((m_nHeight - j - 1) * m_nWidth + i) + 2]	= src[3 * ((height - j - 1) * width + i) + 2];
		}	
	}
	*/

	// Dissolve Selected Image to original image, repeat if needed
	for (int i = 0; i < m_nWidth; i++){
		int j = i % 2;
		for (; j < m_nHeight - 1; j += 2){
			int srcI = i % width;
			int srcJ = j % height;
			m_ucDissolveImage[3 * ((m_nHeight - j - 1) * m_nWidth + i)] = src[3 * ((height - srcJ - 1) * width + srcI)];
			m_ucDissolveImage[3 * ((m_nHeight - j - 1) * m_nWidth + i) + 1] = src[3 * ((height - srcJ - 1) * width + srcI) + 1];
			m_ucDissolveImage[3 * ((m_nHeight - j - 1) * m_nWidth + i) + 2] = src[3 * ((height - srcJ - 1) * width + srcI) + 2];
		}
	}
	printf(" %d, %d\n", m_nWidth * m_nHeight, width * height);
	
}

void ImpressionistDoc::blurView()
{
	if (!m_ucBitmap) return;

	for (int i = 1; i < m_nWidth - 1; i++) {
		for (int j = 1; j < m_nHeight - 1; j++) {
			double sum = 0;
			for (int k = 0; k < 3; k++) {
				for (int l = 0; l < 3; l++) {
					int x = i - 1 + l;
					int y = j - 1 + k;

					sum += m_ucBitmap[(x + y*m_nWidth) * 3];
				}
			}
			unsigned char blurVal = sum / 25;

			for (int m = 0; m < 3; m++){
				m_ucBlurImage[(i + j*m_nWidth) * 3 + m] = blurVal;
			}
		}
	}
}