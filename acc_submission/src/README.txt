The objective of the small project is to show different programming skills - c, c++, java, scripting, library creation, linux internals, etc.


Important note:
---------------
The solution differs a small bit from the requirement

Requirement:
------------
______________    _______     ____    _____   _____________
| JVM process |<->| NL  | <->|IPC| <->| NL|<->|JVM Process|
--------------    -------    -----    -----   -------------
NL: Native Libraries 
NP: Native Program 

Java 8 is used

Implementation:
---------------
_____________    _______     ____    _____  
| JVM process |<->|  NL | <->|IPC| <->| NP|
--------------    -------    -----    -----

NL: Implemented in C with some C++ libraries. C++ compiler used for compilation/library building.
NP: C++ program

IPC mechanism:
--------------

Shared memory is chosen as the IPC between the processes, while semaphores are used to notify on the events of input/output readiness.


Shared memory (with semaphores) is chosen due to minimal latencies and the proecesses can be blocking (no other job)

Limitation:
With System V semaphores, the OS expects the semaphore operations to be symmetric(equal number of ups/downs on a sempahore) for the same process. Due to this, if the same process continuously does lock operation for 32768 times, the sem operation fails afterwards. Solution to this is not explored. Otherwise it is very much scalable solution. The next choice would be unix domain sockets. UDS would be more simpler to implement.


Build script:
-------------

Makefile is NOT added. A simple build script (which compiles each time, irrespective of the source code is altered or not) is added. The script builds JVM process, NL and NP and then launches all the applications. NP program is launched from the JVM process. The application waits for user input from the JVM process and passes to the NP. Once the native program processes the input, it returns the output via the IPC to JVM process. The JVM process prints the output and validates if it is as expected.

PATH: src/cowsay.sh

Testing:
--------
An API is written (which can be used in any other Java program) for testing different scenarios. Same is used to test the code, but commented in the final code.



Design documentation:

Java -> CowSayConverter.java - main progarm that launches CowSay converter, asks the user of the input and converts the human text into 'cow text' and validates if the output is as per the requirement.
NL -> human.c - writes the human text into the shared memory and signals 'cowsay' converter. Once the text is converted to 'cow say', the output is given back to the Java program
NP -> cow.cpp - converts the human text into cow text by reading the input in shared memory and writing the output in shared memory. Once the output is written, the human is signalled using semaphore.
shm_util.cpp - contains libraries for IPC



