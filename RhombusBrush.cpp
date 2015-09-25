//
// RhombusBrush.cpp
//
// The implementation of Triangle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "rhombusbrush.h"

extern float frand();

RhombusBrush::RhombusBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void RhombusBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	m_size = (float)pDoc->getBrushSize();
	m_angle = (float)dlg->getBrushLineAngle();

	glPointSize(m_size);

	BrushMove(source, target);
}

void RhombusBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
	}

	glMatrixMode(GL_MODELVIEW);
	
	for (int i = 0; i < 2; i++){
		glPushMatrix();

		glTranslatef(target.x, target.y - m_size/2, 0);
		glRotatef(m_angle, 0, 0, 1);
		glRotatef(180 * i, 0, 0, 1);
		glTranslatef(-target.x, -target.y + m_size / 2, 0);

		glBegin(GL_TRIANGLES);

		SetColor(source);
		glVertex3f(target.x - 0.5 * m_size, target.y - 0.5 * m_size, 0.0);
		glVertex3f(target.x + 0.5 * m_size, target.y - 0.5 * m_size, 0.0);
		glVertex3f(target.x, target.y + 0.5 * m_size, 0.0);

		glEnd();

		glPopMatrix();
	}

	
}

void RhombusBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

