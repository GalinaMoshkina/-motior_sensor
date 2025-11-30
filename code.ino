#include "FastLED.h" // загрузили библиотеку FastLed для управления адресными светодиодами

#define NUM_LEDS 50 // Тут указывается количество светодиодов в ленте
#define PIN 12 // PIN подключений к гирлянде (константа)
// unsigned long timing; // Переменная для хранения точки отсчета

CRGB leds[NUM_LEDS]; //сообщаем библиотеке количество пикселей(светодиодов)
byte baza = 0; //переменная byte для реализации одного из эффектов

bool wait = true; //переменная boolean для управления количеством сообщений
int ledcase = 0; //переменная для управления режимами 0-4

void setup() {
  Serial.begin(9600); //активируем серийный порт на скорости 9600
  FastLED.addLeds<WS2812, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip ); //данные для управления гирляндой, используемые библиотекой fastled
  FastLED.setBrightness(50); // устанавливаем яркость светодиодов на гирлянде
  // pinMode(PIN_PIR, INPUT); //устанавливаем PIN датчика в положение чтения данных
  pinMode(PIN, OUTPUT); // устанавливаем PIN гирлянды в положение отправки данных
  pinMode(13, OUTPUT); // устанавливаем PIN 13 ардуино для визуализации срабатывания датчика (на 13 PIN подключен светодиод на плате Arduino)
  FastLED.clear(); //очищаем ленту
  FastLED.show(); //загружаем данные в ленту
  attachInterrupt(1, btnIsr, RISING); // вводим прерывание на 3м пине ардуино для считывания показателей датчика движения независимо от точки программы
  Serial.println("Program started"); //сообщаем в серийный порт, что программа запущена
}

void loop() { //основной цикл
  switch (ledcase) {//используем оператор выбора для переключения состояний гирлянды при обнаружении движения
    case 0:
      if (wait) { //используем условный оператор if чтобы сообщение было отправлено в серийный порт только 1 раз
        Serial.println("wait");
        wait = false; //изменяем значение переменной
      }
      break;
    case 1:
      if (wait == false){
        ledRed(); // запускаем функцию ленты - включить 1 цвет
        wait = true;
      }
      break; //обязательное условие оператора, а то не будет проверять остальные условия
    case 2:
      if (wait) {
        ledRainbow(); // запускаем функцию ленты - радуга
        wait = false;
      }
      break;
    case 3:
      if (wait == false){
        Serial.println("LED RUN");
        wait = true;
      }
      ledRun();
      // запускаем функцию ленты, включить 1 цвет// запускаем функцию ленты - бегущий огонь
      break;
    case 4:
      ledOff(); // запускаем функцию выключения ленты
      ledcase = 0;
      break;
  }
}

void ledOff() {//функция выключения гирлянды
  digitalWrite(13, 1); // включаем сигнальный светодиод на ардуино
  FastLED.clear(); //очищаем ленту
  FastLED.show(); //загружаем данные в ленту
  digitalWrite(13, 0); // выключаем диод на ардуино
  Serial.println("LED OFF");
}

void btnIsr() {
  Serial.println("Motion detected");
  if (ledcase < 5){
    ledcase++; // увеличиваем значение переменной ledcase на 1
    FastLED.clear(); //очищаем ленту
    FastLED.show();
  }
}

void ledRed() { // функции включения гирлянды
  FastLED.clear(); //очищаем ленту
  for (int i = 0; i < NUM_LEDS; i++) { /*для получения эффекта мы используем цикл FOR,
    который позволяет выполнять одинаковые действия определенное количество раз
    никак начинается с присвоения переменной значения 0 и при последующем проходе, каждый раз увеличивая переменную на 1(i+)
    пока не достигнет значения переменной NUM_LED*/
    leds[i] = CRGB::Green; // тут командуем, какой включить свет
  }
  Serial.println("LED RED");
  FastLED.show(); //загружаем данные в ленту
}

void ledRainbow() {
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CHSV(baza + i * 5, 255, 255);
  baza++;
  FastLED.show();
  delay(20);
  Serial.println("LED RAINBOW");
}

void ledRun() {
  fadeToBlackBy(leds, NUM_LEDS, 2);
  int pos = beatsin16(13, 0, NUM_LEDS - 1);
  leds[pos] += CHSV(baza++, 255, 192);
  FastLED.show();
  delay(30);
}
