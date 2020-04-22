#ifndef _eventdisplay_h_
#define _eventdisplay_h_

#include "hough.h"
#include "ellipsefitter.h"



/// Event display for EllipseFitter Results
/// Grabs hit pixels and hough results from HoughResults object
/// Grabs dimensions of display from CircleHough binning
void new_event_display( const unsigned long long iev,
			CircleHough & ch,
			const HoughResults & hcr,
			const EllipseFitter & el );



#endif
