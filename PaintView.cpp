//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintview.h"
#include "ImpBrush.h"
#include "iostream"


#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isDrawAll = 0;
static int		isAnEvent=0;
static Point	coord;

PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;

}


void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA

	if(!valid())
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );
	}

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;

	if ( isDrawAll)
		{

			glDrawBuffer(GL_FRONT);

			isDrawAll = 0;

			int tempSize = m_pDoc->m_pUI->getBrushSize();

			int spacing = m_pDoc->m_pUI->getBrushSpacing();
			for (int i = 0; i < m_nDrawWidth; i += spacing){
				for (int j = 0; j < m_nDrawHeight; j += spacing){

					if (m_pDoc->m_pUI->getBrushRandomSize()){
						m_pDoc->m_pUI->setBrushSize(rand() % tempSize + 1);
					}

					Point source(i + m_nStartCol, m_nEndRow - j);
					Point target(i, m_nWindowHeight - j);

					m_pDoc->m_pCurrentBrush->BrushBegin(source, target);

				}
			}

			m_pDoc->m_pUI->setBrushSize(tempSize);

			SaveCurrentContent();
			RestoreContent();

		}
	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();
	}

	if ( m_pDoc->m_ucPainting && isAnEvent) 
	{

		// Clear it after processing.
		isAnEvent	= 0;	

		Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		Point target( coord.x, m_nWindowHeight - coord.y );
		
		// This is the event handler
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:
			RestoreContent();
			m_pDoc->m_pCurrentBrush->BrushBegin( source, target );
			break;
		case LEFT_MOUSE_DRAG:
			m_pDoc->m_pCurrentBrush->BrushMove( source, target );
			break;
		case LEFT_MOUSE_UP:
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );

			SaveCurrentContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
			RestoreContent();
			original.x = target.x;
			original.y = target.y;

			break;
		case RIGHT_MOUSE_DRAG:
			RestoreContent();
			glBegin(GL_LINE);

			GLubyte color[3];
			color[0] = GLubyte(255);
			color[1] = GLubyte(0);
			color[2] = GLubyte(0);

			glColor3ubv(color);
			glVertex2d(original.x, original.y);
			glVertex2d(target.x, target.y);

			glEnd();

			break;
		case RIGHT_MOUSE_UP:
			if (m_pDoc->m_pCurrentBrush->BrushName() == "Lines" || m_pDoc->m_pCurrentBrush->BrushName() == "Scattered Lines"){
				int size = abs(target.x - original.x);
				if (size > 40){
					m_pDoc->m_pUI->setBrushSize( 40);
				}
				else{
					m_pDoc->m_pUI->setBrushSize( size);
				}
				m_pDoc->m_pUI->setBrushLineAngle(atan((float)(target.y - original.y) / (target.x - original.x)) * 180 / M_PI + 180);
			}
			RestoreContent();

			break;

		default:
			printf("Unknown event!!\n");		
			break;
		}
	}

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

}


int PaintView::handle(int event)
{
	switch(event)
	{
	case FL_ENTER:
	    redraw();
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent=1;
		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
		redraw();
		break;
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		redraw();
		break;
	case FL_MOVE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		break;
	default:
		return 0;
		break;

	}

	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	unsigned char* old = new unsigned char [ m_pDoc->m_nWidth * m_pDoc->m_nHeight * 3];
	memcpy(old, m_pDoc->m_ucPainting, m_pDoc->m_nWidth * m_pDoc->m_nHeight * 3);

	m_pHistory.push_back( old);

	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(GL_FRONT);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
}


void PaintView::RestoreContent()
{
	glDrawBuffer(GL_BACK);

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);

	//glDrawBuffer(GL_FRONT);
}

void PaintView::undoDraw()
{
	if ( !m_pHistory.empty()){
		if (m_pDoc->m_ucPainting) delete[] m_pDoc->m_ucPainting;
		m_pDoc->m_ucPainting = m_pHistory.back();
		//std::cout << m_pDoc->m_ucPainting << std::endl;
		m_pHistory.pop_back();
		redraw();
	}
}

void PaintView::setDrawAll()
{
	isDrawAll = 1;
	redraw();
}
