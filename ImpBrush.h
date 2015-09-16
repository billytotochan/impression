#ifndef IMPBRUSH_H
#define IMPBRUSH_H

//
// ImpBrush.h
//
// The header file of virtual brush. All the other brushes inherit from it.
//

#include <stdlib.h>

// Each brush type has an associated constant.
enum
{
	BRUSH_POINTS = 0,
	BRUSH_LINES,
	BRUSH_CIRCLES,
	BRUSH_SCATTERED_POINTS,
	BRUSH_SCATTERED_LINES,
	BRUSH_SCATTERED_CIRCLES,
	NUM_BRUSH_TYPE // Make sure this stays at the end!
};

enum
{
	STROKE_DIRECTION_SLIDER_RIGHT_MOUSE = 0,
	STROKE_DIRECTION_GRADIENT,
	STROKE_DIRECTION_BRUSH_DIRECTION,
	NUM_BRUSH_STROKE_DIRECTION
};

enum
{
	STYLE_IMPRESSIONIST = 0,
	STYLE_EXPRESSIONIST,
	STYLE_COLOR_WASH,
	STYLE_POINTILLIST,
	STYLE_CUSTOMIZE,
	NUM_PAINTLY_STYLE
};

enum
{
	STROKE_CURVE_BRUSH = 0,
	STROKE_BSPLINE_BRUSH,
	STROKE_CIRCLE_BRUSH,
	STROKE_CLIP_LINE_BRUSH,
	STROKE_LINE_BRUSH,
	NUM_PAINTLY_STROKE
};

class ImpressionistDoc; // Pre-declaring class

class Point 
{
public:
	Point() {};
	Point(int xx, int yy) { x = xx; y = yy; };

	int x, y;
};

class ImpBrush 
{
protected:
	ImpBrush::ImpBrush( ImpressionistDoc* pDoc = NULL, char* name = NULL );

public:
	// The implementation of your brush should realize these virtual functions
	virtual void BrushBegin( const Point source, const Point target ) = 0;
	virtual void BrushMove( const Point source, const Point target ) = 0;
	virtual void BrushEnd( const Point source, const Point target ) = 0;

	// according to the source image and the position, determine the draw color
	void SetColor( const Point source );

	// get Doc to communicate with it
	ImpressionistDoc* GetDocument( void );

	// Return the name of the brush (not used in this version).
	char* BrushName(void);

	static int			c_nBrushCount;	// How many brushes we have,
	static ImpBrush**	c_pBrushes;		// and what they are.

	//static int			c_nStrokeDirectionCount;	// How many stroke directions we have,
	//static ImpBrush**	c_pStrokeDirections;		// and what they are.

private:
	ImpressionistDoc*	m_pDoc;

	// Brush's name (not used in this version).
	char*				m_pBrushName;
};

#endif