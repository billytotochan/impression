//
// originalview.cpp
//
// The code maintaining the original view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "originalview.h"

#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static Point	coord;

OriginalView::OriginalView(int			x, 
						   int			y, 
						   int			w, 
						   int			h, 
						   const char*	l)
							: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;

}

void OriginalView::draw()
{
	if(!valid())
	{
		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		// Tell openGL to read from the front buffer when capturing
		// out paint strokes 
		glReadBuffer( GL_FRONT );
		ortho();

	}

	glClear( GL_COLOR_BUFFER_BIT );

	if ( m_pDoc->m_ucBitmap ) 
	{
		// note that both OpenGL pixel storage and the Windows BMP format
		// store pixels left-to-right, BOTTOM-to-TOP!!  thus all the fiddling
		// around with startrow.

		m_nWindowWidth=w();
		m_nWindowHeight=h();

		int drawWidth, drawHeight;
		GLvoid* bitstart;

		// we are not using a scrollable window, so ignore it
		Point scrollpos;	// = GetScrollPosition();
		scrollpos.x=scrollpos.y=0;

		drawWidth	= min( m_nWindowWidth, m_pDoc->m_nWidth );
		drawHeight = min(m_nWindowHeight, m_pDoc->m_nHeight);

		int	startrow	= m_pDoc->m_nHeight - (scrollpos.y + drawHeight);
		if ( startrow < 0 ) 
			startrow = 0;

		int m_nStartRow = startrow;
		int m_nEndRow = startrow + drawHeight;
		int m_nStartCol = scrollpos.x;
		int m_nEndCol = m_nStartCol + drawWidth;

		bitstart = m_pDoc->m_ucBitmap + 3 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);

		// just copy image to GLwindow conceptually
		glRasterPos2i(0, m_nWindowHeight - drawHeight);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, m_pDoc->m_nWidth);
		glDrawBuffer(GL_BACK);
		glDrawPixels(drawWidth, drawHeight, GL_RGB, GL_UNSIGNED_BYTE, bitstart);

		//testing
		Point target(coord.x, m_nWindowHeight - coord.y);

		glBegin(GL_LINE);

		GLubyte color[3];
		color[0] = GLubyte(255);
		color[1] = GLubyte(0);
		color[2] = GLubyte(0);

		glColor3ubv(color);
		glVertex2d(target.x, target.y);
		//testing end

		glEnd();
	}
			
	glFlush();
}

void OriginalView::refresh()
{
	redraw();
}

void OriginalView::resizeWindow(int	width, 
								int	height)
{
	resize(x(), y(), width, height);
}

void OriginalView::edgeImage()
{
	if (!m_pDoc->m_ucBitmap) return;
	static const char gx[][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
	static const char gy[][3] = { { -1, -2, -1 }, { 0, 0, 0 }, { 1, 2, 1 } };
	unsigned char color[3][3];
	int sumX = 0;
	int sumY = 0;

	for (int x = 0; x < m_pDoc->m_nPaintWidth; x++)
	{
		for (int y = 0; y < m_pDoc->m_nHeight; y++)
		{
			sumX = 0;
			sumY = 0;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					color[i][j] = (unsigned char)(
						m_pDoc->GetOriginalPixel(x - 1 + j, y - 1 + i)[0] * 0.299 + 
						m_pDoc->GetOriginalPixel(x - 1 + j, y - 1 + i)[1] * 0.587 + 
						m_pDoc->GetOriginalPixel(x - 1 + j, y - 1 + i)[2] * 0.114
					);
					sumX += color[i][j] * gx[i][j];
					sumY += color[i][j] * gy[i][j];
				}
			}
			unsigned char color = 0xFF;
			/*if (sumX * sumX + sumY * sumY > m_pDoc->m_pUI->getBrushEdgeThreshold() *  m_pDoc->m_pUI->getBrushEdgeThreshold())
			{
				color = 0xFF;
			}*/
			m_pDoc->m_ucEdgeImage[3 * (y * m_pDoc->m_nWidth + x)] = color;
			m_pDoc->m_ucEdgeImage[3 * (y * m_pDoc->m_nWidth + x) + 1] = color;
			m_pDoc->m_ucEdgeImage[3 * (y * m_pDoc->m_nWidth + x) + 2] = color;

		}
	}
	refresh();
}

