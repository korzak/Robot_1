//                   БИБЛИОТЕКИ:
#include             <iarduino_HC_SR04_int.h>                       // подключаем библиотеку для работы с датчиком
//                   НОМЕРА ВЫВОДОВ:
const uint8_t        pinSensorL   = A3;                             // Вывод к которому подключен датчик находящийся слева         (по направлению движения)
const uint8_t        pinSensorC   = A4;                             // Вывод к которому подключен датчик находящийся по центру     (по направлению движения)
const uint8_t        pinSensorR   = A5;                             // Вывод к которому подключен датчик находящийся справа        (по направлению движения)
const uint8_t        pinSens_TRIG = 2;                              // Вывод к которому подключен датчик расстояния HC_SR04        (вывод обозначенный на датчике как TRIG)
const uint8_t        pinSens_ECHO = 3;                              // Вывод к которому подключен датчик расстояния HC_SR04        (вывод обозначенный на датчике как ECHO)
const uint8_t        pinShield_LH = 7;                              // Вывод направления к которому подключен левый мотор          (по направлению движения)
const uint8_t        pinShield_LE = 6;                              // Вывод ШИМ         к которому подключен левый мотор          (по направлению движения)
const uint8_t        pinShield_RE = 5;                              // Вывод ШИМ         к которому подключен левый мотор          (по направлению движения)
const uint8_t        pinShield_RH = 4;                              // Вывод направления к которому подключен левый мотор          (по направлению движения)
//                   ОБЪЕКТЫ:
iarduino_HC_SR04_int hcsr(pinSens_TRIG, pinSens_ECHO);              // Объект hcsr для работы с библиотекой iarduino_HC_SR04       (вывод TRIG, вывод ECHO)
//                   УСТАНАВЛИВАЕМЫЕ ЗНАЧЕНИЯ:
const uint16_t       valSensor1   = 930;                            // Показание датчика находящегося на линии                     (указывается для конкретной трассы)
const uint16_t       valSensor0   = 730;                            // Показание датчика находящегося вне линии                    (указывается для конкретной трассы)
const uint8_t        valSpeed     = 255;                            // Максимальная скорость                                       (число от 1 до 255)
const uint32_t       tmrDelay     = 2000;                           // Время в течении которого требуется остановиться             (если в течении этого времени состояние остаётся неопределённым (линия не обнаружена), то требуется остановиться)
const uint8_t        valTurning   = 10;                             // Крутизна поворотов (скорость реакции)                       (число от 1 до 255)
const uint8_t        valDistance  = 10;                             // Минимально допустимое расстояние до объекта в сантиметрах   (если расстояние будет меньше, то требуется остановитьтся)
const bool           arrRoute[2]  = {1,1};                          // Направление движения для каждого мотора                     (зависит от полярности,  нулевой элемент - правый мотор, первый элемент - левый мотор)
//                   РАССЧИТЫВАЕМЫЕ ЗНАЧЕНИЯ:
      uint8_t        arrSpeed[2];                                   // Рассчитываемая скорость для каждого мотора                  (число от 1 до valSpeed, нулевой элемент - правый мотор, первый элемент - левый мотор)
      uint16_t       valSensorM;                                    // Рассчитываемое среднее значение датчика                     (значение между цветом линии и цветом вне линии)
      uint8_t        valSensor;                                     // Биты рассчитываемых логических уровней всех датчиков        (0bxxxxxLCR)
      bool           flgLine;                                       // Флаг указывающий на то, что используется светлая линия      (0 - тёмная линия, 1 - светлая линия)
      int8_t         flgTurning;                                    // Флаг наличия и направления поворота                         (0 - не поворачиваем, -1 - поворачиваем налево, +1 - поворачиваем направо)
      bool           flgPWM;                                        // Флаг указывающий на то, что требуется изменить ШИМ моторов  (0 - тёмная линия, 1 - светлая линия)
      bool           flgStop;                                       // Флаг указывающий на необходимость остановиться              (0 - без остановки, 1 - требуется остановиться)
      bool           flgDistance;                                   // Флаг обнаружения препятствия                                (0 - не обнаружено, 1 - обнаружено)
      uint32_t       tmrMillis;                                     // Время совершения последней операции                         (в миллисекундах)
void setup(){
//  Узнаём цвет линии используемой на трассе, если он светлый, то устанавливаем флаг lineColor тёмный
    flgLine = (valSensor0>valSensor1);                              // Если условие (valSensor0>valSensor1) выполняется значит линия светлая и флаг flgLine установится в 1, иначе он сбросится в 0
//  Вычисляем среднее значение между показаниями датчиков на линии и все линии
    if(flgLine){valSensorM=valSensor1+(valSensor0-valSensor1)/2;}   // Если на трассе используется светлая линия
    else       {valSensorM=valSensor0+(valSensor1-valSensor0)/2;}   // Если на трассе используется тёмная линия
//  Устанавливаем значение скорости обоих моторов
    arrSpeed[1]=valSpeed;                                           // Максимальная скорость на левом моторе
    arrSpeed[0]=valSpeed;                                           // Максимальная скорость на правом моторе
//  Устанавливаем флаг ШИМ, сбрасываем флаг наличия поворота, флаг остановки и флаг обнаружения припятствий
    flgPWM=1; flgTurning=0; flgStop=0; flgDistance=0;
//  Устанавливаем режим работы выводов и направление обоих моторов
    pinMode     (pinSensorL,   INPUT      );                        // Конфигурируем вывод pinSensorL   как вход                   (для получения данных от левого датчика линии)
    pinMode     (pinSensorC,   INPUT      );                        // Конфигурируем вывод pinSensorC   как вход                   (для получения данных от центрального датчика линии)
    pinMode     (pinSensorR,   INPUT      );                        // Конфигурируем вывод pinSensorR   как вход                   (для получения данных от правого датчика линии)
    pinMode     (pinShield_LH, OUTPUT     );                        // Конфигурируем вывод pinShield_LH как выход                  (для управления направлением движения левого мотора)
    pinMode     (pinShield_LE, OUTPUT     );                        // Конфигурируем вывод pinShield_LE как выход                  (для управления скоростью вращения левого мотора, при помощи ШИМ)
    pinMode     (pinShield_RE, OUTPUT     );                        // Конфигурируем вывод pinShield_RE как выход                  (для управления скоростью вращения правого мотора, при помощи ШИМ)
    pinMode     (pinShield_RH, OUTPUT     );                        // Конфигурируем вывод pinShield_RH как выход                  (для управления направлением движения правого мотора)
    digitalWrite(pinShield_LH, arrRoute[1]);                        // Устанавливаем на выходе pinShield_LH уровень arrRoute[1]    (направление движения левого мотора)
    digitalWrite(pinShield_RH, arrRoute[0]);                        // Устанавливаем на выходе pinShield_RH уровень arrRoute[0]    (направление движения правого мотора)
//  Выводим показания центрального датчика линии
    Serial.begin(9600); while(!Serial){}                            // Инициируем передачу данных по последовательному порту       (на скорости 9600 бит/сек)
    Serial.println(analogRead(pinSensorC));                         // Выводим показания центрального датчика линии                (для указания значений константам valSensor0 и valSensor1)
//  Устанавливаем задержку и обновляем время совершения последней операции
    delay(2000);
    tmrMillis = millis();
}
void loop(){
//  Читаем показания датчиков и преобразуем их в логические уровни  // (1 - датчик на линии, 0 - датчик вне линии)
    valSensor  =  0;                                                // сбрасываем все биты переменной valSensor
    valSensor |= ((analogRead(pinSensorL)>valSensorM)^flgLine)<<2;  // Устанавливаем 2 бит переменной valSensor в 1 если левый   датчик находится на линии, иначе оставляем бит сброшенным в 0
    valSensor |= ((analogRead(pinSensorC)>valSensorM)^flgLine)<<1;  // Устанавливаем 1 бит переменной valSensor в 1 если средний датчик находится на линии, иначе оставляем бит сброшенным в 0
    valSensor |= ((analogRead(pinSensorR)>valSensorM)^flgLine)<<0;  // Устанавливаем 0 бит переменной valSensor в 1 если правый  датчик находится на линии, иначе оставляем бит сброшенным в 0
                                                                    // РАССМОТРИМ ТРИ ПРЕДЫДУЩИЕ СТРОКИ: Каждая строка устанавливает или сбрасывает свой бит переменной valSensor в зависимости от того, находится датчик на линии или нет.
                                                                    // Оператор составного побитового ИЛИ "|=" выполнит побитовое ИЛИ между переменной valSensor и результатом всех вычислений, полученное значение запишется в valSensor.
                                                                    // Оператор сравнения ">" вернет "1" если значение analogRead(номер_вывода) больше чем значение valSensorM, а значит датчик находится над объектом, который темнее чем значение valSensorM,
                                                                    // Результат возвращённый оператором сравнения ">" нам подходит если используется тёмная линия, но если используется светлая линия, то результат нужно инвертировать ...
                                                                    // Оператор побитового XOR "^" выполнит эту инверсию, только если установлен флаг flgLine, указывающий о том, что используется светлая линия
                                                                    // Оператор побитового сдвига влево "<<" сдвинет результат на указанное число бит, таким образом каждый результат займет свою позицию.
//  Определяем действия в соответствии с текущим положением датчиков
    switch(valSensor){ //            Сохраняем время:    Меняем флаг ШИМ:   Меняем флаг поворота:  Меняем флаг остановки:
        case 0b000:                                      flgPWM=flgTurning;                        flgStop=!flgTurning; break;  // 000 - Если все датчики находятся вне линии                 (неопределённое состояние - если до этого мы не поворачивали, то резко останавливаемся, а если поворачивали, то продолжаем поворот в ту же сторону)
        case 0b010:                  tmrMillis=millis(); flgPWM=flgTurning; flgTurning=0;          flgStop=0;           break;  // 010 - Если только центральный датчик находится на линии    (продолжаем движение прямо)
        case 0b100:                                                                                                             // 100 - Если только левый датчик находится на линии          (поворачиваем влево)
        case 0b110:                  tmrMillis=millis(); flgPWM=1;          flgTurning=-1;         flgStop=0;           break;  // 110 - Если левый и центральный датчики находятся на линии  (поворачиваем влево)
        case 0b001:                                                                                                             // 001 - Если только правый датчик находится на линии         (поворачиваем вправо)
        case 0b011:                  tmrMillis=millis(); flgPWM=1;          flgTurning=1;          flgStop=0;           break;  // 011 - Если правый и центральный датчики находятся на линии (поворачиваем вправо)
        default:                                         flgPWM=1;                                                      break;  // 1x1 - Если правый и левый датчики находятся на линии       (неопределённое состояние)
    }
    if(tmrMillis>millis())         { tmrMillis=0;}                                                                              // Избавляемся от переполнения millis();
    if(tmrMillis+tmrDelay<millis()){                     flgPWM=1;          flgTurning=0;          flgStop=1;           }       // Останавливаемся если линия потеряна на более чем tmrDelay мс
    if(hcsr.distance()<valDistance){ tmrMillis=millis(); flgPWM=1;          flgDistance=1;                              }       // Останавливаемся если обнаружено препятствие
    else if(flgDistance)           { tmrMillis=millis(); flgPWM=1;          flgDistance=0;                              }       // Продолжаем движение если препятствие исчезло
//  Устанавливаем ШИМ для моторов
    if(flgPWM){flgPWM=0;                                                                                                        // Если установлен флаг flgPWM, то сбрасываем его и устанавливаем ШИМ ...
      switch(flgTurning){ // Скорость левого мотора:           Скорость правого мотора:
        case -1:             if(arrSpeed[1]>0){arrSpeed[1]--;} arrSpeed[0]=valSpeed;                                    break;  // Уменьшаем скорость левого  мотора (поворачиваем налево)
        case  0:             arrSpeed[1]=valSpeed;             arrSpeed[0]=valSpeed;                                    break;  // Устанавливаем одинаковую скорость (едим прямо)
        case  1:             arrSpeed[1]=valSpeed;             if(arrSpeed[0]>0){arrSpeed[0]--;}                        break;  // Уменьшаем скорость правого мотора (поворачиваем направо)
      } if(flgStop){         arrSpeed[1]=0;                    arrSpeed[0]=0;}                                                  // Останавливаемся если установлен флаг flgStop
        if(flgDistance){     arrSpeed[1]=0;                    arrSpeed[0]=0;}                                                  // Останавливаемся если установлен флаг flgDistance
//      Выводим ШИМ
        analogWrite(pinShield_LE, arrSpeed[1]);
        analogWrite(pinShield_RE, arrSpeed[0]);
    }
}
