//
// LineBrush.cpp
//
// The implementation of Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "linebrush.h"

extern float frand();

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	m_size = (float) pDoc->getBrushSize();
	m_lineWidth = (float)dlg->getBrushLineWidth();
	m_lineAngle = (float) dlg->getBrushLineAngle();
	m_brushType = dlg->getBrushStrokeDirection();

	//glPointSize(m_size);

	BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
	}

	glMatrixMode(GL_MODELVIEW);

	if (m_brushType == STROKE_DIRECTION_GRADIENT){
		
	}

	float add_x = cos(m_lineAngle * M_PI / 180) * m_size;
	float add_y = sin(m_lineAngle * M_PI / 180) * m_size;
	float dx = 0;
	float dy = 0;


	if (( m_lineAngle > 45 && m_lineAngle < 135 ) || ( m_lineAngle > 225 && m_lineAngle < 315)){
		dx = m_lineWidth / 2;
	}
	else{
		dy = m_lineWidth / 2;
	}

	//glPushMatrix();
	//glTranslatef(target.x, target.y, 0.0f);
	//glRotatef(m_lineAngle, 0, 0, 1.0f);
	glBegin(GL_TRIANGLE_STRIP);
	SetColor(source);

	glVertex2f(target.x + add_x + dx, target.y + add_y + dy);
	glVertex2f(target.x + add_x - dx, target.y + add_y - dy);
	glVertex2f(target.x - add_x + dx, target.y - add_y + dy);
	glVertex2f(target.x - add_x - dx, target.y - add_y - dy);
	
	//glVertex2f(m_size / 2, -m_lineWidth / 2);
	//glVertex2f(-m_size / 2, m_lineWidth / 2);
	//glVertex2f(m_size / 2, m_lineWidth / 2);
	glEnd();
	//glPopMatrix();
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

