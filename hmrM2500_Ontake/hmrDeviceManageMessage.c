#ifndef HMR_DEVICEMANAGEMESSAGE_C_INC
#define HMR_DEVICEMANAGEMESSAGE_C_INC 102
#
/*
===hmrDeviceManage===

v1_02/131019 amby
	���b�Z�[�W�֐�����A���̑��̋@�\�̓C���^�[�t�F�[�X�ցc

v1_01/131013 hmIto
	�C���^�[�t�F�[�X��啝�ɕύX������
		���[�h����N���b�N���x�w������O
*/

/*===hmrDeviceManage===
'D'
���[�h�擾�R�}���h
>> 0xA0 
<< 0xA0, DeviceManageMode, ClockModeSpeed  
	DeviceManageMode : 0x00:normal, 0x10:sleep, 0x20:roaming  
	ClockModeSpeed   : 0x00:normal, 0x01:low, 0x02:high  

Device Manage Mode �ݒ� 

�X���[�v�ݒ�R�}���h�i���s�͂���Ȃ��j
>> 0x10, 0x10, secSleepLength[2],secRemLength[2]
	secSleepLength�b�Ő����T�C�N���B�����AsecSleepLength�b����������ԁB
	secSleepLength��sint16�̐����A�܂�1�`32767�b��9���ԋ�������
<< 0x10, 0x10, secSleepLength[2],secRemLength[2],Code
	�ݒ�l�Ǝ��s�p�R�[�h���Ԃ��Ă���
<< 0x10, 0xF1
	�ݒ�l���ُ�ȏꍇ�́A0xF1���߂�

�X���[�v���s�R�}���h
>> 0x10, 0x11, Code
	���s�p�R�[�h�𑗐M���āA���s���m�F
		���O�̃X���[�v�ݒ�R�}���h�Ŏ擾�������s�p�R�[�h�̂ݗL��
<< �Ȃ��i�����j


���[�~���O�ݒ�R�}���h�i���s�͂���Ȃ��j
>> 0x10, 0x20, ModuleID[1],secRoamLength[1]
	ModuleID�̒ʐM���W���[����L���ɂ���
		ModuleID 0:RF, 1:MP
	secRoamLength�b�ҋ@������A���̃��W���[���ɖ߂�(���m�ɂ�secRoamLength�b���ƂɒT�������ς���)
	secRoamLength�ɂ�uint8�̐����A�܂�1�`255�b������
<< 0x10, 0x20,ModuleID,Code
	�ݒ�l�Ǝ��s�p�R�[�h���Ԃ��Ă���
<< 0x10, 0xF2
	�ݒ�l���ُ�ȏꍇ�́A0xF2���߂�
���[�~���O���s�R�}���h
>> 0x10, 0x21,Code
	���s�p�R�[�h�𑗐M���āA���s���m�F
		���O�̃X���[�v�ݒ�R�}���h�Ŏ擾�������s�p�R�[�h�̂ݗL��
<< �Ȃ��i���[�~���O�j

���[�h���Z�b�g�i�m�[�}�����[�h���s�j�R�}���h
>> 0x10, 0x00
	�m�[�}�����[�h
<< 0x10, 0x00


KILL�ݒ�R�}���h�i���s�͂���Ȃ��j
>> 0x20
<< 0x20,Code
	�ݒ�l�Ǝ��s�p�R�[�h���Ԃ��Ă���
<< 0xF2
	�ݒ�l���ُ�ȏꍇ�́A0xF1���߂�
KILL���s�R�}���h
>> 0x21,Code
	���s�p�R�[�h�𑗐M���āA���s���m�F
		���O��KILL�ݒ�R�}���h�Ŏ擾�������s�p�R�[�h�̂ݗL��
<< �Ȃ��i�i���j


�N���b�N�ݒ�p�R�}���h
>> 0x30, Speed
	Speed : 0:normal, 1:low, 2:high 	
<< 0x30, Speed 

initialize , sleep �Ƃ��̃C���t�H�[�����l����

*/

#include<stdlib.h>
#define HMLIB_NOLIB
#ifndef HMR_DEVICEMANAGEMESSAGE_INC
#	include"hmrDeviceManageMessage.h"
#endif
#ifndef HMR_UTILITIES_INC
#	include "hmrUtility.h"
#endif

// mode flag
struct{
	unsigned Info_i:1;	     // ��񑗐M���[�h��
	
	unsigned SleepMode_i:1;  // sleep mode ��
	//unsigned SleepModeFail:1; // sleep mode �󗝎��s(���͂�������������)
	
	unsigned RoamingMode_i:1;// roaming mode ��
	//unsigned RoamingModeFail:1; // roaming mode �󗝎��s

	unsigned NormalMode_i:1; // normal mode ��
	unsigned KillCom_i:1;    // kill command �� 
	
	unsigned ClockMode_i:1;  // clock �ύX�R�}���h��
	unsigned ClockModeFail:1;
	
	unsigned SleepModeCodeFail:1;   // sleep mode exe �󗝂�����Code�F�����s
	unsigned RoamingModeCodeFail:1; // roaming mode exe �󗝂��������s
	unsigned KillCodeFail:1;		  // normal mode exe �󗝂��������s
}_devmngmsg_Mode={0,0,0,0,0,0,0,0,0,0};

// ���b�N�p�̃R�[�h
unsigned char _devmngmsg_sleep_Code=0;	//���b�N�����p�̃R�[�h
unsigned char _devmngmsg_roaming_Code=0;	//���b�N�����p�̃R�[�h
unsigned char _devmngmsg_kill_Code=0;	//���b�N�����p�̃R�[�h

uint16 _devmngmsg_sleep_sec_rem;
uint16 _devmngmsg_sleep_sec_nonRem;
unsigned char _devmngmsg_clockMode;
uint16 _devmngmsg_roaming_sec_interval;
//unsigned char _devmngmsg_roaming_ID;

/*
// initialize �֐�
void devmngmsg_initialize(){
	_devmngmsg_Mode.Info_i = hmLib_false;
	_devmngmsg_Mode.SleepMode_i = hmLib_false;
	_devmngmsg_Mode.RoamingMode_i = hmLib_false;
	_devmngmsg_Mode.NormalMode_i = hmLib_false;
	_devmngmsg_Mode.KillCom_i = hmLib_false;
	_devmngmsg_Mode.ClockMode_i = hmLib_false;
	_devmngmsg_Mode.ClockModeFail = hmLib_false;
	_devmngmsg_Mode.SleepModeCodeFail = hmLib_false;
	_devmngmsg_Mode.RoamingModeCodeFail = hmLib_false;
	_devmngmsg_Mode.KillCodeFail = hmLib_false;

	//devmng_initialize();
}
// finalize �֐�
void devmngmsg_finalize(){
	//devmng_finalize();
}
*/

hmLib_boolian devmngmsg_listen(cstring Str){

	//�f�[�^�T�C�Y�m�F
	if(cstring_size(&Str)==0)return hmLib_true;

	//1byte�ڂŃ��[�h�𕪂���
	switch(cstring_getc(&Str,0)){

	// Infomation mode
	case 0xA0:
		_devmngmsg_Mode.Info_i = hmLib_true;
		return hmLib_false;

    // �e��ҋ@���[�h�ݒ�
	case 0x10:
		if(cstring_size(&Str)==1)return hmLib_true;
		switch(cstring_getc(&Str,1)){
		case 0x00:// normal 
			devmng_mode_set(NormalMode);

			_devmngmsg_Mode.NormalMode_i = hmLib_true;
			return hmLib_false;

		case 0x10:// sleep mode
			if(cstring_size(&Str)!=6)return hmLib_true;
			_devmngmsg_sleep_sec_nonRem = ((uint16)cstring_getc(&Str,2) & 0x00FF) + (uint16)cstring_getc(&Str,3) * 256;
			_devmngmsg_sleep_sec_rem = ((uint16)cstring_getc(&Str,4) &0x00FF) + (uint16)cstring_getc(&Str,5) * 256;
			// sleep mode�@�ݒ菀��
			devmng_sleep_setInterval(_devmngmsg_sleep_sec_nonRem, _devmngmsg_sleep_sec_rem); 
			//devmng_sleep_getInterval(&_devmngmsg_sleep_sec_nonRem, &_devmngmsg_sleep_sec_rem); 
			// code �擾
			_devmngmsg_sleep_Code = lockcode();

			_devmngmsg_Mode.SleepMode_i = hmLib_true;
			return hmLib_false;

		case 0x11:// sleep execute mode
			if(cstring_size(&Str)!=3)return hmLib_true;
			// code �̃`�F�b�N
			if(cstring_getc(&Str,2) ==  _devmngmsg_sleep_Code){// ��v
				// inform �֐��̃Z�b�g

				// sleep �̎��s
				devmng_mode_set(SleepMode);
			}else{
				// ���s�ʒm
				_devmngmsg_Mode.SleepModeCodeFail = hmLib_true;
			}
			return hmLib_false;
		
		case 0x20:// roaming mode 
			if(cstring_size(&Str)!=4)return hmLib_true;
			_devmngmsg_roaming_sec_interval =  ((uint16)cstring_getc(&Str,2) & 0x00FF) + (uint16)cstring_getc(&Str,3) * 256;
			// sleep mode�@�ݒ菀��
			devmng_roaming_setInterval(_devmngmsg_roaming_sec_interval); 
			//devmng_roaming_getInterval(&_devmngmsg_roaming_sec_interval); 
			// code �擾
			_devmngmsg_roaming_Code = lockcode();
			_devmngmsg_Mode.RoamingMode_i = hmLib_true;
			return hmLib_false;

		case 0x21:// roaming execute mode
			if(cstring_size(&Str)!=3)return hmLib_true;
			// code �̃`�F�b�N
			if(cstring_getc(&Str,2) ==  _devmngmsg_roaming_Code){// ��v
				// inform �֐��̃Z�b�g

				// roaming �̎��s
				devmng_mode_set(RoamingMode);
			}else{
				// ���s�ʒm
				_devmngmsg_Mode.RoamingModeCodeFail = hmLib_true;
			}
			return hmLib_false;
	}

	case 0x20:
		_devmngmsg_kill_Code =  lockcode();
		_devmngmsg_Mode.KillCom_i = hmLib_true;
		return hmLib_false;

	case 0x21:
		if(cstring_size(&Str)!=2)return hmLib_true;
		// code �̃`�F�b�N
		if(cstring_getc(&Str,1) ==  _devmngmsg_kill_Code){// ��v
			// kill �̎��s
			devmng_kill();
		}else{
			// ���s�ʒm
			_devmngmsg_Mode.KillCodeFail = hmLib_true;
		}
		return hmLib_false;

	case 0x30:
		if(cstring_size(&Str)!=2)return hmLib_true;
		// mode �̃`�F�b�N
		_devmngmsg_clockMode = cstring_getc(&Str,1); 
		if(_devmngmsg_clockMode == 0){
			devmng_clock_set(NormalClock);
		}else if(_devmngmsg_clockMode == 1){
			devmng_clock_set(LowClock);
		}else if(_devmngmsg_clockMode == 2){
			devmng_clock_set(HighClock);
		}else{
			_devmngmsg_Mode.ClockModeFail = hmLib_true;
			return hmLib_false;
		}
		// clock mode ��
		_devmngmsg_Mode.ClockMode_i = hmLib_true;
		return hmLib_false;
	default:
		return hmLib_true;
	}
}
hmLib_boolian devmngmsg_talk(cstring* pStr){
	devmng_mode mode;
	devmng_clock clockMode;
	//hmLib_uint16 sumadc;
	if(_devmngmsg_Mode.Info_i){
		cstring_construct_dynamic(pStr,4);
		cstring_putc(pStr,0,0xA0);

		mode = devmng_mode_get();
		if(mode == NormalMode){
			cstring_putc(pStr,1,0x00);
		}else if(mode == SleepMode){
			cstring_putc(pStr,1,0x10);
		}else if(mode == RoamingMode){
			cstring_putc(pStr,1,0x20);
		}

		clockMode = devmng_clock_get();
		if(clockMode == NormalClock){
			cstring_putc(pStr,2,0x00);
		}else if(clockMode == LowClock){
			cstring_putc(pStr,2,0x01);
		}else if(clockMode == HighClock){
			cstring_putc(pStr,2,0x02);
		}

		// RF or MP 
		if(hmr_uart1_getModuleID()==UART1_MODULEID_RF){
			cstring_putc(pStr,3,0x00);
		}else{
			cstring_putc(pStr,3,0x01);
		}

		_devmngmsg_Mode.Info_i=hmLib_false;
		return hmLib_false;

	}else if(_devmngmsg_Mode.NormalMode_i){
		cstring_construct_dynamic(pStr,2);
		cstring_putc(pStr,0,0x10);
		cstring_putc(pStr,1,0x00);

		_devmngmsg_Mode.NormalMode_i=hmLib_false;
		return hmLib_false;

	}else if(_devmngmsg_Mode.SleepMode_i){
		cstring_construct_dynamic(pStr,7);
		cstring_putc(pStr,0,0x10);
		cstring_putc(pStr,1,0x10);
		cstring_putc(pStr,2,(unsigned char)(_devmngmsg_sleep_sec_nonRem&0x00FF));
		cstring_putc(pStr,3,(unsigned char)((_devmngmsg_sleep_sec_nonRem>>8)&0x00FF));
		cstring_putc(pStr,4,(unsigned char)(_devmngmsg_sleep_sec_rem&0x00FF));
		cstring_putc(pStr,5,(unsigned char)((_devmngmsg_sleep_sec_rem>>8)&0x00FF));
		cstring_putc(pStr,6,_devmngmsg_sleep_Code);

		_devmngmsg_Mode.SleepMode_i=hmLib_false;
		return hmLib_false;

	}else if(_devmngmsg_Mode.SleepModeCodeFail){
		cstring_construct_dynamic(pStr,2);
		cstring_putc(pStr,0,0x10);
		cstring_putc(pStr,1,0xF1);
		
		_devmngmsg_Mode.SleepModeCodeFail=hmLib_false;
		return hmLib_false;

	}else if(_devmngmsg_Mode.RoamingMode_i){
		cstring_construct_dynamic(pStr,6);
		cstring_putc(pStr,0,0x10);
		cstring_putc(pStr,1,0x20);
		cstring_putc(pStr,2,(unsigned char)(_devmngmsg_roaming_sec_interval&0x00FF));
		cstring_putc(pStr,3,(unsigned char)((_devmngmsg_roaming_sec_interval>>8)&0x00FF));
		cstring_putc(pStr,4,_devmngmsg_roaming_Code);

		//�ύX��̒ʐM��i��Ԃ�
		if(hmr_uart1_getModuleID()==UART1_MODULEID_RF){
			cstring_putc(pStr,5,0x01); // ����RF�Ȃ玟��MP
		}else{
			cstring_putc(pStr,5,0x00); // ����MP�Ȃ玟��RF 
		}

		_devmngmsg_Mode.RoamingMode_i=hmLib_false;
		return hmLib_false;

	}else if(_devmngmsg_Mode.RoamingModeCodeFail){
		cstring_construct_dynamic(pStr,2);
		cstring_putc(pStr,0,0x10);
		cstring_putc(pStr,1,0xF2);
		
		_devmngmsg_Mode.RoamingModeCodeFail=hmLib_false;
		return hmLib_false;

	}else if(_devmngmsg_Mode.KillCom_i){
		cstring_construct_dynamic(pStr,2);
		cstring_putc(pStr,0,0x20);
		cstring_putc(pStr,1,_devmngmsg_kill_Code);

		_devmngmsg_Mode.KillCom_i=hmLib_false;
		return hmLib_false;
	}else if(_devmngmsg_Mode.KillCodeFail){
		cstring_construct_dynamic(pStr,1);
		cstring_putc(pStr,1,0xF2);
		
		_devmngmsg_Mode.KillCodeFail=hmLib_false;
		return hmLib_false;

	}else if(_devmngmsg_Mode.ClockMode_i){
		cstring_construct_dynamic(pStr,2);
		cstring_putc(pStr,0,0x30);
		cstring_putc(pStr,1,_devmngmsg_clockMode);

		_devmngmsg_Mode.ClockMode_i=hmLib_false;
		return hmLib_false;

	}else if(_devmngmsg_Mode.ClockModeFail){
		cstring_construct_dynamic(pStr,1);
		cstring_putc(pStr,0,0xF3);

		_devmngmsg_Mode.ClockModeFail=hmLib_false;
		return hmLib_false;
	}

	return hmLib_true;
}
void devmngmsg_setup_listen(void){return;}
void devmngmsg_setup_talk(void){
	//_thermo_Mode.Data=_thermo_Mode.SendData;
    /*Debug*///	_thermo_Mode.Data=1;
}

sint16 devmngmsg_task_inform(sint16 interval){
	_devmngmsg_Mode.Info_i = hmLib_true;
	return interval;
}

/*
//=== �N���b�N���[�h����@�\ ===
//�N���b�N���w�胂�[�h�ōăX�^�[�g����
void _devmngmsg_clockmode_restartClock(devmngmsg_clockmode Mode_);
//�N���b�N���N���b�N���[�h�ōăX�^�[�g����
void _devmngmsg_clockmode_restartClockDefault();
//�N���b�N���[�h��ݒ肷��
void devmngmsg_clockmode_set(devmngmsg_clockmode ClockMode_);
//�N���b�N���[�h���擾����
devmngmsg_clockmode devmngmsg_clockmode_get(void);


//=== �N���b�N���[�h����@�\ ===
//�f�t�H���g�N���b�N
devmngmsg_clockmode x_devmngmsg_clockmode_Mode;
//�N���b�N���w�胂�[�h�ōăX�^�[�g����
void _devmngmsg_clockmode_restartClock(devmngmsg_clockmode Mode_){
	//�N���b�N���[�h�ݒ菈��
	hmr_clock_finalize();

	switch(Mode_){
	case LowClockMode:
		hmr_clock_initialize(CLOCK_SLOW);
		break;
	case HighClockMode:
		hmr_clock_initialize(CLOCK_FAST);
		break;
	default:
		hmr_clock_initialize(CLOCK_NORMAL);
		break;
	}

	//�N���b�N�ݒ��̂���
	hmr_uart1_initialize(hmr_uart1_getModuleID());
	hmr_uart2_initialize();
	hmr_adc_initialize();
}
//�N���b�N���N���b�N���[�h�ōăX�^�[�g����
void _devmngmsg_clockmode_restartClockDefault(){
	_devmngmsg_clockmode_restartClock(x_devmngmsg_clockmode_Mode);
}
//�N���b�N���[�h��ݒ肷��
void devmngmsg_clockmode_set(devmngmsg_clockmode ClockMode_){
	//�����N���b�N���[�h���w�肳�ꂽ�疳��
	if(x_devmngmsg_clockmode_Mode==ClockMode_)return;

	//�N���b�N���[�h��ύX
	x_devmngmsg_clockmode_Mode=ClockMode_;

	//�N���b�N���[�h�ύX�����[�h���䕔�ɒʒm
	_devmngmsg_mode_informClockMode();
}
//�N���b�N���[�h���擾����
devmngmsg_clockmode devmngmsg_clockmode_get(void){return x_devmngmsg_clockmode_Mode;}

//=== �X���[�v���[�h���� ===
//���������N�����ɌĂяo�����֐��|�C���^
devmngmsg_vFp_v x_devmngmsg_sleep_InformSleep;
devmngmsg_vFp_v x_devmngmsg_sleep_InformWakeup;
//�������m�������̐؂�ւ����ɌĂяo�����֐��|�C���^
devmngmsg_vFp_v x_devmngmsg_sleep_InformRem;
devmngmsg_vFp_v x_devmngmsg_sleep_InformNonRem;
//�������̃C���^�[�o������ϐ�
sint16 x_devmngmsg_sleep_secNonRem=540;
sint16 x_devmngmsg_sleep_secRem=60;
sint16 x_devmngmsg_sleep_secCnt=0;
boolian x_devmngmsg_sleep_IsRem;
//�X���[�v���[�h�̃p�����[�^���擾����
void devmngmsg_sleep_getInterval(sint16& secNonRem,sint16& secRem){
	secNonRem=x_devmngmsg_sleep_secNonRem;
	secRem=x_devmngmsg_sleep_secRem;
}
//�X���[�v���[�h�̃p�����[�^��ύX����
void devmngmsg_sleep_setInterval(sint16 secNonRem, sint16 secRem){
	if(secNonRem<0)secNonRem=0;
	if(secRem<1)secRem=1;

	x_devmngmsg_sleep_secNonRem=secNonRem;
	x_devmngmsg_sleep_secRem=secRem;
}
//�X���[�v���[�h�̃p�����[�^���擾����
void devmngmsg_sleep_registSleepInformFunction(devmngmsg_vFp_v InformSleep,devmngmsg_vFp_v InformWakeUp){
	x_devmngmsg_sleep_InformSleep=InformSleep;
	x_devmngmsg_sleep_InformWakeup=InformWakeUp;
}
//�����X���[�v�ɓ���/�o��ۂ̒ʒm�֐���o�^����
void devmngmsg_sleep_registRemInformFunction(devmngmsg_vFp_v InformRem,devmngmsg_vFp_v InformNonRem){
	x_devmngmsg_sleep_InformRem=InformRem;
	x_devmngmsg_sleep_InformNonRem=InformNonRem;
}
//sleep��Ԃɂقނ��ς���֐�
void _devmngmsg_sleep_start(){	
	//�X���[�v�J�n��ʒm����
	if(x_devmngmsg_sleep_InformSleep)x_devmngmsg_sleep_InformSleep();

	// ���W���[���d�����Ƃ�
	_devmngmsg_module_power_off();

	// �N���b�N�X�s�[�h���Ƃ�
	_devmngmsg_clockmode_restartClock(LowClockMode);
}
//sleep��Ԃ̂قނ���N�����֐�
void _devmngmsg_sleep_stop(void){
	// �N���b�N�X�s�[�hNORMAL��
	_devmngmsg_clockmode_restartClockDefault();

	// ���W���[���d�����A
	_devmngmsg_module_power_on();

	//�X���[�v�I����ʒm����
	if(x_devmngmsg_sleep_InformWakeup)x_devmngmsg_sleep_InformWakeup();

	//���荞�݋���
	uart1_enable_fget_interrupt();
}
//������Ԃ��̎擾�֐�
boolian _devmngmsg_sleep_isRem(void){return x_devmngmsg_sleep_IsRem;}
//�X���[�v���̃^�X�N�֐�
void _devmngmsg_sleep_interrupt_function(sint16 Interval_){
	x_devmngmsg_sleep_secCnt+=Interval_;

	if(x_devmngmsg_sleep_IsRem){
		if(x_devmngmsg_sleep_secCnt>=x_devmngmsg_sleep_secRem){
			x_devmngmsg_sleep_IsRem=0;
			x_devmngmsg_sleep_secCnt=0;
			if(x_devmngmsg_sleep_InformNonRem)x_devmngmsg_sleep_InformNonRem();
		}
	}else{
		if(x_devmngmsg_sleep_secCnt>=x_devmngmsg_sleep_secNonRem){
			x_devmngmsg_sleep_IsRem=1;
			x_devmngmsg_sleep_secCnt=0;
			if(x_devmngmsg_sleep_InformRem)x_devmngmsg_sleep_InformRem();
		}
	}
}

//==================== Roaming ===================
unsigned char x_devmngmsg_roaming_Code=0;	//���b�N�����p�̃R�[�h
boolian x_devmngmsg_roaming_IsRoaming=0;	//���[�~���O���ł��邱�Ƃ������t���O
sint16 x_devmngmsg_roaming_secInterval=0;		//Roaming�؂�ւ��̒���
sint16 x_devmngmsg_roaming_secCnt=0;		//Roaming��Ԃ̃J�E���^
sint16 _devmngmsg_roaming_task(sint16 Sec_){
	//�J�E���^��i�߂�
	++_devmngmsg_roaming_Cnt;

	//���[�~���O�؊�����
	if(_devmngmsg_roaming_Cnt>HMR_DEVCTR_ROAMING_LENGTH){
		if(uart1_getModuleID()==UART1_MODULEID_RF){
			HMR_PIN_RedLED_set(1);
			uart1_initialize(UART1_MODULEID_MP);
		}else{
			HMR_PIN_RedLED_set(0);
			uart1_initialize(UART1_MODULEID_RF);
		}
		_devmngmsg_roaming_Cnt=0;
	}
}
//���[�~���O���s�p�֐�
//	wdt�𖳌��ɂ��āA�ʂ̒ʐM���W���[���ł̒ʐM�����݂�
void _devmngmsg_roaming_start(void){
	x_devmngmsg_roaming_IsRoaming=0;
	x_devmngmsg_roaming_secCnt=0;
	disable_wdt();

	if(hmr_uart1_getModuleID()==UART1_MODULEID_RF){
		HMR_PIN_RedLED_set(1);
		hmr_uart1_initialize(UART1_MODULEID_MP);
	}else{
		HMR_PIN_RedLED_set(0);
		hmr_uart1_initialize(UART1_MODULEID_RF);
	}

	//timer4_5�̊��荞�ݗL����
	timer4_5_finalize();
	timer4_5_initialize(10000);		//10�b���Ƃ̊��荞��
	timer4_5_enable_interrupt();	//���荞�ݗL����
}
//���[�~���O�I���p�֐�
void _devmngmsg_roaming_stop(void){
	timer4_5_disable_interrupt();

	restart_wdt();
	//Config�ɍ��킹�āAWDT�Z�b�g
	if(Config_DisableWatchDogTimer)enable_wdt();

	//timer4_5�̊��荞�ݗL����
	timer4_5_finalize();
	timer4_5_initialize(1000);		//1�b���Ƃ̊��荞��
	timer4_5_enable_interrupt();	//���荞�ݗL����
}
//���[�~���O���[�h�̃p�����[�^���擾����
void devmngmsg_roaming_getInterval(sint16& secInterval){
	secInterval=x_devmngmsg_roaming_secInterval;
}
//���[�~���O���[�h�̃p�����[�^��ݒ肷��
void devmngmsg_roaming_setInterval(sint16 secInterval){
	if(secInterval<=10)secInterval=10;
	x_devmngmsg_roaming_secInterval=secInterval;
}

//=== ��{����֐� ===
//�f�o�C�X�������֐�
void devmngmsg_initialize(void){
	//�d������x�؂�
	_devmngmsg_power(0);

	//�f�o�C�X������
	hmr_device_initialize();

	//�N���b�N������
	if(Config_DefaultHighClock)hmr_clock_smart_initialize(CLOCK_HIGH);
	else hmr_clock_smart_initialize(CLOCK_NORMAL);	

	//���荞�ݐݒ菉����
	hmr_interrupt_initalize();

	//�ʐM�f�o�C�X�̏�����
	if(Config_DefaultMobilePhone)hmr_uart1_smart_initialize(UART1_MODULEID_MP);
	else hmr_uart1_smart_initialize(UART1_MODULEID_RF);

	rs_fputc('O',Stream_VMC);
	rs_fputc('K',Stream_VMC);
	rs_fputc((unsigned char)(RCON),Stream_VMC);
	rs_fputc((unsigned char)(RCON>>8),Stream_VMC);
	rs_fputc(0x0d,Stream_VMC);
	rs_fputc(0x0a,Stream_VMC);

	hmr_uart2_initialize();
	hmr_spi2_initialize();
	hmr_i2c_initialize();
	hmr_adc_initialize();

	rs_fputc('C',Stream_VMC);
	rs_fputc('m',Stream_VMC);
	rs_fputc(0x0d,Stream_VMC);
	rs_fputc(0x0a,Stream_VMC);

	delay_ms(500);
	_devmngmsg_power(1);
	
	rs_fputc('P',Stream_VMC);
	rs_fputc('w',Stream_VMC);
	rs_fputc(0x0d,Stream_VMC);
	rs_fputc(0x0a,Stream_VMC);

	//WatchDogTimer
	if(Config_DisableWatchDogTimer)disable_wdt();
	else enable_wdt();

	//task�V�X�e�����N������
	task_initialize();
}
//�f�o�C�X�I�[���֐�
void devmngmsg_finalize(void){
	//task�V�X�e�����I������
	task_finalize();

	//uart1_fget/fput��~
	uart1_disable_fget_interrupt();
	uart1_disable_fput_interrupt();
	uart2_disable_fget_interrupt();
	uart2_disable_fput_interrupt();

	//�ʐM�f�o�C�X�̏I�[��
	hmr_uart1_finalize();
	hmr_uart2_finalize();
	hmr_spi2_finalize();
	hmr_i2c_finalize();
	hmr_adc_finalize();

	//devmngmsg�p���[
	_devmngmsg_power(0);

	//�f�o�C�X�̏I�[������
	hmr_clock_finalize();
	hmr_interrupt_finalize();
	hmr_device_finalize();
}
//�f�o�C�X�����I��
void devmngmsg_kill(void){
	// �I�[��
	devmngmsg_finalize();
	// �������g���؂�
	_devmngmsg_pic_power_off();
}
void _devmngmsg_module_power_off(){
	//sprite�J������~
	sprite_finalize();	//timer1,uart2_fget/fput���~
	sprite_power(0);

	//�S�d��off
//	spi_selectGPS(0);
//	spi_selectDrive(0);
//	rf_power(0);
//	mp_power(0);
//	subPIC_power(0);
//	i2c_power(0);
//	adc_power(0);
//	h2s_powerSensor(0);
	co2_powerPump(0);
	co2_powerSensor(0);
//	servo_power(0);
}
void _devmngmsg_module_power_on(){
	//�@�����͕ۗ���

	//sprite�J�����N��
	sprite_power(1);


	//�S�d��off
//	spi_selectGPS(0);
//	spi_selectDrive(0);
//	rf_power(0);
//	mp_power(0);
//	subPIC_power(0);
//	i2c_power(0);
//	adc_power(0);
//	h2s_powerSensor(0);
	co2_powerPump(0);
	co2_powerSensor(0);
//	servo_power(0);
}
//�ʐM�n�̃p���[�ꊇ����֐�
void _devmngmsg_power(boolian onoff){
	spi_selectGPS(0);
	spi_selectDrive(0);
	subPIC_power(onoff);
	hmr_uart1_power_switch(onoff,hmr_uart1_getModuleID());
	i2c_power(onoff);
	adc_power(onoff);
    HMR_PIN_ADCPW_CH7_set(onoff);
    HMR_PIN_ADCPW_CH8_set(onoff);
}


//=== ���[�h����@�\ ===
devmngmsg_mode x_devmngmsg_mode_Mode;
//�f�o�C�X���[�h��ύX����
void devmngmsg_mode_set(devmngmsg_mode Mode_){
	switch(Mode_){
	case SleepMode:
		switch(_devmngmsg_Mode){
		case RoamingMode:
			_devmngmsg_roaming_stop();
			break;
		}

		//���[�h�ύX
		_devmngmsg_Mode=SleepMode;
		_devmngmsg_sleep_start();

		return;
	case RoamingMode:
		switch(_devmngmsg_Mode){
		case SleepMode:
			_devmngmsg_sleep_stop();
			break;
		}

		//���[�h�ύX
		_devmngmsg_Mode=RoamingMode;
		_devmngmsg_roaming_start();

		return;
	default:
		switch(_devmngmsg_Mode){
		case RoamingMode:
			_devmngmsg_roaming_stop();
			break;
		case SleepMode:
			_devmngmsg_sleep_stop();
			break;
		}

		//���[�h�ύX
		_devmngmsg_Mode=NormalMode;

		return;
	}

}
//�f�o�C�X���[�h���擾����
devmngmsg_mode devmngmsg_mode_get(void){return x_devmngmsg_mode_Mode;}
//�N���b�N���[�h�ύX�ɑΉ�����
void _devmngmsg_mode_informClockMode(void){
	//�X���[�v���̂ݖ���
	if(x_devmngmsg_mode_Mode==SleepMode && !_devmngmsg_sleep_isRem())return;

	_devmngmsg_clockmode_restartClockDefault();
}

//==================== ���荞�݊֐� ====================
/*
//�X���[�v�A����у��[�~���O���s�p�֐�
void timer4_5_interrupt(void){
	//�t���O�N���A�Ǝ����̃��Z�b�g
	timer4_5_clear_interrupt_flag();
	timer4_5_reset();

	//�X���[�v���[�h�̊��荞�ݐ���
	if(devmng_is_sleep()){

	}//���[�~���O���[�h�̊��荞�ݐ���
	else if(devmng_is_roaming()){
		_devmng_sleep_isRem();
	}//�ʏ�̃^�X�N�����̏ꍇ
	else{
		task_interrupt_function();
	}
}
*/


#
#endif
