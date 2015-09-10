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
	m_width = (float) dlg->getBrushLineWidth();
	m_angle = (float) dlg->getBrushLineAngle();

	glPointSize(m_size);

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
	glPushMatrix();
	glTranslatef(target.x, target.y, 0.0f);
	glRotatef(m_lineAngle, 0, 0, 1.0f);
	glBegin(GL_TRIANGLE_STRIP);
	SetColor(source);

	glVertex2f(-m_size / 2, -m_lineWidth / 2);
	glVertex2f(m_size / 2, -m_lineWidth / 2);
	glVertex2f(-m_size / 2, m_lineWidth / 2);
	glVertex2f(m_size / 2, m_lineWidth / 2);
	glEnd();
	glPopMatrix();
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

