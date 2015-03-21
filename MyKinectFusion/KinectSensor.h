#pragma once

#include "stdafx.h"


class KinectSensor
{
public:

	/// Constructor
	KinectSensor();

	/// Destructor
	~KinectSensor();

	/// Connect kinect sensor
	HRESULT CreateFirstConnected();
	HRESULT ToggleNearMode();

private:

	/// handle kinect operation
	INuiSensor* m_pNuiSensor;
	HANDLE m_hNextDepthFrameEvent;
	HANDLE m_pDepthStreamHandle;
	bool m_bNearMode;


};

