################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/HerderFState.cpp \
../src/HerderMState.cpp \
../src/HerderPState.cpp \
../src/JEngine.cpp \
../src/JRoute.cpp \
../src/JlibHerder.cpp \
../src/JlibSDL.cpp \
../src/TileManager.cpp 

OBJS += \
./src/HerderFState.o \
./src/HerderMState.o \
./src/HerderPState.o \
./src/JEngine.o \
./src/JRoute.o \
./src/JlibHerder.o \
./src/JlibSDL.o \
./src/TileManager.o 

CPP_DEPS += \
./src/HerderFState.d \
./src/HerderMState.d \
./src/HerderPState.d \
./src/JEngine.d \
./src/JRoute.d \
./src/JlibHerder.d \
./src/JlibSDL.d \
./src/TileManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


