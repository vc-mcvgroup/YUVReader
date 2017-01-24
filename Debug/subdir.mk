################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Operations.cpp \
../TAppEncCfg.cpp \
../TAppEncTop.cpp \
../TComChromaFormat.cpp \
../TComPicYuv.cpp \
../TVideoIOYuv.cpp \
../encmain.cpp \
../program_options_lite.cpp 

OBJS += \
./Operations.o \
./TAppEncCfg.o \
./TAppEncTop.o \
./TComChromaFormat.o \
./TComPicYuv.o \
./TVideoIOYuv.o \
./encmain.o \
./program_options_lite.o 

CPP_DEPS += \
./Operations.d \
./TAppEncCfg.d \
./TAppEncTop.d \
./TComChromaFormat.d \
./TComPicYuv.d \
./TVideoIOYuv.d \
./encmain.d \
./program_options_lite.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


