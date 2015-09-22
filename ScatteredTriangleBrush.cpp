//
// ScatteredTriangleBrush.cpp
//
// The implementation of Scattered Triangle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredtrianglebrush.h"

extern float frand();

ScatteredTriangleBrush::ScatteredTriangleBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void ScatteredTriangleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	m_size = (float)pDoc->getBrushSize();

	glPointSize(m_size);

	BrushMove(source, target);
}

void ScatteredTriangleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	float Ax, Ay;

	if (pDoc == NULL) {
	}

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	for (int i = 0; i < 3; i++){

		glBegin(GL_TRIANGLES);

		Ax = target.x - m_size / 2 + rand() % (int)m_size;
		Ay = target.y - m_size / 2 + rand() % (int)m_size;
		SetColor(Point((int)Ax, (int)Ay));
		glVertex3f(Ax - 0.5 * m_size, Ay - 0.5 * m_size, 0.0);
		glVertex3f(Ax + 0.5 * m_size, Ay - 0.5 * m_size, 0.0);
		glVertex3f(Ax, Ay + 0.5 * m_size, 0.0);

		glEnd();
	}
}

void ScatteredTriangleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

