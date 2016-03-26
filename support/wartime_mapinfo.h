/** WarTime-Project On GitHub
*   https://github.com/Kiritow/WarTime-Project.git
*/

#ifndef WARTIME_MAPINFO
#define WARTIME_MAPINFO

#include <cmath>

struct _block
{
	
};

using MAP = _block;
using MAPHANDLE = MAP*;

struct _point
{
    int x,y;
};
using POINT = _point;
using DURATION = double;
DURATION getDistance(POINT a,POINT b)
{
    return std::sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

#endif /* End of WARTIME_MAPINFO*/
