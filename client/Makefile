TARGET_FILE		=client
TARGET_C_FILE	=${TARGET_FILE}.c

.PHONY: client

client:
	gcc -I${TMAXDIR} -L${TMAXDIR}/lib  -lcli  ${TARGET_C_FILE} -o ${TARGET_FILE}
