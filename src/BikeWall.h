#pragma once

#include "ofVec2f.h"
#include "ofxIldaFrame.h"

/**
 * Class representing a wall drawn by bikes as a series of line segments.
 */
class BikeWall
{
public:
	/**
	 * Creates a new wall with the specified color.
	 * @param c The color of the line.
	 */
	BikeWall(ofColor c);
	~BikeWall();

	/**
	 * Moves the last point to a new position.
	 * @param point The normalized position of the new point
	 */
	void ExtendLast(ofVec2f point);
	/**
	 * Adds a new point to the wall (thus adding a new line segment)
	 * @param point The normalized position of the new point
	 */
	void AddNewLine(ofVec2f point);

	/**
	 * Adds the line segments to the Ilda point queue (frame)
	 * @param frame The Ilda frame to add the points to.
	 * @param soundShift The shift in hue produced by the sound engine
	 */
	void Update(ofxIlda::Frame* frame, float soundShift);
	/**
	 * Deletes the line segments
	 */
	void Clear();

	/**
	 * Returns the closest point on the wall to a specified point (in normalized coordinates)
	 * @param p The point
	 * @param outDistance Pointer to the distance, if needed (can be null)
	 * @return The Point on the wall closest to the specified point.
	 */
	ofVec2f getClosestPointTo(ofVec2f p, float* outDistance = nullptr);
	/**
	 * Checks if a rectangle intersects the wall.
	 * @param shape The rectangle
	 * @return True, if the rectangle intersects the wall.
	 */
	bool Intersects(ofRectangle shape);

	int BikeIdx; ///< The index of the bike this wall belongs to.
private:

	ofColor Color; ///< The base color of the wall (without sound shift)
	ofxIlda::Poly poly; ///< The underlying polyline
	int pointCount; ///< The number of points this wall has
};
