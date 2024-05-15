/* r_action.ino
	Johnny • V
	https://www.youtube.com/watch?v=txVRw637OI8
   ---------------------------------------
   Used Pins
   ---------------------------------------
*/
#ifdef USE_ROBOT_007

// https://forum.arduino.cc/t/servo-read-and-servo-write-help/137733/12
/* SOUND
  https://роботехника18.рф/%D0%BF%D1%8C%D0%B5%D0%B7%D0%BE%D0%BF%D0%B8%D1%89%D0%B0%D0%BB%D0%BA%D0%B0-%D0%B0%D1%80%D0%B4%D1%83%D0%B8%D0%BD%D0%BE/
*/

/**
  Made with Marlin Bitmap Converter
  https://marlinfw.org/tools/u8glib/converter.html
*/

/* ROBOT --------- GPIO -
   MB_LED_PIN ..D0 (16)
   SOUND_PIN ...D1 (05)
   BTN PIN .....D2 (04)
   LED DIN .....D3 (00)
   DS18B20 .....D4 (02)
   MOSFET ......D5 (14)
   SERVO_1 .....D6 (12)
   SERVO_HEAD .....D8 (15)

   MOTION_PIN..SD2 (09)
   ............SD3 (10)
*/

// -----------------------

uint32_t activityTimer;
int s_temp;
int s_hum;
bool isPlaying = false;
byte activity;
const byte SHOW_DELAY = 16;

// -----------------------
// ======================================
// servo motors
// ======================================
void initRobotPin() {
  //  LOG.print ("initRobotPin • ");
  showIcon(0, 500);
  // INPUT_PULLUP

  pinMode(analogInPin, INPUT);  /* sensor light   */
  pinMode(MOTION_PIN, INPUT);   /* sensor motion  */

  /*
    pinMode(DHTPIN, INPUT);       // sensor DTH
    pinMode(MB_LED_PIN, OUTPUT);
    digitalWrite(MB_LED_PIN, LOW);
  */

  /* init servo */
  pinMode(MOTION_PIN, INPUT);
  servoHead.attach(SERVO_HEAD, 90);
  servoHead.setSpeed(60);            // limit speed
  servoHead.setAccel(0.3);           // set acceleration (acceleration and braking)
  servoHead.smoothStart();
}

// ======================================
void  prepareUpdate() {
  ONflag = false;
  changePower();
  showLogo(0, 1000);
  runScenario(129U);
  dress_state = 0U; eyes_state = 9U;
}

// ======================================
void showActivityLevel(byte level) {
  /* extCtrl • CRL_SOUND = 2 */
  byte val_level = level * 0.35;
  if (extCtrl == 0U) {
    display.drawFastHLine(0, 47, display.width(), BLACK);
    if (val_level > 0 & !eff_auto) {
      display.drawFastHLine((display.width() - val_level) / 2, 47, val_level, WHITE);
    }
    // LOG.printf_P(PSTR("• %3d | val_level %3d\n\r"), action_timeout, val_level);
  }
}

// ======================================
void lightControls() {
  uint16_t analog_val = analogRead(analogInPin);
  if (analog_val < 32U) {
    night = true;
  }
  if (analog_val > 80U) {
    night = false;
  }
  if (!dawnFlag) {
    if ( night & !ONflag) {
      for (uint8_t i = modes[currentMode].Brightness; i > 5; i = constrain(i - 8, 5, modes[currentMode].Brightness)) {
        FastLED.setBrightness(i);
        FastLED.delay(1);
      }
      // LOG.println("• Set Low Brightness");
    } else { /* default */
      setBrightnessLevel();
    }
  }
}

// ======================================
void runRobotAction() {
  static bool motion;
  const byte TIMEOUT = 160U;
  /* servo controlls -------- */
  servoHead.tick();
  if (intro) {
    Introduction();
    runScenario(1);
    return;
  }

  if ((extCtrl > 0U) | (scenario == 129U)) return;

  if (millis() - activityTimer >= 30) {
    activityTimer = millis();
    action_timeout++;
    if (activity == TIMEOUT - 2) showActualParams();

    //    if (action_timeout % 10 == 0U | activity == 1U ) LOG.printf_P(PSTR("• %3d | Scenario %3d | sleep_mode %d | activity %3d |\n\r"), action_timeout, lastScenario, sleep_mode, activity);


    if (activity == 1U) {
      // LOG.printf_P(PSTR("• %3d | Scenario %3d | sleep_mode %d | activity %3d | s_timeout •%2d \n\r"), action_timeout, lastScenario, sleep_mode, activity, sound_timeout);
      activity = 0;
      sleep_mode = true;
      runScenario(ONflag ? 1 : 0);
      display.clearDisplay();
    }

    if (activity == 0U) {
      if (action_timeout > 25U) motion = digitalRead(MOTION_PIN);
      if (motion ) {
        /* sensor motion on */
        runScenario(2U);
        activity = TIMEOUT;
        sleep_mode = false;
        SoundGreetings(sound_timeout);
        action_timeout = 5U;
        // activation IOT Lithouse ----------
        SendCommandToGroup("192.168.1.50", 1, night, floor(s_temp));
      } else {
        display.clearDisplay();
        // if (sound_timeout % 5 == 0)
        // showParam(1, "TIMEOUT", String(sound_timeout), "");
      }
    }

    /* alarms & timer off icon */
    if (dawnFlag) {
      showIcon(4, 5);
    } else {
      if (TimerManager::TimerRunning) {
        showIcon(2, 5);
      } else {
        showActivityLevel(activity);
      }
    }

    if (activity > 1U) activity--;
    /* show sleep icon */
    if (!ONflag & (activity == 2U)) showIcon(15, 1000);

    /* debug */
    // showDebugMsg("AT.. " + String(action_timeout), "ST... " + String(sound_timeout), 1);
    /*
      //    showDebugMsg("T.. " + String(action_timeout), "A... " + String(activity), 1);
      //    showProgress();

      if (action_timeout % 8U == 0U) {
      LOG.printf_P(PSTR("• %3d | Scenario %3d | sleep_mode %d | activity %3d | isSoundPlay •%2d \n\r"), action_timeout, lastScenario, sleep_mode, activity, isSoundPlay);
      } */

    /* read dht sensor -------- */
    if (action_timeout == 10U) readSensorDHT();

    /* read light sensor ------ */
    if (action_timeout == 20U) lightControls();


    /* test sound
      if (sound_timeout < 250) sound_timeout++;
      if (sound_timeout == 250) {
      action_timeout = 0;
      showIcon(9, 1);
      EnvironmentMsg(s_temp);
      sound_timeout++;
      }
    */

    if (action_timeout == 100U) {
      // activation IOT Lithouse ----------
      SendCommandToGroup("192.168.1.50", 0, night, floor(s_temp));
      if (sound_timeout < 250) sound_timeout++;
      if (sound_timeout == 250) {
        showIcon(9, 1);
        EnvironmentMsg(s_temp);
        sound_timeout++;
      }
      LOG.printf_P(PSTR("• | Scenario %2d | sleep %d | activity %3d | sound_timeout •%3d \n\r"), lastScenario, sleep_mode, activity, sound_timeout);
    }

    if (action_timeout == 110U) {
      action_timeout = 0;
    }

    /* run multi scenarios ---- */
    if (action_timeout == 250U) {
      if (scenarios) {
        runScenario(scenario + 1);
      } else {
        runScenario(1);
      }
    }
    display.display();
  }
}

// ======================================
void startMultiScenarios() {
  scenarios = true;
  action_timeout = 230U;
}

// ======================================
void Scenario() {
  if (lastScenario == scenario) {
    return;
  }

  if (extCtrl > 0) scenario = 1;

  switch (scenario) {
    case 0:   /* off */
      dress_state = 0U; eyes_state = 0U;
      scenarios = false;
      break;
    case 1:   /* onn */
      dress_state = 1U; eyes_state = 1U;
      scenarios = false;
      HeadRotation(90U);
      break;

    case 2:   /* find target */
      dress_state = 1U; eyes_state = 1U;
      HeadRotation(random8(40U, 140U));
      startMultiScenarios();
      // HeadRotation(random8(80U, 100U));
      break;
    case 3:    /* end find target */
      dress_state = 1U; eyes_state = 1U;
      scenarios = false;
      HeadRotation(90U);
      break;

    case 4:   /* fire */
      dress_state = 1U; eyes_state = 2U;
      scenarios = false;
      break;
    /* ------------- */

    /* high activity */
    case 5:
      dress_state = 3U; eyes_state = 3U;
      HeadRotation(random8(20U, 160U));
      startMultiScenarios();
      break;
    case 6:
      dress_state = 3U; eyes_state = 3U;
      HeadRotation(85 + random8(10U));
      scenarios = false;
      //      startMultiScenarios();
      break;

    /* ------------- */
    case 10:  /* deep dream */
      dress_state = 1U; eyes_state = 9U;
      FPSdelay = 128;
      scenarios = false;
      break;
    case 15:  /* deep dream */
      dress_state = 5U; eyes_state = 9U;
      break;
    /* ------------- */
    case 128:   /* prepare ota mode */
      dress_state = 0U; eyes_state = 1U;
      HeadRotation(0);
      startMultiScenarios();
      break;
    case 129:   /* ota mode */
      dress_state = 0U; eyes_state = 9U;
      FPSdelay = 50;
      scenarios = false;
      break;
    /* ------------- */

    default:
      break;
  }
  lastScenario = scenario;

#ifdef GENERAL_DEBUG
  LOG.printf_P(PSTR("• %3d | Go Scenario •%2d | scenarios %3d | head •%3d \n\r"), action_timeout, lastScenario, scenarios,  headAngle);
#endif
}

// ======================================
void drawLabel(byte activity) {
  CRGB curCol =  CHSV(128 + activity, 200U, 255U);
  drawPixelXY(CENTER_X_MINOR - 3, HEIGHT - 2, curCol);
  drawPixelXY(CENTER_X_MINOR - 2, HEIGHT - 2, curCol);
  drawPixelXY(CENTER_X_MINOR - 3, HEIGHT - 3, curCol);
  drawPixelXY(CENTER_X_MINOR - 2, HEIGHT - 3, curCol);
}

// ======================================
void drawDress() {
  if (dawnFlag) return;

  const byte BR_STEP = 24U;
  byte dress_color = 96U;
  byte collar_color = 255U;

  if (isNight() | (scenario == 0U)) {
    /* fade dress */
    if (!ONflag & !dawnFlag ) {
      fadeToBlackBy(leds, NUM_LEDS, 64U);
      FastLED.show();
    }
  } else {

    if (s_temp < 20) {
      collar_color = 255U;
      dress_color = 245U;
    }

    if (s_temp > 23) {
      dress_color = 160U;
      collar_color = 32U;
    }

    if (!ONflag & (extCtrl == 0U)) {
      for (int8_t y = 0; y < HEIGHT; y++) {
        for (int8_t x = 0; x < WIDTH; x++) {
          if (y == (HEIGHT - 1) | ((x == CENTER_X_MINOR) & (y > HEIGHT - 4))) {
            leds[XY(x, y)] = CHSV(collar_color, ((collar_color == 255) ? 0U : 255U), 160U);
          } else {
            leds[XY(x, y)] = CHSV(dress_color, 255U, 180U - y * BR_STEP);
          }
        }
      }
      leds[XY( CENTER_X_MINOR, HEIGHT - 1)] = CRGB::Black;
      drawLabel(activity);
      if (!dawnFlag) FastLED.show();
    }
  }
}

// ======================================
void RobotLedEffects(byte flagDress, byte flagEyes) {
  static byte eyes_delay;
  CRGB color_l;   /* right eye */
  CRGB color_r;   /* left  eye */

  color_l = night ? CRGB::Green : CRGB::White;
  color_r = night ? CRGB::Green : CRGB::White;

  switch (flagEyes) {
    case 0:   /* sleep */
      color_l = CRGB::Black;
      color_r = CRGB::Black;
      break;
    case 1:   /* blinking or wink*/
      EVERY_N_SECONDS(6) {
        if (random(8) < 6U) {
          leds[1] = CRGB::Black;    // right eye
          leds[0] = CRGB::Black;    // left  eye
          FastLED.show();
          eyes_delay = 5; //25;
        } else {
          leds[1] = CRGB::Black;    // right eye
          leds[0] = color_l;        // left  eye
          FastLED.show();
          eyes_delay = 15; //75;
        }
      }
      break;
    case 2:   /* shooting */
      color_l = CRGB::Black;        // right eye
      break;
    case 3:   /* infrared */
      leds[1] =  CHSV( 245U, 255U, 200U);  // right eye
      leds[0] =  CHSV( 245U, 255U, 200U);  // left  eye
      FastLED.show();
      eyes_delay = 15; //75;
      break;
    case 9:   /* ota */
      deltaValue ++; /* ota only */
      color_l = CHSV( 245U, 255U, abs(180U - deltaValue ));
      color_r = CRGB::Black;          // right eye
      break;
    default:
      color_l = CRGB::Magenta;
      color_r = CHSV(190, 255U, 255);
      break;
  }

  /* затримка для підмигування та моргання очима */
  if (eyes_delay == 0U) {
    leds[1] = color_r;    // right eye
    leds[0] = color_l;    // left  eye
  } else {
    if ((eyes_delay > 0) & (eyes_delay < 255)) eyes_delay--;
    // LOG.printf_P(PSTR("• %3d | Scenario •%3d | eyes_delay •%3d |\n\r"), action_timeout, lastScenario, eyes_delay);
  }

  /* draw dress -------------- */
  drawDress();
}

// ======================================
void showWiFiLogo() {
  display.clearDisplay();
  display.drawBitmap(16, 8, logo_wifi16, 32, 16, 1);
  display.display();
  delay(1000);
}

// ======================================
void animeWiFiLogo(bool esp_mode) {
  display.clearDisplay();
  display.setCursor(0, 36);
  printlnTextCenter(esp_mode ? "CONNECT" : "AP MODE", 1, WHITE);
  display.display();
  for (int16_t y = 0; y < 4; y ++) {
    for (int16_t i = 5; i < 20; i += 2) {
      display.drawCircle(display.width() / 2, 16, i - 4, BLACK);
      if (i < 17) display.drawCircle(display.width() / 2, 16, i, WHITE);
      if (esp_mode) {
        display.fillRect(16, 7, 32, 18, BLACK);
        display.drawBitmap(16, 8, logo_wifi16, 32, 16, 1);
      } else {
        display.fillRect(27, 6, 10, 20, BLACK);
        display.drawBitmap(28, 7, logo_ap, 8, 18, 1);
      }
      display.display();
      delay(25);
    }
  }
}

// ======================================
void showIcon(byte icon_id, uint16_t delay_interval) {
  const byte POS_Y = 0;
  display.clearDisplay();
  //  LOG.printf_P(PSTR("• %3d | Scenario •%2d | icon_id •%2d |\n\r"), action_timeout, lastScenario, icon_id);
  switch (icon_id) {
    case 0:   /* robot */
      display.drawBitmap(16, POS_Y, icon_robot, 32, 32, 1);
      break;
    case 1:   /* lamp */
      display.drawBitmap(16, POS_Y, icon_lamp, 32, 32, 1);
      break;
    case 2:   /* timer */
      display.drawBitmap(16, POS_Y, icon_wait, 32, 32, 1);
      break;
    case 3:   /* cycle */
      display.drawBitmap(16, POS_Y, icon_cycle, 32, 32, 1);
      display.setCursor(0, 32);
      printlnTextCenter("| " + String(currentMode) + " |", 1, WHITE);
      break;
    case 4:   /* alarm */
      display.drawBitmap(16, POS_Y, icon_alarm, 32, 32, 1);
      break;
    case 5:   /* media */
      display.drawBitmap(16, POS_Y, icon_media, 32, 32, 1);
      break;
    case 6:   /* cpu */
      display.drawBitmap(16, POS_Y, icon_cpu, 32, 32, 1);
      break;
    case 7:   /* gif */
      display.drawBitmap(16, POS_Y, icon_gif, 32, 32, 1);
      break;
    case 8:   /* debug */
      display.drawBitmap(16, POS_Y, icon_debug, 32, 32, 1);
      break;
    case 9:   /* temperature */
      display.drawBitmap(16, POS_Y, icon_t, 32, 32, 1);
      break;
    case 10:   /* debug small */
      display.drawBitmap(20, POS_Y + 8, ico_debug, 24, 24, 1);
      break;
    default:  /* sleep */
      display.drawBitmap(16, POS_Y, icon_sleep, 32, 32, 1);
      break;
  }
  display.display();
  delay(delay_interval);
}

// ======================================
void showDebugMsg(String msg1, String msg2, uint16_t delay_interval) {
  const byte POS_Y = 0;
  display.clearDisplay();
  display.drawBitmap(20, POS_Y, ico_debug, 24, 24, 1);
  display.setCursor(0, 22);
  printlnTextCenter(String(msg1), 1, WHITE);
  printlnTextCenter(String(msg2), 1, WHITE);
  display.display();
  delay(delay_interval);
}

// ======================================
void showUsageCPU(byte val) {
  display.clearDisplay();
  const byte POS_Y = 0;
  display.drawBitmap(16, POS_Y, icon_cpu, 32, 32, 1);
  display.setCursor(0, 34);
  printlnTextCenter("Usage " + String(val) + "%", 1, WHITE);
  display.display();
}

// ======================================
void swapMode(byte val) {
  isServo = (val == 1);
  display.clearDisplay();
  const byte POS_Y = 0;
  display.drawBitmap(16, POS_Y, isServo ? icon_robot : icon_lamp, 32, 32, 1);
  display.setCursor(0, 34);
  String str = isServo ? "SERVO ON" : "SERVO OFF";
  printlnTextCenter(str, 1, WHITE);
  display.display();
}

// ======================================
void showLogo(byte logo_id, uint16_t delay_interval) {
  display.clearDisplay();
  switch (logo_id) {
    case 0:   /* brain */
      display.drawBitmap(8, 0, logo_brain, 48, 48, 1);
      break;
    case 1:   /* robot */
      display.drawBitmap(8, 0, logo_robot, 48, 48, 1);
      break;
  }
  display.display();
  delay(delay_interval);
}

// ======================================
String FomatXX(int val) {
  if (val < 10) {
    return "0" + String(val);
  } else {
    return String(val);
  }
}

// ======================================
void readSensorDHT() {
  switch (sensor.read()) {  // читаем показания датчика
    case DHT_OK:
      s_temp = (int) (sensor.tem * 10) / 10.f - 7;
      s_hum = (int) (sensor.hum * 10) / 10.f;
      // LOG.println((String) "DHT 11: " + s_hum + "% | " + s_temp + "*C");
      break;
    case DHT_ERROR_CHECKSUM:   LOG.println("DHT 11: HE PABEHCTBO KC");                     break;
    case DHT_ERROR_DATA:       LOG.println("DHT 11: OTBET HE COOTBETCTB. CEHCOPAM 'DHT'"); break;
    case DHT_ERROR_NO_REPLY:   LOG.println("DHT 11: HET OTBETA");                          break;
    default:                   LOG.println("DHT 11: ERROR");                               break;
  }
}

// ======================================
void showParam(byte PosY, String title, String val, String unit) {
  display.clearDisplay();
  display.setCursor(0, PosY);
  printlnTextCenter(title, 1, WHITE);
  printlnTextCenter(val + unit, 2, WHITE);
  display.display();
}

// ======================================
void showActualParams() {
  static byte idx;
  if ( extCtrl == 2U) return;
  if (dawnFlag | TimerManager::TimerRunning) return;
  switch (idx) {
    case 0:   /* time */
      showTime();
      break;
    case 1:   /* temp */
      showParam(0, "TEMP", String(s_temp), "\"C");
      break;
    case 2:   /* shumg */
      showParam(0, "HUMIDITY", String(s_hum), "%");
      break;
    //    case 3:
    //      showParam(1, "TIMEOUT", String(sound_timeout), "");
    //      break;
    default:
      idx = 0;
      showIcon(10, 1);
      // showIcon(ONflag, 1);
      // showTime();
      return;
  }
  idx++;
  // if (idx > 4)  idx = 0;
}

// ======================================
void showTime() {
  time_t currentLocalTime = getCurrentLocalTime();
  String str_time = FomatXX(hour(currentLocalTime)) + ":" + FomatXX(minute(currentLocalTime));

  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(4, 0);
  printlnTextCenter("TIME", 1, WHITE);
  display.setTextSize(2);
  display.setCursor(4, 20);
  display.println(str_time);
  display.display();
}

// ======================================
byte printlnTextCenter(String txt, byte font_size, byte color) {
  display.setTextSize(font_size);
  int pos = (63 - txt.length() * 5 * font_size) / 2;
  display.setCursor(pos, display.getCursorY() + 5);
  display.println(txt);
}

// ======================================
void showEffectNum() {
  if (eff_auto & ( extCtrl == 0U)) {
    showIcon(3, 1);
  } else {
    showParam(5, "EFFECT", String(currentMode), "");
  }
  SoundMute();
  // sleep_mode = false;
  action_timeout = 0;
  activity = 120;
}

// ======================================
void showPlayerCommand(String cmd) {
  showIcon(5, 1);
  display.setCursor(0, 34);
  printlnTextCenter(cmd, 1, WHITE);
  delay(1000);

  sleep_mode = false;
  action_timeout = 95;
  // activity = SHOW_DELAY;
}

// ======================================
void showBrightnessVal() {
  showParam(5, "BRIGHT", String( (byte) (modes[currentMode].Brightness / 2.55)), "");
}

// ======================================
void showSpeedVal() {
  showParam(5, "SPEED", String(modes[currentMode].Speed), "");
}

// ======================================
void showScaleVal() {
  showParam(5, "SCALE", String(modes[currentMode].Scale), "%");
}

// ======================================
void showVersion() {
  // Clear the buffer.
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  printlnTextCenter("VERSION", 1, WHITE);
  printlnTextCenter( VERSION, 1, WHITE);
  display.display();
  delay(5);
}

// ======================================
void showProgress() {
  // Clear the buffer.
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  printlnTextCenter("PROGRESS", 1, WHITE);
  // printlnTextCenter( String( (byte) (modes[currentMode].Brightness / 2.55)) + "%", 2, WHITE);
  printlnTextCenter(String(action_timeout), 2, WHITE);
  //  printlnTextCenter("M " + String(sleep_mode) + " | S " + String(scenario), 1, WHITE);
  //printlnTextCenter("s" + String(scenario) + " " + String(sleep_mode) + " - " + String(activity), 1, WHITE);
  printlnTextCenter("S." + String(scenario) + " | M." + String(sleep_mode), 1, WHITE);
  // display.drawLine((display.width() - activity * 4) / 2, display.height() - 1, (display.width() + activity * 4) / 2, display.height() - 1, WHITE);

  // display.drawFastHLine((display.width() - activity * 4) / 2, display.height() - 1, activity * 4, WHITE);
  //drawFastHLine((display.width() - activity) / 2, display.height() - 1, activity, WHITE);
  display.display();
}

// ======================================
void showSoundSpectr(char *packetBuffer) {
  display.clearDisplay();
  for (uint8_t x = 0U; x < 30; x++) {
    display.drawPixel(2 + x * 2, 24, WHITE);
    if (x > 7 & x < 24) {
      byte val = (46 / 100.0) * packetBuffer[(x - 2)];
      display.drawFastVLine(x * 2, 24 - val / 2, val, WHITE);
    }
  }
  display.display();
}

// ======================================
void Introduction() {
  EVERY_N_SECONDS(5) {

    switch (ff_x) {
      case 0:
        showIcon(0, 5);
        // send_command(0x0F, FEEDBACK, 1, 4);
        // SoundMsg(4);
        break;
      case 1:
        SoundIntro(1);
        // ff_x++;
        break;
      case 41:
        SoundIntro(2);
        // ff_x++;
        break;
      case 51:
        SoundIntro(3);
        break;
      case 500:
        intro = false;
        ff_x = 0;
        break;
      default:
        break;

    }
    if (ff_x > 0) {
      showParam(5, "INTRO", String(ff_x), "");
    }
    if (ff_x % 5 == 0) {
      // ff_x++;
      //      servoHead.setTargetDeg(random8(60, 120));
      HeadRotation(random8(60, 120));
    }
    ff_x++;
  }
}

// ======================================
void HeadRotation(byte deg) {
  if (isServo) {
    servoHead.setTargetDeg(deg);
  }
}

// ======================================
// END
// ======================================
// ======================================
void PrintData() {
  // return;
#ifdef GENERAL_DEBUG
  LOG.printf_P(PSTR("• %3d | Scenario •%2d | dress •%2d | eye •%2d |"), action_timeout, lastScenario, dress_state, eyes_state);
  LOG.print (" | T • ");
  LOG.print(temperature_air);
  LOG.print ("°C\n\r");
#endif
}

// ======================================
//void testdrawchar(void) {
//  display.setTextSize(1);
//  display.setTextColor(WHITE);
//  display.setCursor(0, 0);
//
//  for (uint8_t i = 0; i < 168; i++) {
//    if (i == '\n') continue;
//    display.write(i);
//    if ((i > 0) && (i % 21 == 0))
//      display.println();
//  }
//  display.display();
//  delay(1);
//}

// ======================================
//void testServo() {
//  // LOG.print (" | SERVO_HEAD angle • ");
//  for (angle = 0; angle <= 90; angle++) {
//    servoHead.write(angle); // сообщаем микро серво угол поворота
//    delay(20); // ждем 20 мс, пока серво не сделает поворот
//  }
//  // цикл для поворота от 90 до 0 градусов
//  for (angle = 90; angle >= 0; angle--) {
//    servoHead.write(angle); // сообщаем микро серво угол поворота
//    delay(20); // ждем 20 мс, пока серво не сделает поворот
//  }
//}

#endif
