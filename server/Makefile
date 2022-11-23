TARGET_FILE		=server
SERVER_TBC_FILE	=${TARGET_FILE}.tbc
TARGET_C_FILE	=${TARGET_FILE}.c


SERVER_FILE		=banksvr
SVCTAB_FILE		=${SERVER_FILE}_svctab.c



server: 
	tbpc ${SERVER_TBC_FILE} INCLUDE=${TMAXDIR}
	gcc -I${TB_HOME}/client/include -I${TMAXDIR} -L${TMAXDIR}/lib -lsvr -lnodb -ltbs -ltbxa -L${TB_HOME}/client/lib -ltbertl ${TMAXDIR}/lib64/sdl.o  ${TARGET_C_FILE} ${TMAXDIR}/svct/${SVCTAB_FILE} -o ${SERVER_FILE}
	cp ${SERVER_FILE} ${TMAXDIR}/appbin/

