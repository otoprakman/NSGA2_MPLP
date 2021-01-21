#pragma once
// A C++ program to check if a given facility lies inside a given polygon 
// Refer https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
// for explanation of functions onSegment(), orientation() and doIntersect() 

// Define Infinite (Using INT_MAX caused overflow problems) 
#define INF 100 

bool isInside(std::vector<facility> polygon, int n, facility p);

// Given three colinear facilitys p, q, r, the function checks if 
// facility q lies on line segment 'pr' 
bool onSegment(facility p, facility q, facility r)
{
	if (q.CoordX <= std::max(p.CoordX, r.CoordX) && q.CoordX >= std::min(p.CoordX, r.CoordX) &&
		q.CoordY <= std::max(p.CoordY, r.CoordY) && q.CoordY >= std::min(p.CoordY, r.CoordY))
		return true;
	return false;
}

// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int orientation(facility p, facility q, facility r)
{
	float val = (q.CoordY - p.CoordY) * (r.CoordX - q.CoordX) - (q.CoordX - p.CoordX) * (r.CoordY - q.CoordY);
	//cout << "VAL: " << val << std::endl;
	if (val == 0)
		return 0; // colinear
	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// The function that returns true if line segment 'p1q1' 
// and 'p2q2' intersect. 
bool doIntersect(facility p1, facility q1, facility p2, facility q2)
{
	// Find the four orientations needed for general and 
	// special cases 
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case 
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases 
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1 
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	// p1, q1 and p2 are colinear and q2 lies on segment p1q1 
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2 
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	// p2, q2 and q1 are colinear and q1 lies on segment p2q2 
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false; // Doesn't fall in any of the above cases 
}

// Returns true if the facility p lies inside the polygon[] with n vertices 
bool isInside(std::vector<facility> polygon, int n, facility p)
{
	// There must be at least 3 vertices in polygon[] 
	if (n < 3) return false;

	// Create a facility for line segment from p to infinite 
	facility extreme = { INF, p.CoordY, 0, 0 };

	// Count intersections of the above line with sides of polygon 
	int count = 0, i = 0;
	do
	{
		int next = (i + 1) % n;

		// Check if the line segment from 'p' to 'extreme' intersects 
		// with the line segment from 'polygon[i]' to 'polygon[next]' 
		if (doIntersect(polygon[i], polygon[next], p, extreme))
		{
			// If the facility 'p' is colinear with line segment 'i-next', 
			// then check if it lies on segment. If it lies, return true, 
			// otherwise false 
			if (orientation(polygon[i], p, polygon[next]) == 0)
				return onSegment(polygon[i], p, polygon[next]);

			count++;
		}
		i = next;
	} while (i != 0);

	//cout << "Mutation-3::Determine if Possible Locations outside of Hull: COMPLETED" << std::endl;

	//cout << "Is inside: " << (count & 1) << std::endl;
	// Return true if count is odd, false otherwise 
	return count & 1; // Same as (count%2 == 1) 
}


