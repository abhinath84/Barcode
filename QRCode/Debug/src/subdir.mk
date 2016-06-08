################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/main.cpp \
../src/qrbitbuffer.cpp \
../src/qrcode.cpp \
../src/qrdataencode.cpp \
../src/qrsegment.cpp \
../src/qrutility.cpp 

OBJS += \
./src/main.o \
./src/qrbitbuffer.o \
./src/qrcode.o \
./src/qrdataencode.o \
./src/qrsegment.o \
./src/qrutility.o 

CPP_DEPS += \
./src/main.d \
./src/qrbitbuffer.d \
./src/qrcode.d \
./src/qrdataencode.d \
./src/qrsegment.d \
./src/qrutility.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


