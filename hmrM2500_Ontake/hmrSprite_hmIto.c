#ifndef HMR_SPRITE_C_INC
#define HMR_SPRITE_C_INC 16
#
#define HMLIB_NOLIB
#include"hmrType.h"
#include<hmLib_v3_04/cqueue.h>
#ifndef HMR_SPRITE_INC
#	include"hmrSprite.h"
#endif
#ifndef HMR_HMR_SPRITEBUFFER_INC_INC
#	include "hmrSpriteBuffer.h"
#endif
#ifndef HMR_UTILITY_INC
#	include "hmrUtility.h"
#endif
/*
#ifndef HMR_DEVICECONFIG_INC
#	include"hmrDeviceConfig.h"
#endif
#ifndef HMR_SPRITEINTERFACE_INC
#	include"hmrSpriteInterface.h"
#endif
#ifndef HMR_MESSAGE_INC
#	include"hmrMessage.h"
#endif
*/
#ifdef __cplusplus
using namespace hmLib;
#endif
//�ʐ^�f�[�^�p�P�b�g�̏��
typedef struct{
	spritebuf_id BufID;	//�g�p�o�b�t�@ID
	uint16 DataPos;		//�������܂ꂽ�f�[�^�̈ʒu
}sprite_pac_info;
//================ Sprite����ϐ� =================
//takl�҂��̃o�b�t�@�L���[
sprite_pac_info x_sprite_TalkableBufData[spritebuf_Num+1];
cqueue _sprite_TalkableBufQue;
//�����t���b�V��ON/OFF
boolian sprite_AutoLight=0;
//���݂̃~�j�p�P�b�g���[�h
boolian sprite_MiniPacMode=0;
//�~�j�p�P�b�g���[�h��V������̗v��
boolian _sprite_reserve_MiniPacMode=0;
//���C�g��ON/OFF
boolian _sprite_LightData=0;
//�A���B�e���[�hON/OFF
boolian _sprite_AutoTakePic=0;
//�ʐ^�擾���̎ʐ^�T�C�Y
uint8 _sprite_Pixel=0;
//�A���B�e���[�h���̎ʐ^�T�C�Y
uint8 _sprite_AutoTakePixel=0;
//���܂łœǂݍ��񂾑S�f�[�^��
uint16 _sprite_PicDataPos=0;
//�ʐ^�̑S�T�C�Y
uint16 _sprite_PicDataSize=0;
//���݃f�[�^��M���̃o�b�t�@ID
spritebuf_id _sprite_BufID=0;
//���ݗv�����̃f�[�^�̃T�C�Y
uint16 _sprite_RequestSize=0;

//=============== SpriteInterface���̓z�� ==================
//�J�����ɓn����o�b�t�@����������true
boolian x_sprite_itf_IsWaitBuf=hmLib_false;
boolian _sprite_itf_is_waitBuf(void){return x_sprite_itf_IsWaitBuf;}
//�ʐ^��b�f�[�^�擾�I�����ɌĂ΂��֐��ASpriteInterface�ɓn��
void _sprite_itf_fnFinGetPicInfo(void){
	//�ʐ^��b�f�[�^�擾
	_sprite_PicDataPos=0;
	_sprite_PicDataSize=spriteitf_getAllPicDataSize();

	//�f�[�^�����ɍs��
	_sprite_itf_readPicData();
}
//�ʐ^�ǂݍ��݊������ɌĂ΂��֐��CSpriteInterface�ɓn��
void _sprite_itf_fnFinReadPicData(void){
	//���荞�ݓI�t���O���N���A
	spriteitf_clear_ReadPicDatFlag();
	
	//interface����o�b�t�@�����[�X
	spritebuf_release_fromSpriteInterface(_sprite_BufID);

	//�G���[�A�������Z�b�g���͐i�܂Ȃ�
	if(spriteitf_isError() || spriteitf_getStatus()==spriteitf_Status_PowerReset)return;

	//�擾���I������f�[�^�����Z
	_sprite_PicDataPos+=_sprite_RequestSize;

	//�S�ăf�[�^���擾�ς݂Ȃ�I���A���Ă��Ȃ���Ύ��̃f�[�^�����ɍs��
	if(_sprite_PicDataPos>=_sprite_PicDataSize)spriteitf_endTakePic(0);
	else _sprite_itf_readPicData();
}
//�ʐ^�f�[�^�ǂݍ���
boolian _sprite_itf_readPicData(){
	uint16 RequestNum=0;
	sprite_pac_info PacketInfo;
	unsigned char* BufPtr=0;
	uint16* CntPtr=0;

	//���łɎB�e�������Ă���ꍇ�̓G���[
	if(_sprite_PicDataSize<=_sprite_PicDataPos)return hmLib_true;

	//�v���T�C�Y���m��
	RequestNum=(uint16)(_sprite_PicDataSize-_sprite_PicDataPos);
	if(sprite_MiniPacMode){
		if(RequestNum>spritebuf_DataSize_Small)RequestNum=(uint16)(spritebuf_DataSize_Small/8)*8;
	}else{
		if(RequestNum>spritebuf_DataSize)RequestNum=(uint16)(spritebuf_DataSize/8)*8;
	}

	_sprite_RequestSize=RequestNum;

	//�o�b�t�@�m��
	_sprite_BufID=spritebuf_create(RequestNum);

	//�m�ۂɎ��s�����ꍇ�́AWaitBuf�t���O�𗧂ĂĂ���
	if(_sprite_BufID==0){
		x_sprite_itf_IsWaitBuf=hmLib_true;
		return hmLib_true;
	}

	//talk���̂��߂ɂ��̃p�P�b�g�̏���ێ�
	PacketInfo.BufID=_sprite_BufID;
	PacketInfo.DataPos=_sprite_PicDataPos;

	
	spritebuf_share_withSpriteInterface(_sprite_BufID,&BufPtr,&CntPtr);

	if(BufPtr==0 || CntPtr==0){
		spritebuf_release_fromSpriteInterface(_sprite_BufID);
		spritebuf_release(_sprite_BufID);
		return hmLib_true;
	}
	//interface���̎n��
	if(spriteitf_beginReadPicDat(BufPtr,_sprite_PicDataPos,CntPtr,RequestNum,_sprite_itf_fnFinReadPicData)){
		spritebuf_release_fromSpriteInterface(_sprite_BufID);
		spritebuf_release(_sprite_BufID);
		return hmLib_true;
	}
	x_sprite_itf_IsWaitBuf=hmLib_false;

	//�L���[�ɋl�߂�
	cqueue_push(&_sprite_TalkableBufQue,&PacketInfo);

	return hmLib_false;
}
//sprite_itf�������֐�
void _sprite_itf_initialzie(){
	sprite_power(1);
	spriteitf_initialize();
	_spriteitf_resistInformPwReset(_sprite_itf_initialzie,_sprite_itf_finalize);
}
//sprite_itf�I�[���֐�
void _sprite_itf_finalize(){
	if(_sprite_BufID){
		spritebuf_release_fromSpriteInterface(_sprite_BufID);
		_sprite_BufID=0;
	}
	spriteitf_finalize();
	sprite_power(0);
}
//=============== Message���̓z�� ==================
struct{
	unsigned SendPicInfo:1;				//�ʐ^�������łɑ��������ǂ���
	unsigned GetPicInfo:1;				//�ʐ^���擾�̗L��
	unsigned GetPicture:1;				//�ʐ^�擾���[�h
	unsigned SetAutoTakePic:1;			//�A���B�e���[�h�ݒ�
	unsigned SetLightPW:1;				//���C�g��ON/OFF�����郂�[�h
	unsigned SetAutoLight:1;			//���C�g����ON/OFF�ݒ胂�[�h
	unsigned SetMiniPacMode:1;			//�~�j�p�P�b�g���[�h�ݒ胂�[�h
	unsigned SendMiniPacACK:1;			//�~�j�p�P�b�g���[�h�ݒ�ACK���M���[�h�i�J�����f�[�^�擾���̃p�P�b�g�T�C�Y�ύX�������j
	unsigned RequestIntReset:1;			//�������Z�b�g�v�����[�h
	unsigned SetAutoReset:1;			//�G���[�������������Z�b�g�ݒ胂�[�h
	unsigned SendError:1;				//�G���[���M���[�h
	unsigned SendDevicePw:1;				//�����f�[�^���M
	unsigned DevicePw:1;			//�����f�[�^���M���[�h
	unsigned DevicePw_i:1;			//�����f�[�^���M���[�h��
}_sprite_Mode={0,0,0,1,1,1,0,1,0,1,0,0,0,1};
void sprite_setup_listen(void){return;}
void sprite_setup_talk(void){
	//�G���[�C�������Z�b�g�̎��̓o�b�t�@�����[�X
	if(spriteitf_isError()){
		_sprite_PicDataPos=0;
		_sprite_PicDataSize=0;
		spritebuf_release_fromSpriteInterface(_sprite_BufID);
	}

	//�ʐ^�f�[�^�ǂݍ��ݑ҂�������Ƃ�
	if(_sprite_itf_is_waitBuf() && spriteitf_isIdle()){
		_sprite_itf_readPicData();
	}

	//������񑗐M���[�h��
	if(_sprite_Mode.DevicePw)_sprite_Mode.SendDevicePw=hmLib_true;

	//�~�j�p�P�b�g���[�h�ݒ�i�J�����f�[�^�擾���̃p�P�b�g�T�C�Y�ύX��h���j
	if(spriteitf_can_TakePic() && _sprite_Mode.SetMiniPacMode){
		_sprite_Mode.SetMiniPacMode=hmLib_false;
		sprite_MiniPacMode=_sprite_reserve_MiniPacMode;
		_sprite_Mode.SendMiniPacACK=hmLib_true;
	}

	//TakePicInfo����������Ă��邩
	if(_sprite_Mode.SendPicInfo && spriteitf_can_getPicInfo()){
		_sprite_Mode.SendPicInfo=hmLib_false;
		_sprite_Mode.GetPicInfo=hmLib_true;
	}

	//Message�ɓn����f�[�^�����邩�H
	if(!cqueue_empty(&_sprite_TalkableBufQue))_sprite_Mode.GetPicture=hmLib_true;

	//�A���B�e���[�h���Ɏʐ^�B�e���Ă������Ƃ��A�B�e�v��������
	if(_sprite_AutoTakePic && spriteitf_can_TakePic()){
		_sprite_Mode.SendPicInfo=hmLib_true;

		//���C�g��_������
		if(sprite_AutoLight){
			sprite_powerLight(1);
			delay_ms(200);
		}

		//SPRITE�Ŏʐ^�B�閽�ߑ���
		if(spriteitf_beginTakePic(_sprite_AutoTakePixel,_sprite_itf_fnFinGetPicInfo)){
			_sprite_Mode.SendPicInfo=hmLib_false;
			return;
		}
		_sprite_Pixel=_sprite_AutoTakePixel;

		//���C�g����������
		if(sprite_AutoLight){
			sprite_powerLight(0);
		}
	}
}
hmLib_boolian sprite_listen(cstring Str){
	static uint8 c=0;
	
	//�f�[�^�T�C�Y�m�F
	if(cstring_size(&Str)==0)return hmLib_true;

	c=cstring_getc(&Str,0);

	//1byte�ڂŃ��[�h�𕪂���
	switch(c){
	case 0x00://�ʐ^�擾���[�h
		if(cstring_size(&Str)!=2)return hmLib_true;
		//�ʐ^�B���Ă�����ԂŁA�B�e�v���������ꍇ
		if(spriteitf_can_TakePic()){
			_sprite_Mode.SendPicInfo=hmLib_true;
			//PictureSize�ݒ�
			if(cstring_getc(&Str,1)>=2){
				c=spriteitf_PictureSize_640_480;
			}else if(cstring_getc(&Str,1)==1){
				c=spriteitf_PictureSize_320_240;
			}else{
				c=spriteitf_PictureSize_160_120;
			}
			
			//���C�g��_��
			if(sprite_AutoLight){
				sprite_powerLight(1);
				delay_ms(200);
			}

			//SPRITE�Ŏʐ^�B�閽�ߑ���
			if(spriteitf_beginTakePic(c,_sprite_itf_fnFinGetPicInfo)){
				_sprite_Mode.SendPicInfo=hmLib_false;
				return hmLib_false;
			}
			//PictureSize������Ă���
			_sprite_Pixel=c;

			//���C�g������
			if(sprite_AutoLight){
				sprite_powerLight(0);
			}
 		}
		return hmLib_false;
	case 0x10://�A���B�e���[�hON
		if(cstring_size(&Str)<2)return hmLib_true;
		_sprite_Mode.SetAutoTakePic=hmLib_true;
		_sprite_AutoTakePic=1;

		//PictureSize�ݒ�
		if(cstring_getc(&Str,1)>=2){
			c=spriteitf_PictureSize_640_480;
		}else if(cstring_getc(&Str,1)==1){
			c=spriteitf_PictureSize_320_240;
		}else{
			c=spriteitf_PictureSize_160_120;
		}

		_sprite_AutoTakePixel = c;
		return hmLib_false;

	case 0x11://�A���B�e���[�hOFF
		_sprite_Mode.SetAutoTakePic=hmLib_true;
		_sprite_AutoTakePic=0;
		return hmLib_false;
	case 0x30:	//���C�g��ON�v��
		_sprite_Mode.SetLightPW=hmLib_true;
		//���C�gON
		sprite_powerLight(1);
		//�v���ݒ�����炤
		_sprite_LightData=1;
		return hmLib_false;
	case 0x31://���C�g��OFF�v��
		_sprite_Mode.SetLightPW=hmLib_true;
		//���C�gOFF
		sprite_powerLight(0);
		//�v���ݒ�����炤
		_sprite_LightData=0;
		return hmLib_false;
	case 0x40:	//�����t���b�V��ON�ݒ�
		_sprite_Mode.SetAutoLight=hmLib_true;
		//�����t���b�V�����[�hON
		sprite_AutoLight=1;
		return hmLib_false;
	case 0x41:	//�����t���b�V��OFF�ݒ�
		_sprite_Mode.SetAutoLight=hmLib_true;
		//���C�g�������Ă���
		sprite_powerLight(0);
		//�����t���b�V�����[�hOFF
		sprite_AutoLight=0;
		return hmLib_false;
	case 0x50:	//�~�j�p�P�b�g���[�hON�ݒ�
		_sprite_Mode.SetMiniPacMode=hmLib_true;
		//�~�j�p�P�b�g���[�hON�\��
		_sprite_reserve_MiniPacMode=1;
		return hmLib_false;
	case 0x51:	//�~�j�p�P�b�g���[�hOFF�ݒ�
		_sprite_Mode.SetMiniPacMode=hmLib_true;
		//�~�j�p�P�b�g���[�hOFF�\��
		_sprite_reserve_MiniPacMode=0;
		return hmLib_false;
	case 0x60:	//�������Z�b�g�v��
		_sprite_Mode.RequestIntReset=hmLib_true;
		//���Z�b�g���s��
		spriteitf_CommandReset();
		return hmLib_false;
	case 0x70:	//�������Z�b�g�v��
		sprite_finalize();
		spriteitf_resistInformPwReset(_sprite_itf_finalize,_sprite_itf_initialize);
		//�������Z�b�g��������
		spriteitf_PowerReset();
		return hmLib_false;
	case 0x80:	//�G���[�������������Z�b�gON�ݒ�
		_sprite_Mode.SetAutoReset=hmLib_true;
		//AutoReset�@�\ON
		spriteitf_setAutoResetMode(1);
		return hmLib_false;
	case 0x81:	//�G���[�������������Z�b�gOFF�ݒ�
		_sprite_Mode.SetAutoReset=hmLib_true;
		//AutoReset�@�\OFF
		spriteitf_setAutoResetMode(0);
		return hmLib_false;
	case 0xB0:	//�����f�[�^���M���[�hON
		_sprite_Mode.DevicePw=hmLib_true;
		_sprite_Mode.DevicePw_i=hmLib_true;
		_sprite_itf_initialize();
		return hmLib_false;
	case 0xB1:	//�����f�[�^���M���[�hOFF
		_sprite_Mode.DevicePw=hmLib_false;
		_sprite_Mode.DevicePw_i=hmLib_true;
		_sprite_itf_finalize();
		return hmLib_false;
	default:
		return hmLib_true;
	}
}
hmLib_boolian sprite_talk(cstring* pStr){
	sprite_pac_info PacketInfo;
	msg_vFp_v FpInformDestruct;
	msg_dsizeFp_v FpSendableNum;

	//�A���B�e���[�h��ON/OFF�ݒ�
	if(_sprite_Mode.SetAutoTakePic){
		//�t���O�����낷
		_sprite_Mode.SetAutoTakePic=hmLib_false;
		cstring_construct_dynamic_safe(pStr,1);
		if(_sprite_AutoTakePic)cstring_putc(pStr,0,0x10);
		else cstring_putc(pStr,0,0x11);

		return hmLib_false;
	}
	//���C�g��ON/OFF
	else if(_sprite_Mode.SetLightPW){
		//�t���O�����낷
		_sprite_Mode.SetLightPW=hmLib_false;

		cstring_construct_dynamic_safe(pStr,1);
		if(_sprite_LightData)cstring_putc(pStr,0,0x30);
		else cstring_putc(pStr,0,0x31);
		return hmLib_false;
	}
	//�t���b�V���@�\ON/OFF�̐ݒ�
	else if(_sprite_Mode.SetAutoLight){
		//�t���O�����낷
		_sprite_Mode.SetAutoLight=hmLib_false;

		cstring_construct_dynamic_safe(pStr,1);
		if(sprite_AutoLight)cstring_putc(pStr,0,0x40);
		else cstring_putc(pStr,0,0x41);

		return hmLib_false;
	}
	//�~�j�p�P�b�g���[�h���ǂ����̐ݒ�
	else if(_sprite_Mode.SendMiniPacACK){
		//�t���O�����낷
		_sprite_Mode.SendMiniPacACK=hmLib_false;

		cstring_construct_dynamic_safe(pStr,1);
		if(sprite_MiniPacMode)cstring_putc(pStr,0,0x50);
		else cstring_putc(pStr,0,0x51);

		return hmLib_false;
	}
	//�������Z�b�g�v��
	else if(_sprite_Mode.RequestIntReset){
		//�t���O�����낷�D
		_sprite_Mode.RequestIntReset=hmLib_false;

		cstring_construct_dynamic_safe(pStr,2);
		cstring_putc(pStr,0,0x60);
		cstring_putc(pStr,1, spriteitf_getStatus());

		return hmLib_false;
	}
	//�G���[�������������Z�b�gON�EOFF�ݒ�
	else if(_sprite_Mode.SetAutoReset){
		//�t���O�����낷
		_sprite_Mode.SetAutoReset=hmLib_false;

		cstring_construct_dynamic_safe(pStr,1);

		if(spriteitf_getAutoResetMode())cstring_putc(pStr,0,0x80);
		else cstring_putc(pStr,0,0x81);

		return hmLib_false;
	}else if(_sprite_Mode.DevicePw_i){
		_sprite_Mode.DevicePw_i=hmLib_false;

		cstring_construct_dynamic_safe(pStr,1);
		if(_sprite_Mode.DevicePw)cstring_putc(pStr,0,0xB0);
		else cstring_putc(pStr,0,0xB1);

		return hmLib_false;
	
	//�������擾
	}else if(_sprite_Mode.SendDevicePw){
		//�t���O�����낷�D
		_sprite_Mode.SendDevicePw=hmLib_false;

		cstring_construct_dynamic_safe(pStr,4);
		cstring_putc(pStr,0,0xA0);
		cstring_putc(pStr,1,spriteitf_getStatus());			//���݂̃X�e�[�^�X
		cstring_putc(pStr,2,spriteitf_isError());				//���݃G���[��Ԃ��ǂ���
		cstring_putc(pStr,3,spriteitf_getErrorCode());		//�G���[���̃X�e�[�^�X

		return hmLib_false;
	}
	//�ʐ^�擾���[�hACK�ԐM
	else if(_sprite_Mode.GetPicInfo){
		//�t���O�����낷
		_sprite_Mode.GetPicInfo=hmLib_false;

		if(spriteitf_isError()){
			sprite_powerLight(0);
			return hmLib_false;
		}

		//ACK���M
		cstring_construct_dynamic_safe(pStr,4);
		cstring_putc(pStr,0,(unsigned char)(0x00));
		cstring_putc(pStr,1,(unsigned char)(_sprite_Pixel));
		cstring_putc(pStr,2,(unsigned char)(spriteitf_getAllPicDataSize()));
		cstring_putc(pStr,3,(unsigned char)(spriteitf_getAllPicDataSize()>>8));

		return hmLib_false;
	}
	//�ʐ^�B�eor�f�[�^�v�����[�h
	else if(_sprite_Mode.GetPicture){
		//�t���O�����낷
		_sprite_Mode.GetPicture=hmLib_false;

		if(spriteitf_isError() || spriteitf_getStatus()==spriteitf_Status_PowerReset)return hmLib_false;

		//talk�҂��L���[����z���o��
		PacketInfo=(*(sprite_pac_info*)cqueue_getptr(&_sprite_TalkableBufQue));
		cqueue_pop(&_sprite_TalkableBufQue);
		if(PacketInfo.BufID==0)return hmLib_true;

		//message�ɓn��
		spritebuf_share_withMesasge(PacketInfo.BufID,pStr,PacketInfo.DataPos,&FpSendableNum,&FpInformDestruct);

		// �I�[�����\��
		msg_talkconfig_useInformDestruct(FpInformDestruct);
		//VMC�ւ̑��M�\���ʒm
		msg_talkconfig_useSendable(FpSendableNum);

		//�����[�X
		spritebuf_release(PacketInfo.BufID);

		delay_ms(5);

		return hmLib_false;
	}
	return hmLib_true;
}
void _sprite_iniVal(void){
	_sprite_PicDataSize=0;
	sprite_AutoLight=0;
	sprite_MiniPacMode=0;
	_sprite_reserve_MiniPacMode=0;
	_sprite_LightData=0;
	_sprite_AutoTakePic=0;
	_sprite_Pixel=0;
	_sprite_AutoTakePixel=0;
	_sprite_PicDataPos=0;
	_sprite_PicDataSize=0;
	_sprite_BufID=0;
	_sprite_RequestSize=0;
	//�L���[�R���X�g���N�g
	cqueue_construct_static(&_sprite_TalkableBufQue,x_sprite_TalkableBufData,x_sprite_TalkableBufData+spritebuf_Num+1,sizeof(sprite_pac_info));
}
void _sprite_finVal(void){
	_sprite_PicDataSize=0;
	sprite_AutoLight=0;
	sprite_MiniPacMode=0;
	_sprite_reserve_MiniPacMode=0;
	_sprite_LightData=0;
	_sprite_AutoTakePic=0;
	_sprite_Pixel=0;
	_sprite_AutoTakePixel=0;
	_sprite_PicDataPos=0;
	_sprite_PicDataSize=0;
	_sprite_BufID=0;
	_sprite_RequestSize=0;
	//�L���[�f�X�g���N�g
	cqueue_destruct(&_sprite_TalkableBufQue);
}
//=============== Sprite���J�֐� ===================
void sprite_initialize(void){
	_sprite_itf_initialize();
	_sprite_iniVal();

	//cqueue_construct_static(&_sprite_TalkableBufQue,x_sprite_TalkableBufData,x_sprite_TalkableBufData+spritebuf_Num+2,sizeof(sprite_pac_info));
}
void sprite_finalize(void){
	sprite_pac_info PacketInfo;

	//�o�b�t�@�����[�X
	while(!cqueue_empty(&_sprite_TalkableBufQue)){
		PacketInfo=(*(sprite_pac_info*)cqueue_getptr(&_sprite_TalkableBufQue));
		spritebuf_release(PacketInfo.BufID);
		cqueue_pop(&_sprite_TalkableBufQue);
	}

	_sprite_finVal();
	_sprite_itf_finalize();
}
sint16 sprite_task_inform(sint16 interval){
	_sprite_Mode.DevicePw_i=hmLib_true;
	return interval;
}
#
#endif
