// ===============================================================================================================
// ConstantsUser.h
// ===============================================================================================================

// !!! рекоомендуется перед прошивкой лампы прописать установки домашней сети | ssid и password | в файле data/config.json
//     чтобы не заморачиваться с работой в режиме точки доступа и уже после настроить свои предпочтения по вкусу, через web интерфейс
//     в этом файле сосредоточены наиболее часто меняемые настройки, зависящие от конструкции лампы и предпочтений пользователя
// ===============================================================================================================
#include <ESP8266WebServer.h>

// === СХЕМОТЕХНІКА  ===================
// =====================================

#define IOT_TYPE                (3U)                        // введено поняття типу пристрою, потрібно для сумісності з прогамним забезпеченням 
//                                                          | 0 • оригінальна лампа | 1 • лампа javelin | 2 • робот | 3 • рорбот 007 | 4 • IOT lighthouse |

// ROBOT -------------------------------
#define USE_ROBOT_007
#ifdef USE_ROBOT_007                                                                             //   GPIO
const int analogInPin =          A0;                        // ESP8266 AnalogPin       ADC0   • (A0)
#define DHTPIN                 (16U)                        // пін DHT 11                     • (D0) (16U)
/* LCD SSD1306 ========================= */
/*LCD SCL                                                                                     • (D1) ( 5U)
  LCD SDA                                                                                     • (D2) ( 4U)
  LED_PIN                       (0U)                        // пін управління матриці         • (D3) ( 0U) */
#define MOTION_PIN              (2U)                        // motion sensor                  • (D4) ( 2U)
/*#define MB_LED_PIN            (2U)                        // blue led на платі              • (D4) ( 2U)
  #define BTN_PIN              (14U)                        // пін кнопки                     • (D5) (14U)
  #define MP3_TX_PIN           (12U)                        // контакт TX                     • (D6) (12U)
  #define MP3_RX_PIN           (13U)                        // контакт RX                     • (D7) (13U) */
#define SERVO_HEAD             (15U)                        // servo head <-|->               • (D8) (15U)
#define EYES_BRIGHTNESS        (15U)
#endif

// --- DF PLayer -----------------------
#define MP3_TX_PIN            (12U)                         // В СЛУЧАЕ ОТСУТСТВИЯ ПЛЕЕРА ЗАКОМЕНТМРОВАТЬ СТРОКУ!!! Определяет вывод TX (D6)(RX на плеере)  
#define MP3_RX_PIN            (13U)                         // Определяет вывод RX (D7)(TX на плеере) программного последовательного порта
#define MP3_DEBUG
#ifdef MP3_TX_PIN
//#define CHECK_MP3_CONNECTION                              // Закоментируйте эту строку если нужно, чтобы лампа не проверяла наличие связи с МР3 плеером
//#define DF_PLAYER_IS_ORIGINAL                           // Если используеися плеер с чипом, отличным от AS20HGN402 ,закоментируйте эту строку
#ifndef  DF_PLAYER_IS_ORIGINAL
//#define DF_PLAYER_GD3200x                               // Якщо у вас такій чіп у плеєрі и цей рядок не закоментований, то для озвучування часу використовуються декілька мов
#endif  //DF_PLAYER_IS_ORIGINAL
#endif  //MP3_TX_PIN


// --- КНОПКА --------------------------
#define ESP_USE_BUTTON                                      // если строка не закомментирована, должна быть подключена кнопка (иначе лампа может регистрировать "фантомные" нажатия и некорректно устанавливать яркость)
#define BUTTON_IS_SENSORY       (1U)                        // если у вас не сенсорная, а обычная кнопка, поменяйте в этой строчке 1 на нолик 
#define BUTTON_LOCK_ON_START                                // с этой строкой, если в момент включения лампы в розетку успеть нажать кнопку, или если вы забудете кнопку подключить, лампа отключит реакцию на нажатие кнопки до следующего раза
// #define BTN_PIN                 (4U)                        // пин кнопки               (D2)
#define BTN_PIN                 (14U)                        // пин кнопки               (D5)
#define BUTTON_CAN_SET_SLEEP_TIMER                          // с этой строчкой, если на лампе набить кнопкой 5х кратно (один раз), лампа мигнёт дважды синим и если она выключена , то включится.                                                         
#ifdef  BUTTON_CAN_SET_SLEEP_TIMER
#define BUTTON_SET_SLEEP_TIMER1 (5U)                        // Таймер выключения лампы будет установлен на BUTTON_SET_SLEEP_TIMER1 минут.
#endif

// -------------------------------------
// #define MOSFET_PIN              (16U)
// #define MOSFET_PIN              (14U)                       // пин MOSFET транзистора (D5) - может быть использован для управления питанием матрицы/ленты (если раскомментировать строку)
//                                                             в оригинальной прошивке использовался (5U) - (D1) желаемо на ваш выбор
// #define MOSFET_LEVEL          (HIGH)                         // логический уровень, в который будет установлен пин MOSFET_PIN, когда матрица включена - HIGH или LOW (если раскомментировать)
//#define ALARM_PIN             (16U)                       // пин состояния будильника (D0) - может быть использован для управления каким-либо внешним устройством на время работы будильника (если раскомментировать)
//#define ALARM_LEVEL           (HIGH)                      // логический уровень, в который будет установлен пин ALARM_PIN, когда "рассвет"/будильник включен (если раскомментировать)

// === ФАЙЛОВА СИСТЕМА  ================
//#define USE_LittleFS                                      // Раскомментируйте эту строку, если вместо файловой системы SPIFFS хотите использовать файловую систему LittlFS
//                                                             на текущий момент предпочтительно использовать SPIFFS так как вы не будете видеть содержимое папок в LittlFS
//                                                             все остальное будет работать

// === LED МАТРИЦЯ =====================                       або одна матриця або стрічка типове підключення наприклад матриця (16х16) або стрічка
#define LED_PIN                  (0U)                        // пін управління матриці або стрічки
#define MATRIX_TYPE              (0U)                        // тип матриці: 0 - зигзаг, 1 - параллельна      (D3)

/* robot VII */
// #define WIDTH                   (26U)                       // ширина матриці
// #define HEIGHT                   (5U)                       // висота матриці
/* conflict library Adafruit_SSD1306 */
static const uint8_t WIDTH = 26;
static const uint8_t HEIGHT = 5;
#define COLOR_ORDER             (GRB)                        // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB
#define CONNECTION_ANGLE         (0U)                        // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#define STRIP_DIRECTION          (0U)                        // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
#define CURRENT_LIMIT          (500U)                        // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

//#define COMPOSIT_MATRIX                                     // виберіть потрібне на основі конструкції лампи
//                                                             дві або три матриці 8х32 | за замовчуванням ящо оригінальна конструкція «Javelin»
//                                                             наприклад для матриці 16х16 COMPOSIT_MATRIX треба закоментувати
//
// оригінальна конструкція лампи -------
// #define JAVELIN                                             // розкоментувати якщо повторюєте оригінальну схему лампи «JAVELIN»
#ifdef  JAVELIN
#define ROUND_MATRIX            (35U)                       // кількість led в матриці індикатора стану
#define LIGHT_MATRIX            (3U)                        // кількість led в матриці підсвітки
#define MATRIX_LEVEL            (128U)                      // яскраість led в матриці індикатора стану
#define BACKLIGHT_PIN           (5U)                        // пин (D1) - підсвітка WEMOS на монтажній платі (не обовязковый элемент)
#define OTA_PIN                 (12U)                       // пін індикатора активності OTA  (D6)
#define MB_LED_PIN              (2U)                        // пін blue led на платі Wemos    (D4)
#define BTN_JAVELIN_PIN         (13U)                       // пін механічної кнопки Javelin  (D7)
#endif

// при неправильной настройке матрицы вы получите предупреждение "Wrong matrix parameters! Set to default"
// шпаргалка по настройке матрицы здесь: https://alexgyver.ru/wp-content/uploads/2018/11/scheme3.jpg
// (но в ней, кажется, перепутаны местами квадратики в 4м столбце, а может, и ещё какие-то)

// --- LED СТРІЧКА ----------------------
// если у вас не обычная матрица, а сборка из ленты или гирлянда, и возникают непонятной природы артефакты в рисунке эффекта, попробуйте менять следующие три строчки
//#define FASTLED_ALLOW_INTERRUPTS      (1)                 // default: 1; // Use this to force FastLED to allow interrupts in the clockless chipsets (or to force it to disallow), overriding the default on platforms that support this. Set the value to 1 to allow interrupts or 0 to disallow them.
#define FASTLED_INTERRUPT_RETRY_COUNT   (0)                 // default: 2; // Use this to determine how many times FastLED will attempt to re-transmit a frame if interrupted for too long by interrupts
#define FASTLED_ESP8266_RAW_PIN_ORDER                       // FASTLED_ESP8266_RAW_PIN_ORDER, FASTLED_ESP8266_D1_PIN_ORDER or FASTLED_ESP8266_NODEMCU_PIN_ORDER
#ifdef USE_ROBOT_007
#define NUM_LEDS                (uint16_t)(WIDTH * HEIGHT) + 9
#else
#define NUM_LEDS                (uint16_t)(WIDTH * HEIGHT)
#endif
#define SEGMENTS                (1U)                        // диодов в одном "пикселе" (для создания матрицы из кусков ленты). вряд ли эффекты будут корректно работать, если изменить этот параметр

// === ПРОГРАМНЕ ЗАБЕЗПЕЧЕННЯ ===========
#define USE_ROUTER_ONLY                                     // не перемикатись в режим Access Point якщо вже було підключення до роутера при старті (актуально якщо довго стартуе роутер)
#define USE_SOUND_VISUALISER                                // візуалізатор звуку, закоментуйте якщо не використовуєте

// === РЕЖИМЫ РАБОТЫ  ===================
uint8_t SLEEP_TIMER             (3U);                       // Время автоотключения лампы (час)
#define USE_MULTIPLE_LAMPS_CONTROL                          // Включить код для управления несколькими лампами (workgroup)

// === ЕФЕКТИ ===========================
#define USE_RANDOM_SETS_IN_APP                              // раскоментируйте эту строчку, если в вашем приложении появилась поддержка команд "RND_"
//#define USE_TIME_EFFECT                                     // раскоментируйте эту строчку, если Хотите мспользовать еааусе часы (в файл effects4.json добавьте строчку {"n":"Чacы","v":[1,245,1,100,1]},)

// === НАЛАШТУВАННЯ ТЕКСТА З ПРОКРУТКОЮ =
#define LOW_FONT                                            // розмір шрифта 5Х5 якщо закоментувати рядок, тоді 5Х8
#define TEXT_DIRECTION          (1U)                        // 1 - по горизонтали, 0 - по вертикали
#define MIRR_V                  (0U)                        // отразить текст по вертикали (0 / 1)
#define MIRR_H                  (0U)                        // отразить текст по горизонтали (0 / 1)                                   
#define TEXT_HEIGHT             (0U)                        // высота, на которой бежит текст (от низа матрицы)
#define SPACE                   (1U)                        // пробел
#define LETTER_COLOR            (CRGB::White)               // цвет букв по умолчанию
// #define SHOW_IP_TO_START                                    // если строка не закомментирована, при удачном подключении к роутеру будет выводиться IP лампы


// === ЗОВНІШНЄ УПРАВЛІННЯ ==============
#define USE_CPU_USAGE
#define USE_MQTT              (false)                       // true - используется mqtt клиент, false - нет
#if USE_MQTT
static const char MqttServer[]         PROGMEM = "XXX.XXX.XXX.XXX";              // IP адреса MQTT брокера
static const uint16_t MqttPort                 = 8883U;                         // порт MQTT брокера
#define MQTT_RECONNECT_TIME    (10U)                         // время в секундах перед подключением к MQTT брокеру в случае потери подключения

/* для індефікації на стороні брокера наприклад щоб програмно ідефікувати пристрій можна бути програмно вирахувати по data з топіку
  IoT/ MqttClientIdPrefix[]  + "/" + chipID "/" наприклад виходячи з функціоналу "LedLamp/", "Sensors/", "Remote/", "Gibrid/"  */
static const char MqttClientIdPrefix[] PROGMEM = "Gibrid/"; // до нього буде додано унікальний ESP.getChipId
/* login password ялщо потрібно*/
static const char MqttUser[]           PROGMEM = "";                            // користувач MQTT брокера
static const char MqttPassword[]       PROGMEM = "";                            // пароль користувача MQTT брокера

#endif

// =====================================
// Developers Constants
// =====================================
// DEBUG -------------------------------
// #define GENERAL_DEBUG                                       // если строка не закомментирована, будут выводиться отладочные сообщения
#define OTA                                                 // если строка не закомментирована, модуль будет ждать два последовательных запроса пользователя на прошивку по воздуху (два четрёхкратных нажатия на кнопку)
#ifdef OTA
#define ESP_OTA_PORT          (8266U)                       // номер порта, который будет "прослушиваться" в ожидании команды прошивки по воздуху
#endif

#define GENERAL_DEBUG_TELNET  (true)                        // true - отладочные сообщения будут выводиться в telnet вместо Serial порта (для удалённой отладки без подключения usb кабелем)
#define TELNET_PORT           (23U)                         // номер telnet порта
#if (defined(GENERAL_DEBUG) && GENERAL_DEBUG_TELNET)
WiFiServer telnetServer(TELNET_PORT);                       // telnet сервер
WiFiClient telnet;                                          // обработчик событий telnet клиента
bool telnetGreetingShown = false;                           // признак "показано приветствие в telnet"
#define LOG                   telnet
#else
#define LOG                   Serial
#endif
//--------------------------------------
