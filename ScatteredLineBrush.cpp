//
// ScatteredLineBrush.cpp
//
// The implementation of Scattered Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredlinebrush.h"

extern float frand();

ScatteredLineBrush::ScatteredLineBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void ScatteredLineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	m_size = (float)pDoc->getBrushSize();
	m_lineWidth = (float)dlg->getBrushLineWidth();
	m_lineAngle = (float)dlg->getBrushLineAngle();
	m_brushType = dlg->getBrushStrokeDirection();

	//glPointSize(m_size);

	BrushMove(source, target);
}

void ScatteredLineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	float Ax, Ay;

	if (pDoc == NULL) {
	}

	glMatrixMode(GL_MODELVIEW);

	if (m_brushType == STROKE_DIRECTION_GRADIENT){
		float factorR = 0.3f;
		float factorG = 0.6f;
		float factorB = 0.11f;
		
		static const char gx[][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
		static const char gy[][3] = { { -1, -2, -1 }, { 0, 0, 0 }, { 1, 2, 1 } };
		unsigned char color[3][3];
		int gxSum = 0;
		int gySum = 0;

		gxSum = 0;
		gySum = 0;
		for (int i = 0; i < 3; i++)	{
			for (int j = 0; j < 3; j++)	{
				color[i][j] = (unsigned char)(pDoc->GetOriginalPixel(target.x - 1 + j, target.y - 1 + i)[0] * factorR +
					pDoc->GetOriginalPixel(target.x - 1 + j, target.y - 1 + i)[1] * factorG +
					pDoc->GetOriginalPixel(target.x - 1 + j, target.y - 1 + i)[2] * factorB
					);
				gxSum += color[i][j] * gx[i][j];
				gySum += color[i][j] * gy[i][j];
			}
		}
		m_lineAngle = atan2((float)gySum, (float)gxSum) * 180 / M_PI;

	}
	if (m_brushType == STROKE_DIRECTION_BRUSH_DIRECTION){
		m_lineAngle = pDoc->m_pUI->m_paintView->getBrushDirection();
	}

	float add_x = cos(m_lineAngle * M_PI / 180) * m_size / 2;
	float add_y = sin(m_lineAngle * M_PI / 180) * m_size / 2;
	float dx = 0;
	float dy = 0;


	if ((m_lineAngle > 45 && m_lineAngle < 135) || (m_lineAngle > 225 && m_lineAngle < 315)){
		dx = m_lineWidth / 2;
	}
	else{
		dy = m_lineWidth / 2;
	}

	//glPushMatrix();
	//glTranslatef(target.x, target.y, 0.0f);
	//glRotatef(m_lineAngle, 0, 0, 1.0f);
	for (int i = 0; i < 3; i++){
		glBegin(GL_TRIANGLE_STRIP);

		Ax = target.x - m_size / 2 + rand() % (int)m_size;
		Ay = target.y - m_size / 2 + rand() % (int)m_size;
		SetColor(Point((int)Ax, (int)Ay));

		glVertex2f(Ax + add_x + dx, Ay + add_y + dy);
		glVertex2f(Ax + add_x - dx, Ay + add_y - dy);
		glVertex2f(Ax - add_x + dx, Ay - add_y + dy);
		glVertex2f(Ax - add_x - dx, Ay - add_y - dy);

		//glVertex2f(m_size / 2, -m_lineWidth / 2);
		//glVertex2f(-m_size / 2, m_lineWidth / 2);
		//glVertex2f(m_size / 2, m_lineWidth / 2);
		glEnd();
	}

	
	//glPopMatrix();
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

