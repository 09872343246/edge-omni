savedcmd_sht30.mod := printf '%s\n'   sht30.o | awk '!x[$$0]++ { print("./"$$0) }' > sht30.mod
