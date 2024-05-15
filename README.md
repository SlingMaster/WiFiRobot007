# WiFiRobot007
Arduino project IOT WiFi Robot ver 007

(EN)
## WiFi model of the robot "Robot 007", or "Botan" or simply "Professor Dowell's Head".
### Firmware WiFiRobot007»
### Version 6.XX 
Firmware for WiFi Robot 007 and cross-platform software for control.
The project combines robotics and a lamp created with addressable LEDs.
The purpose of the project is not only to join robotics but also to combine the design of the lamp with electronic components.

Link to firmware and software:
https://github.com/SlingMaster/WiFiRobot007.git

* To repeat, you can use the Instruction.doc file.
* The required libraries are located in the libraries archive.
* The sound files are located in the SD archive; you need to make a copy and paste them into the DFPlayer.

Schemes in the directory \Schemes

The following modules are used in the design:
Modules:
    * Wemos ESP8266; 
    * DFPlayer; 
    * 0.66 Inch OLED LED LCD Display Shield (compatible with WEMOS D1 MINI); 
    * TTP223 – touch button;
    * SG-90 – servo motor for head movement;
Sensors:
    * DHT-11 – for temperature and humidity measurement; 
    * TEMT6000 – for light measurement; 
    * SR-602 – for motion detection;

All applications for "WiFi Lamp Javelin" are compatible with the firmware for this project.

## Actions of the touch button for this firmware:
    * single click – turn on or off the lamp;
    * double click – next effect;
    * triple click – previous effect;
    * quadruple click – start effects playback in a loop;
    * fivefold click – start the fire effect (intimate setting);
    * sixfold click – start the lamp shutdown timer in 5 minutes;
    * sevenfold click – change the lamp operating mode: from WiFi access point to WiFi client or vice versa;
    * long press – increase or decrease the brightness;
    * single click and hold – increase or decrease the speed;
    * double click and hold – increase or decrease the scale;
    * hold when the lamp is off – turn on the "White Light" effect;

Additional videos about other Arduino projects can be found on the YouTube channel SlingMasterJSC, 
by following the link: 
https://www.youtube.com/@SlingMasterJSC

---

(UA)
## WiFi модель робота «Робот 007», або «Ботан» чи просто «Голова професора Доуеля». 
### Firmware «WiFiRobot007»
### Version 6.XX 
Прошивка для WiFi Робота 007 та кросплатформне програмне забезпечення для керування.
Проект поєднує робототехніку та лампу, створену на адресних світлодіодах.
Мета проекту не лише долучитися до робототехніки, але й поєднати дизайн лампи з електронними компонентами.

посилання на прошивку та програмне забезпечення:
https://github.com/SlingMaster/WiFiRobot007.git

* для повтору можна скористатись іструкцією Instruction.doc
* потрібні библіотеки знаходяться в архіві libraries 
* файли озвучки знаходяться в архіві SD, треба зробити копію та вставити в DFPlayer. 

схема в директорії \Schemes

В конструкції використовано
Модулі:
    * Wemos ESP8266; 
    * DFPlayer; 
    * 0.66 Inch OLED LED LCD Dispaly Shield (сумісний з WEMOS D1 MINI); 
    * TTP223 – сенсорна кнопка; 
    * SG-90 – сервопривод для управління рухом голови;
Сенсори:
    * DHT-11 – для вимірювання температури та вологості;
    * TEMT6000 – для вимірювання освітлення; 
    * SR-602 – для виявлення руху;


Всі додатки для "WiFi Lamp Javelin" сумісні з прошивкою для цього проекту.

## Дії сенсорної кнопки для цієї прошивки:
    * одноразовий клік – увімкнення або вимкнення світильника;
    * дворазовий клік – наступний ефект;
    * триразовий клік – попередній ефект;
    * чотириразовий клік – запуск відтворення ефеків у циклі;
    * п'ятиразовий клік – запуск ефекту вогонь (інтимна обстановка);
    * шестиразовий клік – запуск таймера вимкнення лампи через 5 хвилин;
    * семиразовий клік – зміна робочого режиму лампи: з WiFi точки доступу на WiFi клієнт чи навпаки;
    * утримання – збільшення чи зменшення "яскравості";
    * одноразовий клік та утримання – збільшення чи зменшення «швидкості»;
    * дворазовий клік та утримання – збільшення чи зменшення «масштабу»;
    * утримання при вимкненій лампі – включає ефект «Біле Світло»;

Додаткові відео про інші проекти Arduino можна знайти на YouTube-каналі SlingMasterJSC, 
перейшовши за посиланням: 
https://www.youtube.com/@SlingMasterJSC

---

## Change Log

(EN)

### Firmware WiFiRobot007»| 115 effects
### Version 6.00 
1. 115 effects for the lamp.
2. Sound accompaniment of some effects.
3. Sound and light behavior scenarios when motion and light sensors are triggered.
4. Indication of time, temperature, humidity, and modes on the OLED display.
5. Compatibility with applications:
    - Remote Control – cross-platform control software.
    - Color Music - sound visualization.
    - CPU Utilization - Windows application allows visualization of processor load on the lamp.
(UA)

### Firmware WiFiRobot007»| 115 ефектіd
### Версія 6.00 
1. 115 ефектів для світильника.
2. Звуковий супровід деяких ефектів.
3. звукові і свілові сценарії поведіки при спрацюванні датчика руху та освітлення.
4. Індикація часу, температури, вологості та режимів на OLED дисплеї.
5. Сумісність з додатками:
	- Remote Control – кросплатформне програмне забезпечення для керування.
	- Color Music - візуалізація звуку.
	- CPU Utilization - додаток для Windows дозволяє візуалізувати на лампі величину завантаження процесора.

---