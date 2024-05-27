/*
    https://voxworker.com/ua  | Якісна озвучка тексту онлайн українською мовою
    https://ytmp3.nu/J7Dm/    | online converter YouTube to MP3
    https://elevenlabs.io/    | Michael

  • file •              • message •
  ---------------------------------------------------------
  [ DIR 001 ]           • sound effects •
  001_RotaRise          | Рота, підйом!
  002_GoToTheBathhouse  | Підйом, швидше збирайся в баню, тебе вже чекають!
  005_Music             | Ооо, музоон. Давно б так.
  008_CarefulWithBrains | Обережно з мізкаами. Я не хочу бути дауном.
  009_GoodMorning       | Добрий ранок!
  010_GoodDay           | Добрий день!
  011_GoodEvening       | Добрий вечір!
  012_YouRunning        | Що ти швендяєш туди сюди?
  013_WasItSuccessful   | Як подорож до холодильника? Була вдалою?

  014_VeryCold          | Брр.. Льодовиковий період. Щось дуже холодно.  Ice Age
  015_Cold              | Якось мерзлякувато, аж сервопривод закляк.
  016_Heat              | Ооо, потепліло. Нарешті літо прийшло.
  017_VeryHeat          | Спека нєможлівая, аж кури хєкають та асфальт порєпався.
  018_GoodNews          | Чи є файні новини? путіт ще не здох?
  ---------------------------------------------------------
  096_                  | Чого ти по хаті бродиш. Спати заважаєш.
  097_QuicklyOnThePotty | Швидко на горшоок і спати. Всім вести себе тихо. Добраніч.
  098_GoodNight         | Доброї ночі.

  099 • 099_IllBeBackEN | I'll Be Back
  100 • 100_Shtirlic    | Default
  129 • 129_DisturbSleep| Чого ти по хаті бродиш. Спати заважаєш.
  ---------------------------------------------------------
  [ DIR 009 ]          • intro         |

  [ DIR 020 ]          • sound effects
  020_HotWater          | Вода холоднюча, аж зуби зводить.
  021_HotWater          | Воду не гріють. Влітку вода в калюжі тепліша. Комфортна тільки для гемороою.
  022_HotWater          | Ооо. Теплеенька пішла. Мабуть щось у лісі здохло. Швидко всі в душ.
  023_HotWater          | Ооо боже, Сталось неможливе. Вода гаряча, Кидаєм всі справи, сьогодні банний день.
  030_hymn              | Ще не вмерла України і слава, і воля.
  090_NewYear           | NewYear Msg • З Новим роком!

  [ DIR 021 ]           • music

  [ DIR 040 ]           • cycled sound for effect Ocean
                          001_SeaWave | 002_OceanWave
  [ DIR 050 ]           • cycled sound for effect Forest
                          001_forest | 002_forest | 003_forest
  [ DIR 060 ]           • cycled sound for effect Flicker
  024_Cricket           | Цвіркун

  | example |
  #ifdef USE_ROBOT_007
  SoundMsg(90);
  #endif
*/

#ifdef USE_ROBOT_007

static int last_val;

// ======================================
void SoundFlasks() {
  // EVERY_N_SECONDS(1) {
  if (!dawnFlag) {
    time_t currentLocalTime = getCurrentLocalTime();
    byte mm = minute(currentLocalTime);
    byte ss = second(currentLocalTime);

    if (mm == 0 & ss == 0) {
      SoundMsg(3);
      // LOG.printf_P(PSTR("Sound Flasks | %2d:%2d |\n\r"), mm, ss);
    }
  }
  // }
}

// ======================================
void SoundGreetings(uint8_t idx) {
  if (dawnFlag) return;
  time_t currentLocalTime = getCurrentLocalTime();
  byte hh = hour(currentLocalTime);
  /* debug */
  byte mm = minute(currentLocalTime);
  byte ss = second(currentLocalTime);
  LOG.printf_P(PSTR("%2d:%2d:%2d | Greetings | Msg | IDX • %3d | isSoundPlay • %2d | sleep • %2d  \n\r"), hh, mm, ss, idx, isSoundPlay, sleep_mode);

  if ( idx > 200) {   /* Greetings */
    if (hh < 10) SoundMsg(9);
    if (hh > 17) SoundMsg((hh >= 21) ? 14 : 11);
    if ((hh >= 10) & (hh <= 17)) SoundMsg(10);
  } else {
    if ( idx > 60) { /* Was It Successful */
      SoundMsg(13);
    } else {          /* You Running */
      if ( idx > 10) {
        if ( idx > 25) {
          SoundMsg(12);
        } else {
          if (hh < 6) SoundMsg(129);
        }
      }
    }
  }
  // activity = 0;
  sound_timeout = 0;
}

// ======================================
void HeatNetworksMsg(int temp) {
  time_t currentLocalTime = getCurrentLocalTime();
  byte mm = minute(currentLocalTime);
  if ((mm % 5U) != 0U) return;

  isSoundPlay = true;
  send_command(0x06, FEEDBACK, 0, eff_volume);
  delay(mp3_delay);

  if (temp < 30) {
    /* LOG.printf_P(PSTR("б-р-р Вода холодна, зуби зводить | %2d |\n\r"), temp); */
    send_command(0x0F, FEEDBACK, 20, 20);
  } else {
    if (temp < 36) {
      /* LOG.printf_P(PSTR("воду не гріють, комфортна тільки для геморою | %2d |\n\r"), temp); */
      send_command(0x0F, FEEDBACK, 20, 21);
    } else {
      if (temp < 45) {
        /* LOG.printf_P(PSTR("щось у лісі здохло, швидше всі в душ | %2d |\n\r"), temp); */
        send_command(0x0F, FEEDBACK, 20, 22);
      } else {
        /* LOG.printf_P(PSTR("сталось не можливе, сьогодні банний день | %2d |\n\r"), temp); */
        send_command(0x0F, FEEDBACK, 20, 23);
      }
    }
  }
  delay(mp3_delay);
}

// ======================================
void SoundMute() {
  isSoundPlay = false;
  send_command(0x06, FEEDBACK, 0, 0);
  delay(mp3_delay);
}

// ======================================
void SoundMsg(byte msg) {
  if (intro | isSoundPlay) return;
  isSoundPlay = true;
  if (sound) {
    time_t currentLocalTime = getCurrentLocalTime();
    byte hh = hour(currentLocalTime);
    uint8_t thisDay = getDayOfWeek(currentLocalTime);

    if ((msg <= 90) & isNight()) {
      // LOG.println(" SoundMsg | Sound Mute");
      SoundMute();
      return;
    }

    // LOG.println(" SoundMsg | Sound play • " + String(msg));
    // LOG.printf_P(PSTR(" SoundMsg • %3d  | This Day •%2d | isSoundPlay •%2d \n\r"), msg, thisDay, isSoundPlay);

    send_command(0x06, FEEDBACK, 0, (msg > 127) ? 6 : eff_volume);
    delay(mp3_delay);

    switch (msg) {
      case 0:               // 005_Shtirlic
        send_command(0x0F, FEEDBACK, 1, 5);
        break;
      case 2:
        break;
      case 3:               //  003_Flasks.mp3
        send_command(0x0F, FEEDBACK, 1, 3);
        break;
      case 4:
        send_command(0x0F, FEEDBACK, 1, 4);
        break;
      case 5:
        send_command(0x0F, FEEDBACK, 1, 5);
        break;
      case 6:
        send_command(0x0F, FEEDBACK, 1, 6);
        break;
      case 7:
        send_command(0x0F, FEEDBACK, 1, 7);
        break;
      case 8:
        send_command(0x0F, FEEDBACK, 1, 8);
        break;
      case 9:
        send_command(0x0F, FEEDBACK, 1, 9);
        break;
      case 10:
        send_command(0x0F, FEEDBACK, 1, 10);
        break;
      case 11:
        send_command(0x0F, FEEDBACK, 1, 11);
        break;
      case 12:
        send_command(0x0F, FEEDBACK, 1, (thisDay == 5) ? 19 : 12);
        break;
      case 13:
        send_command(0x0F, FEEDBACK, 1, 13);
        break;
      case 14:
        send_command(0x0F, FEEDBACK, 1, 97);
        break;

      /* effects */
      case 30:
        send_command(0x0F, FEEDBACK, 20, 30);
        break;
      case 90:            /* ChristmasTree */
        send_command(0x0F, FEEDBACK, 20, 90);
        break;
      /* ======== */

      case 98:
        /* thisDay = 5 | Saturday */
        send_command(0x0F, FEEDBACK, 1, (thisDay == 5) ? 97 : 98);
        break;
      case 99:              // 099_IllBeBackEN.mp3
        send_command(0x0F, FEEDBACK, 1, 99);
        break;
      case 100:             // 100_Shtirlic
        send_command(0x0F, FEEDBACK, 1, 100);
        break;
      case 129:             // 129_DisturbSleep
        send_command(0x0F, FEEDBACK, 1, 129);
        break;
      default:              // 005_Shtirlic
        send_command(0x0F, FEEDBACK, 1, 5);
        break;
    }

    delay(mp3_delay);
  }
}

// ======================================
void EnvironmentMsg(int temp) {
  /*
    014_VeryCold   | Брр.. Льодовиковий період. Щось дуже холодно.  Ice Age
    015_Cold       | Якось мерзлякувато, аж сервопривод закляк.
    016_Heat       | Ооо, потепліло. Нарешті літо прийшло.
    017_VeryHeat   | Спека нєможлівая, аж кури хєкають та асфальт порєпався.
    018_GoodNews   | Чи є файні новини? путіт ще не здох?
  */
  byte id = 18U;
  if (isSoundPlay | isNight()) return;
  if (temp < 20) id = (temp < 16) ? 14U : 15U;
  if (temp > 23) id = (temp > 26) ? 17U : 16U;

  isSoundPlay = true;
  send_command(0x06, FEEDBACK, 0, eff_volume);
  delay(mp3_delay);
  send_command(0x0F, FEEDBACK, 1, id);
  delay(mp3_delay);
}

// ======================================
void PlayCycledSoundEffect(byte CurFolder) {
  if (sound) {
    send_command(0x06, FEEDBACK, 0, eff_volume);
    delay(mp3_delay);
    isSoundPlay = true;
    /* play folder */
    send_command(0x17, FEEDBACK, 0, CurFolder);
    delay(mp3_delay);
  }
}

// ======================================
void SoundAlarm(byte cur_day) {
  /* cur_day 0 - 6 sat = 5 */
  if (isSoundPlay) return;
  isSoundPlay = true;
  send_command(0x06, FEEDBACK, 0, eff_volume);
  delay(mp3_delay);
  // LOG.printf_P(PSTR(" Sound Alarm | Cur day •%2d | \n\r"), cur_day);
  send_command(0x0F, FEEDBACK, 1, (cur_day == 5) ? 2 : 1);
  delay(mp3_delay);
}

// ======================================
void SoundIntro(byte msg) {
  isSoundPlay = true;
  send_command(0x06, FEEDBACK, 0, eff_volume);
  delay(mp3_delay);
  send_command(0x0F, FEEDBACK, 9, msg);
  delay(mp3_delay);
}
#endif
