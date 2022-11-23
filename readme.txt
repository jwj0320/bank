##환경설정

sample4.m을 {TMAXDIR}/config 에 넣고
1. cfl -i sample4.m
2. gst


-------------------------------------
1. cd fdl
2. fdlc -c -i mem.f
3. export FDLFILE={fdl path}
--------------------------------------
4. cd {server}
5. make server(프리컴파일, 컴파일, 서버로 복사)
--------------------------------------
6. cd {client}
7. make client 
--------------------------------------
./client
