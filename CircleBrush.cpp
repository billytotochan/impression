//
// CircleBrush.cpp
//
// The implementation of Circle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "circlebrush.h"

extern float frand();

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void CircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	m_size = (float) pDoc->getBrushSize();
	//glPointSize(m_size);

	BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
	}

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glBegin(GL_POLYGON);
	SetColor(source);

	int lineAmount = 100;
	for (int i = 0; i <= lineAmount; i++) {
		glVertex2f(
			target.x + (m_size * cos(i *  M_PI * 2 / lineAmount)),
			target.y + (m_size * sin(i * M_PI * 2 / lineAmount))
			);
	}
	glEnd();
	glPopMatrix();
}

void CircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

