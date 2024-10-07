#include <Arduino.h>
#include <M5ModuleDisplay.h>
#include <M5Unified.h>
#include "displayControl.hpp"

#ifndef __InitializeComponent
#define __InitializeComponent

extern unsigned char icon_Setting[2143];
extern unsigned char icon_Power[1624];
extern unsigned char icon_Right[1407];
extern unsigned char icon_Left[1417];
extern unsigned char icon_Save[1624];

extern unsigned char icon_OK[1859];
extern unsigned char icon_CANCEL[2815];

extern unsigned char icon_RESET[1698];
extern unsigned char icon_QR[1661];

class form
{
public:
    int formWidth;
    int formHeight;

    bool formEnable;
    String formName;
    String valueString;
    M5Canvas Display_Main_Canvas;

    form()
    {
        formName = "BaseClass";
        formEnable = true;
    }

    form(M5Canvas formCanvas, int value)
    {
        formName = "BaseClass";
        formEnable = true;

        Display_Main_Canvas = formCanvas;
        formWidth = Display_Main_Canvas.width();
        formHeight = Display_Main_Canvas.height();
    }

    virtual void draw(float value, String text)
    {
    }

    virtual int touchCheck(m5::touch_detail_t t)
    {
        return -1;
    }
};

// =====================
// form_Top
// =====================
class form_Top : public form
{
private:
    displayButton BTN_Power;
    displayButton BTN_Config;
    displayButton BTN_RESET;
    displayButton BTN_QR;

public:
    form_Top();
    form_Top(M5Canvas formCanvas, int value);
    void draw(float value, String text) override;
    int touchCheck(m5::touch_detail_t t) override;

    enum touchCheckResult
    {
        PowerOff,
        Config,
        Reset,
        QRshow
    };
};

// =====================
// form_ShutdownMessage
// =====================

class form_ShutdownMessage : public form
{
private:
    displayButton BTN_OK;
    displayButton BTN_CANCEL;

public:
    form_ShutdownMessage();
    form_ShutdownMessage(M5Canvas formCanvas, int value);
    void draw(float value, String text);
    int touchCheck(m5::touch_detail_t t);

    enum touchCheckResult
    {
        OK,
        CANCEL
    };
};

// =====================
// form_SaveMessage
// =====================
class form_SaveMessage : public form
{
private:
    displayButton BTN_OK;
    displayButton BTN_CANCEL;

public:
    form_SaveMessage();
    form_SaveMessage(M5Canvas formCanvas, int value);
    void draw(float value, String text);
    int touchCheck(m5::touch_detail_t t);

    enum touchCheckResult
    {
        OK,
        CANCEL
    };
};

// =====================
// form_QR
// =====================
class form_QR : public form
{
private:
    displayButton BTN_OK;
    //    displayButton BTN_CANCEL;

public:
    form_QR();

    /// @brief Initialize Canvas / Button
    form_QR(M5Canvas formCanvas, int value);
    void draw(float value, String text);
    int touchCheck(m5::touch_detail_t t);

    enum touchCheckResult
    {
        OK
    };
};

// =====================
// form_Config
// =====================
class form_Config : public form
{
private:
public:
    displayButton BTN_OK;
    displayButton BTN_CANCEL;

    int Enc_PPR;
    int Enc_TargetLength;
    int Enc_LPR;

    int Enc_PPR_MAX;
    int Enc_TargetLength_MAX;
    int Enc_LPR_MAX;

    int Enc_PPR_MIN;
    int Enc_TargetLength_MIN;
    int Enc_LPR_MIN;

    displayButtonLabel BTN_PPR;
    displayButtonLabel BTN_TargetLength;
    displayButtonLabel BTN_LPR;

    displaySlider SLD_value;

    displayButton BTN_UP;
    displayButton BTN_DOWN;

    form_Config();
    form_Config(M5Canvas formCanvas, int value);
    String getModeTitle();
    String getModeValue();
    void setModeValue(int value);
    void ValueRefreshFromText(String text);

    /// @brief
    /// @param value
    /// @param text Format: Enc_PPR'\t'Enc_TargetLength'\t'Enc_LPR'\t'viewFactor
    void draw(float value, String text);

    /// @brief
    enum touchCheckResult
    {
        OK,
        CANCEL,
        PPR,
        TargetLength,
        LPR,
        UP,
        DOWN,
        valueChange
    };

    /// @brief
    /// @param t
    /// @return
    int touchCheck(m5::touch_detail_t t) override;
};

#endif