/*
* Alma Mater Studiorum - University of Bologna
* First cycle degree in Computer Science
* Computer Programming - 00819
*
* Author: Musiani Samuele #0001069143
*
* precisePoint.hpp: Class for a more precise point in a plane.
*/

#include "point.hpp"

#ifndef PHYSICS_PRECISEPOINT
#define PHYSICS_PRECISEPOINT

namespace phy
{
	class PrecisePoint
	{
		protected:
			double posX;
			double posY;

		public:
			PrecisePoint();
			PrecisePoint(double posX, double posY);
			void set_xPosition(double posX);
			void set_yPosition(double posY);
			double get_xPosition();
			double get_yPosition();

			phy::Point toPoint();
	};
}
#endif
