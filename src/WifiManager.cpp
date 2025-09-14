/*
 * WifiManager.cpp
 *
 *  Created on: 13.09.2025
 *      Author: hocki
 */

/* Start Wifi connection with defined SSID, password */
#define USE_CREDENTIALS

#ifdef USE_CREDENTIALS
/**
    A template for wifi credential file
    -----------------------------------
        credentials.h
            #ifndef CREDENTIALS_H_
            #define CREDENTIALS_H_

            #define WIFI_SSID       "    "   // SSID of accesspoint
            #define WIFI_PASS       "    "   // *assword of access point

            #endif
    -----------------------------------
*/
#include "../credentials.h"
#endif /* USE_CREDENTIALS */

#include <WiFi.h>

#include "Logger.h"
#include "WifiManager.h"


/* Log level for this module */
#define LOG_LEVEL   (LOG_DEBUG)


/**
 * @brief Constructor
 */
WifiManager::WifiManager()
{
}

/**
 * @brief Destructor
 */
WifiManager::~WifiManager()
{
}

void WifiManager::Init(void)
{
    /* Register wifi events listener */
	WiFi.onEvent(
        std::bind(&WifiManager::WiFiEvent, this, std::placeholders::_1));
}

void WifiManager::Loop(void)
{
  	yield(); // -- Yield should not be necessary, but cannot hurt eather.

	ProcessState();
}

WifiManager::tStatus WifiManager::GetStatus(void)
{
    return mStatus;
}

void WifiManager::ProcessState(void)
{
	switch (mState)
	{
		case STATE_BOOT:
			mStatus = STATUS_NOT_CONNECTED;

#ifdef USE_CREDENTIALS
            /* Try to connect to WiFi */
            mState = STATE_CONNECTING;
            ConnectWifi();
#endif /* USE_CREDENTIALS */

			break;

		case STATE_CONNECTING:
			if (WiFi.status() != WL_CONNECTED)
			{
				if ((millis() - mConnectionStart) > mConnectionTimeout)
			    {
					LOG(LOG_ERROR, "Failed to connect after %d millis", mConnectionTimeout);

                    /* Try to reconnect to WiFi */
                    mState = STATE_RECONNECTING;
					mConnectionStart = millis();
					WiFi.reconnect();
			    }
			}
			else
			{
				LOG(LOG_DEBUG, "We are online after %d millis", (millis() - mConnectionStart));
				mState  = STATE_ONLINE;
				mStatus = STATUS_ONLINE;
			}

			break;

		case STATE_RECONNECTING:
			if (WiFi.status() != WL_CONNECTED)
			{
				if ((millis() - mConnectionStart) > mConnectionTimeout)
			    {
					LOG(LOG_ERROR, "Failed to reconnect after %d millis", mConnectionTimeout);

					/* Try again */
					mConnectionStart = millis();
					WiFi.reconnect();
			    }
			}
			else
			{
				LOG(LOG_DEBUG, "We are online after reconnecting");
				mState  = STATE_ONLINE;
				mStatus = STATUS_ONLINE;
			}
			break;

		case STATE_NOT_CONFIGURED:
			break;

		case STATE_AP_MODE:
			break;

		case STATE_ONLINE:
			if (mWifiEventTriggered)
			{
                mWifiEventTriggered = false;

				switch (mWifiEvent)
				{
					case SYSTEM_EVENT_STA_DISCONNECTED:
						 LOG(LOG_DEBUG, "[Event] SYSTEM_EVENT_STA_DISCONNECTED, reconnecting");

						 mState  = STATE_RECONNECTING;
						 mStatus = STATUS_NOT_CONNECTED;

						 mConnectionStart = millis();
						 WiFi.reconnect();
						break;

					default:
						break;
				};

                /* Reset event */
                mWifiEvent = ARDUINO_EVENT_MAX;
			}
			break;

		default:
			break;
	}
}

void WifiManager::ConnectWifi(void)
{
    /* Disconnect from the network (close AP) */
    WiFi.softAPdisconnect(true);
    /* Disconnect from the network */
    WiFi.disconnect(true);
    delay(100);

    /* FIX problem:
     *    unable to connect to the router
     *    if previous connection failed
     *    (the bugfix seems to work)
     */
    /* Disable persistent for ESP32 */
    WiFi.persistent(false);

    /* Start WiFi Station mode */
    WiFi.mode(WIFI_STA);
    delay(100);

    /* Ste connection start time */
    mConnectionStart = millis();

#ifdef USE_CREDENTIALS
    /* Start Wifi connection */
    LOG(LOG_DEBUG, "Start WiFi Station mode, credentials SSID: %s; password: %s",
    		WIFI_SSID, WIFI_PASS);

    WiFi.begin(WIFI_SSID, WIFI_PASS);
#else
    /* Connect to SDK config */
    LOG(LOG_DEBUG, "Start WiFi Station mode, connect to SDK config");
    WiFi.begin();
#endif /* ifdef USE_CREDENTIALS */
}

void WifiManager::WiFiEvent(WiFiEvent_t aEvent)
{
    mWifiEventTriggered = true;
    mWifiEvent = aEvent;

//    LOG(LOG_DEBUG, "[WiFi-event] event: %d", event);
	switch (aEvent)
	{
//		case SYSTEM_EVENT_STA_START:
//			LOG(LOG_DEBUG, "[WiFi-event] station start");
//			break;
//		case SYSTEM_EVENT_STA_STOP:
//			LOG(LOG_DEBUG, "[WiFi-event] station stop");
//			break;
		case SYSTEM_EVENT_STA_CONNECTED:
			LOG(LOG_DEBUG, "[WiFi-event] station connected to AP");
			break;
		case SYSTEM_EVENT_STA_DISCONNECTED:
//TODO there are to much disconnection events if not possible connect to STA with saved SSID isn't exist. FIX IT
//			LOG(LOG_DEBUG, "[WiFi-event] station disconnected from AP");
			break;
		case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
			LOG(LOG_DEBUG, "[WiFi-event] the auth mode of AP connected by ESP32 station changed");
			break;
		case SYSTEM_EVENT_STA_GOT_IP:
			LOG(LOG_DEBUG, "[WiFi-event] station got IP from connected AP %s", WiFi.localIP().toString().c_str());
			break;
		case SYSTEM_EVENT_STA_LOST_IP:
			LOG(LOG_DEBUG, "[WiFi-event] station lost IP and the IP is reset to 0");
			break;
		default:
			break;
    }
}