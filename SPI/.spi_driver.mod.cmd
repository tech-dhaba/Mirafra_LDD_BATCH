savedcmd_/home/avinash/DD/SPI/spi_driver.mod := printf '%s\n'   spi_driver.o | awk '!x[$$0]++ { print("/home/avinash/DD/SPI/"$$0) }' > /home/avinash/DD/SPI/spi_driver.mod
