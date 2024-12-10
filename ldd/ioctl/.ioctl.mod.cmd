savedcmd_/home/tyr/ldd/ioctl/ioctl.mod := printf '%s\n'   ioctl.o | awk '!x[$$0]++ { print("/home/tyr/ldd/ioctl/"$$0) }' > /home/tyr/ldd/ioctl/ioctl.mod
