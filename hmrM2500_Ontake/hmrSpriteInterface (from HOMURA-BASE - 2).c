#ifndef HMR_SPRITEINTERFACE_C_INC
#define HMR_SPRITEINTERFACE_C_INC 107
#
#define HMLIB_NOLIB
#ifndef HMR_SPRITEINTERFACE_INC
#	include "hmrSpriteInterface.h"
#endif

//=====================LED�pdefine==========================//
#define _spriteitf_led_set(val)						sprite_setLED(val)
//=====================�d���pdefine=========================//
#define _spriteitf_power(val)						sprite_power(val)
//=====================timer�pdefine========================//
#define _spriteitf_timer_clear()					timer1_reset()
#define _spriteitf_timer_interrupt					timer1_interrupt
#define _spriteitf_timer_interrupt_disable()		timer1_disable_interrupt()
#define _spriteitf_timer_interrupt_enable()			timer1_enable_interrupt()
#define _spriteitf_timer_interrupt_clear()			timer1_clear_interrupt_flag()
#define _spriteitf_timer_interrupt_initialize(Time)	timer1_initialize(Time)
//==========================================================//

//=====================uart�pdefine=========================//
#define x_spriteitf_uart_initialize()				hmr_uart2_initialize()
#define _spriteitf_uart_fputc(c)					rs_fputc(c,Stream_CMR)
#define _spriteitf_uart_fput_interrupt				uart2_fput_interrupt
#define _spriteitf_uart_fput_interrupt_set()		set_interrupt_fputflag(Stream_CMR)
#define _spriteitf_uart_fput_interrupt_clear()		clear_interrupt_fputflag(Stream_CMR)
#define _spriteitf_uart_fput_interrupt_enable()		enable_interrupt_fput(Stream_CMR)
#define _spriteitf_uart_fput_interrupt_disable()	disable_interrupt_fput(Stream_CMR)
//amPic30����PicC30�ւ̈ڍs�܂ł̈ꎞ�[�u�@spriteitf_PowerReset�֐��Ŏg�p
#define x_spriteitf_uart_disable()					U2MODEbits.UARTEN=0

#define _spriteitf_uart_fgetc()						rs_fgetc(Stream_CMR)
#define _spriteitf_uart_fget_interrupt				uart2_fget_interrupt
#define _spriteitf_uart_fget_interrupt_set()		set_interrupt_fgetflag(Stream_CMR)
#define _spriteitf_uart_fget_interrupt_clear()		clear_interrupt_fgetflag(Stream_CMR)
#define _spriteitf_uart_fget_interrupt_disable()		disable_interrupt_fget(Stream_CMR)
#define _spriteitf_uart_fget_interrupt_enable()	enable_interrupt_fget(Stream_CMR)
//==========================================================//

//===�J�����ʐM�p�R�}���h�Q===
//���Z�b�g
static unsigned char _spriteitf_Cmd_RESET_CMR[4] = {0x56, 0x00, 0x26, 0x00};
spriteitf_command _spriteitf_Cmd_ResetCmr={_spriteitf_Cmd_RESET_CMR,4,70};
//�ʐ^�B�e
static unsigned char _spriteitf_Cmd_TAKE_PICTURE[5] = {0x56, 0x00, 0x36, 0x01, 0x00};
spriteitf_command _spriteitf_Cmd_TakePicture={_spriteitf_Cmd_TAKE_PICTURE,5,5};
//�f�[�^�T�C�Y�𓾂�
static unsigned char _spriteitf_Cmd_GET_DATA_SIZE[5] = {0x56, 0x00, 0x34, 0x01, 0x00};
spriteitf_command _spriteitf_Cmd_GetDataSize={_spriteitf_Cmd_GET_DATA_SIZE,5,9};
//�ʐ^�B��̂�߂�
static unsigned char _spriteitf_Cmd_STOP_TAKE_PICTURE[5] = {0x56, 0x00, 0x36, 0x01, 0x03};
spriteitf_command _spriteitf_Cmd_StopTakePicture={_spriteitf_Cmd_STOP_TAKE_PICTURE,5,5};
//�ȓd�̓��[�h
static unsigned char _spriteitf_Cmd_SAVE_PWR[7] = {0x56,0x00,0x3E,0x03,0x00,0x01,0x01};
spriteitf_command _spriteitf_Cmd_SavePwr={_spriteitf_Cmd_SAVE_PWR,7,4};
//�ȓd�͂��畜�A���[�h
static unsigned char _spriteitf_Cmd_STOP_SAVE_PWR[7] = {0x56,0x00,0x3E,0x03,0x00,0x01,0x00};
spriteitf_command _spriteitf_Cmd_StopSavePwr={_spriteitf_Cmd_STOP_SAVE_PWR,7,5};
//�f�[�^�𓾂�R�}���h(���߂�ADDRESS(8�̔{��)�ƁA�f�[�^���w��)
static unsigned char _spriteitf_varCmd_GET_DATA[16] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00,0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x0B};
spriteitf_command _spriteitf_varCmd_GetData(spriteitf_dsize_t Address_,spriteitf_dsize_t PackSize_){
	spriteitf_command Ans;
	//PackSize�͂W�̔{���Ȃ̂ŁA8�̔{���ɂ���I�I�@���������͓ǂݎ̂Ă���j��
	if(PackSize_%8 != 0){
		PackSize_ = PackSize_ + 8 - PackSize_%8;
	}

	//��M�J�n�A�h���X�w��
	_spriteitf_varCmd_GET_DATA[8] = (unsigned char)(Address_/256);
	_spriteitf_varCmd_GET_DATA[9] = (unsigned char)(Address_%256);

	//��M�p�P�b�g�̃T�C�Y�w��
	_spriteitf_varCmd_GET_DATA[12] = (unsigned char)(PackSize_/256);
	_spriteitf_varCmd_GET_DATA[13] = (unsigned char)(PackSize_%256);

	Ans.Ptr=_spriteitf_varCmd_GET_DATA;
	Ans.Size=16;
	Ans.InSize=10 + PackSize_;

	return Ans;
}
//���k���̎w��(defo 0x36)
static unsigned char _spriteitf_varCmd_SET_COMP_RATE[9]={0x56,0x00,0x31,0x05,0x01,0x01,0x12,0x04,0x36};
spriteitf_command _spriteitf_varCmd_setCompRate(unsigned char ComRate_){
	spriteitf_command Ans;
	//��M�J�n�A�h���X�w��
	_spriteitf_varCmd_SET_COMP_RATE[8] = ComRate_;

	Ans.Ptr=_spriteitf_varCmd_SET_COMP_RATE;
	Ans.Size=9;
	Ans.InSize=6;

	return Ans;
}
//�摜�T�C�Y�̎w��(��`�����R�ʂ�)�v���Z�b�g
/*static unsigned char _spriteitf_varCmd_SET_IMAGE_SIZE_1[9]={0x56,0x00,0x31,0x05,0x04,0x01,0x00,0x19,0xFF};
spriteitf_command _spriteitf_varCmd_SetImageSize(spriteitf_imagesize_t PictureSize_){
	spriteitf_command Ans;
	_spriteitf_varCmd_SET_IMAGE_SIZE_1[8]=PictureSize_;
	Ans.Ptr=_spriteitf_varCmd_SET_IMAGE_SIZE_1;
	Ans.Size=9;
	Ans.InSize=5;
	return Ans;
}*/
//�摜�T�C�Y�̎w��(��`�����R�ʂ�)���Z�b�g�s�v
static unsigned char _spriteitf_varCmd_SET_IMAGE_SIZE[5]={0x56,0x00,0x54,0x01,0xFF};
spriteitf_command _spriteitf_varCmd_SetImageSize(spriteitf_imagesize_t PictureSize_){
	spriteitf_command Ans;
	_spriteitf_varCmd_SET_IMAGE_SIZE[4]=PictureSize_;
	Ans.Ptr=_spriteitf_varCmd_SET_IMAGE_SIZE;
	Ans.Size=5;
	Ans.InSize=5;
	return Ans;
}
//�{�[���[�g�̎w��(��`�����T�ʂ�)
static unsigned char _spriteitf_varCmd_CHANGE_BAUD_RATE[7]={0x56,0x00,0x24,0x03,0x01,0x00,0x00};
spriteitf_command _spriteitf_varCmd_ChangeBaudRate(spriteitf_baudrate_t BaudRate_){
	spriteitf_command Ans;
	_spriteitf_varCmd_CHANGE_BAUD_RATE[5]= (char)(BaudRate_/256);
	_spriteitf_varCmd_CHANGE_BAUD_RATE[6]= (char)(BaudRate_%256);
	Ans.Ptr=_spriteitf_varCmd_CHANGE_BAUD_RATE;
	Ans.Size=7;
	Ans.InSize=5;
	return Ans;
}

//===error �G���[����===
//error���������ʒu�̌ŗL�ԍ�(define���g�p�j
spriteitf_error_t x_spriteitf_error_ErrCode=0;
//catch
spriteitf_vFp_v x_spriteitf_error_Fp_catch=0;
//error�@�\������
void _spriteitf_error_initialize(void){
	x_spriteitf_error_ErrCode=0;
	x_spriteitf_error_Fp_catch=0;
}
//error�@�\�̏I�[��
void _spriteitf_error_finalize(void){
	x_spriteitf_error_ErrCode=0;
	x_spriteitf_error_Fp_catch=0;
}
//�G���[�𔭕�
void _spriteitf_throw(spriteitf_error_t ErrCode_){
	//�G���[���L�^
	x_spriteitf_error_ErrCode=ErrCode_;
	//�L���b�`�����s
	if(x_spriteitf_error_Fp_catch)(*x_spriteitf_error_Fp_catch)();
}
//Error��Ԃ��m�F
boolian spriteitf_isError(void){return x_spriteitf_error_ErrCode!=0;}
//Error�t���O�m�F
spriteitf_error_t spriteitf_getErrorCode(void){return x_spriteitf_error_ErrCode;}
//�G���[�����Z�b�g
void spriteitf_clearErrorCode(void){x_spriteitf_error_ErrCode=0;}
//�G���[catch�֐���o�^
void _spritetif_set_catch(spriteitf_vFp_v Fp_catch_){x_spriteitf_error_Fp_catch=Fp_catch_;}


//===timer �^�C�}�[������===//
/***���ӁFtimer�����̏����͖�����*/
//�^�C���A�E�g���ďo���֐��|�C���^
spriteitf_vFp_v x_spriteitf_timer_fnTimeOut;
//timer�@�\�̏�����
void _spriteitf_timer_initialize(void){
	x_spriteitf_timer_fnTimeOut=0;
	_spriteitf_timer_interrupt_initialize(spriteitf_RecvTimeOut);
	_spriteitf_timer_clear();
	_spriteitf_timer_interrupt_disable();
//	_spriteitf_timer_interrupt_enable();
}
//timer�@�\�̏I�[��
void _spriteitf_timer_finalize(void){
	x_spriteitf_timer_fnTimeOut=0;
	_spriteitf_timer_interrupt_disable();
}
//TimeOut�@�\�����p�\���m�F
boolian _spriteitf_timer_can_start(void){return x_spriteitf_timer_fnTimeOut==0;}
//TimeOut�@�\��L����
void _spriteitf_timer_start(spriteitf_vFp_v fnTimeOut_){
	if(!_spriteitf_timer_can_start())return;
	x_spriteitf_timer_fnTimeOut=fnTimeOut_;
	_spriteitf_timer_clear();
	_spriteitf_timer_interrupt_clear();
	_spriteitf_timer_interrupt_enable();
}
//�O��Ɠ������e�Ń^�C�}�[�𓮂���(stop�����ɍăX�^�[�g)
void _spriteitf_timer_restart(void){
	_spriteitf_timer_clear();
	_spriteitf_timer_interrupt_clear();
	_spriteitf_timer_interrupt_enable();
}
//TimeOut�@�\�𖳌���
void _spriteitf_timer_stop(void){
	x_spriteitf_timer_fnTimeOut=0;
	_spriteitf_timer_interrupt_disable();
}
//�^�C�}�[���荞�݊֐�//spriteitf_RecvTimeOut ms �Ɉ��
void _spriteitf_timer_interrupt(void){
	//���荞�݃t���O����
	_spriteitf_timer_interrupt_clear();
	//��M���^�C���A�E�g���ďo���֐������s
	if(x_spriteitf_timer_fnTimeOut){
		(*x_spriteitf_timer_fnTimeOut)();
	}else _spriteitf_timer_stop();

}


//===uart����M�����݌^===//
//�R�}���h���M�󋵁@�ҋ@:0 ���M:1 ��M:2
uint8 x_spriteitf_uart_Mode=0;
//���݂̒S���R�}���h
spriteitf_command x_spriteitf_uart_Cmd;
//�R�}���h�J�E���^�[
spriteitf_dsize_t x_spriteitf_uart_Cnt=0;
//��M�������Ăяo���֐� ������ACK
spriteitf_vFp_v x_spriteitf_uart_fnFinRecv=0;
//��M���Ăяo���֐��@������data��position
spriteitf_bFp_uc_dsize x_spriteitf_uart_fnRecv=0;
//x_spriteitf_uart_fnTimeOut_Recv()�Ŏ��s�����G���[�����֐��A�����͒ʐM���̃R�}���h�|�C���^
spriteitf_vFp_ucp x_spriteitf_uart_fnTimeOutErr=0;
//uart������
#define _spriteitf_uart_disable() x_spriteitf_uart_disable()
//uart�֘A�̕ϐ�������
void _spriteitf_uart_iniVal(void){
	x_spriteitf_uart_Mode=0;
	x_spriteitf_uart_Cnt=0;
	x_spriteitf_uart_Cmd.Ptr=0;
	x_spriteitf_uart_Cnt=0;
	x_spriteitf_uart_fnFinRecv=0;
	x_spriteitf_uart_fnRecv=0;
	x_spriteitf_uart_fnTimeOutErr=0;
}
#define _spriteitf_uart_finVal() _spriteitf_uart_iniVal()
//����M���荞�ݏ�����
void _spriteitf_uart_initialize(void){
	//�e�ϐ�������
	_spriteitf_uart_iniVal();
	//uart module������
	x_spriteitf_uart_initialize();
	//���荞�ݗ\��
	_spriteitf_uart_fput_interrupt_clear();
	_spriteitf_uart_fput_interrupt_disable();
	_spriteitf_uart_fget_interrupt_clear();
	_spriteitf_uart_fget_interrupt_disable();
}
//����M���荞�ݏI�[��
void _spriteitf_uart_finalize(void){
	//uart���荞�݊֘A���N���A
	_spriteitf_uart_fput_interrupt_disable();
	_spriteitf_uart_fget_interrupt_disable();
	_spriteitf_uart_fput_interrupt_clear();
	_spriteitf_uart_fget_interrupt_clear();
	//uart������
	x_spriteitf_uart_disable();
	//�ϐ��I�[��
	_spriteitf_uart_finVal();
}
//�����݌^�R�}���h�𑗐M�\���̊m�F
boolian _spriteitf_uart_can_sendCommand(void){return x_spriteitf_uart_Mode==0;}
//�����݌^�R�}���h���M
void _spriteitf_uart_sendCommand(const spriteitf_command Cmd_,spriteitf_vFp_v fnFinRecv_,spriteitf_bFp_uc_dsize fnRecv_){
	//�R�}���h�����M�\���m�F
	if(!_spriteitf_uart_can_sendCommand())return;

	//��M���֐�
	x_spriteitf_uart_Mode=1;
	x_spriteitf_uart_Cmd=Cmd_;
	x_spriteitf_uart_Cnt=0;
	x_spriteitf_uart_fnFinRecv=fnFinRecv_;
	x_spriteitf_uart_fnRecv=fnRecv_;

	//���M���荞�݋���
	_spriteitf_uart_fput_interrupt_set();
	_spriteitf_uart_fput_interrupt_enable();
}
//�^�C���A�E�g�����s�֐�
void x_spriteitf_uart_fnTimeOut_Recv(void){
	_spriteitf_timer_stop();
	//���荞�ݒ��~
	_spriteitf_uart_fput_interrupt_disable();
	_spriteitf_uart_fget_interrupt_disable();
	//�G���[����
	if(x_spriteitf_uart_fnTimeOutErr)x_spriteitf_uart_fnTimeOutErr(x_spriteitf_uart_Cmd.Ptr);
	//uart�p�e��ϐ����N���A
	x_spriteitf_uart_Mode=0;
	x_spriteitf_uart_Cmd.Ptr=0;
	x_spriteitf_uart_Cnt=0;

}
//���M�p���荞�݊֐�
void _spriteitf_uart_fput_interrupt(void){
	//���荞�݃t���O�N���A
	_spriteitf_uart_fput_interrupt_clear();

	//���M����
	_spriteitf_uart_fputc(x_spriteitf_uart_Cmd.Ptr[x_spriteitf_uart_Cnt++]);
	//���M�I������
	if(x_spriteitf_uart_Cnt >= x_spriteitf_uart_Cmd.Size){
		//LED�_��
		_spriteitf_led_set(1);
		//�J�E���^�[��0�ɖ߂�
		x_spriteitf_uart_Cnt=0;
		//�����݃��[�h����M�ɕύX
		x_spriteitf_uart_Mode=2;
		//���M���荞�݋֎~
		_spriteitf_uart_fput_interrupt_disable();
		//�^�C�}�[�쓮�\���H�iPowerReset���g���ĂȂ����H�j
		if(_spriteitf_timer_can_start()){
			//�^�C���A�E�g�Ď��J�n
			_spriteitf_timer_start(x_spriteitf_uart_fnTimeOut_Recv);
		}
		//��M���荞�݋���
		_spriteitf_uart_fget_interrupt_enable();
	}
}
//��M�p���荞�݊֐�
void _spriteitf_uart_fget_interrupt(void){
	static unsigned char c;

	//���荞�݃t���O�N���A
	_spriteitf_uart_fget_interrupt_clear();

	//1������M
	c=_spriteitf_uart_fgetc();
	++x_spriteitf_uart_Cnt; 

	//��������M�R�}���h�擪��ǂ�ł��Ȃ���΁A�ǂݎ̂�
	if(x_spriteitf_uart_Cnt==1 && c!=0x76){
		//�J�E���g0�ɂ��ēǂ񂾒l�͖�������
		x_spriteitf_uart_Cnt = 0;
		//�^�C�}�[���X�^�[�g
		_spriteitf_timer_restart();
		return;
	}

	//��M���ďo���֐�
	if(x_spriteitf_uart_fnRecv){
		//�I�����v�����s��ꂽ�ꍇ�ACnt�𒼂���Max�ɕύX
		if((*x_spriteitf_uart_fnRecv)(c,x_spriteitf_uart_Cnt-1))x_spriteitf_uart_Cnt=x_spriteitf_uart_Cmd.InSize;
	}

	//��M�I������
	if(x_spriteitf_uart_Cnt >= x_spriteitf_uart_Cmd.InSize){
		//LED����
		_spriteitf_led_set(0);
		//�����݃��[�h��ʏ�ɕύX
		x_spriteitf_uart_Mode=0;
		//�J�E���^�[��0�ɖ߂�
		x_spriteitf_uart_Cnt=0;
		//��M���荞�݋֎~
		_spriteitf_uart_fget_interrupt_disable();
		//�^�C�}�[�X�g�b�v
		_spriteitf_timer_stop();
		//��M�I�����ďo���֐�
		if(x_spriteitf_uart_fnFinRecv)(*x_spriteitf_uart_fnFinRecv)();
		//�����łʂ���B�^�C���A�E�g�Ď���ON���Ȃ�
		return;
	}
	//�^�C�}�[�ĊJ
	_spriteitf_timer_restart();
}
//��M�^�C���A�E�g���̃G���[�����֐��ݒ�
void _spriteitf_uart_setErrFunc(spriteitf_vFp_ucp fnTimeOutErr){
	x_spriteitf_uart_fnTimeOutErr=fnTimeOutErr;
}

//===�ʏ�֐��Q===//
//���
spriteitf_status_t _spriteitf_Status=spriteitf_Status_Idle;
//�J�����̉摜�T�C�Y
spriteitf_imagesize_t _spriteitf_ImageSize=0;
//getDataSize������オ��(���[�U�[������)
boolian _spriteitf_TakePicFlag=hmLib_false;
//getData or stopPic������ɋ�����(���[�U�[������)
boolian _spriteitf_readPicDatFlag=hmLib_false;
//�ʐM�^�C���A�E�g�������p���[���Z�b�g
boolian _spriteitf_AutoResetFlag=hmLib_false;
//�ʐ^�̃f�[�^�T�C�Y
spriteitf_dsize_t _spriteitf_PicDataSize=0;
//�ʐ^�f�[�^�̎�M�o�b�t�@�ւ̃|�C���^�F���[�U�[��beginReadPicDat�ŋ����Ă����
unsigned char* _spriteitf_BufPtr=0;
//���[�U�[�w��̎�M�f�[�^�������[�U�[�������o�b�t�@�̑傫��
spriteitf_dsize_t _spriteitf_BufSize=0;
//���[�U�[�������o�b�t�@�ɏ������񂾃f�[�^��
spriteitf_dsize_t* _spriteitf_BufWriteNumPtr=0;
//�ʐ^��b�f�[�^��M�������ɌĂ΂�郆�[�U�[��`�֐�
spriteitf_vFp_v _spriteitf_fnInformTakePic=0;
//�ʐ^�f�[�^��M�������ɌĂ΂�郆�[�U�[��`�֐�
spriteitf_vFp_v _spriteitf_fnInformReadPicDat=0;
//�ʐ^�B�e�I�������������ɌĂ΂�郆�[�U�[��`�֐�
spriteitf_vFp_v _spriteitf_fnInformEndTakePic=0;
//�G���[�����֐�
void _spriteitf_fnCatchErr(void){
	//�G���[�R�[�h�����炤
	spriteitf_error_t ErrCode=spriteitf_getErrorCode();
	//�ʐM�^�C���A�E�g�n�̃G���[�̎��A�X�e�[�^�X��Idle�ɂ��ǂ�
	if((ErrCode & spriteitf_error_LowBitMask) == spriteitf_error_TimeOut)_spriteitf_Status=spriteitf_Status_Idle;
}
//��ԕϐ�������
void _spriteitf_iniStatusVal(void){
	_spriteitf_Status=spriteitf_Status_Idle;
	_spriteitf_ImageSize=0;
	_spriteitf_TakePicFlag=hmLib_false;
	_spriteitf_readPicDatFlag=hmLib_false;
	_spriteitf_AutoResetFlag=hmLib_false;
	_spriteitf_PicDataSize=0;
	_spriteitf_BufPtr=0;
	_spriteitf_BufSize=0;
}
//��ԕϐ��I�[��
#define _spriteitf_finStatusVal() _spriteitf_iniStatusVal()
//�X�e�[�^�X�擾�֐�
spriteitf_status_t spriteitf_getStatus(void){return _spriteitf_Status;}
//��Ԃ��ҋ@��Ԓ����H
boolian spriteitf_isIdle(void){return spriteitf_getStatus() == spriteitf_Status_Idle;}
//Request����ImageSize���擾����֐�
spriteitf_imagesize_t spriteitf_getRequestImageSize(void){return _spriteitf_ImageSize;}
//�ʐ^�S�f�[�^�T�C�Y��v���\���H
boolian spriteitf_can_getPicInfo(void){return _spriteitf_TakePicFlag;}
//�ʐ^�S�f�[�^�T�C�Y�v��
spriteitf_dsize_t spriteitf_getAllPicDataSize(void){return _spriteitf_PicDataSize;}
//�ʐ^�f�[�^�p�o�b�t�@�Ƀf�[�^�͂��ׂė��܂��Ă��邩�H
boolian spriteitf_can_getPicData(void){return _spriteitf_readPicDatFlag;}
//�ʐ^�f�[�^�p�o�b�t�@�T�C�Y�v��
spriteitf_dsize_t spriteitf_sizePicData(void){return _spriteitf_BufSize;}
//�ʐ^��b�f�[�^��M�����t���O���N���A
void spriteitf_clear_TakePicFlag(void){_spriteitf_TakePicFlag=hmLib_false;}
//�f�[�^��M�����t���O���N���A
void spriteitf_clear_ReadPicDatFlag(void){_spriteitf_readPicDatFlag=hmLib_false;}
//�ʐ^�B���Ă������H
boolian spriteitf_can_TakePic(void){return (!_spriteitf_TakePicFlag && !_spriteitf_readPicDatFlag && spriteitf_isIdle());}
//��M�^�C���A�E�g���̃G���[�֐�
void _spriteitf_fnRecvTimeOutErr(unsigned char* ptr){
	_spriteitf_uart_fget_interrupt_disable();
	//�G���[�𓊂���
	if(ptr==_spriteitf_varCmd_SET_IMAGE_SIZE){
		_spriteitf_ImageSize=0;
		_spriteitf_throw(spriteitf_error_TimeOut_SetImageSize);
	}else if(ptr==_spriteitf_Cmd_TAKE_PICTURE){
		_spriteitf_throw(spriteitf_error_TimeOut_TakePic);
	}else if(ptr==_spriteitf_Cmd_GET_DATA_SIZE){
		_spriteitf_throw(spriteitf_error_TimeOut_GetDataSize);
	}else if(ptr==_spriteitf_varCmd_GET_DATA){
		_spriteitf_throw(spriteitf_error_TimeOut_GetData);
		//�f�[�^�擾�p�ϐ��I�[��
		_spriteitf_BufWriteNumPtr=0;
		_spriteitf_BufPtr=0;
		_spriteitf_BufSize=0;
	}else if(ptr==_spriteitf_Cmd_STOP_TAKE_PICTURE){
		_spriteitf_throw(spriteitf_error_TimeOut_StopTakePic);
	}else if(ptr==_spriteitf_Cmd_RESET_CMR){
		_spriteitf_throw(spriteitf_error_TimeOut_CommandReset);
	}else _spriteitf_throw(spriteitf_error_TimeOut_unUsedCommand);

	if(_spriteitf_AutoResetFlag)spriteitf_PowerReset();

}

//--------------------�ʐ^��b���ǂݍ��݊֐�����------------------
//GetDataSize�������֐�
boolian _spriteitf_fnRecv_getPicInfo(unsigned char c,spriteitf_dsize_t pos){
	//DataSize_high��M
	if(pos == 7){
		_spriteitf_PicDataSize=(((spriteitf_dsize_t)c)<<8)&0xFF00;
	}//DataSize_low��M
	else if(pos == 8){
		//DataSize����������
		_spriteitf_PicDataSize+=((spriteitf_dsize_t)c)&0x00FF;
	}
	return 0;	
}
//�ʐ^��b�f�[�^��M�����쓮�֐�
void _spriteitf_fnFinTakePicCommand(void){
	//�J�����փR�}���h���M�ł��Ȃ����G���[�𓊂���
	if(!_spriteitf_uart_can_sendCommand()){
		switch (_spriteitf_Status)
		{
		case spriteitf_Status_SetImageSize:
			return _spriteitf_throw(spriteitf_error_TakePic_cannotsend_ActiveImageSize);
		case spriteitf_Status_ActiveImageSize:
			return _spriteitf_throw(spriteitf_error_TakePic_cannotsend_TakePic);
		case spriteitf_Status_TakePic:
			return _spriteitf_throw(spriteitf_error_TakePic_cannotsend_ReadPicInfo);
		default:
			return;
		}
	}

	//�X�e�[�^�X�J�ځ{�R�}���h���M
	switch (_spriteitf_Status)
	{
	case spriteitf_Status_SetImageSize:
		_spriteitf_Status=spriteitf_Status_TakePic;
		return _spriteitf_uart_sendCommand(_spriteitf_Cmd_TakePicture,_spriteitf_fnFinTakePicCommand,0);
	case spriteitf_Status_TakePic:
		_spriteitf_Status=spriteitf_Status_ReadPicInfo;
		return _spriteitf_uart_sendCommand(_spriteitf_Cmd_GetDataSize,_spriteitf_fnFinTakePicCommand,_spriteitf_fnRecv_getPicInfo);
	case spriteitf_Status_ReadPicInfo:
		_spriteitf_Status=spriteitf_Status_Idle;
		_spriteitf_TakePicFlag=hmLib_true;
		if(_spriteitf_fnInformTakePic!=0)(*_spriteitf_fnInformTakePic)();
//		_spriteitf_fnInformTakePic=0;
		return;
	default:
		//�z��O�̃X�e�[�^�X���A�G���[�𓊂���
		if(_spriteitf_Status==spriteitf_Status_Idle)_spriteitf_throw(spriteitf_error_TakePic_invalidStatus_Idle);
		else if(_spriteitf_Status==spriteitf_Status_ReadPicData)_spriteitf_throw(spriteitf_error_TakePic_invalidStatus_ReadPicData);
		else if(_spriteitf_Status==spriteitf_Status_EndTakePic)_spriteitf_throw(spriteitf_error_TakePic_invalidStatus_EndTakePic);
		else if(_spriteitf_Status==spriteitf_Status_CommandReset)_spriteitf_throw(spriteitf_error_TakePic_invalidStatus_CommandReset);
//		else if(_spriteitf_Status==spriteitf_Status_PowerReset)_spriteitf_throw(spriteitf_error_TakePic_invalidStatus_PowerReset);
		return;
	}
}
//�ʐ^�B�e�J�n�v��
boolian spriteitf_beginTakePic(spriteitf_imagesize_t ImageSize_,spriteitf_vFp_v fnInformTakePic){
	//�ʐ^�B�e�J�n���Ă����Ȃ����̓X���[
	if(_spriteitf_TakePicFlag)return hmLib_true;
	if(_spriteitf_Status!=spriteitf_Status_Idle)return hmLib_true;
	if(!_spriteitf_uart_can_sendCommand())return hmLib_true;

	_spriteitf_fnInformTakePic=fnInformTakePic;
	//ImageSize_���ُ�ȏꍇ�ɂ�160_120�ɐݒ�
	if(ImageSize_ != spriteitf_PictureSize_160_120 && ImageSize_!=spriteitf_PictureSize_320_240 && ImageSize_!=spriteitf_PictureSize_640_480){
		ImageSize_ = spriteitf_PictureSize_160_120;
	}

	//�v���B�e�T�C�Y��������
	if(ImageSize_==_spriteitf_ImageSize){
		_spriteitf_Status=spriteitf_Status_TakePic;
		//�J�����ɎB�e�v��
		_spriteitf_uart_sendCommand(_spriteitf_Cmd_TakePicture,_spriteitf_fnFinTakePicCommand,0);
	}//�B�e�v���T�C�Y���O��ƈقȂ�Ƃ�
	else{
		_spriteitf_Status=spriteitf_Status_SetImageSize;
		_spriteitf_ImageSize=ImageSize_;
		//�J�����Ɏʐ^�T�C�Y�ύX�v��
		_spriteitf_uart_sendCommand(_spriteitf_varCmd_SetImageSize(_spriteitf_ImageSize),_spriteitf_fnFinTakePicCommand,0);

	}
	return hmLib_false;
}

//--------------------�ʐ^�f�[�^�ǂݍ��݊֐�����--------------------
#define _spriteitf_GetPicCmdHeaderSize 5
#define _spriteitf_JPEGTrmnSize 2
static const unsigned char _spriteitf_JPEGTrmn[_spriteitf_JPEGTrmnSize]={0xFF,0xD9};
boolian _spriteitf_fnRecv_getPicData(unsigned char c,spriteitf_dsize_t cnt){
	if(cnt>=(_spriteitf_GetPicCmdHeaderSize) && cnt<(_spriteitf_GetPicCmdHeaderSize+_spriteitf_BufSize)){
		//�f�[�^����M
		_spriteitf_BufPtr[cnt-(_spriteitf_GetPicCmdHeaderSize)]=c;
		if(_spriteitf_BufWriteNumPtr)(*_spriteitf_BufWriteNumPtr)+=1;
/*
		//�O�̂��߃f�[�^�������������Ȃ����m�F
		if(cnt>_spriteitf_GetPicCmdHeaderSize+_spriteitf_JPEGTrmnSize && _spriteitf_BufPtr[cnt-(_spriteitf_GetPicCmdHeaderSize+1)-1]==_spriteitf_JPEGTrmn[0] && _spriteitf_BufPtr[cnt-(_spriteitf_GetPicCmdHeaderSize+1)]==_spriteitf_JPEGTrmn[1]){
		}
*/
	}
	return hmLib_false;
}
//�ʐ^�f�[�^�ǂݍ��݋쓮�֐�
void _spriteitf_fnFinReadPicDat(void){
	switch (_spriteitf_Status)
	{
	case spriteitf_Status_ReadPicData:
		//Idle��Ԃɖ߂�
		_spriteitf_Status=spriteitf_Status_Idle;
		//�ǂݍ��݉\�t���O��������
		_spriteitf_readPicDatFlag=hmLib_true;
		//ReadTakePic�������ɌĂяo���֐������[�U�[����^�����Ă���Ύ��s
		if(_spriteitf_fnInformReadPicDat!=0)(*_spriteitf_fnInformReadPicDat)();
		//�ϐ�������
//		_spriteitf_fnInformReadPicDat=0;
//		_spriteitf_BufPtr=0;
//		_spriteitf_BufSize=0;
//		_spriteitf_BufWriteNumPtr=0;
		return;
	default:
		//�z��O�̃X�e�[�^�X���A�G���[�𓊂���
		if(_spriteitf_Status==spriteitf_Status_Idle)_spriteitf_throw(spriteitf_error_ReadPicData_invalidStatus_Idle);
		else if(_spriteitf_Status==spriteitf_Status_SetImageSize)_spriteitf_throw(spriteitf_error_ReadPicData_invalidStatus_SetImageSize);
		else if(_spriteitf_Status==spriteitf_Status_ActiveImageSize)_spriteitf_throw(spriteitf_error_ReadPicData_invalidStatus_ActiveImageSize);
		else if(_spriteitf_Status==spriteitf_Status_TakePic)_spriteitf_throw(spriteitf_error_ReadPicData_invalidStatus_TakePic);
		else if(_spriteitf_Status==spriteitf_Status_ReadPicInfo)_spriteitf_throw(spriteitf_error_ReadPicData_invalidStatus_ReadPicInfo);
		else if(_spriteitf_Status==spriteitf_Status_EndTakePic)_spriteitf_throw(spriteitf_error_ReadPicData_invalidStatus_EndTakePic);
		else if(_spriteitf_Status==spriteitf_Status_CommandReset)_spriteitf_throw(spriteitf_error_ReadPicData_invalidStatus_CommandReset);
//		else if(_spriteitf_Status==spriteitf_Status_PowerReset)_spriteitf_throw(spriteitf_error_ReadPicData_invalidStatus_PowerReset);
		return;
	}
}
//�ʐ^�f�[�^�ǂݍ��ݗv��	BufPtr:�f�[�^����Ăق����ꏊ,begin:�ʐ^�f�[�^�̓ǂݏo���J�n�ʒu�i8�̔{���j,size:begin����ǂݏo���f�[�^��
boolian spriteitf_beginReadPicDat(unsigned char* BufPtr_,spriteitf_dsize_t begin_,spriteitf_dsize_t* size_ptr_,spriteitf_dsize_t size_,spriteitf_vFp_v fnInformReadPicData){
	//�ǂݍ��ݗv�����Ă����Ȃ����̓X���[
	if(!_spriteitf_TakePicFlag || _spriteitf_readPicDatFlag)return hmLib_true;
	if(_spriteitf_Status!=spriteitf_Status_Idle)return hmLib_true;
	if(!_spriteitf_uart_can_sendCommand())return hmLib_true;
	//�ǂݏo���ʒu��8�̔{���łȂ��Ƃ��͋���
	if(begin_%8 !=0)return hmLib_true;

	_spriteitf_fnInformReadPicDat=fnInformReadPicData;
	//��M�o�b�t�@�ƃT�C�Y�����炤
	_spriteitf_BufPtr=BufPtr_;
	_spriteitf_BufSize=size_;
	_spriteitf_BufWriteNumPtr=size_ptr_;
	if(_spriteitf_BufPtr==0)return hmLib_true;
	_spriteitf_Status=spriteitf_Status_ReadPicData;
	//�J�����Ɏʐ^�f�[�^��v��
	_spriteitf_uart_sendCommand(_spriteitf_varCmd_GetData(begin_,size_),_spriteitf_fnFinReadPicDat,_spriteitf_fnRecv_getPicData);
	return hmLib_false;
}
//�ʐ^�ǂݍ��ݍς݃f�[�^���擾
uint16 spriteitf_getReadDataNum(void){
	if(_spriteitf_BufWriteNumPtr)return (*_spriteitf_BufWriteNumPtr);
	else return 0;
}

//----------------------�B�e�I���֐�����----------------------------
//�B�e�I�������쓮�֐�
void _spriteitf_fnFinEndTakePic(void){
	switch (_spriteitf_Status)
	{
	case spriteitf_Status_EndTakePic:
		//Idle��Ԃɖ߂�
		_spriteitf_Status=spriteitf_Status_Idle;
		//Take Pic�n�̃t���O��S�ăN���A
		_spriteitf_TakePicFlag=hmLib_false;
		_spriteitf_readPicDatFlag=hmLib_false;
		//EndTakePic�������ɌĂяo���֐������[�U�[����^�����Ă���Ύ��s
		if(_spriteitf_fnInformEndTakePic!=0)(*_spriteitf_fnInformEndTakePic)();
		//�ϐ�������
//		_spriteitf_fnInformEndTakePic=0;
		return;
	default:
		//�z��O�̃X�e�[�^�X���A�G���[�𓊂���
		if(_spriteitf_Status==spriteitf_Status_Idle)_spriteitf_throw(spriteitf_error_EndTakePic_invalidStatus_Idle);
		else if(_spriteitf_Status==spriteitf_Status_SetImageSize)_spriteitf_throw(spriteitf_error_EndTakePic_invalidStatus_SetImageSize);
		else if(_spriteitf_Status==spriteitf_Status_ActiveImageSize)_spriteitf_throw(spriteitf_error_EndTakePic_invalidStatus_ActiveImageSize);
		else if(_spriteitf_Status==spriteitf_Status_TakePic)_spriteitf_throw(spriteitf_error_EndTakePic_invalidStatus_TakePic);
		else if(_spriteitf_Status==spriteitf_Status_ReadPicInfo)_spriteitf_throw(spriteitf_error_EndTakePic_invalidStatus_ReadPicInfo);
		else if(_spriteitf_Status==spriteitf_Status_ReadPicData)_spriteitf_throw(spriteitf_error_EndTakePic_invalidStatus_ReadPicData);
		else if(_spriteitf_Status==spriteitf_Status_CommandReset)_spriteitf_throw(spriteitf_error_EndTakePic_invalidStatus_CommandReset);
//		else if(_spriteitf_Status==spriteitf_Status_PowerReset)_spriteitf_throw(spriteitf_error_EndTakePic_invalidStatus_PowerReset);
		return;
	}
}
//�B�e�I���v��
boolian spriteitf_endTakePic(spriteitf_vFp_v fnInformEndTakePic){
	//�I���v�����Ă����Ȃ����̓X���[
	if(!_spriteitf_TakePicFlag || _spriteitf_readPicDatFlag)return hmLib_true;
	if(_spriteitf_Status!=spriteitf_Status_Idle)return hmLib_true;
	if(!_spriteitf_uart_can_sendCommand())return hmLib_true;
	
	_spriteitf_fnInformEndTakePic=fnInformEndTakePic;
	_spriteitf_Status=spriteitf_Status_EndTakePic;
	_spriteitf_uart_sendCommand(_spriteitf_Cmd_StopTakePicture,_spriteitf_fnFinEndTakePic,0);
	return hmLib_false;
}

//------------------------�R�}���h���Z�b�g�֐�����---------------------
//�R�}���h���Z�b�g�쓮�֐�
void _spriteitf_fnFin_CommandReset(void){
	switch (_spriteitf_Status)
	{
	case spriteitf_Status_CommandReset:
		//�t���O���N���A
		_spriteitf_TakePicFlag=hmLib_false;
		_spriteitf_readPicDatFlag=hmLib_false;
		_spriteitf_Status=spriteitf_Status_Idle;
		return;
	default:
		//�z��O�̃X�e�[�^�X���A�G���[�𓊂���
		if(_spriteitf_Status==spriteitf_Status_Idle)_spriteitf_throw(spriteitf_error_CommandReset_invalidStatus_Idle);
		else if(_spriteitf_Status==spriteitf_Status_SetImageSize)_spriteitf_throw(spriteitf_error_CommandReset_invalidStatus_SetImageSize);
		else if(_spriteitf_Status==spriteitf_Status_ActiveImageSize)_spriteitf_throw(spriteitf_error_CommandReset_invalidStatus_ActiveImageSize);
		else if(_spriteitf_Status==spriteitf_Status_TakePic)_spriteitf_throw(spriteitf_error_CommandReset_invalidStatus_TakePic);
		else if(_spriteitf_Status==spriteitf_Status_ReadPicInfo)_spriteitf_throw(spriteitf_error_CommandReset_invalidStatus_ReadPicInfo);
		else if(_spriteitf_Status==spriteitf_Status_ReadPicData)_spriteitf_throw(spriteitf_error_CommandReset_invalidStatus_ReadPicData);
		else if(_spriteitf_Status==spriteitf_Status_EndTakePic)_spriteitf_throw(spriteitf_error_CommandReset_invalidStatus_EndTakePic);
		else if(_spriteitf_Status==spriteitf_Status_PowerReset)_spriteitf_throw(spriteitf_error_CommandReset_invalidStatus_PowerReset);
		return;
	}
}
//�R�}���h���Z�b�g�֐�
boolian spriteitf_CommandReset(void){
	//�R�}���h���Z�b�g���Ă����Ȃ����̓X���[
	if(!_spriteitf_uart_can_sendCommand())return hmLib_true;
	if(_spriteitf_Status!=spriteitf_Status_Idle)return hmLib_true;

	_spriteitf_Status=spriteitf_Status_CommandReset;
	_spriteitf_uart_sendCommand(_spriteitf_Cmd_ResetCmr,_spriteitf_fnFin_CommandReset,0);

	return hmLib_false;	
}

//-------------------------�������Z�b�g�֐�����-------------------------
//�p���[���Z�b�g�쓮�֐�
void _spriteitf_fnPowerReset(void){
	static unsigned char count = 0;
	static unsigned char power = 0;
	boolian tmpAutoPwResetFlag=0;
	//�J�E���^��i�߂�
	++count;

	//5�b�ȏ�ҋ@�����ꍇ�͓�����
	if(count > (unsigned char)(5000/spriteitf_RecvTimeOut + 1)){
		count = 0;		//�������Z�b�g�I��
		power = 0;
		//�����Ńp���[���Z�b�g�����AIdle�ɖ߂�
		_spriteitf_Status=spriteitf_Status_Idle;
		_spriteitf_timer_stop();
		return;
	}//�ҋ@��3�b�ȏ�œd���������Ă��Ȃ��ꍇ
	else if(power==0 && count > (unsigned char)(3000/spriteitf_RecvTimeOut + 1)){
		//�I�[���Ńt���O�̏�Ԃ�Y��Ă��܂��̂ňꎞ�ޔ�
		tmpAutoPwResetFlag=_spriteitf_AutoResetFlag;
		//spriteitf������
		spriteitf_initialize();
		//�ޔ������Ă����t���O���
		_spriteitf_AutoResetFlag=tmpAutoPwResetFlag;
		//uart�Đڑ�(�Đڑ��Ɠ����ɃJ���������������Ă���̂ŁC�^�C�}�[�X�g�b�v�͍Ō�ɂ��Ȃ��ƁC��M���荞�݂��N�����Ď�M���荞�݊֐����̃^�C�}�[���Z�b�g�ɏ�����ă��Z�b�g���畜�A�ł��Ȃ��Ȃ�)
		power=1;						//�d������ꂽ��
	}
	_spriteitf_timer_stop();
	_spriteitf_timer_start(_spriteitf_fnPowerReset);
//	_spriteitf_timer_restart();//�Đڑ��Ɠ����ɃJ���������������Ă���̂ŁC�^�C�}�[�X�g�b�v�͍Ō�ɂ��Ȃ��ƁC��M���荞�݂��N�����Ď�M���荞�݊֐����̃^�C�}�[���Z�b�g�ɏ�����ă��Z�b�g���畜�A�ł��Ȃ��Ȃ�
	return;
}
//�p���[���Z�b�g�֐�
boolian spriteitf_PowerReset(void){
	//�I�[���Ńt���O�̏�Ԃ�Y��Ă��܂��̂ňꎞ�ޔ�
	boolian tmpAutoPwResetFlag=_spriteitf_AutoResetFlag;
	//spriteitf�I�[��
	spriteitf_finalize();
	//�ޔ������Ă����t���O���
	_spriteitf_AutoResetFlag=tmpAutoPwResetFlag;
	//Status�J��
	_spriteitf_Status=spriteitf_Status_PowerReset;
	//�^�C�}�[�ݒ�
	_spriteitf_timer_start(_spriteitf_fnPowerReset);
	return hmLib_false;
}
//�I�[�g�p���[���Z�b�g�@�\�̃Z�b�g
void spriteitf_setAutoResetMode(boolian val){_spriteitf_AutoResetFlag=val;}
//�I�[�g�p���[���Z�b�g�@�\�̏�Ԋm�F
boolian spriteitf_getAutoResetMode(void){return _spriteitf_AutoResetFlag;}

//------------------------�������E�I�[���֐�--------------------------
//������
void spriteitf_initialize(void){
	//�J�������W���[���d��ON
	_spriteitf_power(1);
	//uart������
	_spriteitf_uart_initialize();
	//timer������
	_spriteitf_timer_initialize();
	//error������
	_spriteitf_error_initialize();
	//�ϐ�������
	_spriteitf_iniStatusVal();
	_spriteitf_fnInformTakePic=0;
	_spriteitf_fnInformReadPicDat=0;
	_spriteitf_fnInformEndTakePic=0;
	//�G���[�����o�^
	_spritetif_set_catch(_spriteitf_fnCatchErr);
	_spriteitf_uart_setErrFunc(_spriteitf_fnRecvTimeOutErr);
}
//�I�[��
void spriteitf_finalize(void){
	//�ϐ��I�[��
	_spriteitf_finStatusVal();
	_spriteitf_fnInformTakePic=0;
	_spriteitf_fnInformReadPicDat=0;
	_spriteitf_fnInformEndTakePic=0;
	//�G���[�����I�[��
	_spritetif_set_catch(0);
	_spriteitf_uart_setErrFunc(0);
	//error�I�[��
	_spriteitf_error_finalize();
	//timer�I�[��
	_spriteitf_timer_finalize();
	//uart�I�[��
	_spriteitf_uart_finalize();
	//�J�������W���[���d��OFF
	_spriteitf_power(0);
}

#endif
