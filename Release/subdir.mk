################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Bullet.cpp \
../Colour.cpp \
../Helix.cpp \
../LedNode.cpp \
../Main.cpp \
../Node.cpp \
../VirtualNode.cpp 

OBJS += \
./Bullet.o \
./Colour.o \
./Helix.o \
./LedNode.o \
./Main.o \
./Node.o \
./VirtualNode.o 

CPP_DEPS += \
./Bullet.d \
./Colour.d \
./Helix.d \
./LedNode.d \
./Main.d \
./Node.d \
./VirtualNode.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/share/arduino/hardware/arduino/cores/arduino -I"/home/joost/Documents/TUDelft/Interactive Environments/Hangover/libraries/StandardCplusplus" -I/usr/share/arduino/libraries/SoftwareSerial/ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


