# build executable from C++ files

all: aws.cpp client.cpp serverA.cpp serverB.cpp serverC.cpp monitor.cpp
	g++ -std=gnu++14 -o aws aws.cpp
	g++ -std=gnu++14 -o client client.cpp
	g++ -std=gnu++14 -o serverA serverA.cpp
	g++ -std=gnu++14 -o serverB serverB.cpp
	g++ -std=gnu++14 -o serverC serverC.cpp
	g++ -std=gnu++14 -o monitor monitor.cpp

.PHONY: aws
aws:aws
	./aws

.PHONY: serverA
serverA:serverA
	./serverA

.PHONY: serverB
serverB:serverB
	 ./serverB

.PHONY: serverC
serverC:serverC
	 ./serverC

.PHONY: monitor
monitor:monitor
	 ./monitor

.PHONY: clean
clean:
	rm -rf aws client serverA serverB serverC monitor

