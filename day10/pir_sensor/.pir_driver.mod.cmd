savedcmd_/home/prince/linux_dd/day10/pir_sensor/pir_driver.mod := printf '%s\n'   pir_driver.o | awk '!x[$$0]++ { print("/home/prince/linux_dd/day10/pir_sensor/"$$0) }' > /home/prince/linux_dd/day10/pir_sensor/pir_driver.mod
