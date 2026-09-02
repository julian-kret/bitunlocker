Автономний пристрій для введення ключа розблокування BitLocker
Апаратна база: Arduino Leonardo (ATmega32U4).
 
ВИКОРИСТАННЯ

На ПК з збереженим ключем завантажити ключ на пристрій, підключивши його через USB та вивівши ключ у відповідний порт.
Під'єднати пристрій до ПК на якому потрібно ввести ключ та натиснути кнопку TYPE_BUTTON.
Кнопка RESET_BUTTON служить для видалення ключа з пристрою (працює лише при підключенні до ПК).

Завантаження ключа:
 Python:
```python 
   python3 -c "import serial, time; s = serial.Serial('/dev/ttyACM0', 115200); s.dtr = True; time.sleep(0.5); s.write(b'XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX\n'); s.flush()"
 ```bash
 
 Bash:
```bash 
   (stty 115200 raw -clocal -echo hupcl; sleep 0.5; echo "XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX") <>/dev/ttyACM0 >&0
```bash

 PowerShell:
```powershell 
   $p = New-Object System.IO.Ports.SerialPort("COM3", 115200); $p.DtrEnable = $true; $p.Open(); Start-Sleep -m 500; $p.WriteLine("XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX-XXXXXX"); $p.Close()
```bash


