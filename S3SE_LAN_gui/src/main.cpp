#include <Arduino.h>
#include <M5Unified.h>
#include <EEPROM.h>
#include <Wire.h>
#include <VL53L1X.h>

#include "displayControl.hpp"
#include "forms.hpp"

form *FormView;
form_Top Form_Top;
form_ShutdownMessage Form_ShutdownMessage;
form_SaveMessage Form_SaveMessage;
form_Config Form_Config;
form_QR Form_QR;

/// @brief signal pin assign A-Phase
VL53L1X tofSensor;

/// @brief Encorder Count
int Enc_Count = 0;
int Enc_CountLast = 0;

static m5::touch_state_t prev_state;
static constexpr const char *state_name[16] =
    {"none", "touch", "touch_end", "touch_begin", "___", "hold", "hold_end", "hold_begin", "___", "flick", "flick_end", "flick_begin", "___", "drag", "drag_end", "drag_begin"};

int prev_x = 0;
int prev_y = 0;

/// @brief QRcode text
String QRcodeText = "";

/// @brief ConfigForm Up/Down Step
int ConfigStep = 100;

/// @brief BatteryLevelValueBuff
int BatteryLevelValue_last = 0;

/// @brief Encorder Profile Struct
struct DATA_SET
{
  /// @brief Encorder resolustion
  int Enc_PPR;

  /// @brief WheelDiameter[mm]
  int Enc_LPR;

  /// @brief TargetLength[mm]
  int Enc_TargetLength;
};
/// @brief Encorder Profile
DATA_SET data;

/// @brief Main Display
M5GFX Display_Main;
M5Canvas Display_Main_Canvas(&Display_Main);

void taskGetEncoder(void *pvParameters)
{
  while (1)
  {
    Enc_Count = tofSensor.read();
  }
}

void LoadEEPROM()
{
  EEPROM.begin(50); // 50byte
  EEPROM.get<DATA_SET>(0, data);

  if (data.Enc_PPR <= 0)
  {
    data.Enc_PPR = 1000;
    data.Enc_TargetLength = 6000;
    data.Enc_LPR = 1;
  }
}

void SetupTofSensor()
{
  M5.Ex_I2C.begin();
  tofSensor.setBus(&Wire);
  tofSensor.setTimeout(500);

  if (!tofSensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1)
      ;
  }

  // Use long distance mode and allow up to 50000 us (50 ms) for a
  // measurement. You can change these settings to adjust the performance of
  // the sensor, but the minimum timing budget is 20 ms for short distance
  // mode and 33 ms for medium and long distance modes. See the VL53L1X
  // datasheet for more information on range and timing limits.
  tofSensor.setDistanceMode(VL53L1X::Long);
  tofSensor.setMeasurementTimingBudget(50000);

  // Start continuous readings at a rate of one measurement every 50 ms (the
  // inter-measurement period). This period should be at least as long as the
  // timing budget.
  tofSensor.startContinuous(50);
}

void InitializeDisplay()
{
  M5.Lcd.printf("Start MainMonitor Initialize\r\n"); // LCDに表示

  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setBrightness(50);

  // Main Display initialize
  M5.setPrimaryDisplayType(m5gfx::board_M5StackCoreS3SE);

  // M5GFX initialize
  Display_Main = M5.Display;
  int w = Display_Main.width();
  int h = Display_Main.height();

  // Create sprite for MainDisplay
  Display_Main_Canvas.createSprite(w, h);
  Display_Main_Canvas.setTextColor(0xffd500);
  Display_Main_Canvas.setFont(&fonts::lgfxJapanGothic_12);
  Display_Main_Canvas.setTextColor(0xffd500);
}

void M5Begin()
{
  auto cfg = M5.config();
  cfg.serial_baudrate = 19200;
  M5.begin(cfg);
}

void setup()
{
  M5Begin();
  LoadEEPROM();
  SetupTofSensor();
  InitializeDisplay();
  xTaskCreatePinnedToCore(taskGetEncoder, "Task0", 4096, NULL, 1, NULL, 0); // start task
  disableCore0WDT();                                                        // stop watch dog timer : core0

  Form_Top = form_Top(Display_Main_Canvas, 0);
  Form_ShutdownMessage = form_ShutdownMessage(Display_Main_Canvas, 0);
  Form_SaveMessage = form_SaveMessage(Display_Main_Canvas, 0);
  Form_Config = form_Config(Display_Main_Canvas, 0);
  Form_QR = form_QR(Display_Main_Canvas, 0);

  FormView = &Form_Top;
  FormView->formEnable = true;

  int BatteryLevelValue = M5.Power.getBatteryLevel();
  FormView->draw(0, "\t" + String(BatteryLevelValue));
}

void loop()
{
  M5.update();
  int BatteryLevelValue = M5.Power.getBatteryLevel();

  auto count = M5.Touch.getCount();
  if (count)
  {
    static m5::touch_state_t prev_state;
    auto t = M5.Touch.getDetail();
    if (prev_state != t.state)
    {
      prev_state = t.state;

      if (prev_x != t.x || prev_y != t.y)
      {
        prev_x = t.x;
        prev_y = t.y;

        int touchIndex = FormView->touchCheck(t);

        //================
        // Form_Top
        //================
        if (touchIndex >= 0 && FormView == &Form_Top)
        {
          switch (touchIndex)
          {
          case form_Top::touchCheckResult::PowerOff:
            FormView = &Form_ShutdownMessage;
            FormView->draw(0, "");

            break;

          case form_Top::touchCheckResult::Config:
            FormView = &Form_Config;
            Form_Config.Enc_LPR_MIN = data.Enc_LPR / 2;
            Form_Config.Enc_LPR_MAX = data.Enc_LPR * 2;
            Form_Config.Enc_PPR_MIN = data.Enc_LPR / 4;
            Form_Config.Enc_PPR_MAX = data.Enc_LPR * 4;
            Form_Config.Enc_TargetLength_MIN = (int)(data.Enc_TargetLength / 2.0);
            Form_Config.Enc_TargetLength_MAX = (int)(data.Enc_TargetLength * 2.0);
            FormView->draw(0, String(data.Enc_PPR) + "\t" + String(data.Enc_TargetLength) + "\t" + String(data.Enc_LPR));
            break;

          case form_Top::touchCheckResult::Reset:
            Enc_Count = 0;
            FormView->draw(0, "");
            break;

          case form_Top::touchCheckResult::QRshow:
            FormView = &Form_QR;
            QRcodeText = String(data.Enc_LPR * Enc_Count / data.Enc_PPR / 1000.0, 3U);
            FormView->draw(0, QRcodeText);
            break;

          default:
            FormView = &Form_Top;
            FormView->draw(0, "\t" + String(BatteryLevelValue));
          }
          touchIndex = 0;
        }
        else if (touchIndex >= 0 && FormView == &Form_QR)
        {
          switch (touchIndex)
          {
          case form_QR::touchCheckResult::OK:
            FormView = &Form_Top;
            Enc_CountLast = 0;
            FormView->draw(0, "\t" + String(BatteryLevelValue));

          default:
            break;
          }
          touchIndex = 0;
        }
        else if (touchIndex >= 0 && FormView == &Form_ShutdownMessage)
        {
          switch (touchIndex)
          {
          case form_ShutdownMessage::touchCheckResult::OK:
            M5.Power.powerOff();
            break;

          case form_ShutdownMessage::touchCheckResult::CANCEL:
            FormView = &Form_Top;
            FormView->draw(0, "\t" + String(BatteryLevelValue));
            break;

          default:
            FormView = &Form_Top;
            FormView->draw(0, "\t" + String(BatteryLevelValue));
          }

          touchIndex = 0;
        }
        else if (touchIndex >= 0 && FormView == &Form_SaveMessage)
        {
          switch (touchIndex)
          {
          case form_SaveMessage::touchCheckResult::OK:
            data.Enc_LPR = Form_Config.Enc_LPR;
            data.Enc_PPR = Form_Config.Enc_PPR;
            data.Enc_TargetLength = Form_Config.Enc_TargetLength;

            EEPROM.put<DATA_SET>(0, data);
            EEPROM.commit();

          case form_SaveMessage::touchCheckResult::CANCEL:
            FormView = &Form_Top;
            FormView->draw(0, "\t" + String(BatteryLevelValue));
            break;

          default:
            FormView = &Form_Top;
            FormView->draw(0, "\t" + String(BatteryLevelValue));
          }
          touchIndex = 0;
        }
        else if (touchIndex >= 0 && FormView == &Form_Config)
        {
          switch (touchIndex)
          {
          case form_Config::touchCheckResult::OK:
            FormView = &Form_SaveMessage;
            FormView->draw(0, "");
            break;

          case form_Config::touchCheckResult::CANCEL:
            FormView = &Form_Top;
            FormView->draw(0, "\t" + String(BatteryLevelValue));
            break;

          case form_Config::touchCheckResult::PPR:
            Form_Config.BTN_PPR.enable = true;
            Form_Config.BTN_TargetLength.enable = false;
            Form_Config.BTN_LPR.enable = false;
            ConfigStep = 1;
            FormView->draw(0, String(data.Enc_PPR) + "\t" + String(data.Enc_TargetLength) + "\t" + String(data.Enc_LPR));
            break;

          case form_Config::touchCheckResult::TargetLength:
            Form_Config.BTN_PPR.enable = false;
            Form_Config.BTN_TargetLength.enable = true;
            Form_Config.BTN_LPR.enable = false;
            ConfigStep = 100;
            FormView->draw(0, String(data.Enc_PPR) + "\t" + String(data.Enc_TargetLength) + "\t" + String(data.Enc_LPR));
            break;

          case form_Config::touchCheckResult::LPR:
            Form_Config.BTN_PPR.enable = false;
            Form_Config.BTN_TargetLength.enable = false;
            Form_Config.BTN_LPR.enable = true;
            ConfigStep = 1;
            FormView->draw(0, String(data.Enc_PPR) + "\t" + String(data.Enc_TargetLength) + "\t" + String(data.Enc_LPR));
            break;

          case form_Config::touchCheckResult::valueChange:
            data.Enc_LPR = Form_Config.Enc_LPR;
            data.Enc_PPR = Form_Config.Enc_PPR;
            data.Enc_TargetLength = Form_Config.Enc_TargetLength;
            FormView->draw(0, String(data.Enc_PPR) + "\t" + String(data.Enc_TargetLength) + "\t" + String(data.Enc_LPR));
            break;

          case form_Config::touchCheckResult::UP:
            Serial.println("form_Config::touchCheckResult::UP: " + String((Form_Config.getModeValue()).toInt() + ConfigStep));
            Form_Config.setModeValue((Form_Config.getModeValue()).toInt() + ConfigStep);
            FormView->draw(0, "");
            break;

          case form_Config::touchCheckResult::DOWN:
            Form_Config.setModeValue((Form_Config.getModeValue()).toInt() - ConfigStep);
            FormView->draw(0, "");
            break;

          default:
            FormView->draw(0, String(data.Enc_PPR) + "\t" + String(data.Enc_TargetLength) + "\t" + String(data.Enc_LPR));
          }

          touchIndex = 0;
        }
      }
    }
  }

  if (FormView == &Form_Top && (Enc_CountLast != Enc_Count || BatteryLevelValue_last != BatteryLevelValue))
  {
    Enc_CountLast = Enc_Count;
    float countValue = data.Enc_LPR * Enc_Count / data.Enc_PPR;
    int RemainTime = (data.Enc_TargetLength - abs(countValue)) / data.Enc_LPR;

    String Message = "";
    if (data.Enc_TargetLength > 0)
    {
      Message = "Last " + String(RemainTime);
    }

    FormView->draw(countValue / 1000.0, Message + "\t" + String(BatteryLevelValue));
    Serial.print(".");
  }

  BatteryLevelValue_last = BatteryLevelValue;
  delay(50);
}