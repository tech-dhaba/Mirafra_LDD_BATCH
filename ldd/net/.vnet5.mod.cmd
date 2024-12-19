savedcmd_/home/tyr/ldd/net/vnet5.mod := printf '%s\n'   vnet5.o | awk '!x[$$0]++ { print("/home/tyr/ldd/net/"$$0) }' > /home/tyr/ldd/net/vnet5.mod
