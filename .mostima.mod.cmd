savedcmd_mostima.mod := printf '%s\n'   mostima.o | awk '!x[$$0]++ { print("./"$$0) }' > mostima.mod
