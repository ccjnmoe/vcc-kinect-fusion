//////////////////////////////////////////////////////////////////////////
// 
// This file handle methods with Kinect Sensor
//
//////////////////////////////////////////////////////////////////////////


#include "KinectSensor.h"

/// Constructor
KinectSensor::KinectSensor()
{
	m_pNuiSensor = NULL;

}


/// Destructor
KinectSensor::~KinectSensor()
{

}

/// Create connection with kinect sensor, open depth image stream and toggle near mode
HRESULT KinectSensor::CreateFirstConnected()
{
	INuiSensor * pNuiSensor;
	HRESULT hr;

	int iSensorCount = 0;
	hr = NuiGetSensorCount(&iSensorCount);
	if (FAILED(hr)) { return hr; }

	/// Look at each Kinect Sensor
	for (int i=0 ; i<iSensorCount ; i++)
	{
		/// Create the sensor so we can check status, if we can't create it, move on to the next
		hr = NuiCreateSensorByIndex(i, &pNuiSensor);
		if(FAILED(hr))
		{
			continue;
		}

		/// Get the status of the sensor, and if connected, then we can initialize it
		hr = pNuiSensor->NuiStatus();
		if(S_OK == hr)
		{
			m_pNuiSensor = pNuiSensor;
			break;
		}

		/// This sensor wasn't OK, so releaase it since we're not using it
		pNuiSensor->Release();
	}

	if(NULL == m_pNuiSensor)
	{
		return E_FAIL;
	}

	/// Initialize th Kinect and specify that we'll be using depth
	hr = m_pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX);
	if(FAILED(hr)) { return hr; }

	/// Create a event that will be signaled when depth data is available
	m_hNextDepthFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);  ///////////what are these attribute stand for ?

	/// Open a depth image stream to receive depth frames
	hr = m_pNuiSensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX,NUI_IMAGE_RESOLUTION_640x480,0,2,m_hNextDepthFrameEvent,&m_pDepthStreamHandle);
	if(FAILED(hr)) { return hr;}

	/// Start with near mode on
	ToggleNearMode();

	std::cout<<"link success, image stream open!"<<std::endl;
	return hr;
}


/// Toggle between near and default mode
HRESULT KinectSensor::ToggleNearMode()
{
	HRESULT hr = E_FAIL;

	if(m_pNuiSensor)
	{
		hr = m_pNuiSensor->NuiImageStreamSetImageFrameFlags(m_pDepthStreamHandle, m_bNearMode ? 0:NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE);
		
		if(SUCCEEDED(hr))
		{
			m_bNearMode = !m_bNearMode;
		}
	}
	return hr;

}

