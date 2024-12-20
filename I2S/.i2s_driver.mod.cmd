savedcmd_/home/avinash/DD/I2S/i2s_driver.mod := printf '%s\n'   i2s_driver.o | awk '!x[$$0]++ { print("/home/avinash/DD/I2S/"$$0) }' > /home/avinash/DD/I2S/i2s_driver.mod
