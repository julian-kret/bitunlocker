/*
 * Автономний пристрій для введення ключа розблокування BitLocker
 * Апаратна база: Arduino Leonardo (ATmega32U4).
 * 
 * ВИКОРИСТАННЯ
 * 
 * На ПК з збереженим ключем завантажити ключ на пристрій, підключивши його через USB та вивівши ключ у відповідний порт.
 * Під'єднати пристрій до ПК на якому потрібно ввести ключ та натиснути кнопку TYPE_BUTTON.
 * Кнопка RESET_BUTTON служить для видалення ключа з пристрою (працює лише при підключенні до ПК).
 * 
 * Завантаження ключа:
 *  Python: 
 *    python3 -c "import serial, time; s = serial.Serial('/dev/ttyACM0', 115200); s.dtr = True; time.sleep(0.5); s.write(b'XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX\n'); s.flush()"
 *  
 *  Bash:
 *    (stty 115200 raw -clocal -echo hupcl; sleep 0.5; echo "XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX") <>/dev/ttyACM0 >&0
 *    
 *  PowerShell:
 *    $p = New-Object System.IO.Ports.SerialPort("COM3", 115200); $p.DtrEnable = $true; $p.Open(); Start-Sleep -m 500; $p.WriteLine("XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX"); $p.Close()
 * 
*/


#include <Bounce2.h>
#include <Keyboard.h>
#include <EEPROM.h>


#define RESET_BUTTON_PIN 7 
#define TYPE_BUTTON_PIN 4

#define MAX_KEY_LENGTH 64

#define EEPROM_KEY_ADDRESS 0



class BitLockerUnlockKey {
  private:
    String key;

    void smartKeyboardPrintln(String text, int delayMs) {
      // 1. Посимвольно перебираємо весь рядок
      for (int i = 0; i < text.length(); i++) {
        Keyboard.write(text[i]); // Друкуємо один символ
        delay(delayMs);          // Чекаємо вказану кількість мілісекунд
      }
      
      // 2. Імітуємо натискання Enter в кінці (як це робить println)
      Keyboard.write(KEY_RETURN); 
      delay(delayMs);
    }


    void saveKey() {
      char buffer[MAX_KEY_LENGTH];

      key.toCharArray(buffer, MAX_KEY_LENGTH);
      
      EEPROM.put(EEPROM_KEY_ADDRESS, buffer);
      }


    void restoreKey() {
      char buffer[MAX_KEY_LENGTH];
   
      EEPROM.get(EEPROM_KEY_ADDRESS, buffer);
      buffer[MAX_KEY_LENGTH - 1] = '\0';
      key = buffer;
      }

  public:
    BitLockerUnlockKey() {
      Serial.begin(9600);
      Keyboard.begin(); 
    }
  
    void reset() {
      key = "";
      saveKey();
    }

    void readKey() {
      int bufferIndex = 0;
      
      if(!Serial) {
        return;
      }

      while (Serial.available() > 0) {
        key = Serial.readStringUntil('\n');
        key.trim();
        saveKey();
      }
    }


    void typeKey() {
      restoreKey();
      smartKeyboardPrintln(key,100); 
    }
};


BitLockerUnlockKey bit_locker_unlock_key = BitLockerUnlockKey();

Bounce2::Button reset_btn = Bounce2::Button();
Bounce2::Button type_btn = Bounce2::Button();

void setup() {
  // put your setup code here, to run once:

  delay(1000);  // Затримка для спрощеня перепрошивання без натиснення RESET
  
  reset_btn.attach(RESET_BUTTON_PIN, INPUT_PULLUP);
  reset_btn.interval(25);
  
  type_btn.attach(TYPE_BUTTON_PIN, INPUT_PULLUP);
  type_btn.interval(25);
}

int getBtnPressed() {
  int result;
  
  reset_btn.update();
  type_btn.update();

  if(reset_btn.pressed()){
    result = RESET_BUTTON_PIN; 
  }
  else if(type_btn.pressed()){
    result = TYPE_BUTTON_PIN;
  }
  else
    result = 0;

  return result;
}



void loop() {
  // put your main code here, to run repeatedly:

  int keyPressed = getBtnPressed();
 
  switch (keyPressed) {
    case RESET_BUTTON_PIN:
      bit_locker_unlock_key.reset();
      break;
    case TYPE_BUTTON_PIN:
      bit_locker_unlock_key.typeKey();
      break;
  }

  bit_locker_unlock_key.readKey();
  
}
