//
// ScatteredCircleBrush.cpp
//
// The implementation of Scattered Circle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredcirclebrush.h"

extern float frand();

ScatteredCircleBrush::ScatteredCircleBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void ScatteredCircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	m_size = (float)pDoc->getBrushSize();
	//glPointSize(m_size);

	BrushMove(source, target);
}

void ScatteredCircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	float Ax, Ay;

	if (pDoc == NULL) {
	}

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();


	for (int i = 0; i < 3; i++){
		glBegin(GL_POLYGON);

		Ax = target.x - m_size / 2 + rand() % (int)m_size;
		Ay = target.y - m_size / 2 + rand() % (int)m_size;
		SetColor(Point((int)Ax, (int)Ay));

		int lineAmount = 100;
		for (int i = 0; i <= lineAmount; i++) {
			glVertex2f(
				Ax + (m_size * cos(i *  M_PI * 2 / lineAmount)),
				Ay + (m_size * sin(i * M_PI * 2 / lineAmount))
			);
		}

		glEnd();
	}

	glPopMatrix();
}

void ScatteredCircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

