#!/bin/bash

# Автоматизація введення коду розблокування BitLocker
#
# Використання:
# 	./BitUnLocker.bash "Ключ відновлення BitLocker XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX.TXT" /dev/ttyXXXX
# 	де
# 		- "Ключ відновлення BitLocker XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX.TXT" - ім'я файлу, у який 
# 		  BitLocker збергігає ключ відновлення;
#
# 		- /dev/ttyXXXX - послідовний порт, до якого підключено пристрій.
#

# Отримання ключа з файлу (параметр 1)
KEY=$(tr -d '\0\r\n' < "$1" |
      grep -Eo '([0-9]{6}[^0-9]+){7}[0-9]{6}' |
      sed 's/[^0-9]/-/g')

# Вивід ключа у порт (параметр 2)
(stty 115200 raw -clocal -echo hupcl; sleep 0.5; echo "$KEY") <>"$2" >&0

