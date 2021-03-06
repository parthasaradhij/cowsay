#!/bin/sh


g++ -I../inc -o ../bin/cow cow.cpp shm_util.cpp 


g++ -I/usr/lib/jvm/java-8-oracle/include -I/usr/lib/jvm/java-8-oracle/include/linux/ -I../inc -c -Wall -fPIC  cow.cpp shm_util.cpp human.c 

g++ -shared -o libcowsay.so cow.o shm_util.o human.o 
mv libcowsay.so ../lib/

mv *.o ../bin

cd ../java

javac -d bin src/com/andium/cowsay/hci/CowSayConverter.java 


cd ..
/usr/lib/jvm/java-8-oracle/bin/java -Djava.library.path=./lib -Dfile.encoding=UTF-8 -classpath java/bin com.andium.cowsay.hci.CowSayConverter


