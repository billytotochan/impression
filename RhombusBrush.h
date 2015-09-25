//
// RhombusBrush.h
//
// The header file for Rhombus Brush. 
//

#ifndef RHOMBUSBRUSH_H
#define RHOMBUSBRUSH_H

#include "ImpBrush.h"

class RhombusBrush : public ImpBrush
{
public:
	RhombusBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);

private:

	float m_size;
	float m_angle;
};

#endif