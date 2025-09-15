/*
 * TimeManager.cpp
 *
 *  Created on: 08.09.2025
 *      Author: hocki
 */

#include <TimeLib64.h>
#include <ESPNtpClient.h>

#include "Logger.h"

#include "TimeManager.h"


/* Log level for this module */
#define LOG_LEVEL   (LOG_DEBUG)

#define NTP_SERVER_NAME     PSTR("pool.ntp.org")
#define NTP_TIMEOUT         5000    // 5 sec
#define NTP_SYNC_PERIOD     600     // 10 min

#define TIME_ZONE           TZ_Europe_Berlin


/**
 * @brief Constructor
 */
TimeManager::TimeManager()
{
    // do nothing
}

/**
 * @brief Destructor
 */
TimeManager::~TimeManager()
{
    /* Clear pointers */
    mpMinuteEventCallback = nullptr;
}

void TimeManager::Init(void)
{
    /* Initialize local time with compilation time */
    DateTimeNS::tDateTime wDateTime = GetCompileTime();
    SetDateTime(wDateTime);

    /* Init time variables */
    mPrevTime = GetDateTime();

    /* Register NTP sync events callback */
	NTP.onNTPSyncEvent(
        std::bind(&TimeManager::HandleNTPSyncEvent, this, std::placeholders::_1));

    /* Set time zone */
    NTP.setTimeZone(TIME_ZONE);
    /* Set sync parameters */
    NTP.setInterval(NTP_SYNC_PERIOD);
    NTP.setNTPTimeout(NTP_TIMEOUT);
//    NTP.setMinSyncAccuracy(5000);
//    NTP.settimeSyncThreshold(3000);

    /* Start NTP client */
    NTP.begin(NTP_SERVER_NAME, false);
}

void TimeManager::Loop(void)
{
    /* Get current time */
 	DateTimeNS::tDateTime wCurrTime = GetDateTime();

    /* Check if a minute event should be fired */
    if ((mPrevTime.mTime.mHour   != wCurrTime.mTime.mHour) ||
        (mPrevTime.mTime.mMinute != wCurrTime.mTime.mMinute))
    {
        /* Notify callback */
        if (mpMinuteEventCallback != nullptr)
        {
            mpMinuteEventCallback->NotifyDateTime(wCurrTime);
        }
    }

    /* Update previous time */
    mPrevTime = wCurrTime;
};

/**
 * @brief Register a callback for minute events
 */
void TimeManager::RegisterMinuteEventCallback(NotifyTimeCallback* apCallback)
{
    /* Check input parameter */
    if (apCallback != nullptr)
    {
        mpMinuteEventCallback = apCallback;
    }
}

DateTimeNS::tDateTime TimeManager::ConvertTime(time_t aTime)
{
    DateTimeNS::tDateTime wDateTime;

    /* Parse given time */
    tmElements_t tm;
    breakTime(aTime, tm);

    /*Fill time struct with the values */
    wDateTime.mTime.mSecond 	= tm.Second;
    wDateTime.mTime.mMinute 	= tm.Minute;
    wDateTime.mTime.mHour   	= tm.Hour;
    //
    wDateTime.mDate.mDay 	  	= tm.Day;
    wDateTime.mDate.mWeekDay 	= tm.Wday;
    wDateTime.mDate.mMonth  	= tm.Month;

    /* Convert tm.Year (offset from 1970) to the calendar year */
    wDateTime.mDate.mYear   	= tmYearToCalendar(tm.Year);

    return wDateTime;
}

void TimeManager::SetDateTime(time_t aTime)
{
    DateTimeNS::tDateTime wDateTime = ConvertTime(aTime);

    /* Set new local time */
    SetDateTime(wDateTime);
}

/**
 * @brief Set current time
 */
void TimeManager::SetDateTime(DateTimeNS::tDateTime aDateTime)
{
    /* Set new local time */
	setTime(aDateTime.mTime.mHour, aDateTime.mTime.mMinute, aDateTime.mTime.mSecond,
			aDateTime.mDate.mDay, aDateTime.mDate.mMonth, aDateTime.mDate.mYear);
}

/**
 * @brief Get current time
 */
DateTimeNS::tDateTime TimeManager::GetDateTime(void)
{
    /* Get current time */
    time_t wCurrTime = now();
    /* and convert it */
    DateTimeNS::tDateTime wDateTime = ConvertTime(wCurrTime);

	return wDateTime;
}

/**
 * @brief Returns the compile date and time
 */
DateTimeNS::tDateTime TimeManager::GetCompileTime(void)
{
    DateTimeNS::tDateTime wDateTime;

    char compMon[4], *m;

    strncpy(compMon, __DATE__, 3);
    compMon[3] = '\0';
    m = strstr(DateTimeNS::mMonthsStr, compMon);

    wDateTime.mDate.mMonth  = ((m - DateTimeNS::mMonthsStr) / 3 + 1);
    wDateTime.mDate.mDay    = atoi(__DATE__ + 4);
    wDateTime.mDate.mYear   = atoi(__DATE__ + 7) /* - 1970*/;

    wDateTime.mTime.mHour   = atoi(__TIME__);
    wDateTime.mTime.mMinute = atoi(__TIME__ + 3);
    wDateTime.mTime.mSecond = atoi(__TIME__ + 6);

    return wDateTime;
}

void TimeManager::HandleNTPSyncEvent(NTPEvent_t aEvent)
{
    /* Process event */
    switch (aEvent.event)
    {
        case timeSyncd:
        {
            uint8_t wHour, wMinute, wSecond;
            uint8_t wDay,  wMonth;
            uint16_t wYear;

            /* Time successfully got from NTP server */
            LOG(LOG_DEBUG, "TimeManager::HandleNTPSyncEvent() Sync NTP time successfully done: %s",
                    NTP.getTimeDateString());

            /* Get NTP time and date */
            char* wpTimeStr = NTP.getTimeStr(); // 'HH:MM:SS'  , e.g. 00:23:56
            char* wpDateStr = NTP.getDateStr(); // 'DD/MM/YYYY', e.g. 25/12/2023

            /* Parse time string and date strings */
            if ((sscanf(wpTimeStr, "%d:%d:%d", &wHour, &wMinute, &wSecond) == 3) &&
                (sscanf(wpDateStr, "%d/%d/%d", &wDay,  &wMonth,  &wYear)   == 3))
            {
                tmElements_t wTmElems;

                /* Fill tmElements_t struct with the parsed values */
                wTmElems.Hour   = wHour;
                wTmElems.Minute = wMinute;
                wTmElems.Second = wSecond;
                wTmElems.Day    = wDay;
                wTmElems.Month  = wMonth;
                wTmElems.Year   = CalendarYrToTm(wYear); // Convert full year to tm format

                /* Update local time */
                SetDateTime(
                    makeTime(wTmElems));    // convert tmElements_t to time_t
            }
        }
            break;

        default:
            break;
    }
}