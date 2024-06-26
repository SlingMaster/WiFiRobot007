
// ============= Tixy Land ==============
//        © Martin Kleppe @aemkei
//github.com/owenmcateer/tixy.land-display
//      Create Script Change Effects
//             © SlingMaster
// ======================================
//   набор мат. функций и примитивов для
//            обсчета эффектов
//       © Dmytro Korniienko (kDn)
// ======================================

#define M_PI_2  1.57079632679489661923
static const PROGMEM float LUT[102] = {
  0,           0.0099996664, 0.019997334, 0.029991005, 0.039978687,
  0.049958397, 0.059928156,  0.069885999, 0.079829983, 0.089758173,
  0.099668652, 0.10955953,   0.11942893,  0.12927501,  0.13909595,
  0.14888994,  0.15865526,   0.16839015,  0.17809294,  0.18776195,
  0.19739556,  0.20699219,   0.21655031,  0.22606839,  0.23554498,
  0.24497867,  0.25436807,   0.26371184,  0.27300870,  0.28225741,
  0.29145679,  0.30060568,   0.30970293,  0.31874755,  0.32773849,
  0.33667481,  0.34555557,   0.35437992,  0.36314702,  0.37185606,
  0.38050637,  0.38909724,   0.39762798,  0.40609807,  0.41450688,
  0.42285392,  0.43113875,   0.43936089,  0.44751999,  0.45561564,
  0.46364760,  0.47161558,   0.47951928,  0.48735857,  0.49513325,
  0.50284320,  0.51048833,   0.51806855,  0.52558380,  0.53303409,
  0.54041952,  0.54774004,   0.55499572,  0.56218672,  0.56931317,
  0.57637525,  0.58337301,   0.59030676,  0.59717667,  0.60398299,
  0.61072594,  0.61740589,   0.62402308,  0.63057774,  0.63707036,
  0.64350110,  0.64987046,   0.65617871,  0.66242629,  0.66861355,
  0.67474097,  0.68080884,   0.68681765,  0.69276786,  0.69865984,
  0.70449406,  0.71027100,   0.71599114,  0.72165483,  0.72726268,
  0.73281509,  0.73831260,   0.74375558,  0.74914461,  0.75448018,
  0.75976276,  0.76499283,   0.77017093,  0.77529752,  0.78037310,
  0.78539819,  0.79037325
};

// --------------------------------------
float atan2_fast(float y, float x) {
  //http://pubs.opengroup.org/onlinepubs/009695399/functions/atan2.html
  //Volkan SALMA

  const float ONEQTR_PI = PI / 4.0;
  const float THRQTR_PI = 3.0 * PI / 4.0;
  float r, angle;
  float abs_y = fabs(y) + 1e-10f;      // kludge to prevent 0/0 condition
  if ( x < 0.0f ) {
    r = (x + abs_y) / (abs_y - x);
    angle = THRQTR_PI;
  } else {
    r = (x - abs_y) / (x + abs_y);
    angle = ONEQTR_PI;
  }
  angle += (0.1963f * r * r - 0.9817f) * r;
  if ( y < 0.0f ) {
    return ( -angle );    // negate if in quad III or IV
  } else {
    return ( angle );
  }
}

// --------------------------------------
float atan_fast(float x) {
  /* A fast look-up method with enough accuracy */
  if (x > 0) {
    if (x <= 1) {
      int index = round(x * 100);
      return LUT[index];
    } else {
      float re_x = 1 / x;
      int index = round(re_x * 100);
      return (M_PI_2 - LUT[index]);
    }
  } else {
    if (x >= -1) {
      float abs_x = -x;
      int index = round(abs_x * 100);
      return -(LUT[index]);
    } else {
      float re_x = 1 / (-x);
      int index = round(re_x * 100);
      return (LUT[index] - M_PI_2);
    }
  }
}

// --------------------------------------
float tan2pi_fast(float x) {
  float y = (1 - x * x);
  return x * (((-0.000221184 * y + 0.0024971104) * y - 0.02301937096) * y + 0.3182994604 + 1.2732402998 / y);
}


// --------------------------------------
float code(double t, double i, double x, double y) {
  switch (pcnt) {
    /** © Motus Art @motus_art */
    case 1: /* Plasma */
      hue = 96U; hue2 = 224U;
      return (sin16((x + t) * 8192.0) * 0.5 + sin16((y + t) * 8192.0) * 0.5 + sin16((x + y + t) * 8192.0) * 0.3333333333333333) / 32767.0;
      break;

    case 2: /* Up&Down */
      //return sin(cos(x) * y / 8 + t);
      hue = 255U; hue2 = 160U;
      return sin16((cos16(x * 8192.0) / 32767.0 * y / (HEIGHT / 2.0) + t) * 8192.0) / 32767.0;
      break;

    case 3:
      hue = 255U; hue2 = 96U;
      return sin16((atan_fast(y / x) + t) * 8192.0) / 32767.0;
      break;

    /** © tixy.land website */
    case 4: /* Emitting rings */
      hue = 255U; hue2 = 0U;
      return sin16((t - sqrt3((x - (WIDTH / 2)) * (x - (WIDTH / 2)) + (y - (HEIGHT / 2)) * (y - (HEIGHT / 2)))) * 8192.0) / 32767.0;
      break;

    case 5: /* Rotation  */
      hue = 136U; hue2 = 48U;
      return sin16((PI * 2.5 * atan_fast((y - (HEIGHT / 2)) / (x - (WIDTH / 2))) + 5 * t) * 8192.0) / 32767.0;
      break;

    case 6: /* Vertical fade */
      hue = 160U; hue2 = 0U;
      return sin16((y / 8 + t) * 8192.0) / 32767.0;
      break;

    case 7: /* Waves */
      //return sin(x / 2) - sin(x - t) - y + 6;
      hue = 48U; hue2 = 160U;
      return (sin16(x * 4096.0) - sin16((x - t) * 8192.0)) / 32767.0 - y + (HEIGHT / 2);
      break;

    case 8: /* Drop */
      hue = 136U; hue2 = 160U;
      return fmod(8 * t, 13) - sqrt3((x - (WIDTH / 2)) * (x - (WIDTH / 2)) + (y - (HEIGHT / 2)) * (y - (HEIGHT / 2))); //hypot(x - (WIDTH/2), y - (HEIGHT/2));
      break;

    case 9: /* Ripples @thespite */
      hue = 96U; hue2 = 224U;
      return sin16((t - sqrt3(x * x + y * y)) * 8192.0) / 32767.0;
      break;

    case 10: /* Bloop bloop bloop @v21 */
      hue = 136U; hue2 = 160U;
      return (x - (WIDTH / 2)) * (y - (HEIGHT / 2)) - sin16(t * 4096.0) / 512.0;
      break;

    case 11: /* SN0WFAKER */
      // https://www.reddit.com/r/programming/comments/jpqbux/minimal_16x16_dots_coding_environment/gbgk7c0/
      hue = 96U; hue2 = 160U;
      return sin16((atan_fast((y - (HEIGHT / 2)) / (x - (WIDTH / 2))) + t) * 8192.0) / 32767.0;
      break;
    case 12: /* detunized */
      // https://www.reddit.com/r/programming/comments/jpqbux/minimal_16x16_dots_coding_environment/gbgk30l/
      hue = 136U; hue2 = 160U;
      return sin16((y / (HEIGHT / 2) + t * 0.5) * 8192.0) / 32767.0 + x / 16 - 0.5;
      break;

    /** © @akella | https://twitter.com/akella/status/1323549082552619008 */
    case 13:
      hue = 255U; hue2 = 0U;
      return sin16((6 * atan2_fast(y - (HEIGHT / 2), x) + t) * 8192.0) / 32767.0;
      break;
    case 14:
      hue = 32U; hue2 = 160U;
      return sin16((i / 5 + t) * 16384.0) / 32767.0;
      break;

    /** © Paul Malin | https://twitter.com/P_Malin/ */

    // sticky blood
    // by @joeytwiddle
    //(t,i,x,y) => y-t*3+9+3*cos(x*3-t)-5*sin(x*7)

    //      if (x < 8) {
    //       // hue = 160U;
    //      } else {
    //       // hue = 96U;
    //      }
    //      if ((y == HEIGHT -1)&(x == 8)) {
    //        hue = hue + 30;
    //        if (hue >= 255U) {
    //          hue = 0;
    //        }
    //      }
    //      hue = t/128+8;

    //    case 19: // !!!! paint
    //      // Matrix Rain https://twitter.com/P_Malin/status/1323583013880553472
    //      //return 1. - fmod((x * x - y + t * (fmod(1 + x * x, 5)) * 6), 16) / 16;
    //      return 1. - fmod((x * x - (HEIGHT - y) + t * (1 + fmod(x * x, 5)) * 3), WIDTH) / HEIGHT;
    //      break;

    case 15: /* Burst */
      // https://twitter.com/P_Malin/status/1323605999274594304
      hue = 136U; hue2 = 160U;
      return -10. / ((x - (WIDTH / 2)) * (x - (WIDTH / 2)) + (y - (HEIGHT / 2)) * (y - (HEIGHT / 2)) - fmod(t * 0.3, 0.7) * 200);
      break;

    case 16: /* Rays */
      hue = 255U; hue2 = 0U;
      return sin16((atan2_fast(x, y) * 5 + t * 2) * 8192.0) / 32767.0;
      break;

    case 17: /* Starfield */
      // org | https://twitter.com/P_Malin/status/1323702220320313346
      hue = 255U; hue2 = 160U;
      return !((int)(x + t * 50 / (fmod(y * y, 5.9) + 1)) & 15) / (fmod(y * y, 5.9) + 1);
      //      {
      //        uint16_t _y = HEIGHT - y;
      //        float d = (fmod(_y * _y + 4, 4.1) + 0.85) * 0.5; // коэффициенты тут отвечают за яркость (размер), скорость, смещение, подбираются экспериментально :)
      //        return !((int)(x + t * 7.0 / d) & 15) / d; // 7.0 - множитель скорости
      //      }
      break;

    case 18:
      hue = 255U; hue2 = 0U;
      return sin16((3.5 * atan2_fast(y - (HEIGHT / 2) + sin16(t * 8192.0) * 0.00006, x - (WIDTH / 2) + sin16(t * 8192.0) * 0.00006) + t * 1.5 + 5) * 8192.0) / 32767.0;
      break;

    case 19:
      hue = 255U; hue2 = 224U;
      return (y - 8) / 3 - tan2pi_fast((x / 6 + 1.87) / PI * 2) * sin16(t * 16834.0) / 32767.0;
      break;

    case 20:
      hue = 136U; hue2 = 160U;
      return (y - 8) / 3 - (sin16((x / 4 + t * 2) * 8192.0) / 32767.0);
      break;

    case 21:
      hue = 72U; hue2 = 96U;
      return cos(sin16(x * t * 819.2) / 32767.0 * PI) + cos16((sin16((y * t / 10 + (sqrt3(abs(cos16(x * t * 8192.0) / 32767.0)))) * 8192.0) / 32767.0 * PI) * 8192.0) / 32767.0;
      break;

    case 22: /* bambuk */
      hue = 96U; hue2 = 80U;
      return sin16(x / 3 * sin16(t * 2730.666666666667) / 2.0) / 32767.0 + cos16(y / 4 * sin16(t * 4096.0) / 2.0) / 32767.0;
      break;

    case 23:
      hue = 0U; hue2 = 224U;
      {
        float _x = x - fmod(t, WIDTH);
        float _y = y - fmod(t, HEIGHT);
        return -.4 / (sqrt3(_x * _x + _y * _y) - fmod(t, 2) * 9);
      }
      break;

    case 24: /* honey */
      hue = 255U; hue2 = 40U;
      return sin16(y * t * 2048.0) / 32767.0 * cos16(x * t * 2048.0) / 32767.0;
      break;

    case 25:
      hue = 96U; hue2 = 160U;
      return atan_fast((x - (WIDTH / 2)) * (y - (HEIGHT / 2))) - 2.5 * sin16(t * 8192.0) / 32767.0;
      break;

    default:
      if (pcnt > 25) {
        deltaHue2 += 32;
      }
      pcnt = 1;
      hue = 96U; hue2 = 0U;
      return sin16(t * 8192.0) / 32767.0;
      break;
  }
}

// --------------------------------------
void processFrame(double t, double x, double y) {
  double i = (y * WIDTH) + x;
  double frame = constrain(code(t, i, x, y), -1, 1) * 255;
  if (frame > 0) {
    if ( hue == 255U) {
      drawPixelXY(x, y, CRGB(frame, frame, frame));
    } else {
      drawPixelXY(x, y, CHSV(hue, frame, frame));
    }
  } else {
    if (frame < 0) {
      if (modes[currentMode].Scale < 5) deltaHue2 = 0;
      drawPixelXY(x, y, CHSV(hue2 + deltaHue2, frame * -1, frame * -1));
    } else {
      drawPixelXY(x, y, CRGB::Black);
    }
  }
}

// --------------------------------------
void TixyLand() {
  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      //                     scale | speed
      setModeSettings(random8(100U), random8(255U));
    }
#endif //#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    loadingFlag = false;
    deltaHue = 0;
    pcnt = map(modes[currentMode].Speed, 5, 250, 1U, 25U);
    FPSdelay = 1;
    deltaHue2 = modes[currentMode].Scale * 2.55;
    hue = 255U; hue2 = 0U;
  }
  // *****
  unsigned long milli = millis();
  double t = milli / 1000.0;

  EVERY_N_SECONDS(eff_interval) {
    if ((modes[currentMode].Speed < 5) || (modes[currentMode].Speed > 250)) {
      pcnt++;
    }
  }
  for ( double x = 0; x < WIDTH; x++) {
    for ( double y = 0; y < HEIGHT; y++) {
      processFrame(t, x, y);
    }
  }
}

// =====================================
//                Stars
//            © Marc Merlin
//               доробки
//    © SottNick © kostyamat, © kDn
//      Adaptation © SlingMaster
//                Зірки
// =====================================
void drawStar(float xlocl, float ylocl, float biggy, float little, int16_t points, float dangle, uint8_t koler) { // random multipoint star
  float radius2 = 255.0 / points;
  for (int i = 0; i < points; i++) {
    DrawLine(xlocl + ((little * (sin8(i * radius2 + radius2 / 2 - dangle) - 128.0)) / 128), ylocl + ((little * (cos8(i * radius2 + radius2 / 2 - dangle) - 128.0)) / 128), xlocl + ((biggy * (sin8(i * radius2 - dangle) - 128.0)) / 128), ylocl + ((biggy * (cos8(i * radius2 - dangle) - 128.0)) / 128), ColorFromPalette(*curPalette, koler));
    DrawLine(xlocl + ((little * (sin8(i * radius2 - radius2 / 2 - dangle) - 128.0)) / 128), ylocl + ((little * (cos8(i * radius2 - radius2 / 2 - dangle) - 128.0)) / 128), xlocl + ((biggy * (sin8(i * radius2 - dangle) - 128.0)) / 128), ylocl + ((biggy * (cos8(i * radius2 - dangle) - 128.0)) / 128), ColorFromPalette(*curPalette, koler));

  }
}

// --------------------------------------
void EffectStars() {
#define STARS_NUM (8U)
#define STAR_BLENDER (128U)
#define CENTER_DRIFT_SPEED (6U)
  static uint8_t spd;
  static uint8_t points[STARS_NUM];
  static float color[STARS_NUM] ;
  static int delay_arr[STARS_NUM];
  static float counter;
  static float driftx;
  static float  drifty;
  static float cangle;
  static float  sangle;
  static uint8_t stars_count;
  static uint8_t blur;

  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      //                     scale | speed
      setModeSettings(random8(100U), random8(80U, 255U));
    }
#endif //#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    loadingFlag = false;
    counter = 0.0;
    // стартуем с центра
    driftx = (float)WIDTH / 2.0;
    drifty = (float)HEIGHT / 2.0;

    cangle = (float)(sin8(random8(25, 220)) - 128.0f) / 128.0f; //angle of movement for the center of animation gives a float value between -1 and 1
    sangle = (float)(sin8(random8(25, 220)) - 128.0f) / 128.0f; //angle of movement for the center of animation in the y direction gives a float value between -1 and 1
    spd = modes[currentMode].Speed;
    blur = modes[currentMode].Scale / 2;
    stars_count = WIDTH / 2U;

    if (stars_count > STARS_NUM) stars_count = STARS_NUM;
    for (uint8_t num = 0; num < stars_count; num++) {
      points[num] = map(modes[currentMode].Scale, 1, 255, 3U, 7U); //5; // random8(3, 6);                              // количество углов в звезде
      delay_arr[num] = spd / 5 + (num << 2) + 2U;               // задержка следующего пуска звезды
      color[num] = random8();
    }
  }
  // fadeToBlackBy(leds, NUM_LEDS, 245);
  fadeToBlackBy(leds, NUM_LEDS, 165);
  float speedFactor = ((float)spd / 380.0 + 0.05);
  counter += speedFactor;                                                   // определяет то, с какой скоростью будет приближаться звезда

  if (driftx > (WIDTH - spirocenterX / 2U)) cangle = 0 - fabs(cangle);      //change directin of drift if you get near the right 1/4 of the screen
  if (driftx < spirocenterX / 2U) cangle = fabs(cangle);                    //change directin of drift if you get near the right 1/4 of the screen
  if ((uint16_t)counter % CENTER_DRIFT_SPEED == 0) driftx = driftx + (cangle * speedFactor); //move the x center every so often
  if (drifty > ( HEIGHT - spirocenterY / 2U)) sangle = 0 - fabs(sangle);    // if y gets too big, reverse
  if (drifty < spirocenterY / 2U) sangle = fabs(sangle);                    // if y gets too small reverse

  if ((uint16_t)counter % CENTER_DRIFT_SPEED == 0) drifty = drifty + (sangle * speedFactor); //move the y center every so often

  for (uint8_t num = 0; num < stars_count; num++) {
    if (counter >= delay_arr[num]) {              //(counter >= ringdelay)
      if (counter - delay_arr[num] <= WIDTH + 5) {
        drawStar(driftx, drifty, 2 * (counter - delay_arr[num]), (counter - delay_arr[num]), points[num], STAR_BLENDER + color[num], color[num]);
        color[num] += speedFactor;                // в зависимости от знака - направление вращения
      } else {
        delay_arr[num] = counter + (stars_count << 1) + 1U; // задержка следующего пуска звезды
      }
    }
  }
  blur2d(leds, WIDTH, HEIGHT, blur);
}


// =====================================
//                Mirage
//               © Stepko
//                Міраж
// =====================================
byte buff[WIDTH + 2][HEIGHT + 2];
// -------------------------------------
void blur() {
  uint16_t sum;
  for (byte x = 1; x < WIDTH + 1; x++) {
    for (byte y = 1; y < HEIGHT + 1; y++) {
      sum = buff[x][y];
      sum += buff[x + 1][y];
      sum += buff[x][y - 1];
      sum += buff[x][y + 1];
      sum += buff[x - 1][y];
      sum /= 5;
      buff[x][y] = sum;
    }
  }
}

// -------------------------------------
void drawDot(float x, float y, byte a) {
  uint8_t xx = (x - (int) x) * 255, yy = (y - (int) y) * 255, ix = 255 - xx, iy = 255 - yy;
#define WU_WEIGHT(a, b)((uint8_t)(((a) * (b) + (a) + (b)) >> 8))
  uint8_t wu[4] = {
    WU_WEIGHT(ix, iy),
    WU_WEIGHT(xx, iy),
    WU_WEIGHT(ix, yy),
    WU_WEIGHT(xx, yy)
  };
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (uint8_t i = 0; i < 4; i++) {
    int16_t xn = x + (i & 1), yn = y + ((i >> 1) & 1);
    byte clr = buff[xn][yn];
    clr = constrain(qadd8(clr, (a * wu[i]) >> 8), 0 , 255);
    buff[xn][yn] = clr;
  }
}

// -------------------------------------
void Mirage() {
  const uint8_t divider = 4;
  const uint8_t val = 255;
  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      //                     scale | speed
      setModeSettings(random8(100U), random8(80U, 255U));
    }
#endif //#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    loadingFlag = false;
    hue = 70;
  }

  blur();
  float x1 = (float)beatsin88(15 * modes[currentMode].Speed, divider, WIDTH * divider) / divider;
  float y1 = (float)beatsin88(20 * modes[currentMode].Speed, divider, HEIGHT * divider) / divider;
  float x2 = (float)beatsin88(16 * modes[currentMode].Speed, divider, (WIDTH - 1) * divider) / divider;
  float y2 = (float)beatsin88(14 * modes[currentMode].Speed, divider, HEIGHT * divider) / divider;
  float x3 = (float)beatsin88(12 * modes[currentMode].Speed, divider, (WIDTH - 1) * divider) / divider;
  float y3 = (float)beatsin88(16 * modes[currentMode].Speed, divider, HEIGHT * divider) / divider;

  drawDot(x1 , y1, val);
  drawDot(x1 + 1, y1, val);
  drawDot(x2 , y2, val);
  drawDot(x2 + 1, y2, val);
  drawDot(x3 , y3, val);
  drawDot(x3 + 1, y3, val);

  hue++;
  for (byte x = 1; x < WIDTH + 1; x++) {
    for (byte y = 1; y < HEIGHT + 1; y++) {
      leds[XY(x - 1, y - 1)] = CHSV(hue , buff[x][y], 255);
    }
  }
}

// =====================================
//     Multicolored Dandelions
//      Base Code © Less Lam
//          © SlingMaster
//       Різнобарвні Kульбаби
// https://editor.soulmatelights.com/gallery/2007-amber-rain
// =====================================
class Circle {
  public:
    float thickness = 3.0;
    long startTime;
    uint16_t offset;
    int16_t centerX;
    int16_t centerY;
    int hue;
    int bpm = 10;

    void move() {
      centerX = random(0, WIDTH);
      centerY = random(0, HEIGHT);
    }

    void scroll() {
      centerX--; // = random(0, WIDTH);
      if (centerX < 1) {
        centerX = WIDTH - 1;
      }
      centerY++;
      if (centerY > HEIGHT) {
        centerY = 0;
      }
    }
    void reset() {
      startTime = millis();
      centerX = random(0, WIDTH);
      centerY = random(0, HEIGHT);
      hue = random(0, 255);
      offset = random(0, 60000 / bpm);
    }

    float radius() {
      float radius = beatsin16(modes[currentMode].Speed / 2.5, 0, 500, offset) / 100.0;
      return radius;
    }
};

// -----------------------------------
namespace Circles {
#define NUMBER_OF_CIRCLES WIDTH/2
Circle circles[NUMBER_OF_CIRCLES] = {};

void drawCircle(Circle circle) {
  int16_t centerX = circle.centerX;
  int16_t centerY = circle.centerY;
  int hue = circle.hue;
  float radius = circle.radius();

  int16_t startX = centerX - ceil(radius);
  int16_t endX = centerX + ceil(radius);
  int16_t startY = centerY - ceil(radius);
  int16_t endY = centerY + ceil(radius);

  for (int16_t x = startX; x < endX; x++) {
    for (int16_t y = startY; y < endY; y++) {
      int16_t index = XY(x, y);
      if (index < 0 || index > NUM_LEDS)
        continue;
      double distance = sqrt(sq(x - centerX) + sq(y - centerY));
      if (distance > radius)
        continue;

      uint16_t brightness;
      if (radius < 1) { // last pixel
        // brightness = 0; //255.0 * radius;
        deltaValue = 20;
        brightness = 180;
        // brightness = 0;
      } else {
        deltaValue = 200; // 155 + modes[currentMode].Scale;
        double percentage = distance / radius;
        double fraction = 1.0 - percentage;
        brightness = 255.0 * fraction;
      }
      leds[index] += CHSV(hue, deltaValue, brightness);
    }
  }
}

// -----------------------------
void draw(bool setup) {
  fadeToBlackBy(leds, NUM_LEDS, 100U);
  // fillAll(CRGB::Black);
  for (int i = 0; i < NUMBER_OF_CIRCLES; i++) {
    if (setup) {
      circles[i].reset();
    } else {
      if (circles[i].radius() < 0.5) {
        circles[i].scroll();
      }
    }
    drawCircle(circles[i]);
  }
}
}; // namespace Circles

// ==============
void Dandelions() {
  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      // scale | speed
      setModeSettings(random8(1U, 100U), random8(10U, 255U));
    }
#endif
    loadingFlag = false;
    FastLED.clear();
    Circles::draw(true);
    // deltaValue = 150 + modes[currentMode].Scale;
    deltaValue = 155 + modes[currentMode].Scale;
  }

  // FPSdelay = SOFT_DELAY;
  Circles::draw(false);
}


// ==============
// END ==============
// ==============


// =====================================
//            Poltava Poppies
//       © Stepko and © Sutaburosu
//            Полтавські Маки
// =====================================

static byte XY_angle[WIDTH][HEIGHT];
static byte XY_radius[WIDTH][HEIGHT];

// -------------------------------------
void Poppies() {
  const uint8_t mapp = 255 / WIDTH;
  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      //                     scale | speed
      setModeSettings(random8(100U), random8(80U, 255U));
    }
#endif //#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    loadingFlag = false;
    for (int8_t x = -CENTER_X_MAJOR; x < CENTER_X_MAJOR + (WIDTH % 2); x++) {
      for (int8_t y = -CENTER_Y_MAJOR; y < CENTER_Y_MAJOR + (HEIGHT % 2); y++) {
        XY_angle[x + CENTER_X_MAJOR][y + CENTER_Y_MAJOR] = 128 * (atan2(y, x) / PI);
        XY_radius[x + CENTER_X_MAJOR][y + CENTER_Y_MAJOR] = hypot(x, y); // thanks Sutaburosu
      }
    }
  }
  FastLED.clear();
  static byte speed2 = 128;
  static uint16_t t;
  t += speed2;

  uint16_t t1 = t / 2;
  for (uint8_t x = 0; x < WIDTH; x++) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
      byte angle = sin8(t1 / 2 + XY_angle[x][y] * 3);
      byte radius = (XY_radius[x][y] * mapp) * 2 - t;
      byte noise[3] = { inoise8(angle, radius, t1), inoise8(angle, 12032 + t1, radius), inoise8(radius, 120021 + t1, angle) };
      leds[XY(x, y)] = CRGB((noise[0] < 128) ? 0 : (noise[0] - 128) * 3, (noise[1] < 128) ? 0 : (noise[1] - 128) * 3, (noise[2] < 128) ? 0 : (noise[2] - 128) * 3);
    }
  }
  delay(16);
}

// =====================================
//               Hypnosis
//      © Stepko and © Sutaburosu
// =====================================
/* --------------------------------- */
void Hypnosis() {
  static uint32_t t;
  if (loadingFlag) {
#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings) {
      // scale | speed
      setModeSettings(random8(1U, 255U), random8(150U, 255U));
    }
#endif
    loadingFlag = false;
    FastLED.clear();
    for (int8_t x = -CENTER_X_MAJOR; x < CENTER_X_MAJOR; x++) {
      for (int8_t y = -CENTER_Y_MAJOR; y < CENTER_Y_MAJOR; y++) {
        noise3d[0][x + CENTER_X_MAJOR][y + CENTER_Y_MAJOR] = 128 * (atan2(y, x) / PI);
        noise3d[1][x + CENTER_X_MAJOR][y + CENTER_Y_MAJOR] = hypot(x, y);                    // thanks Sutaburosu
      }
    }
  }

  t += 4;
  for (uint8_t x = 0; x < WIDTH; x++) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
      byte angle = noise3d[0][x][y];
      byte radius = noise3d[1][x][y];
      //      leds[XY(x, y)] = CHSV(t + radius * (255 / WIDTH), 255, sin8(sin8(t + angle * PETALS + ( radius * (255 / WIDTH))) + t * 4 + sin8(t * 4 - radius * (255 / WIDTH)) + angle * PETALS));
      leds[XY(x, y)] = ColorFromPalette(RainbowStripeColors_p, t / 2 + radius + angle, sin8(angle + (radius * 2) - t));
    }
  }
}



// =====================================
//            Poltava Poppies
//       © Stepko and © Sutaburosu
//            Полтавські Маки
// =====================================

//static byte XY_angle[WIDTH][HEIGHT];
//static byte XY_radius[WIDTH][HEIGHT];

// -------------------------------------
//void Poppies() {
//  const uint8_t mapp = 255 / WIDTH;
//  if (loadingFlag) {
//#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
//    if (selectedSettings) {
//      //                     scale | speed
//      setModeSettings(random8(100U), random8(80U, 255U));
//    }
//#endif //#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
//    loadingFlag = false;
//    for (int8_t x = -CENTER_X_MAJOR; x < CENTER_X_MAJOR + (WIDTH % 2); x++) {
//      for (int8_t y = -CENTER_Y_MAJOR; y < CENTER_Y_MAJOR + (HEIGHT % 2); y++) {
//        XY_angle[x + CENTER_X_MAJOR][y + CENTER_Y_MAJOR] = 128 * (atan2(y, x) / PI);
//        XY_radius[x + CENTER_X_MAJOR][y + CENTER_Y_MAJOR] = hypot(x, y); // thanks Sutaburosu
//      }
//    }
//  }
//  FastLED.clear();
//  static byte speed2 = 128;
//  static uint16_t t;
//  t += speed2;
//
//  uint16_t t1 = t / 2;
//  for (uint8_t x = 0; x < WIDTH; x++) {
//    for (uint8_t y = 0; y < HEIGHT; y++) {
//      byte angle = sin8(t1 / 2 + XY_angle[x][y] * 3);
//      byte radius = (XY_radius[x][y] * mapp) * 2 - t;
//      byte noise[3] = { inoise8(angle, radius, t1), inoise8(angle, 12032 + t1, radius), inoise8(radius, 120021 + t1, angle) };
//      leds[XY(x, y)] = CRGB((noise[0] < 128) ? 0 : (noise[0] - 128) * 3, (noise[1] < 128) ? 0 : (noise[1] - 128) * 3, (noise[2] < 128) ? 0 : (noise[2] - 128) * 3);
//    }
//  }
//  delay(16);
//}
