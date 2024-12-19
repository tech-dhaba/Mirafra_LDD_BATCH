savedcmd_/home/tyr/ldd/spi/spi_driver.mod := printf '%s\n'   spi_driver.o | awk '!x[$$0]++ { print("/home/tyr/ldd/spi/"$$0) }' > /home/tyr/ldd/spi/spi_driver.mod
