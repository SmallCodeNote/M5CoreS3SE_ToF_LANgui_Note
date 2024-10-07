#include <Arduino.h>
#include <M5ModuleDisplay.h>
#include <M5Unified.h>
#include "displayControl.hpp"
#include "forms.hpp"

String getStringSplit(String str, char separator, int index)
{
    int len = str.length();
    int startPosition = 0;

    int separatorCount = 0;
    for (; startPosition < len; startPosition++)
    {
        if (separatorCount == index)
            break;

        if (str[startPosition] == separator)
            separatorCount++;
    }

    int endPosition = str.indexOf(separator, startPosition);

    if (!endPosition)
        endPosition = len - 1;

    return str.substring(startPosition, endPosition);
}

// =====================
// form_Top
// =====================
form_Top::form_Top()
{
    formName = "Top";
    formEnable = true;
}
/// @brief Initialize Canvas / Button
form_Top::form_Top(M5Canvas formCanvas, int value)
{
    formName = "Top";
    formEnable = true;

    Display_Main_Canvas = formCanvas;
    formWidth = Display_Main_Canvas.width();
    formHeight = Display_Main_Canvas.height();

    // Config ICON Button ===================
    BTN_Config = displayButton();
    BTN_Config.x = 174;
    BTN_Config.y = 187;
    BTN_Config.width = 52;
    BTN_Config.height = 52;
    BTN_Config.iconData = icon_Setting;

    // Power ICON Button ===================
    BTN_Power = displayButton();
    BTN_Power.x = 249;
    BTN_Power.y = 187;
    BTN_Power.width = 52;
    BTN_Power.height = 52;
    BTN_Power.iconData = icon_Power;
    // ================================

    // RESET ICON Button ===================
    BTN_RESET = displayButton();
    BTN_RESET.x = 249;
    BTN_RESET.y = 0;
    BTN_RESET.width = 52;
    BTN_RESET.height = 52;
    BTN_RESET.iconData = icon_RESET;
    // ================================

    // QR ICON Button ===================
    BTN_QR = displayButton();
    BTN_QR.x = 174;
    BTN_QR.y = 0;
    BTN_QR.width = 52;
    BTN_QR.height = 52;
    BTN_QR.iconData = icon_QR;
    // ================================
}

void form_Top::draw(float value, String text)
{
    String LastCount = getStringSplit(text, '\t', 0);
    String BatteryInfo_Level = getStringSplit(text, '\t', 1);

    if (formEnable)
    {
        Display_Main_Canvas.fillScreen(BLACK);
        BTN_Config.draw(Display_Main_Canvas);
        BTN_Power.draw(Display_Main_Canvas);
        BTN_RESET.draw(Display_Main_Canvas);
        BTN_QR.draw(Display_Main_Canvas);

        Display_Main_Canvas.setCursor(30, 80);
        if (value < 0)
        {
            Display_Main_Canvas.setFont(&fonts::Font8);
            Display_Main_Canvas.printf("%.3f", -value);
        }
        else
        {
            Display_Main_Canvas.setFont(&fonts::Font8);
            Display_Main_Canvas.printf("%.3f", value);
        }

        Display_Main_Canvas.setFont(&fonts::lgfxJapanGothic_24);
        Display_Main_Canvas.setTextColor(0xffd500);
        Display_Main_Canvas.drawString("[" + LastCount + "]", 2, 240 - 24 - 2);

        Display_Main_Canvas.setFont(&fonts::lgfxJapanGothic_24);
        Display_Main_Canvas.setTextColor(0xffd500);
        Display_Main_Canvas.drawString("[BAT=" + BatteryInfo_Level + "%]", 2, 2);

        Display_Main_Canvas.pushSprite(0, 0);
    }
}

int form_Top::touchCheck(m5::touch_detail_t t)
{
    if (formEnable)
    {
        if (BTN_Power.contain(t))
        {
            return PowerOff;
        }
        else if (BTN_Config.contain(t))
        {
            return Config;
        }
        else if (BTN_RESET.contain(t))
        {
            return Reset;
        }
        else if (BTN_QR.contain(t))
        {
            return QRshow;
        }
    }
    return -1;
}

// =====================
// form_ShutdownMessage
// =====================

form_ShutdownMessage::form_ShutdownMessage()
{
    formName = "ShutdownMessage";
    formEnable = true;
}
/// @brief Initialize Canvas / Button
form_ShutdownMessage::form_ShutdownMessage(M5Canvas formCanvas, int value)
{
    formName = "ShutdownMessage";
    formEnable = true;

    Display_Main_Canvas = formCanvas;
    formWidth = Display_Main_Canvas.width();
    formHeight = Display_Main_Canvas.height();

    // OK ICON Button ===================
    BTN_OK = displayButton();
    BTN_OK.x = 0;
    BTN_OK.y = 187;
    BTN_OK.width = 160;
    BTN_OK.height = 52;
    BTN_OK.iconData = icon_OK;
    BTN_OK.enable = true;

    BTN_CANCEL = displayButton();
    // CANCEL ICON Button ===================
    BTN_CANCEL.x = 160;
    BTN_CANCEL.y = 187;
    BTN_CANCEL.width = 160;
    BTN_CANCEL.height = 52;
    BTN_CANCEL.iconData = icon_CANCEL;
    BTN_CANCEL.enable = true;
    // ================================
}

void form_ShutdownMessage::draw(float value, String text)
{
    if (formEnable)
    {
        Display_Main_Canvas.fillScreen(BLACK);
        BTN_OK.draw(Display_Main_Canvas);
        BTN_CANCEL.draw(Display_Main_Canvas);

        Display_Main_Canvas.setFont(&fonts::lgfxJapanGothic_40);
        Display_Main_Canvas.setTextColor(0xffd500);
        Display_Main_Canvas.drawCenterString("Shutdown ?", formWidth / 2, 40);
        Display_Main_Canvas.pushSprite(0, 0);
    }
}

int form_ShutdownMessage::touchCheck(m5::touch_detail_t t)
{
    if (BTN_OK.contain(t))
    {
        return OK;
    }
    else if (BTN_CANCEL.contain(t))
    {
        return CANCEL;
    }
    return -1;
}

// =====================
// form_SaveMessage
// =====================

form_SaveMessage::form_SaveMessage()
{
    formName = "ShutdownMessage";
    formEnable = true;
}

/// @brief Initialize Canvas / Button
form_SaveMessage::form_SaveMessage(M5Canvas formCanvas, int value)
{
    formName = "SaveMessage";
    formEnable = true;

    Display_Main_Canvas = formCanvas;
    formWidth = Display_Main_Canvas.width();
    formHeight = Display_Main_Canvas.height();

    BTN_OK = displayButton();
    // OK ICON Button ===================
    BTN_OK.x = 0;
    BTN_OK.y = 187;
    BTN_OK.width = 160;
    BTN_OK.height = 52;
    BTN_OK.iconData = icon_OK;
    BTN_OK.enable = true;

    BTN_CANCEL = displayButton();
    // CANCEL ICON Button ===================
    BTN_CANCEL.x = 160;
    BTN_CANCEL.y = 187;
    BTN_CANCEL.width = 160;
    BTN_CANCEL.height = 52;
    BTN_CANCEL.iconData = icon_CANCEL;
    BTN_CANCEL.enable = true;
    // ================================
}

void form_SaveMessage::draw(float value, String text)
{
    if (formEnable)
    {
        Display_Main_Canvas.fillScreen(BLACK);
        BTN_OK.draw(Display_Main_Canvas);
        BTN_CANCEL.draw(Display_Main_Canvas);

        Display_Main_Canvas.setFont(&fonts::lgfxJapanGothic_40);
        Display_Main_Canvas.setTextColor(0xffd500);
        Display_Main_Canvas.drawCenterString("Save ?", formWidth / 2, 40);
        Display_Main_Canvas.pushSprite(0, 0);
    }
}

int form_SaveMessage::touchCheck(m5::touch_detail_t t)
{
    if (BTN_OK.contain(t))
    {
        return OK;
    }
    else if (BTN_CANCEL.contain(t))
    {
        return CANCEL;
    }
    return -1;
}

// =====================
// form_QR
// =====================
form_QR::form_QR()
{
    formName = "QR";
    formEnable = true;
}

/// @brief Initialize Canvas / Button
form_QR::form_QR(M5Canvas formCanvas, int value)
{
    formName = "QR";
    formEnable = true;

    Display_Main_Canvas = formCanvas;
    formWidth = Display_Main_Canvas.width();
    formHeight = Display_Main_Canvas.height();

    BTN_OK = displayButton();
    // OK ICON Button ===================
    BTN_OK.x = 160;
    BTN_OK.y = 187;
    BTN_OK.width = 160;
    BTN_OK.height = 52;
    BTN_OK.iconData = icon_OK;
    BTN_OK.enable = true;
    //=====================
}

void form_QR::draw(float value, String text)
{
    if (formEnable)
    {
        Display_Main_Canvas.fillScreen(BLACK);
        BTN_OK.draw(Display_Main_Canvas);
        Display_Main_Canvas.fillRect(0, 0, 320, 180, TFT_WHITE);
        Display_Main_Canvas.qrcode(text, 320 / 2 - 140 / 2, 20, 140, 4);

        Display_Main_Canvas.setFont(&fonts::lgfxJapanGothic_24);
        Display_Main_Canvas.setTextColor(0xffd500);
        Display_Main_Canvas.drawString("[" + text + "]", 2, 240 - 24 - 2);
        Display_Main_Canvas.pushSprite(0, 0);
    }
}

int form_QR::touchCheck(m5::touch_detail_t t)
{
    if (BTN_OK.contain(t))
    {
        return OK;
    }
    return -1;
}

// =====================
// form_Config
// =====================
form_Config ::form_Config()
{
    formName = "ShutdownMessage";
    formEnable = true;
}

/// @brief Initialize Canvas / Button
form_Config ::form_Config(M5Canvas formCanvas, int value)
{
    formName = "Config";
    formEnable = true;

    Display_Main_Canvas = formCanvas;
    formWidth = Display_Main_Canvas.width();
    formHeight = Display_Main_Canvas.height();

    // OK ICON Button ===================
    BTN_OK = displayButton();
    BTN_OK.x = 0;
    BTN_OK.y = 187;
    BTN_OK.width = 160;
    BTN_OK.height = 52;
    BTN_OK.iconData = icon_OK;
    BTN_OK.enable = true;

    // CANCEL ICON Button ===================
    BTN_CANCEL = displayButton();
    BTN_CANCEL.x = 160;
    BTN_CANCEL.y = 187;
    BTN_CANCEL.width = 160;
    BTN_CANCEL.height = 52;
    BTN_CANCEL.iconData = icon_CANCEL;
    BTN_CANCEL.enable = true;
    // ================================

    // PPRBTN_PPR ICON Button ===================
    BTN_PPR = displayButtonLabel();
    BTN_PPR.x = 0;
    BTN_PPR.y = 0;
    BTN_PPR.r = 12;
    BTN_PPR.width = 320 / 3;
    BTN_PPR.height = 48;
    BTN_PPR.label = "PPR";
    BTN_PPR.enable = false;
    // ================================
    // PPRBTN_TargetLength ICON Button ===================
    BTN_TargetLength = displayButtonLabel();
    BTN_TargetLength.x = 320 / 3;
    BTN_TargetLength.y = 0;
    BTN_TargetLength.r = 12;
    BTN_TargetLength.width = 320 / 3;
    BTN_TargetLength.height = 48;
    BTN_TargetLength.label = "TRGT";
    BTN_TargetLength.enable = true;
    // ================================
    // PPRBTN_LPR ICON Button ===================
    BTN_LPR = displayButtonLabel();
    BTN_LPR.x = 320 / 3 * 2;
    BTN_LPR.y = 0;
    BTN_LPR.r = 12;
    BTN_LPR.width = 320 / 3;
    BTN_LPR.height = 48;
    BTN_LPR.label = "LPR";
    BTN_LPR.enable = false;
    // ================================

    // SLD_value displaySlider===================
    SLD_value = displaySlider();
    SLD_value.x = 10;
    SLD_value.y = 120;
    SLD_value.width = 320 / 4 * 3;
    SLD_value.height = 48;
    SLD_value.enable = true;

    // OK ICON Button ===================
    BTN_UP = displayButton();
    BTN_UP.x = 320 - 52;
    BTN_UP.y = 52;
    BTN_UP.width = 52;
    BTN_UP.height = 52;
    BTN_UP.iconData = icon_Right;
    BTN_UP.enable = true;

    // CANCEL ICON Button ===================
    BTN_DOWN = displayButton();
    BTN_DOWN.x = 320 - 52;
    BTN_DOWN.y = 104;
    BTN_DOWN.width = 52;
    BTN_DOWN.height = 52;
    BTN_DOWN.iconData = icon_Left;
    BTN_DOWN.enable = true;
    // ================================
}

String form_Config ::getModeTitle()
{
    if (BTN_PPR.enable)
    {
        return "Pulses per Revolution";
    }
    else if (BTN_TargetLength.enable)
    {
        return "TargetLength";
    }
    else if (BTN_LPR.enable)
    {
        return "Length per Revolution";
    }

    return "";
}

String form_Config ::getModeValue()
{
    if (BTN_PPR.enable)
    {
        return String(Enc_PPR);
    }
    else if (BTN_TargetLength.enable)
    {
        return String(Enc_TargetLength);
    }
    else if (BTN_LPR.enable)
    {
        return String(Enc_LPR);
    }

    return "";
}

void form_Config ::setModeValue(int value)
{
    // Serial.println("setModeValue " + String(value));
    if (BTN_PPR.enable)
    {
        Enc_PPR = value;
    }
    else if (BTN_TargetLength.enable)
    {
        Enc_TargetLength = (value / 100) * 100;
    }
    else if (BTN_LPR.enable)
    {
        Enc_LPR = value;
    }
}

void form_Config ::ValueRefreshFromText(String text)
{

    String str_Enc_PPR = getStringSplit(text, '\t', 0);
    String str_Enc_TargetLength = getStringSplit(text, '\t', 1);
    String str_Enc_LPR = getStringSplit(text, '\t', 2);

    Serial.println(str_Enc_PPR);

    Enc_PPR = str_Enc_PPR.toInt();
    Enc_TargetLength = str_Enc_TargetLength.toInt();
    Enc_LPR = str_Enc_LPR.toInt();
}

/// @brief
/// @param value
/// @param text Format: Enc_PPR'\t'Enc_TargetLength'\t'Enc_LPR'\t'viewFactor
void form_Config ::draw(float value, String text)
{
    if (text.length() > 3)
    {
        ValueRefreshFromText(text);
    }

    if (formEnable)
    {
        Display_Main_Canvas.fillScreen(BLACK);
        BTN_OK.draw(Display_Main_Canvas);
        BTN_CANCEL.draw(Display_Main_Canvas);
        BTN_PPR.draw(Display_Main_Canvas);
        BTN_TargetLength.draw(Display_Main_Canvas);
        BTN_LPR.draw(Display_Main_Canvas);

        BTN_UP.draw(Display_Main_Canvas);
        BTN_DOWN.draw(Display_Main_Canvas);

        // BTN_viewFactor.draw(Display_Main_Canvas);

        if (BTN_PPR.enable)
        {
            SLD_value.setValueMin(Enc_PPR_MIN);
            SLD_value.setValueMax(Enc_PPR_MAX);
            SLD_value.setValue(Enc_PPR);
        }
        else if (BTN_TargetLength.enable)
        {
            SLD_value.setValueMin(Enc_TargetLength_MIN);
            SLD_value.setValueMax(Enc_TargetLength_MAX);
            SLD_value.setValue(Enc_TargetLength);
        }
        else if (BTN_LPR.enable)
        {
            SLD_value.setValueMin(Enc_LPR_MIN);
            SLD_value.setValueMax(Enc_LPR_MAX);
            SLD_value.setValue(Enc_LPR);
        }

        Serial.println("form_Config::draw value= " + String(SLD_value.value) + "/max= " + String(SLD_value.valueMax) + "/min= " + String(SLD_value.valueMin));
        SLD_value.draw(Display_Main_Canvas);

        Display_Main_Canvas.setFont(&fonts::lgfxJapanGothic_24);
        Display_Main_Canvas.setTextColor(0xffd500);

        int textLineIndex = 0;
        int textLineXoffset = 10;
        int textLineStartHeight = 50;
        int textLineHeight = 24;

        Display_Main_Canvas.drawString(getModeTitle(), textLineXoffset, textLineHeight * textLineIndex + textLineStartHeight);
        textLineIndex++;
        Display_Main_Canvas.drawString(getModeValue(), textLineXoffset, textLineHeight * textLineIndex + textLineStartHeight);
        textLineIndex++;

        Display_Main_Canvas.pushSprite(0, 0);
    }
}

/// @brief
/// @param t
/// @return
int form_Config ::touchCheck(m5::touch_detail_t t)
{
    if (BTN_OK.contain(t))
    {
        return OK;
    }
    else if (BTN_CANCEL.contain(t))
    {
        return CANCEL;
    }
    else if (BTN_PPR.contain(t))
    {
        return PPR;
    }
    else if (BTN_TargetLength.contain(t))
    {
        return TargetLength;
    }
    else if (BTN_LPR.contain(t))
    {
        return LPR;
    }
    else if (BTN_UP.contain(t))
    {
        return UP;
    }
    else if (BTN_DOWN.contain(t))
    {
        return DOWN;
    }
    else if (SLD_value.contain(t))
    {
        setModeValue(SLD_value.value);
        return valueChange;
    }
    return -1;
}