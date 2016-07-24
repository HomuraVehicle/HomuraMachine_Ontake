#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/hmrM2500.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/hmrM2500.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../amPic30_v7.c ../amPic30Extend.c ../hmrBattery.c ../hmrCO2.c ../hmrCom.c ../hmrData.c ../hmrDevice.c ../hmrDeviceManage.c ../hmrDeviceManageMessage.c ../hmrDrive.c ../hmrGPS.c ../hmrInertial.c ../hmrM2500.c ../hmrMessage.c ../hmrSprite.c ../hmrSpriteBuffer.c ../hmrSpriteInterface.c ../hmrTask.c ../hmrThermo.c ../hmrUtility.c ../hmrVMC1.c ../hmLib_v3_04/cdeque.c ../hmLib_v3_04/cqueue.c ../hmLib_v3_04/cstring.c ../hmrBufLogger.c C:/Users/homura/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500_Ontake/hmrDevice_kk08.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/amPic30_v7.o ${OBJECTDIR}/_ext/1472/amPic30Extend.o ${OBJECTDIR}/_ext/1472/hmrBattery.o ${OBJECTDIR}/_ext/1472/hmrCO2.o ${OBJECTDIR}/_ext/1472/hmrCom.o ${OBJECTDIR}/_ext/1472/hmrData.o ${OBJECTDIR}/_ext/1472/hmrDevice.o ${OBJECTDIR}/_ext/1472/hmrDeviceManage.o ${OBJECTDIR}/_ext/1472/hmrDeviceManageMessage.o ${OBJECTDIR}/_ext/1472/hmrDrive.o ${OBJECTDIR}/_ext/1472/hmrGPS.o ${OBJECTDIR}/_ext/1472/hmrInertial.o ${OBJECTDIR}/_ext/1472/hmrM2500.o ${OBJECTDIR}/_ext/1472/hmrMessage.o ${OBJECTDIR}/_ext/1472/hmrSprite.o ${OBJECTDIR}/_ext/1472/hmrSpriteBuffer.o ${OBJECTDIR}/_ext/1472/hmrSpriteInterface.o ${OBJECTDIR}/_ext/1472/hmrTask.o ${OBJECTDIR}/_ext/1472/hmrThermo.o ${OBJECTDIR}/_ext/1472/hmrUtility.o ${OBJECTDIR}/_ext/1472/hmrVMC1.o ${OBJECTDIR}/_ext/474781832/cdeque.o ${OBJECTDIR}/_ext/474781832/cqueue.o ${OBJECTDIR}/_ext/474781832/cstring.o ${OBJECTDIR}/_ext/1472/hmrBufLogger.o ${OBJECTDIR}/_ext/2062917857/hmrDevice_kk08.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/amPic30_v7.o.d ${OBJECTDIR}/_ext/1472/amPic30Extend.o.d ${OBJECTDIR}/_ext/1472/hmrBattery.o.d ${OBJECTDIR}/_ext/1472/hmrCO2.o.d ${OBJECTDIR}/_ext/1472/hmrCom.o.d ${OBJECTDIR}/_ext/1472/hmrData.o.d ${OBJECTDIR}/_ext/1472/hmrDevice.o.d ${OBJECTDIR}/_ext/1472/hmrDeviceManage.o.d ${OBJECTDIR}/_ext/1472/hmrDeviceManageMessage.o.d ${OBJECTDIR}/_ext/1472/hmrDrive.o.d ${OBJECTDIR}/_ext/1472/hmrGPS.o.d ${OBJECTDIR}/_ext/1472/hmrInertial.o.d ${OBJECTDIR}/_ext/1472/hmrM2500.o.d ${OBJECTDIR}/_ext/1472/hmrMessage.o.d ${OBJECTDIR}/_ext/1472/hmrSprite.o.d ${OBJECTDIR}/_ext/1472/hmrSpriteBuffer.o.d ${OBJECTDIR}/_ext/1472/hmrSpriteInterface.o.d ${OBJECTDIR}/_ext/1472/hmrTask.o.d ${OBJECTDIR}/_ext/1472/hmrThermo.o.d ${OBJECTDIR}/_ext/1472/hmrUtility.o.d ${OBJECTDIR}/_ext/1472/hmrVMC1.o.d ${OBJECTDIR}/_ext/474781832/cdeque.o.d ${OBJECTDIR}/_ext/474781832/cqueue.o.d ${OBJECTDIR}/_ext/474781832/cstring.o.d ${OBJECTDIR}/_ext/1472/hmrBufLogger.o.d ${OBJECTDIR}/_ext/2062917857/hmrDevice_kk08.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/amPic30_v7.o ${OBJECTDIR}/_ext/1472/amPic30Extend.o ${OBJECTDIR}/_ext/1472/hmrBattery.o ${OBJECTDIR}/_ext/1472/hmrCO2.o ${OBJECTDIR}/_ext/1472/hmrCom.o ${OBJECTDIR}/_ext/1472/hmrData.o ${OBJECTDIR}/_ext/1472/hmrDevice.o ${OBJECTDIR}/_ext/1472/hmrDeviceManage.o ${OBJECTDIR}/_ext/1472/hmrDeviceManageMessage.o ${OBJECTDIR}/_ext/1472/hmrDrive.o ${OBJECTDIR}/_ext/1472/hmrGPS.o ${OBJECTDIR}/_ext/1472/hmrInertial.o ${OBJECTDIR}/_ext/1472/hmrM2500.o ${OBJECTDIR}/_ext/1472/hmrMessage.o ${OBJECTDIR}/_ext/1472/hmrSprite.o ${OBJECTDIR}/_ext/1472/hmrSpriteBuffer.o ${OBJECTDIR}/_ext/1472/hmrSpriteInterface.o ${OBJECTDIR}/_ext/1472/hmrTask.o ${OBJECTDIR}/_ext/1472/hmrThermo.o ${OBJECTDIR}/_ext/1472/hmrUtility.o ${OBJECTDIR}/_ext/1472/hmrVMC1.o ${OBJECTDIR}/_ext/474781832/cdeque.o ${OBJECTDIR}/_ext/474781832/cqueue.o ${OBJECTDIR}/_ext/474781832/cstring.o ${OBJECTDIR}/_ext/1472/hmrBufLogger.o ${OBJECTDIR}/_ext/2062917857/hmrDevice_kk08.o

# Source Files
SOURCEFILES=../amPic30_v7.c ../amPic30Extend.c ../hmrBattery.c ../hmrCO2.c ../hmrCom.c ../hmrData.c ../hmrDevice.c ../hmrDeviceManage.c ../hmrDeviceManageMessage.c ../hmrDrive.c ../hmrGPS.c ../hmrInertial.c ../hmrM2500.c ../hmrMessage.c ../hmrSprite.c ../hmrSpriteBuffer.c ../hmrSpriteInterface.c ../hmrTask.c ../hmrThermo.c ../hmrUtility.c ../hmrVMC1.c ../hmLib_v3_04/cdeque.c ../hmLib_v3_04/cqueue.c ../hmLib_v3_04/cstring.c ../hmrBufLogger.c C:/Users/homura/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500_Ontake/hmrDevice_kk08.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/hmrM2500.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24EP512GU810
MP_LINKER_FILE_OPTION=,--script=p24EP512GU810.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/amPic30_v7.o: ../amPic30_v7.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/amPic30_v7.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/amPic30_v7.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../amPic30_v7.c  -o ${OBJECTDIR}/_ext/1472/amPic30_v7.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/amPic30_v7.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/amPic30_v7.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/amPic30Extend.o: ../amPic30Extend.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/amPic30Extend.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/amPic30Extend.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../amPic30Extend.c  -o ${OBJECTDIR}/_ext/1472/amPic30Extend.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/amPic30Extend.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/amPic30Extend.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrBattery.o: ../hmrBattery.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrBattery.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrBattery.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrBattery.c  -o ${OBJECTDIR}/_ext/1472/hmrBattery.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrBattery.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrBattery.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrCO2.o: ../hmrCO2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrCO2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrCO2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrCO2.c  -o ${OBJECTDIR}/_ext/1472/hmrCO2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrCO2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrCO2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrCom.o: ../hmrCom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrCom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrCom.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrCom.c  -o ${OBJECTDIR}/_ext/1472/hmrCom.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrCom.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrCom.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrData.o: ../hmrData.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrData.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrData.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrData.c  -o ${OBJECTDIR}/_ext/1472/hmrData.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrData.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrData.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrDevice.o: ../hmrDevice.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrDevice.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrDevice.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrDevice.c  -o ${OBJECTDIR}/_ext/1472/hmrDevice.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrDevice.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrDevice.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrDeviceManage.o: ../hmrDeviceManage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrDeviceManage.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrDeviceManage.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrDeviceManage.c  -o ${OBJECTDIR}/_ext/1472/hmrDeviceManage.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrDeviceManage.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrDeviceManage.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrDeviceManageMessage.o: ../hmrDeviceManageMessage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrDeviceManageMessage.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrDeviceManageMessage.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrDeviceManageMessage.c  -o ${OBJECTDIR}/_ext/1472/hmrDeviceManageMessage.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrDeviceManageMessage.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrDeviceManageMessage.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrDrive.o: ../hmrDrive.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrDrive.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrDrive.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrDrive.c  -o ${OBJECTDIR}/_ext/1472/hmrDrive.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrDrive.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrDrive.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrGPS.o: ../hmrGPS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrGPS.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrGPS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrGPS.c  -o ${OBJECTDIR}/_ext/1472/hmrGPS.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrGPS.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrGPS.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrInertial.o: ../hmrInertial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrInertial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrInertial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrInertial.c  -o ${OBJECTDIR}/_ext/1472/hmrInertial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrInertial.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrInertial.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrM2500.o: ../hmrM2500.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrM2500.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrM2500.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrM2500.c  -o ${OBJECTDIR}/_ext/1472/hmrM2500.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrM2500.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrM2500.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrMessage.o: ../hmrMessage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrMessage.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrMessage.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrMessage.c  -o ${OBJECTDIR}/_ext/1472/hmrMessage.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrMessage.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrMessage.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrSprite.o: ../hmrSprite.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrSprite.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrSprite.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrSprite.c  -o ${OBJECTDIR}/_ext/1472/hmrSprite.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrSprite.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrSprite.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrSpriteBuffer.o: ../hmrSpriteBuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrSpriteBuffer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrSpriteBuffer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrSpriteBuffer.c  -o ${OBJECTDIR}/_ext/1472/hmrSpriteBuffer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrSpriteBuffer.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrSpriteBuffer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrSpriteInterface.o: ../hmrSpriteInterface.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrSpriteInterface.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrSpriteInterface.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrSpriteInterface.c  -o ${OBJECTDIR}/_ext/1472/hmrSpriteInterface.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrSpriteInterface.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrSpriteInterface.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrTask.o: ../hmrTask.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrTask.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrTask.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrTask.c  -o ${OBJECTDIR}/_ext/1472/hmrTask.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrTask.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrTask.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrThermo.o: ../hmrThermo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrThermo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrThermo.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrThermo.c  -o ${OBJECTDIR}/_ext/1472/hmrThermo.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrThermo.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrThermo.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrUtility.o: ../hmrUtility.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrUtility.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrUtility.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrUtility.c  -o ${OBJECTDIR}/_ext/1472/hmrUtility.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrUtility.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrUtility.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrVMC1.o: ../hmrVMC1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrVMC1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrVMC1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrVMC1.c  -o ${OBJECTDIR}/_ext/1472/hmrVMC1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrVMC1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrVMC1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/474781832/cdeque.o: ../hmLib_v3_04/cdeque.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/474781832" 
	@${RM} ${OBJECTDIR}/_ext/474781832/cdeque.o.d 
	@${RM} ${OBJECTDIR}/_ext/474781832/cdeque.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmLib_v3_04/cdeque.c  -o ${OBJECTDIR}/_ext/474781832/cdeque.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/474781832/cdeque.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/474781832/cdeque.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/474781832/cqueue.o: ../hmLib_v3_04/cqueue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/474781832" 
	@${RM} ${OBJECTDIR}/_ext/474781832/cqueue.o.d 
	@${RM} ${OBJECTDIR}/_ext/474781832/cqueue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmLib_v3_04/cqueue.c  -o ${OBJECTDIR}/_ext/474781832/cqueue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/474781832/cqueue.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/474781832/cqueue.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/474781832/cstring.o: ../hmLib_v3_04/cstring.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/474781832" 
	@${RM} ${OBJECTDIR}/_ext/474781832/cstring.o.d 
	@${RM} ${OBJECTDIR}/_ext/474781832/cstring.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmLib_v3_04/cstring.c  -o ${OBJECTDIR}/_ext/474781832/cstring.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/474781832/cstring.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/474781832/cstring.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrBufLogger.o: ../hmrBufLogger.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrBufLogger.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrBufLogger.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrBufLogger.c  -o ${OBJECTDIR}/_ext/1472/hmrBufLogger.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrBufLogger.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrBufLogger.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2062917857/hmrDevice_kk08.o: C:/Users/homura/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500_Ontake/hmrDevice_kk08.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2062917857" 
	@${RM} ${OBJECTDIR}/_ext/2062917857/hmrDevice_kk08.o.d 
	@${RM} ${OBJECTDIR}/_ext/2062917857/hmrDevice_kk08.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  C:/Users/homura/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500_Ontake/hmrDevice_kk08.c  -o ${OBJECTDIR}/_ext/2062917857/hmrDevice_kk08.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2062917857/hmrDevice_kk08.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/2062917857/hmrDevice_kk08.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1472/amPic30_v7.o: ../amPic30_v7.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/amPic30_v7.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/amPic30_v7.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../amPic30_v7.c  -o ${OBJECTDIR}/_ext/1472/amPic30_v7.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/amPic30_v7.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/amPic30_v7.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/amPic30Extend.o: ../amPic30Extend.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/amPic30Extend.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/amPic30Extend.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../amPic30Extend.c  -o ${OBJECTDIR}/_ext/1472/amPic30Extend.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/amPic30Extend.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/amPic30Extend.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrBattery.o: ../hmrBattery.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrBattery.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrBattery.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrBattery.c  -o ${OBJECTDIR}/_ext/1472/hmrBattery.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrBattery.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrBattery.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrCO2.o: ../hmrCO2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrCO2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrCO2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrCO2.c  -o ${OBJECTDIR}/_ext/1472/hmrCO2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrCO2.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrCO2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrCom.o: ../hmrCom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrCom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrCom.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrCom.c  -o ${OBJECTDIR}/_ext/1472/hmrCom.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrCom.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrCom.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrData.o: ../hmrData.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrData.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrData.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrData.c  -o ${OBJECTDIR}/_ext/1472/hmrData.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrData.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrData.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrDevice.o: ../hmrDevice.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrDevice.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrDevice.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrDevice.c  -o ${OBJECTDIR}/_ext/1472/hmrDevice.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrDevice.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrDevice.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrDeviceManage.o: ../hmrDeviceManage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrDeviceManage.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrDeviceManage.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrDeviceManage.c  -o ${OBJECTDIR}/_ext/1472/hmrDeviceManage.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrDeviceManage.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrDeviceManage.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrDeviceManageMessage.o: ../hmrDeviceManageMessage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrDeviceManageMessage.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrDeviceManageMessage.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrDeviceManageMessage.c  -o ${OBJECTDIR}/_ext/1472/hmrDeviceManageMessage.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrDeviceManageMessage.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrDeviceManageMessage.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrDrive.o: ../hmrDrive.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrDrive.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrDrive.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrDrive.c  -o ${OBJECTDIR}/_ext/1472/hmrDrive.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrDrive.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrDrive.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrGPS.o: ../hmrGPS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrGPS.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrGPS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrGPS.c  -o ${OBJECTDIR}/_ext/1472/hmrGPS.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrGPS.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrGPS.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrInertial.o: ../hmrInertial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrInertial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrInertial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrInertial.c  -o ${OBJECTDIR}/_ext/1472/hmrInertial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrInertial.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrInertial.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrM2500.o: ../hmrM2500.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrM2500.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrM2500.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrM2500.c  -o ${OBJECTDIR}/_ext/1472/hmrM2500.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrM2500.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrM2500.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrMessage.o: ../hmrMessage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrMessage.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrMessage.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrMessage.c  -o ${OBJECTDIR}/_ext/1472/hmrMessage.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrMessage.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrMessage.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrSprite.o: ../hmrSprite.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrSprite.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrSprite.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrSprite.c  -o ${OBJECTDIR}/_ext/1472/hmrSprite.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrSprite.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrSprite.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrSpriteBuffer.o: ../hmrSpriteBuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrSpriteBuffer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrSpriteBuffer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrSpriteBuffer.c  -o ${OBJECTDIR}/_ext/1472/hmrSpriteBuffer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrSpriteBuffer.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrSpriteBuffer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrSpriteInterface.o: ../hmrSpriteInterface.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrSpriteInterface.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrSpriteInterface.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrSpriteInterface.c  -o ${OBJECTDIR}/_ext/1472/hmrSpriteInterface.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrSpriteInterface.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrSpriteInterface.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrTask.o: ../hmrTask.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrTask.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrTask.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrTask.c  -o ${OBJECTDIR}/_ext/1472/hmrTask.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrTask.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrTask.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrThermo.o: ../hmrThermo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrThermo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrThermo.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrThermo.c  -o ${OBJECTDIR}/_ext/1472/hmrThermo.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrThermo.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrThermo.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrUtility.o: ../hmrUtility.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrUtility.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrUtility.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrUtility.c  -o ${OBJECTDIR}/_ext/1472/hmrUtility.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrUtility.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrUtility.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrVMC1.o: ../hmrVMC1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrVMC1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrVMC1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrVMC1.c  -o ${OBJECTDIR}/_ext/1472/hmrVMC1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrVMC1.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrVMC1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/474781832/cdeque.o: ../hmLib_v3_04/cdeque.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/474781832" 
	@${RM} ${OBJECTDIR}/_ext/474781832/cdeque.o.d 
	@${RM} ${OBJECTDIR}/_ext/474781832/cdeque.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmLib_v3_04/cdeque.c  -o ${OBJECTDIR}/_ext/474781832/cdeque.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/474781832/cdeque.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/474781832/cdeque.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/474781832/cqueue.o: ../hmLib_v3_04/cqueue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/474781832" 
	@${RM} ${OBJECTDIR}/_ext/474781832/cqueue.o.d 
	@${RM} ${OBJECTDIR}/_ext/474781832/cqueue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmLib_v3_04/cqueue.c  -o ${OBJECTDIR}/_ext/474781832/cqueue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/474781832/cqueue.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/474781832/cqueue.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/474781832/cstring.o: ../hmLib_v3_04/cstring.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/474781832" 
	@${RM} ${OBJECTDIR}/_ext/474781832/cstring.o.d 
	@${RM} ${OBJECTDIR}/_ext/474781832/cstring.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmLib_v3_04/cstring.c  -o ${OBJECTDIR}/_ext/474781832/cstring.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/474781832/cstring.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/474781832/cstring.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/hmrBufLogger.o: ../hmrBufLogger.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrBufLogger.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hmrBufLogger.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../hmrBufLogger.c  -o ${OBJECTDIR}/_ext/1472/hmrBufLogger.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/hmrBufLogger.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hmrBufLogger.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2062917857/hmrDevice_kk08.o: C:/Users/homura/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500_Ontake/hmrDevice_kk08.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2062917857" 
	@${RM} ${OBJECTDIR}/_ext/2062917857/hmrDevice_kk08.o.d 
	@${RM} ${OBJECTDIR}/_ext/2062917857/hmrDevice_kk08.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  C:/Users/homura/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500_Ontake/hmrDevice_kk08.c  -o ${OBJECTDIR}/_ext/2062917857/hmrDevice_kk08.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2062917857/hmrDevice_kk08.o.d"        -g -omf=elf -O0 -I"C:/Users/HOMURA/Documents/VisualStudioOnline/homura-vehicle/HomuraSystem/hmrM2500" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/2062917857/hmrDevice_kk08.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/hmrM2500.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/hmrM2500.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -Wl,--local-stack,--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--heap=1024,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="C:/Users/HOMURA/SkyDrive/Projects/hmrM2500",--no-force-link,--smart-io,--report-mem$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/hmrM2500.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/hmrM2500.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,--local-stack,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=1024,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="C:/Users/HOMURA/SkyDrive/Projects/hmrM2500",--no-force-link,--smart-io,--report-mem$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/hmrM2500.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
