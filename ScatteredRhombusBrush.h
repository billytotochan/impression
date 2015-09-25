//
// ScatteredRhombusBrush.h
//
// The header file for Scattered Rhombus Brush. 
//

#ifndef SCATTEREDRHOMBUSBRUSH_H
#define SCATTEREDRHOMBUSBRUSH_H

#include "ImpBrush.h"

class ScatteredRhombusBrush : public ImpBrush
{
public:
	ScatteredRhombusBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);

private:

	float m_size;
	float m_angle;
};

#endif