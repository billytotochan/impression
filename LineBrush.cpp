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
		float factorR = 0.3;
		float factorG = 0.6;
		float factorB = 0.11;

		int sobelX[3][3] = {{ -1, -2, -1 },
							{ 0, 0, 0 },
							{ 1, 2, 1 }};

		int sobelY[3][3] = {{ -1,0,1 }, 
							{ -2,0,2 }, 
							{ -1,0,1 }};

		GLubyte colorR[3][3], colorG[3][3], colorB[3][3];
		float color[3][3];
		int RGx = 0, RGy = 0, GGx = 0, GGy = 0, BGx = 0, BGy = 0;
		float Gx = 0, Gy = 0;
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				Point temp(source.x + j - 1, source.y + i - 1);
				GLubyte tempC[3];
				memcpy(tempC, pDoc->GetOriginalPixel(temp), 3);
				//colorR[i][j] = tempC[0];
				//colorG[i][j] = tempC[1];
				//colorB[i][j] = tempC[2];
				color[i][j] = tempC[0] * factorR + tempC[1] * factorG + tempC[2] * factorB;
				//printf("%d ,", colorR[i][j]);

				//RGx += sobelX[i][j] * colorR[i][j];
				//RGy += sobelY[i][j] * colorR[i][j];
				//GGx += sobelX[i][j] * colorG[i][j];
				//GGy += sobelY[i][j] * colorG[i][j];
				//BGx += sobelX[i][j] * colorB[i][j];
				//BGy += sobelY[i][j] * colorB[i][j];
				Gx += color[i][j] * sobelX[i][j];
				Gy += color[i][j] * sobelY[i][j];

			}
		}
		float angle = 0;
		//angle += atan2((float)RGy, (float)RGx) * 180 / M_PI + 180;
		//angle += atan2((float)GGy, (float)GGx) * 180 / M_PI + 180;
		//angle += atan2((float)BGy, (float)BGx) * 180 / M_PI + 180;
		angle += atan2((float)Gy, (float)Gx) * 180 / M_PI;
		m_lineAngle = angle;

		printf("%f ,", m_lineAngle);
		//printf("%d ,%d \n", RGx, RGy);
		//printf("%d ,%d \n", GGx, GGy);
		//printf("%d ,%d \n", BGx, BGy);

	}

	float add_x = cos(m_lineAngle * M_PI / 180) * m_size / 2;
	float add_y = sin(m_lineAngle * M_PI / 180) * m_size / 2;
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

