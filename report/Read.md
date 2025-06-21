git clone https://github.com/chart21/hpmpc.git
cd hpmpc
git submodule update --init --recursive
---
docker pull yourdockerhubusername/hpmpc:latest
---
docker run -it --network host --cap-add=NET_ADMIN `  -v "${PWD}:/hpmpc" `  --name p0 hpmpc
docker run -it --network host --cap-add=NET_ADMIN `  -v "${PWD}:/hpmpc" `  --name p1 hpmpc
docker run -it --network host --cap-add=NET_ADMIN `  -v "${PWD}:/hpmpc" `  --name p2 hpmpc
docker start -ai p0
---
(make clean)
p0：
make -j PARTY=0 FUNCTION_IDENTIFIER=74 PROTOCOL=5 MODELOWNER=P_0 DATAOWNER=P_1 NUM_INPUTS=40 BITLENGTH=32 DATTYPE=32
p1：
make -j PARTY=1 FUNCTION_IDENTIFIER=74 PROTOCOL=5 MODELOWNER=P_0 DATAOWNER=P_1 NUM_INPUTS=40 BITLENGTH=32 DATTYPE=32
p2：
make -j PARTY=2 FUNCTION_IDENTIFIER=74 PROTOCOL=5 MODELOWNER=P_0 DATAOWNER=P_1 NUM_INPUTS=40 BITLENGTH=32 DATTYPE=32
---
./executables/run-P0.o
./executables/run-P1.o
./executables/run-P2.o









