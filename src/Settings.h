/*
 * Settings.h
 *
 *  Created on: 16.10.2025
 *      Author: hocki
 */
#pragma

#include <Arduino.h>
#include <Preferences.h>

/**
 * @brief Namespace for settings related definitions
 */
namespace SettingsNS
{
    /* Keys for reset counters */
    static constexpr const char* mKeyCounterResetPowerUp  = "CNT_RST_PWRON";
    static constexpr const char* mKeyCounterResetSW       = "CNT_RST_SW";
    static constexpr const char* mKeyCounterResetWdg      = "CNT_RST_WDG";
    static constexpr const char* mKeyCounterResetPanic    = "CNT_RST_PANIC";
    static constexpr const char* mKeyCounterResetBrownout = "CNT_RST_BRNOUT";

    class Settings
    {
    public:
        Settings();
        virtual ~Settings();

        void Clear(void);

        bool HasKey(const String& arKey);
        bool RemoveKey(const String& arKey);

        template<typename T> T GetValue(const String& arKey, T aDefaultValue);
        template<typename T> bool SetValue(const String& arKey, T aValue);

        bool GetBytes(const String& arKey, uint8_t* apData, size_t aDataSize);
        bool SetBytes(const String& arKey, const uint8_t* apData, size_t aDataSize);

        void IncreaseCounter(const String& arKey, uint32_t aNewValue = 0);
        uint32_t GetCounter(const String& arKey);

    private:
        static constexpr const char* mcPrefsName = "prefs";
        Preferences mPrefs;
    };

}   /* end of namespace SettingsNS */


/* Declare the object as extern for global access  */
extern SettingsNS::Settings Settings;
