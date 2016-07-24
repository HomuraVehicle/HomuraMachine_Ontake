#ifndef HMR_DEVICEMANAGE_C_INC
#define HMR_DEVICEMANAGE_C_INC 103
#
#ifndef HMR_DEVICEMANAGE_INC
#	include"hmrDeviceManage.h"
#endif
#ifndef HMR_DEVICE_INC
#	include"hmrDevice.h"
#endif
#ifndef HMR_TASK_INC
#	include"hmrTask.h"
#endif
#ifndef HMR_COM_INC
#	include"hmrCom.h"
#endif
#ifndef HMR_UTILITY_INC
#	include"hmrUtility.h"
#endif


//++++++++++++++++++ protected�錾 +++++++++++++++++
//=== ��{�֐� ===
//�f�o�C�X�̓d���Ǘ�
void _devmng_power(boolian onoff);

//=== ���[�h����@�\ ===
//���[�h����@�\������
void _devmng_mode_initialize(devmng_mode Mode_);
//���[�h����@�\�I�[��
void _devmng_mode_finalize();
//�N���b�N�ύX�̒ʒm�֐�
void _devmng_mode_informClockChange(void);

//=== �N���b�N����@�\ ===
//�N���b�N����@�\������
void _devmng_clock_initialize(devmng_clock Clock_);
//�N���b�N����@�\�I�[��
void _devmng_clock_finalize();
//�N���b�N���x�̕ύX�֐�
void _devmng_clock_restartClock(devmng_clock Clock_);
//�N���b�N���x���f�t�H���g�ɕύX
void _devmng_clock_restartClockDefault();

//=== �^�X�N���� ===
//�^�X�N���䏉����
void _devmng_task_initialize();
//�^�X�N����I�[��
void _devmng_task_finalize();
//�^�X�N���䏈���֐�
void _devmng_task_interrupt_function(sint16 Interval_);

//=== �X���[�v���[�h���� ===
//�X���[�v���[�h���䏉����
void _devmng_sleep_initialize(void);
//�X���[�v���[�h����I�[��
void _devmng_sleep_finalize(void);
//�X���[�v�ɓ��鏈��
void _devmng_sleep_start();
//�X���[�v����o�鏈��
void _devmng_sleep_stop(void);
//�X���[�v���䏈���֐�
void _devmng_sleep_interrupt_function(sint16 Interval_);
//�X���[�v���̊��荞�݋֎~�֐�
void _devmng_sleep_disable_interrupt(void);
//�X���[�v���̊��荞�ݕ��A�֐�
void _devmng_sleep_enable_interrupt(void);


//=== ���[�~���O���[�h���� ===
//���[�~���O���[�h���䏉����
void _devmng_roaming_initialize(void);
//���[�~���O���[�h����I�[��
void _devmng_roaming_finalize(void);
//���[�~���O�ɓ��鏈��
void _devmng_roaming_start(void);
//���[�~���O����o�鏈��
void _devmng_roaming_stop(void);
//���[�~���O���䏈���֐�
void _devmng_roaming_interrupt_function(sint16 Interval_);

//=== ���荞�݊֐����� ===
void _devmng_interrupt_initialize(void);
void _devmng_interrupt_finalize(void);


//++++++++++++++++++ ���� +++++++++++++++++
//=== ���[�h����@�\ ===
//�f�o�C�X�̌����[�h
volatile devmng_mode x_devmng_mode_Mode=NormalMode;
//���[�h����@�\������
void _devmng_mode_initialize(devmng_mode Mode_){
	//�w��̃f�t�H���g���[�h�ŊJ�n
	x_devmng_mode_Mode=Mode_;
}
//���[�h����@�\�I�[��
void _devmng_mode_finalize(){
	x_devmng_mode_Mode=NormalMode;
}
//�f�o�C�X���[�h��ύX����
void devmng_mode_set(devmng_mode Mode_){
	switch(Mode_){
	case SleepMode:
		switch(x_devmng_mode_Mode){
		case RoamingMode:
			_devmng_roaming_stop();
			break;
		default:
				break;
		}

		//?????
		x_devmng_mode_Mode=SleepMode;
		_devmng_sleep_start();

		return;
	case RoamingMode:
		switch(x_devmng_mode_Mode){
		case SleepMode:
			_devmng_sleep_stop();
			break;
		default:
			break;
		}

		//?????
		x_devmng_mode_Mode=RoamingMode;
		_devmng_roaming_start();

		return;
	default:
		switch(x_devmng_mode_Mode){
		case RoamingMode:
			_devmng_roaming_stop();
			break;
		case SleepMode:
			_devmng_sleep_stop();
			break;
                    default:
                        break;
		}

		//?????
		x_devmng_mode_Mode=NormalMode;

		return;
	}

}
//�f�o�C�X���[�h���擾����
volatile devmng_mode devmng_mode_get(void){return x_devmng_mode_Mode;}
//�N���b�N�ύX�̒ʒm�֐�
void _devmng_mode_informClockChange(void){
	//�X���[�v���m�������������̓p�X����
	if(devmng_sleep_isSleep() && !devmng_sleep_isRem())return;

	//�N���b�N���[�h���������ɔ��f
	_devmng_clock_restartClockDefault();
}

//=== �N���b�N����@�\ ===
//�f�o�C�X�̌��N���b�N
volatile devmng_clock x_devmng_clock_Clock;
//�N���b�N����@�\������
void _devmng_clock_initialize(devmng_clock Clock_){
	//�w��N���b�N�ɕύX
	x_devmng_clock_Clock=Clock_;

	//�������ɃN���b�N���x���f
	_devmng_clock_restartClockDefault();
}
//�N���b�N����@�\�I�[��
void _devmng_clock_finalize(){
	hmr_clock_finalize();
}
//�N���b�N���x�̕ύX�֐�
void _devmng_clock_restartClock(devmng_clock Clock_){
	//��x�N���b�N���I��
	hmr_clock_finalize();

	switch(Clock_){
	case LowClock:
		hmr_clock_initialize(CLOCK_SLOW);
		break;
	case HighClock:
		hmr_clock_initialize(CLOCK_FAST);
		break;
	default:
		hmr_clock_initialize(CLOCK_NORMAL);
		break;
	}

	//�N���b�N�ύX���ق��̃��W���[���ɂ��ʒm
//	hmr_uart1_initialize(hmr_uart1_getModuleID(),);
//	hmr_uart2_initialize(38400);
//	hmr_adc_initialize();
}
//�N���b�N���x���f�t�H���g�ɕύX
void _devmng_clock_restartClockDefault(){
	_devmng_clock_restartClock(x_devmng_clock_Clock);
}
//�N���b�N��ύX����
void devmng_clock_set(devmng_clock Clock_){
	//���łɐݒ�ς݂Ȃ疳��
	if(x_devmng_clock_Clock==Clock_)return;

	//�ύX�𔽉f���āA���[�h����ɒʒm
	x_devmng_clock_Clock=Clock_;
	_devmng_mode_informClockChange();
}
//�N���b�N���擾����
volatile devmng_clock devmng_clock_get(void){return x_devmng_clock_Clock;}

//=== �^�X�N���� ===
#define x_devmng_task_BufSize 32
task x_devmng_task_Buf[x_devmng_task_BufSize];
taskmaster x_devmng_TaskMaster;
//�^�X�N���䏉����
void _devmng_task_initialize(){
	//task����
	task_construct_static(&x_devmng_TaskMaster,x_devmng_task_Buf,x_devmng_task_Buf+x_devmng_task_BufSize);
}
//�^�X�N����I�[��
void _devmng_task_finalize(){
	//task�j��
	task_destruct(&x_devmng_TaskMaster);
}
//�^�X�N��V�K�o�^�ł���m�F����
boolian devmng_task_can_start(){return task_can_start(&x_devmng_TaskMaster);}
//�^�X�N��o�^�i��d�o�^�͉���j
boolian devmng_task_start(sint16 Interval,task_function TaskFp,task_inform TaskInformStopFp){
	return task_start(&x_devmng_TaskMaster,Interval,TaskFp,TaskInformStopFp);
}
//�^�X�N��o�^�i��d�o�^�ł��Ă��܂��̂ŁA�댯�j
boolian devmng_task_quick_start(sint16 Interval,task_function TaskFp,task_inform TaskInformStopFp){
	return task_quick_start(&x_devmng_TaskMaster,Interval,TaskFp,TaskInformStopFp);
}
//�^�X�N��ݒ�ύX
boolian devmng_task_restart(task_function TaskFp,sint16 Interval){
	return task_restart(&x_devmng_TaskMaster,TaskFp,Interval);
}
//�^�X�N���폜
boolian devmng_task_stop(task_function TaskFp){
	return task_stop(&x_devmng_TaskMaster,TaskFp);
}
//�^�X�N���䏈���֐�
void _devmng_task_interrupt_function(sint16 Interval_){
	task_interrupt_function(&x_devmng_TaskMaster,Interval_);
}

//=== �X���[�v���[�h���� ===
//�X���[�v/�o���ʒm�p�֐�
typedef struct{
	devmng_vFp_v InformSleep;
	devmng_vFp_v InformWakeUp;
}devmng_sleep_InformSleepFn;
#define x_devmng_sleep_InformSleepFnBufSize 16
devmng_sleep_InformSleepFn x_devmng_sleep_InformSleepFnBuf[x_devmng_sleep_InformSleepFnBufSize];
unsigned char x_devmng_sleep_InformSleepFnBufPos=0;
//����/�m�������ʒm�p�֐�
typedef struct{
	devmng_vFp_v InformRem;
	devmng_vFp_v InformNonRem;
}devmng_sleep_InformRemFn;
#define x_devmng_sleep_InformRemFnBufSize 16
devmng_sleep_InformRemFn x_devmng_sleep_InformRemFnBuf[x_devmng_sleep_InformRemFnBufSize];
unsigned char x_devmng_sleep_InformRemFnBufPos=0;
//�X���[�v���̎��ԃJ�E���g�n
sint16 x_devmng_sleep_secNonRem=540;
sint16 x_devmng_sleep_secRem=60;
sint16 x_devmng_sleep_secCnt=0;
//�X���[�v��ԊǗ��p�t���O
volatile boolian x_devmng_sleep_IsSleep=0; 
volatile boolian x_devmng_sleep_IsRem=0;
boolian x_devmng_sleep_RequestEndRem=0;
//�X���[�v���[�h���䏉����
void _devmng_sleep_initialize(void){
	x_devmng_sleep_InformSleepFnBufPos=0;
	x_devmng_sleep_InformRemFnBufPos=0;

	x_devmng_sleep_secNonRem=540;
	x_devmng_sleep_secRem=60;
	x_devmng_sleep_secCnt=0;
	x_devmng_sleep_IsSleep=0;
	x_devmng_sleep_IsRem=0;
	x_devmng_sleep_RequestEndRem=0;
}
//�X���[�v���[�h����I�[��
void _devmng_sleep_finalize(void){
	//�X���[�v���Ȃ��~
	if(x_devmng_sleep_IsSleep)_devmng_sleep_stop();

	x_devmng_sleep_InformSleepFnBufPos=0;
	x_devmng_sleep_InformRemFnBufPos=0;

	x_devmng_sleep_secNonRem=540;
	x_devmng_sleep_secRem=60;
	x_devmng_sleep_secCnt=0;
	x_devmng_sleep_IsSleep=0;
	x_devmng_sleep_IsRem=0;
}
//�����X���[�v��Ԃ��ǂ������擾����
volatile boolian devmng_sleep_isRem(void){return x_devmng_sleep_IsRem && x_devmng_sleep_IsSleep;}
//�X���[�v��Ԃ��ǂ������擾����
volatile boolian devmng_sleep_isSleep(void){return x_devmng_sleep_IsSleep;}
//�X���[�v���[�h�̃p�����[�^���擾����
void devmng_sleep_getInterval(sint16* secNonRem,sint16* secRem){
	*secNonRem=x_devmng_sleep_secNonRem;
	*secRem=x_devmng_sleep_secRem;
}
//�X���[�v���[�h�̃p�����[�^��ݒ肷��
void devmng_sleep_setInterval(sint16 secNonRem, sint16 secRem){
	if(secNonRem<0)secNonRem=0;
	if(secRem<1)secRem=1;

	x_devmng_sleep_secNonRem=secNonRem;
	x_devmng_sleep_secRem=secRem;
}
//���������N�����ɌĂяo�����ʒm�֐���o�^�ł��邩
boolian devmng_sleep_can_registSleepInformFunction(){return x_devmng_sleep_InformSleepFnBufPos!=x_devmng_sleep_InformSleepFnBufSize;}
//���������N�����ɌĂяo�����ʒm�֐���o�^����
boolian devmng_sleep_registSleepInformFunction(devmng_vFp_v InformSleep,devmng_vFp_v InformWakeUp){
	if(!devmng_sleep_can_registSleepInformFunction())return hmLib_true;
	x_devmng_sleep_InformSleepFnBuf[x_devmng_sleep_InformSleepFnBufPos].InformSleep=InformSleep;
	x_devmng_sleep_InformSleepFnBuf[x_devmng_sleep_InformSleepFnBufPos].InformWakeUp=InformWakeUp;
	++x_devmng_sleep_InformSleepFnBufPos;

	return hmLib_false;
}
//�����X���[�v�ɓ���/�o��ۂ̒ʒm�֐���o�^�ł��邩
boolian devmng_sleep_can_registRemInformFunction(){return x_devmng_sleep_InformRemFnBufPos!=x_devmng_sleep_InformRemFnBufSize;}
//�����X���[�v�ɓ���/�o��ۂ̒ʒm�֐���o�^����
boolian devmng_sleep_registRemInformFunction(devmng_vFp_v InformRem,devmng_vFp_v InformNonRem){
	if(!devmng_sleep_can_registRemInformFunction())return hmLib_true;
	x_devmng_sleep_InformRemFnBuf[x_devmng_sleep_InformRemFnBufPos].InformRem=InformRem;
	x_devmng_sleep_InformRemFnBuf[x_devmng_sleep_InformRemFnBufPos].InformNonRem=InformNonRem;
	++x_devmng_sleep_InformRemFnBufPos;

	return hmLib_false;
}
//�X���[�v�֓��鏈��
void _devmng_sleep_start(){
	devmng_sleep_InformSleepFn* Itr;

	//���łɃX���[�v���Ȃ�I��
	if(x_devmng_sleep_IsSleep)return;

	//�J�E���^�[�����Z�b�g
	x_devmng_sleep_secCnt=0;
	x_devmng_sleep_IsRem=0;

	//�X���[�v��ԂɕύX
	x_devmng_sleep_IsSleep=1;

	// ���荞�݋֎~
	_devmng_sleep_disable_interrupt();

	//Inform�֐�
	for(Itr=x_devmng_sleep_InformSleepFnBuf;Itr!=x_devmng_sleep_InformSleepFnBuf+x_devmng_sleep_InformSleepFnBufSize;++Itr){
		//if(x_devmng_sleep_InformSleepFnBuf->InformSleep)x_devmng_sleep_InformSleepFnBuf->InformSleep();
		if(Itr->InformSleep)Itr->InformSleep();	
	}

	//�d���𗎂Ƃ�
	_devmng_power(0);

	//�N���b�N���x�𗎂Ƃ�
	_devmng_clock_restartClock(LowClock);
}
//�X���[�v����o�鏈��
void _devmng_sleep_stop(void){
	devmng_sleep_InformSleepFn* Itr;

	//���łɔ�X���[�v���Ȃ�I��
	if(!x_devmng_sleep_IsSleep)return;

	//�N���b�N���x���f�t�H���g�ɖ߂�
	_devmng_clock_restartClockDefault();

	//�d����߂� ���łɃ��������Ȃ̂ŃJ�b�g
	//_devmng_power(1);

	//�X���[�v��Ԃ�����
	x_devmng_sleep_IsSleep=0;

	//Inform�֐�
	for(Itr=x_devmng_sleep_InformSleepFnBuf;Itr!=x_devmng_sleep_InformSleepFnBuf+x_devmng_sleep_InformSleepFnBufSize;++Itr){
		//if(x_devmng_sleep_InformSleepFnBuf->InformWakeUp)x_devmng_sleep_InformSleepFnBuf->InformWakeUp();
		if(Itr->InformWakeUp)Itr->InformWakeUp();
	}

	// ���荞�݋���
	_devmng_sleep_enable_interrupt();

}
// �X���[�v���̊��荞�݋֎~
void _devmng_sleep_disable_interrupt(){
	// Main stream �̊��荞�݋֎~
	streamVMC_disable_fget_interrupt();
	//streamVMC_disable_fput_interrupt();
	// Camera stream �̊��荞�݋֎~
	streamCMR_disable_fget_interrupt();
	//streamCMR_disable_fput_interrupt();
	// �J���� ��Timer ���荞�݋֎~
	timerCMR_disable_interrupt();
}
// �X���[�v���̊��荞�ݕ��A
void _devmng_sleep_enable_interrupt(){
	// Main stream �̎�M���荞�݊J�n
	streamVMC_enable_fget_interrupt();
	// Camera stream �̎�M���荞�݊J�n
	streamCMR_enable_fget_interrupt();
	// �J���� ��Timer ���荞�݊J�n
	timerCMR_enable_interrupt();
}
//�X���[�v���䏈�������֐�
void _devmng_sleep_interrupt_function(sint16 Interval_){
	devmng_sleep_InformRemFn* Itr;

	//SleepCnt�����Z
	x_devmng_sleep_secCnt+=Interval_;

	//������Ԃ̂Ƃ� 
	if(x_devmng_sleep_IsRem){
		//���荞�݂ŋ}�ɓd���𗎂Ƃ��̂͊댯�Ȃ̂ŁA���N�G�X�g���������Ă���(���ۂ̏I���́Awork�֐��ɈϏ�)
		if(x_devmng_sleep_secCnt>=x_devmng_sleep_secRem)x_devmng_sleep_RequestEndRem=1;
	}//�m��������Ԃ̂Ƃ�
	else{
		if(x_devmng_sleep_secCnt>=x_devmng_sleep_secNonRem){
			//�J�E���^�[��������
			x_devmng_sleep_secCnt=0;
			
			//�N���b�N���x���f�t�H���g�ɖ߂�
			_devmng_clock_restartClockDefault();

			//�d��������
			_devmng_power(1);
			// ����҂�
			exclusive_delay_ms(300);
			HMR_PIN_RedLED_set(1);
			exclusive_delay_ms(300);
			HMR_PIN_RedLED_set(0);
			exclusive_delay_ms(300);
			HMR_PIN_RedLED_set(1);
			exclusive_delay_ms(300);
			HMR_PIN_RedLED_set(0);
			exclusive_delay_ms(300);
			HMR_PIN_RedLED_set(1);

			//������ԂɈڍs
			x_devmng_sleep_IsRem=1;

			//Inform�֐�
			for(Itr=x_devmng_sleep_InformRemFnBuf;Itr!=(x_devmng_sleep_InformRemFnBuf+x_devmng_sleep_InformRemFnBufSize);++Itr){
				//if(x_devmng_sleep_InformRemFnBuf->InformRem)x_devmng_sleep_InformRemFnBuf->InformRem();
				if(Itr->InformRem)Itr->InformRem();
			}

			// ���荞�݋���
			_devmng_sleep_enable_interrupt();

		}
	}
}
//�X���[�v���䏈���ʏ�֐�
void _devmng_sleep_interrupt_yield(void){
	devmng_sleep_InformRemFn* Itr;

	//���������I���v���֐�
	if(x_devmng_sleep_RequestEndRem){
		//�t���O��������
		x_devmng_sleep_RequestEndRem=0;
		//�J�E���^�[��������
		x_devmng_sleep_secCnt=0;

		// ���荞�݂��֎~
		_devmng_sleep_disable_interrupt();

		//�m��������ԂɈڍs
		x_devmng_sleep_IsRem=0;

		//Inform�֐�
		for(Itr=x_devmng_sleep_InformRemFnBuf;Itr!=x_devmng_sleep_InformRemFnBuf+x_devmng_sleep_InformRemFnBufSize;++Itr){
			//if(x_devmng_sleep_InformRemFnBuf->InformNonRem)x_devmng_sleep_InformRemFnBuf->InformNonRem();
			if(Itr->InformNonRem)Itr->InformNonRem();
		}

		//�N���b�N���x�𗎂Ƃ�
		_devmng_clock_restartClock(LowClock);

		//�d���𗎂Ƃ�
		_devmng_power(0);
	}
}
//=== ���[�~���O���[�h���� ===
//���[�~���O�̊Ԋu����ϐ�
sint16 x_devmng_roaming_secInterval=0;
sint16 x_devmng_roaming_secCnt=0;	
//���[�~���O�̏�ԕϐ�
volatile boolian x_devmng_roaming_IsRoaming=0;
//���[�~���O���[�h���䏉����
void _devmng_roaming_initialize(void){
	x_devmng_roaming_secInterval=0;
	x_devmng_roaming_secCnt=0;
	x_devmng_roaming_IsRoaming=0;
}
//���[�~���O���[�h����I�[��
void _devmng_roaming_finalize(void){
	//���[�~���O���Ȃ�A��~
	if(x_devmng_roaming_IsRoaming)_devmng_roaming_stop();

	x_devmng_roaming_secInterval=0;
	x_devmng_roaming_secCnt=0;
	x_devmng_roaming_IsRoaming=0;
}
//���[�~���O���[�h�̃p�����[�^���擾����
void devmng_roaming_getInterval(sint16* secInterval){
	*secInterval=x_devmng_roaming_secInterval;
}
//���[�~���O���[�h�̃p�����[�^��ݒ肷��
void devmng_roaming_setInterval(sint16 secInterval){
	//���[�~���O�Ԋu��10�b�ȏ�ł���K�v
	if(secInterval<=10)secInterval=10;
	x_devmng_roaming_secInterval=secInterval;
}
//���[�~���O��Ԃ��ǂ������擾����
volatile boolian _devmng_roaming_isRoaming(void){return x_devmng_roaming_IsRoaming;}
//���[�~���O��Ԃɓ���
void _devmng_roaming_start(void){
	//���łɃ��[�~���O���Ȃ疳��
	if(x_devmng_roaming_IsRoaming)return;

	//�J�E���^���Z�b�g
	x_devmng_roaming_secCnt=0;

	//���[�~���O��Ԃֈڍs
	x_devmng_roaming_IsRoaming=1;

	//�ʐM�f�o�C�X�؂�ւ�
	if(hmr_uart1_getModuleID()==UART1_MODULEID_RF){
		HMR_PIN_RedLED_set(1);
		hmr_uart1_initialize(UART1_MODULEID_MP,devmng_MP_BaudRate);
	}else{
		HMR_PIN_RedLED_set(0);
		if(Config_RFDebugMode)hmr_uart1_initialize(UART1_MODULEID_RF,devmng_RFDebug_BaudRate);
		else hmr_uart1_initialize(UART1_MODULEID_RF,devmng_RF_BaudRate);
	}
}
//���[�~���O��Ԃ�����
void _devmng_roaming_stop(void){
	//���łɃ��[�~���O���łȂ��Ȃ疳��
	if(!x_devmng_roaming_IsRoaming)return;

	//���[�~���O��Ԃ�����
	x_devmng_roaming_IsRoaming=0;
}
//���[�~���O���䏈���֐�
void _devmng_roaming_interrupt_function(sint16 Interval_){
	//�J�E���^�[��i�߂�
	x_devmng_roaming_secCnt+=Interval_;
	
	//�J�E���^�[���؂�ւ��l�𒴂����ꍇ�́A�ʐM�f�o�C�X�؂�ւ�
	if(x_devmng_roaming_secCnt>x_devmng_roaming_secInterval){
		if(hmr_uart1_getModuleID()==UART1_MODULEID_RF){
			HMR_PIN_RedLED_set(1);
			hmr_uart1_initialize(UART1_MODULEID_MP,devmng_MP_BaudRate);
		}else{
			HMR_PIN_RedLED_set(0);
			if(Config_RFDebugMode)hmr_uart1_initialize(UART1_MODULEID_RF,devmng_RFDebug_BaudRate);
			else hmr_uart1_initialize(UART1_MODULEID_RF,devmng_RF_BaudRate);
		}
		x_devmng_roaming_secCnt=0;
	}
}

//=== ��{����֐� ===
//�f�o�C�X�������֐�
void devmng_initialize(void){
	//�f�o�C�X������
	hmr_device_initialize();

	//Pic���̂̃p���[�����Ă���
	hmr_device_power(1);

	//�f�o�C�X�̓d���𗎂Ƃ�
	_devmng_power(0);

	//���[�h�V�X�e��������
	_devmng_mode_initialize(NormalMode);

	//�N���b�N������
	if(Config_DefaultHighClock)_devmng_clock_initialize(HighClock);
	else _devmng_clock_initialize(NormalClock);

	//���荞�ݏ�����
	hmr_interrupt_initalize();

	///�ʐM�n���W���[��������
	if(Config_DefaultMobilePhone){
		hmr_uart1_initialize(UART1_MODULEID_MP,devmng_MP_BaudRate);
		com_wdt_restart();
		com_wdt_enable();
	}else{
		if(Config_RFDebugMode)hmr_uart1_initialize(UART1_MODULEID_RF,devmng_RFDebug_BaudRate);
		else hmr_uart1_initialize(UART1_MODULEID_RF,devmng_RF_BaudRate);
		com_wdt_disable();
	}

	hmr_uart2_initialize(38400);
	hmr_spi2_initialize();
	hmr_i2c_initialize();
	hmr_adc_initialize();

	//�f�o�C�X�̓d��������
	delay_ms(500);
	_devmng_power(1);
	
	//devmng���W���[��������
	_devmng_sleep_initialize();
	_devmng_roaming_initialize();
	_devmng_task_initialize();
	_devmng_interrupt_initialize();

	//WatchDogTimer�J�n
	if(Config_DisableWatchDogTimer)disable_wdt();
	else enable_wdt();
}
//�f�o�C�X�I�[���֐�
void devmng_finalize(void){
	//WatchDogTimer�I��
	disable_wdt();

	//devmng���W���[���I�[��
	_devmng_task_finalize();
	_devmng_sleep_finalize();
	_devmng_roaming_finalize();
	_devmng_interrupt_finalize();

	//�ʐM�n���W���[���I�[��
	hmr_uart1_finalize();
	hmr_uart2_finalize();
	hmr_spi2_finalize();
	hmr_i2c_finalize();
	hmr_adc_finalize();

	//�f�o�C�X�̓d���𗎂Ƃ�
	_devmng_power(0);

	//���荞�ݏI��
	hmr_interrupt_finalize();

	//�N���b�N�I��
	_devmng_clock_finalize();

	//���[�h�Ǘ��I��
	_devmng_mode_finalize();

	//�f�o�C�X���̂��I��
	hmr_device_finalize();
}
//�f�o�C�X�����I��
void devmng_kill(void){
	//���g�̓d����؂�
	hmr_device_power(0);
}
//�f�o�C�X�̓d���Ǘ��֐�
void _devmng_power(boolian onoff){
	//�d���I�t�̏ꍇ�́A���W���[�������؂�
	if(!onoff){
		sprite_power(0);
		sprite_powerLight(0);
		co2_powerPump(0);
		co2_powerSensor(0);
		h2s_powerSensor(0);
		spi_selectGPS(0);
		spi_selectDrive(0);
	}

	hmr_uart1_power_switch(onoff,hmr_uart1_getModuleID());
	subPIC_power(onoff);
	i2c_power(onoff);
	adc_power(onoff);
}
//���݂̒ʐM�悪�S��d�ʐM�Ή�����Ԃ�
boolian devmng_isFullDuplex(void){
	return hmr_uart1_getModuleID()==UART1_MODULEID_MP || Config_RFDebugMode;
}

//=== ���荞�݊֐� ===
//�^�X�N�A�X���[�v�A����у��[�~���O���s�p�֐�
void devmng_interrupt_function(sint16 Interval_){
	//���[�h�ɂ���Ċ��荞�ݏ����𕪊�
	switch(devmng_mode_get()){
	case SleepMode:
		_devmng_sleep_interrupt_function(Interval_);
		break;
	case RoamingMode:
		_devmng_roaming_interrupt_function(Interval_);
		break;
	default:
		_devmng_task_interrupt_function(Interval_);
		break;
	}
}
//���荞�݂�main���[�v�ւ̈Ϗ������֐�
boolian devmng_interrupt_yield(void){
	_devmng_sleep_interrupt_yield();

	//�X���[�v�����O
	if(devmng_sleep_isSleep() && !devmng_sleep_isRem()){
		//__asm__("PWRSAV #IDLE_MODE");
		Idle();
		//Sleep();
		return hmLib_true;
	}

	return hmLib_false;
}

//=== ���荞�ݐ���p�֐�
// ini fin �֐�
void _devmng_interrupt_initialize(){}
void _devmng_interrupt_finalize(){}

// main stream ���荞�݂̋��֎~�����������Ă悢����Ԃ��֐�
boolian devmng_interrupt_can_enable_streamVMC_fget_interrupt(){
	return !( devmng_sleep_isSleep() && !devmng_sleep_isRem() );
}
boolian devmng_interrupt_can_disable_streamVMC_fget_interrupt(){
	return hmLib_true;//!( devmng_sleep_isSleep() && !devmng_sleep_isRem() );
}
boolian devmng_interrupt_can_enable_streamVMC_fput_interrupt(){
	return !( devmng_sleep_isSleep() && !devmng_sleep_isRem() );
}
boolian devmng_interrupt_can_disable_streamVMC_fput_interrupt(){
	return hmLib_true;//!( devmng_sleep_isSleep() && !devmng_sleep_isRem() );
}

// camera stream ���荞�݂̋��֎~�����������Ă悢����Ԃ��֐�
boolian devmng_interrupt_can_enable_streamCMR_fget_interrupt(){
	return !( devmng_sleep_isSleep() && !devmng_sleep_isRem() );
}
boolian devmng_interrupt_can_disable_streamCMR_fget_interrupt(){
	return hmLib_true;//!( devmng_sleep_isSleep() && !devmng_sleep_isRem() );
}
boolian devmng_interrupt_can_enable_streamCMR_fput_interrupt(){
	return !( devmng_sleep_isSleep() && !devmng_sleep_isRem() );
}
boolian devmng_interrupt_can_disable_streamCMR_fput_interrupt(){
	return hmLib_true;//!( devmng_sleep_isSleep() && !devmng_sleep_isRem() );
}

// camera timer  ���荞�݂̋��֎~�����������Ă悢����Ԃ��֐�
boolian devmng_interrupt_can_enable_timerCMR_interrupt(){
	return !( devmng_sleep_isSleep() && !devmng_sleep_isRem() );
}
boolian devmng_interrupt_can_disable_timerCMR_interrupt(){
	return hmLib_true;//!( devmng_sleep_isSleep() && !devmng_sleep_isRem() );
}
// DevMng timer �̊��荞�݂̋��֎~�����������Ă悢����Ԃ��֐�
boolian devmng_interrupt_can_enable_timerDevmng_interrupt(){
	return hmLib_true;
}
boolian devmng_interrupt_can_disable_timerDevmng_interrupt(){
	return hmLib_true;
}

// main stream �̊��荞�݋��֎~�֐�
boolian devmng_interrupt_enable_streamVMC_fget_interrupt(){
	if(devmng_interrupt_can_enable_streamVMC_fget_interrupt()){
		streamVMC_enable_fget_interrupt();
		return hmLib_true;
	}else{
		return hmLib_false;
	}
}
boolian devmng_interrupt_disable_streamVMC_fget_interrupt(){
	if(devmng_interrupt_can_disable_streamVMC_fget_interrupt()){
		streamVMC_disable_fget_interrupt();
		return hmLib_true;
	}else{
		return hmLib_false;
	}
}
boolian devmng_interrupt_enable_streamVMC_fput_interrupt(){
	if(devmng_interrupt_can_enable_streamVMC_fput_interrupt()){
		streamVMC_enable_fput_interrupt();
		return hmLib_true;
	}else{
		return hmLib_false;
	}
}
boolian devmng_interrupt_disable_streamVMC_fput_interrupt(){
	if(devmng_interrupt_can_disable_streamVMC_fput_interrupt()){
		streamVMC_disable_fput_interrupt();
		return hmLib_true;
	}else{
		return hmLib_false;
	}
}

// camera stream �̊��荞�݋��֎~�֐�
boolian devmng_interrupt_enable_streamCMR_fget_interrupt(){
	if(devmng_interrupt_can_enable_streamCMR_fget_interrupt()){
		streamCMR_enable_fget_interrupt();
		return hmLib_true;
	}else{
		return hmLib_false;
	}
}
boolian devmng_interrupt_disable_streamCMR_fget_interrupt(){
	if(devmng_interrupt_can_disable_streamCMR_fget_interrupt()){
		streamCMR_disable_fget_interrupt();
		return hmLib_true;
	}else{
		return hmLib_false;
	}
}
boolian devmng_interrupt_enable_streamCMR_fput_interrupt(){
	if(devmng_interrupt_can_enable_streamCMR_fput_interrupt()){
		streamCMR_enable_fput_interrupt();
		return hmLib_true;
	}else{
		return hmLib_false;
	}
}
boolian devmng_interrupt_disable_streamCMR_fput_interrupt(){
	if(devmng_interrupt_can_disable_streamCMR_fput_interrupt()){
		streamCMR_disable_fput_interrupt();
		return hmLib_true;
	}else{
		return hmLib_false;
	}
}

// timer camera �̊��荞�݋��֎~�֐�
boolian devmng_interrupt_enable_timerCMR_interrupt(){
	if(devmng_interrupt_can_enable_timerCMR_interrupt()){
		timerCMR_enable_interrupt();
		return hmLib_true;
	}else{
		return hmLib_false;
	}
}
boolian devmng_interrupt_disable_timerCMR_interrupt(){
	if(devmng_interrupt_can_disable_timerCMR_interrupt()){
		timerCMR_disable_interrupt();
		return hmLib_true;
	}else{
		return hmLib_false;
	}
}

// timer Device manager �̊��荞�݋��֎~�֐�
boolian devmng_interrupt_enable_timerDevmng_interrupt(){
	if(devmng_interrupt_can_enable_timerDevmng_interrupt()){
		timerDevmng_enable_interrupt();
		return hmLib_true;
	}else{
		return hmLib_false;
	}
}
boolian devmng_interrupt_disable_timerDevmng_interrupt(){
	if(devmng_interrupt_can_disable_timerDevmng_interrupt()){
		timerDevmng_disable_interrupt();
		return hmLib_true;
	}else{
		return hmLib_false;
	}
}

#
#endif
