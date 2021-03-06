//
// originalview.cpp
//
// The code maintaining the original view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "originalview.h"

#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#endif

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
	view = 0;

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

	unsigned char* to_load = 0;
	switch (view)
	{
	case EDGE_VIEW:
		to_load = m_pDoc->m_ucEdgeImage;
		break;
	case ANOTHER_VIEW:
		to_load = m_pDoc->m_ucAnotherImage;
		break;
	case DISSOLVE_VIEW:
		to_load = m_pDoc->m_ucDissolveImage;
		break;
	case BLUR_VIEW:
		to_load = m_pDoc->m_ucBlurImage;
		break;
	default:
		to_load = m_pDoc->m_ucBitmap;
		break;
	}

	if (to_load)
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

		bitstart = to_load + 3 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);

		// just copy image to GLwindow conceptually
		glRasterPos2i(0, m_nWindowHeight - drawHeight);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, m_pDoc->m_nWidth);
		glDrawBuffer(GL_BACK);
		glDrawPixels(drawWidth, drawHeight, GL_RGB, GL_UNSIGNED_BYTE, bitstart);

		//testing
		//glDrawBuffer(GL_FRONT);

		Point target(coord.x, m_nWindowHeight - coord.y);

		glPointSize(5);
		glBegin(GL_POINT);


		GLubyte color[3];
		color[0] = GLubyte(255);
		color[1] = GLubyte(0);
		color[2] = GLubyte(0);

		glColor3ubv(color);
		glVertex2d(target.x, target.y);
		//printf("%d, %d\n", target.x, target.y);

		glEnd();

		//testing end
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

void OriginalView::setView(int type)
{
	view = type;
	/*if (type == DISSOLVE_VIEW) {
		m_pDoc->dissolveImage(10);
	}*/
	refresh();
}

void OriginalView::drawPointer(Point target)
{
	coord.x = target.x;
	coord.y = target.y;
	redraw();
}