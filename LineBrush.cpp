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

	m_size = pDoc->getBrushSize();
	m_width = dlg->getBrushLineWidth();
	m_angle = dlg->getBrushLineAngle();
	//glPointSize((float)m_size);

	glPointSize((float)m_size);

	BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
	}

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glBegin(GL_POLYGON);
		SetColor(source);

		glVertex2f(target.x - m_size / 2, target.y - m_width / 2);
		glVertex2f(target.x + m_size / 2, target.y - m_width / 2);
		glVertex2f(target.x + m_size / 2, target.y + m_width / 2);
		glVertex2f(target.x - m_size / 2, target.y + m_width / 2);

	glVertex2d(target.x, target.y);

	glEnd();
	glPopMatrix();
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

