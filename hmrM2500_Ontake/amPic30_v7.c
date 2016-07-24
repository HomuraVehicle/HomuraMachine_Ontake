#ifndef AMPIC30_C_INC
#define AMPIC30_C_INC 701
#
#define HMLIB_NOLIB
//
//���������@dsPIC C30�R���p�C���p���C�u�����@����������
//							Ver. 3-1
//*�f�[�^�^(bit)
//	int 16
//  long 32
//  long long 64
//  double 32
//  long double 64
//
//
//
//**�o���@
//	�E���荞�݂ɂ��ā@
//		���荞�݂ɂ͂��ꂼ�ꊄ�荞�݃t���O���p�ӂ���Ă���	
//		���Ƃ��΂t�r�`�q�s�̏ꍇ��U1TXIF,U1RXIF������B
//		���̊��荞�݃t���O���ǂ̂悤�ȏ����łn�m�ɂ��邩�́A
//		�e�@�\�̃��W�X�^(USART���W�X�^)�Ŏw�肷��K�v������
//	
//	�E���M�A��M�֐��̐���
//		getc,putc,kbhit�̐���
//		OERR=0 �͎�M�o�b�t�@�I�[�o�[�t���[�G���[�̎�����
//
//	�Edelay�֐��ɂ��ā@
//�@�@	delay�֐��̂��߂�timer2,3���g�����̂ŁAtimer2,3�͎g�p�ł��Ȃ��d�l�ɂȂ��Ă���I�I
//
//  �E�`�c�ϊ�����ۂ́A�|�[�g�̓��o�͂��ԈႢ�Ȃ����邱�ƁB
//		Vref�ɂ��Ă����͂ɂ���K�v������
//
//
//
//
//
//
//
//:::::::::::::::::::�[�[�X�V�����[�[:::::::::::::::::::::::::
//
//  100724	UART�ɂ��ă��C�u�������i�߂�
//	100807	UART�ɂ��ă��C�u���������@
//			�^�C�}�[�A���Ԋ֌W�̃��C�u����������
//	100808�@�`�c�ϊ��ɂ��Ă̊֐��Q���쐬
//			Vref�����܂������Ă��Ȃ�(��H�̖�肩�H)
//			SPI�ɂ��Ă͓���`�F�b�N�ς�
//	100809  WDT�@�\�������Ɠ������Ƃ��m�F
//	100911	delay_us()������
//�@�@�@	set_tris(),output_high,low����
//  131019  Timer4,5 ini_timer4 �Ƀo�O�𔭌�(�r�b�g�V�t�g�̌������Ԉ���Ă���)�@�|���C��
//			Timer �̐��x���グ�邽�߂ɁA�v���X�P�[����1:1�ɕύX���Ă����			
//
//
//
//

//#include <p33FJ256GP710.h>
//#include <timer.h>
#include "amPic30_v7.h"

#include "hmrDebug.h"
#include "hmrDevice.h"
/*
//config(���C���ł���ăN��)�@�Q�l��(���̊֐��Q�̓���m�F�ςݐݒ��) 
//
//�@WDT�I�t�ɂ��Ă���̂́A�����Ń\�t�g�E�F�A�I��ON,OFF�ł��邽��
//  WDT�͓����̕ʂ̃N���b�N�Ō��܂�̂ŁA���C���N���b�N�̑����ɂ͈ˑ����Ȃ�
//  WDT��WINDOW���[�h�́AWDT������3/4�ȍ~�ɁAWDT�N���A���Ȃ��ƁA���Z�b�g��������Ƃ������҂Ȃ̂�OFF
//	�����N���b�N��PLL�ɂ���đ����ł���B 
//�@
//
_FOSCSEL( FNOSC_FRC);		//Internal FRC(7.37MHz)
_FOSC( FCKSM_CSDCMD & OSCIOFNC_ON & POSCMD_NONE ); //HS
_FWDT( FWDTEN_OFF & WINDIS_OFF & WDTPRE_PR128 & WDTPOST_PS512 );//WDT 2s
_FPOR( FPWRT_PWR64 );//power on timer 64ms 
*/

uint64 clock = 7370000;//�N���b�N
uint8 _am_delay_ms_isUsed; // delay_ms ���g���Ă���t���O(2�d�Ăяo���ɂ�閳�����[�v�΍�)
uint8 _am_delay_ms2_isUsed;

//�K���K�v�ȏ������֐�
void ini_clock(uint64 _clock){//�N���b�N�̐ݒ�(�����l7.37MH)�{�^�C�}�[�̐ݒ�(�K�����Ă�������)  
	clock = _clock;//�N���b�N��ێ�

	//Timer2�ݒ�^^delay_ms()�p^^
	T2CONbits.T32 = 0;			//16bit mode
	__asm__("NOP");
	T2CONbits.TCKPS = 1;		//�v���X�P�[���P�E�W
	__asm__("NOP");
	T2CONbits.TCS = 0;			//�����N���b�N�g��
	__asm__("NOP");
	T2CONbits.TGATE = 0;		//�Q�[�g���ԗݎZ����
	PR2 = (uint16)((double)clock / (2.*8.*1000.) + 0.5);//�����1ms�̎����ɂȂ�
	TMR2 = 0;					//T2�J�E���g���N���A
	_am_delay_ms_isUsed = 0;	// delay_ms ���g���Ă��邩�ǂ���

	//Timer3�ݒ�^^delay_us()�p^^
	T3CONbits.TCKPS = 0;		//�v���X�P�[���P�E1
	__asm__("NOP");
	T3CONbits.TCS = 0;			//�����N���b�N�g��
	__asm__("NOP");
	T3CONbits.TGATE = 0;		//�Q�[�g���ԗݎZ����
	PR3 = (uint16)((double)clock / (2.*100000.) + 0.5);//�����10us�̎����ɂȂ�
	TMR3 = 0;//T3�J�E���g���N���A

	//Timer6�ݒ�^^delay_ms2()�p^^
	T6CONbits.T32 = 0;			//16bit mode
	__asm__("NOP");
	T6CONbits.TCKPS = 1;		//�v���X�P�[���P�E�W
	__asm__("NOP");
	T6CONbits.TCS = 0;			//�����N���b�N�g��
	__asm__("NOP");
	T6CONbits.TGATE = 0;		//�Q�[�g���ԗݎZ����
	PR6 = (uint16)((double)clock / (2.*8.*1000.) + 0.5);//�����1ms�̎����ɂȂ�
	TMR6 = 0;					//T6�J�E���g���N���A
	_am_delay_ms2_isUsed = 0;	// delay_ms2 ���g���Ă��邩�ǂ���
}

//�V���A���ʐM�n�֐�
void ini_UART1(uint32 baudrate){//UART1�ݒ�{UART1�g�p����
	U1MODEbits.UARTEN = 1;		//UART1�L����
	U1STAbits.UTXEN = 1;		//UART���M����
	U1BRG = (int)((double)clock/(2.*(double)baudrate*16.) + 0.5 -1.) ;//�{�[���[�g�ݒ�(���̒l��8bit�����Ȃ��悤�ɒ��ӁI)
	U1STAbits.UTXISEL1 = 0;		//���M���荞�݃��[�h�̑I���F���M�o�b�t�@�ɋ󂫂��ł����犄�荞��
	__asm__("NOP");
	U1STAbits.UTXISEL0 = 0;		//���M���荞�݃��[�h�̑I���F���M�o�b�t�@�ɋ󂫂��ł����犄�荞��
	__asm__("NOP");
	U1STAbits.URXISEL = 0;		//UART��M���荞�ݐݒ�(�P��M�����犄�荞��)
}
void ini_UART2(uint32 baudrate){//UART2�ݒ�{UART2�g�p����
	U2MODEbits.UARTEN = 1;		//UART1�L����
	U2STAbits.UTXEN = 1;		//UART���M����
	U2BRG = (int)((double)clock/(2.*(double)baudrate*16.) + 0.5 -1.) ;//�{�[���[�g�ݒ�(���̒l��8bit�����Ȃ��悤�ɒ��ӁI)
	U2STAbits.UTXISEL1 = 0;		//���M���荞�݃��[�h�̑I���F���M�o�b�t�@�ɋ󂫂��ł����犄�荞��
	__asm__("NOP");
	U2STAbits.UTXISEL0 = 0;		//���M���荞�݃��[�h�̑I���F���M�o�b�t�@�ɋ󂫂��ł����犄�荞��
	__asm__("NOP");
	U2STAbits.URXISEL = 0;		//UART��M���荞�ݐݒ�(�P��M�����犄�荞��)
}
//�V���A���ʐM�ɂ����銄�荞�݂̎g����
/*
���荞�݂������Ă��(�ȉ��̊֐��R)
���M���荞�݂́A���M�o�b�t�@�ɋ󂫂��ł����犄�荞��ł���B
��M���荞�݂́A������M�����犄�荞��ł���B
���C���֐��̎n�܂�O�̂Ƃ���ŁA�ȉ��̂悤�Ɋ��荞�ݏ����������Ă��Ηǂ�
steam �̏����ԈႢ�ɒ���
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void){//UART1�ɂ�鑗�M���荞��
	clear_interrupt_fputflag(1);		//���荞�݃t���O�N���A
	fputc(data,1);						//�f�[�^���M
}
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void){//UART2�ɂ���M���荞��
	clear_interrupt_fgetflag(2); 		//���荞�݃t���O�N���A
	data = fgetc(2);					//�f�[�^��M
}
*/
void clear_interrupt_fgetflag(uint8 stream){
	if(stream&1)IFS0bits.U1RXIF = 0;
	if(stream&2)IFS1bits.U2RXIF = 0;
}
void clear_interrupt_fputflag(uint8 stream){
	if(stream&1)IFS0bits.U1TXIF = 0;
	if(stream&2)IFS1bits.U2TXIF = 0;
}
void set_interrupt_fgetflag(uint8 stream){
	if(stream&1)IFS0bits.U1RXIF = 1;
	if(stream&2)IFS1bits.U2RXIF = 1;
}
void set_interrupt_fputflag(uint8 stream){
	if(stream&1)IFS0bits.U1TXIF = 1;
	if(stream&2)IFS1bits.U2TXIF = 1;
}
void enable_interrupt_fget(uint8 stream){
	//UART1��M���荞�݋���
	if(stream&1){
		IFS0bits.U1RXIF = U1STAbits.URXDA;
		IEC0bits.U1RXIE = 1;
	}
	//UART2��M���荞�݋���
	if(stream&2){
		IFS1bits.U2RXIF = U2STAbits.URXDA;
		IEC1bits.U2RXIE = 1;
	}
}
void enable_interrupt_fput(uint8 stream){//���M���荞�݋���
	//UART1���M���荞�݋���
	if(stream&1)IEC0bits.U1TXIE = 1;
	//UART2���M���荞�݋���
	if(stream&2)IEC1bits.U2TXIE = 1;
}
void disable_interrupt_fget(uint8 stream){
	//UART1��M���荞�݋֎~
	if(stream&1)IEC0bits.U1RXIE = 0;
	//UART2��M���荞�݋֎~
	if(stream&2)IEC1bits.U2RXIE = 0;
}
void disable_interrupt_fput(uint8 stream){
	//UART1���M���荞�݋֎~
	if(stream&1)IEC0bits.U1TXIE = 0;
	//UART2���M���荞�݋֎~
	if(stream&2)IEC1bits.U2TXIE = 0;
}

//���荞�݃t���O��ێ�����֐��@�����@�댯�B�g���ȁI�I
uint8 tmp_disable_interrupt_fget(uint8 stream){
	uint8 ans=0;
	if((stream&1) && IEC0bits.U1RXIE){
		disable_interrupt_fget(stream);
		ans|=1;
	}
	if((stream&2) && IEC1bits.U2RXIE){
		disable_interrupt_fget(stream);
		ans|=2;
	}
	return ans;
}
uint8 tmp_disable_interrupt_fput(uint8 stream){
	uint8 ans=0;
	if((stream&1) && IEC0bits.U1TXIE){
		disable_interrupt_fget(stream);
		ans|=4;
	}
	if((stream&2) && IEC1bits.U2TXIE){
		disable_interrupt_fget(stream);
		ans|=8;
	}
	return ans;
}
uint8 tmp_disable_interrupt_fgetput(uint8 stream){
	uint8 ans=0;
	if(stream&1){
		if(IEC0bits.U1TXIE && IEC0bits.U1RXIE){
			IEC0bits.U1RXIE = 0;
			__asm__("NOP");
			IEC0bits.U1TXIE = 0;
			ans|=1+4;
		}else if(IEC0bits.U1TXIE){
			IEC0bits.U1TXIE = 0;
			ans|=4;
		}else if(IEC0bits.U1RXIE){
			IEC0bits.U1RXIE = 0;
			ans|=1;
		}
	}
	if(stream&2){
		if(IEC1bits.U2TXIE && IEC1bits.U2RXIE){
			IEC1bits.U2RXIE = 0;
			__asm__("NOP");
			IEC1bits.U2TXIE = 0;
			ans|=2+8;
		}else if(IEC1bits.U2TXIE){
			IEC1bits.U2TXIE = 0;
			ans|=8;
		}else if(IEC1bits.U2RXIE){
			IEC1bits.U2RXIE = 0;
			ans|=2;
		}
	}
	return ans;
}
void replace_interrupt_fget(uint8 c,uint8 stream){
	if(stream&1) IEC0bits.U1RXIE=(c&1);
	if(stream&2) IEC1bits.U2RXIE=((c&2)==2);
}
void replace_interrupt_fput(uint8 c,uint8 stream){
	if(stream&1) IEC0bits.U1TXIE=((c&4)==4);
	if(stream&2) IEC1bits.U2TXIE=((c&8)==8);
}
void replace_interrupt_fgetput(uint8 c,uint8 stream){
	uint8 u;
	if(stream&1){
		u=IFS0bits.U1RXIF+2*IFS0bits.U1TXIF;
		IFS0bits.U1RXIF=0;
		IFS0bits.U1TXIF=0;
		IEC0bits.U1RXIE=(c&1);
		IEC0bits.U1TXIE=((c&4)==4);
		IFS0bits.U1RXIF=(u&1);
		IFS0bits.U1TXIF=((u&2)==2);
	}
	if(stream&2){
		u=IFS1bits.U2RXIF+2*IFS1bits.U2TXIF;
		IFS1bits.U2RXIF=0;
		IFS1bits.U2TXIF=0;
		IEC1bits.U2RXIE=((c&2)==2);
		IEC1bits.U2TXIE=((c&8)==8);
		IFS1bits.U2RXIF=(u&1);
		IFS1bits.U2TXIF=((u&2)==2);
	}
}

//rs�֐�
void rs_fputc(uint8 data, uint8 stream){//�P�������M����
	if(stream == 1){
		while(U1STAbits.UTXBF == 1);
		U1TXREG = data;
	}else if(stream == 2){
		while(U2STAbits.UTXBF == 1);
		U2TXREG = data;
	}
}

uint8 rs_fgetc(uint8 stream){//�P������M����
	if(stream == 1){
		while(U1STAbits.URXDA == 0);
		if(U1STAbits.OERR==1)U1STAbits.OERR = 0;//�����o�b�t�@�[���I�[�o�[�t���[���Ă�����A�N���A����
		return U1RXREG;
	}else if(stream == 2){
		while(U2STAbits.URXDA == 0);
		if(U2STAbits.OERR==1)U2STAbits.OERR = 0;//�����o�b�t�@�[���I�[�o�[�t���[���Ă�����A�N���A����
		return U2RXREG;
	}else return 0;
}

uint8 rs_kbhit(uint8 stream){//��M�o�b�t�@�[�ɓ����Ă��邩�H�P�Ȃ�yes
	if(stream == 1){
		return (uint8)U1STAbits.URXDA; 
	}else if(stream == 2){
		return (uint8)U2STAbits.URXDA; 
	}else return 0;
}



//�֗��֐�
void delay_ms(uint16 time){//time ms�������ԑ҂����Ă��	
	uint32 j;
	if(_am_delay_ms_isUsed != 0)return;
	_am_delay_ms_isUsed = 1;
	TMR2 = 0;
	T2CONbits.TON = 1;//timer2�쓮
	for(j=0;j<time;++j){//time ms�҂�
		TMR2 = 0;
		IFS0bits.T2IF = 0;
		while(!(IFS0bits.T2IF));//1ms�҂�
	}
	T2CONbits.TON = 0;//timer2�~�߂�
	_am_delay_ms_isUsed = 0;
}
void delay_us(uint16 time){// time us�������ԑ҂����Ă��(���݂�10us)
	uint32 j;
	TMR3 = 0;
	T3CONbits.TON = 1;//timer3�쓮
	for(j=0;j<(time/10);++j){//time us�҂�
		TMR3 = 0;
		IFS0bits.T3IF = 0;
		while(IFS0bits.T3IF==0);//10us�҂�						 
	}
	T3CONbits.TON = 0;//timer3�~�߂�
}

void delay_ms2(uint16 time){//time ms�������ԑ҂����Ă��	
	uint32 j;
	if(_am_delay_ms2_isUsed != 0)return;
	_am_delay_ms2_isUsed = 1;
	TMR6 = 0;
	T6CONbits.TON = 1;//timer6�쓮
	for(j=0;j<time;++j){//time ms�҂�
		TMR6 = 0;
		IFS2bits.T6IF = 0;
		while(IFS2bits.T6IF==0);//1ms�҂�						 
	}
	T6CONbits.TON = 0;//timer6�~�߂�
	_am_delay_ms2_isUsed = 0;
}

//timer1�p�֐�
void ini_timer1(uint16 time){//time(ms)�i1�`500ms�̊ԁj�Ɉ�񊄂荞��
	T1CONbits.TCKPS = 3;		//prescaller 1/256
	PR1 = (int)((double)clock*(double)time / (2.*256.*1000.) + 0.5);//PR1���w��
	TMR1= 0;					//T1�J�E���g���N���A
}
/*
//���荞�݂̎g����
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void){//T1IF�ɂ�鑗�M���荞��
	IFS0bits.T1IF = 0;			//T1���荞�݃t���O���N���A
	TMR1 = 0;					//T1�J�E���g���N���A
	PORTAbits.RA0 += 1;			//LED�̏�Ԃ�ω�(����)
}
*/
void enable_interrupt_timer1(){//���荞�݋���
	if(IEC0bits.T1IE==0){
		TMR1 = 0;					//T1�J�E���g���N���A
		IEC0bits.T1IE = 1;			//T1���荞�݂�����	
		T1CONbits.TON = 1;			//T1�J�E���g�J�n
	}
}
void disable_interrupt_timer1(){//���荞�ݏI��
	if(IEC0bits.T1IE!=0){
		IEC0bits.T1IE = 0;			//T1���荞�݂��֎~	
		T1CONbits.TON = 0;			//T1�J�E���g�I��
		TMR1 = 0;					//T1�J�E���g���N���A
	}
}
uint8 can_interrupt_timer1(){
	return IEC0bits.T1IE;
}
uint8 tmp_disable_interrupt_timer1(){
	if(IEC0bits.T1IE==0)return 0;
	disable_interrupt_timer1();
	return 1;
}
void replace_interrupt_timer1(uint8 c){
	if(IEC0bits.T1IE==0 && c!=0){
		TMR1 = 0;					//T1�J�E���g���N���A
		IEC0bits.T1IE = 1;			//T1���荞�݂�����	
		T1CONbits.TON = 1;			//T1�J�E���g�J�n
	}else if(IEC0bits.T1IE!=0 && c==0){
		IEC0bits.T1IE = 0;			//T1���荞�݂��֎~	
		T1CONbits.TON = 0;			//T1�J�E���g�I��
		TMR1 = 0;					//T1�J�E���g���N���A
	}
}
void reset_timer1(){
	TMR1 = 0;					//T1�J�E���g���N���A
}

//timer4�p�֐�
void ini_timer4(uint16 time){//time(ms)�i1�`50000ms�̊ԁj�Ɉ�񊄂荞��
	uint32 val=(uint32)((double)clock*(double)time / (2.* 1. *1000.));
	T5CONbits.TON = 0; // Stop any 16-bit Timer3 operation
	T4CONbits.TON = 0; // Stop any 16/32-bit Timer3 operation
	__asm__("NOP");
	T4CONbits.T32 = 1;			//32bit mode	
	__asm__("NOP");
	T4CONbits.TCKPS = 0;		//prescaller 1/1 
	__asm__("NOP");	
	T4CONbits.TCS = 0;			//�����N���b�N�g��
	__asm__("NOP");
	T4CONbits.TGATE = 0;		//�Q�[�g���ԗݎZ����
	PR5 = (uint16)(val>>16);//���PR
	PR4 = (uint16)(val);//����PR���w��
	TMR5 = 0;					//T4�J�E���g���N���A
	TMR4 = 0;
	T4CONbits.TON = 1; // Start 32-bit Timer
}
/*
//���荞�݂̎g����
void __attribute__((interrupt, no_auto_psv)) _T5Interrupt(void){//T5(4�ł͂Ȃ�����)IF�ɂ�鑗�M���荞��
	IFS1bits.T5IF = 0;			//T5���荞�݃t���O���N���A
	TMR5 = 0;
	TMR4 = 0;                   //T4�J�E���g���N���A
	PORTAbits.RA0 += 1;			//LED�̏�Ԃ�ω�(����)
}
*/
void enable_interrupt_timer4(){//���荞�݋���
	if(IEC1bits.T5IE==0){//���荞�݂�timer�T���W�X�^�Ő��䂷��d�l
		TMR5 = 0;
		TMR4 = 0; //T4�J�E���g���N���A
		IEC1bits.T5IE = 1;			//T4���荞�݂�����	
		T4CONbits.TON = 1;			//T4�J�E���g�J�n
	}
}
void disable_interrupt_timer4(){//���荞�ݏI��
	if(IEC1bits.T5IE!=0){
		IEC1bits.T5IE = 0;			//T1���荞�݂��֎~	
		T4CONbits.TON = 0;			//T1�J�E���g�I��
		TMR5 = 0;					//T1�J�E���g���N���A
		TMR4 = 0; 
	}
}
void reset_timer4(){
	TMR5 = 0;					//T1�J�E���g���N���A
	TMR4 = 0;
}



//AD�ϊ��֘A�֐�
/*
void setup_adc_ports(uint32 port){//AD�ϊ��Ɏg���|�[�g���߁i�� port = AN1|AN3|AN23) 
	uint32 port1 = 0xFFFF0000;
	uint32 port2 = 0x0000FFFF;  
	port1 = port & port1;		//���16�r�b�g�̂ݎc��
	port1 = port1>>16;			//16�r�b�g�E�ɃV�t�g
	AD1PCFGH = ~((uint32)port1);	//���]���āA�|�[�g�\�����W�X�^�ɓ����B0�Ȃ�AD�|�[�g�Ƃ��Ďg���B
								//AN16~31�s���̐ݒ�
	port2 = port & port2;		
	AD1PCFGL = ~((uint32)port2);	//AN0~15�s���̐ݒ�
}
*/
//AD�ϊ��Ɏg��Vref����(ADC1)
void setup_adc1_Vref(uint8 mode){
	AD1CON2bits.VCFG = mode;
}
//12bit���[�h�ɂ��邩�ۂ�
void setup_adc1_12bit(uint8 Y_N){
	if(Y_N == 1){
		AD1CON1bits.AD12B = 1;		//12bit���[�h
	}else{
		AD1CON1bits.AD12B = 0;		//10bit���[�h
	}
}
//AD�ϊ����Ԃ�20us�ʂɂȂ�悤�Ɏ����ݒ肷��֐�
void setup_adc1_time_auto(){
	AD1CON1bits.SSRC = 7;		//�����J�E���^�ŃT���v�����O�I�����ĕϊ��J�n
	AD1CON3bits.SAMC = 0;		//Auto�T���v�����ԁ@�O
	__asm__("NOP");
	AD1CON3bits.ADCS = (int)((double)clock/(2.*1000000.)) + 21;//�ϊ��N���b�N�I��
															//�P�^�[���ϊ�(T������ADCS)���P��s�ȏ�ɂȂ�悤��		
	AD1CON1bits.ADON = 1;		//ADC�L��
}
//�`�����l���w��֐�(���̌㎞�ԑ҂��͕K�v�Ȃ�)
void set_adc_channel(uint8 ch){
	AD1CHS0bits.CH0SA = ch;//�`�����l���w��
}
//ADC�f�[�^�ǂݍ��݊֐�(�Q�Ous���炢������)
uint32 read_adc(){
	AD1CON1bits.SAMP = 1;	 //�T���v�����O�X�^�[�g
	while(!AD1CON1bits.DONE);//�ϊ��I���܂ő҂�
	AD1CON1bits.DONE=0;		//�I���X�e�[�^�X��߂�
	return ADC1BUF0;		//���ʂ�ԐM
}
// AD�ϊ��̃��[�h�̐ݒ�C1�`�����l�����ƂɂP�X�L�������邩�C�܂Ƃ߂�16�X�L�������邩
void set_adc_sampleMode(uint32 mode){
    if(mode == EVERY){
        AD1CON2bits.SMPI = 0;// set DONE flag every 1 sample operation
        __asm__("NOP");
		AD1CON2bits.BUFM = 0;//Always starts filling the buffer from the start address
    }else{
        //������
    }
}

//SPI�p�֐�
void setup_spi(uint8 mode){

	//�N���b�N�J�n�v���g�R���̐ݒ�
	if((mode & 0x02) == 0x02){//CLOCK_H_TO_L �ŃX�^�[�g
		SPI1CON1bits.CKP = 1;	
	}else{//CLOCK L to H �ŃX�^�[�g
		SPI1CON1bits.CKP = 0;
	}
	
	//MASTER or SLAVE
	if((mode & 0x01)%2 == 0){//MASTER mode
		SPI1CON1bits.MSTEN = 1;		//�}�X�^�[���[�h�ɐݒ�
		//�N���b�N�����P
		if((mode & 0x1C)==0x00){
			SPI1CON1bits.PPRE = 3;		//1st = 1:1
		}else if((mode & 0x1C)==0x04){
			SPI1CON1bits.PPRE = 2;		//1st = 4:1
		}else if((mode & 0x1C)==0x08){
			SPI1CON1bits.PPRE = 1;		//1st = 16:1
		}else if((mode & 0x1C)==0x10){
			SPI1CON1bits.PPRE = 0;		//1st = 64:1
		}
		//�N���b�N�����Q
		SPI1CON1bits.SPRE = ((~((mode & 0xE0)>>5)) & 0x07);//�����Q�̐ݒ�

		//SPI�P�N��	
		SPI1STATbits.SPIROV = 0;	//��M�N���A
		__asm__("NOP");
		SPI1STATbits.SPIEN = 1;		//SPI1�L��

	}else{//SLAVE mode
		SPI1BUF = 0x00;				//�o�b�t�@���N���A
		SPI1CON1bits.MSTEN = 0;		//�X���[�u���[�h�ɐݒ�
		SPI1CON1bits.SMP = 0;		//smp�N���A
		if((mode & 0x10) != 0x10){	//ss���[�h���g���Ƃ�
			SPI1CON1bits.SSEN = 1;	//SSpin��ON�ɂ���
		}
		//SPI�P�N��	
		SPI1STATbits.SPIROV = 0;	//��M�N���A
		__asm__("NOP");
		SPI1STATbits.SPIEN = 1;		//SPI1�L��
	}
}

void setup_spi2(uint8 mode){
	//�N���b�N�J�n�v���g�R���̐ݒ�
	if((mode & 0x02) == 0x02){//CLOCK_H_TO_L �ŃX�^�[�g
		SPI2CON1bits.CKP = 1;	
	}else{//CLOCK L to H �ŃX�^�[�g
		SPI2CON1bits.CKP = 0;
	}
	
	//MASTER or SLAVE
	if((mode & 0x01)%2 == 0){//MASTER mode
		SPI2CON1bits.MSTEN = 1;		//�}�X�^�[���[�h�ɐݒ�
		//�N���b�N�����P
		if((mode & 0x1C)==0x00){
			SPI2CON1bits.PPRE = 3;		//1st = 1:1
		}else if((mode & 0x1C)==0x04){
			SPI2CON1bits.PPRE = 2;		//1st = 4:1
		}else if((mode & 0x1C)==0x08){
			SPI2CON1bits.PPRE = 1;		//1st = 16:1
		}else if((mode & 0x1C)==0x10){
			SPI2CON1bits.PPRE = 0;		//1st = 64:1
		}
		//�N���b�N�����Q
		SPI2CON1bits.SPRE = ((~((mode & 0xE0)>>5)) & 0x07);//�����Q�̐ݒ�

		//SPI2�N��	
		SPI2STATbits.SPIROV = 0;	//��M�N���A
		__asm__("NOP");
		SPI2STATbits.SPIEN = 1;		//SPI2�L��

	}else{//SLAVE mode
		SPI2BUF = 0x00;				//�o�b�t�@���N���A
		SPI2CON1bits.MSTEN = 0;		//�X���[�u���[�h�ɐݒ�
		__asm__("NOP");
		SPI2CON1bits.SMP = 0;		//smp�N���A
		if((mode & 0x10) != 0x10){	//ss���[�h���g���Ƃ�
			SPI2CON1bits.SSEN = 1;	//SSpin��ON�ɂ���
		}
		//SPI2�N��	
		SPI2STATbits.SPIROV = 0;	//��M�N���A
		__asm__("NOP");
		SPI2STATbits.SPIEN = 1;		//SPI2�L��
	}
}

/*
void spi_write(uint16 data){   
	if(SPI1CON1bits.MODE16){       
        SPI1BUF = data;			//16bit���M���[�h
	}else{ 
        SPI1BUF = data & 0xff;  //8bit���M���[�h�@(default) 
	}
    while(SPI1STATbits.SPITBF);		//���M�\��ԂɂȂ�܂ő҂�. SPITBF 1:TXB is full 0:TXB is enpty
    data = SPI1BUF;             //�ǂݎ̂�
}
*/
/*
void spi2_write(uint16 data){   
	if(SPI2CON1bits.MODE16){       
        SPI2BUF = data;			//16bit���M���[�h
	}else{ 
        SPI2BUF = data & 0xff;  //8bit���M���[�h�@(default) 
	}
    while(SPI2STATbits.SPITBF);		//���M�\��ԂɂȂ�܂ő҂�. SPITBF 1:TXB is full 0:TXB is enpty
    data = SPI2BUF;             //�ǂݎ̂�
}
*/

uint32 spi_read(uint16 data){   
	SPI1STATbits.SPIROV = 0;			//��M�I�[�o�[�t���[�t���O���Z�b�g
	if(SPI1CON1bits.MODE16){       
        SPI1BUF = data;			//16bit���M���[�h
	}else{ 
        SPI1BUF = data & 0xff;  //8bit���M���[�h�@(default) 
	}
	while(!SPI1STATbits.SPIRBF);		//��M�҂�
	return SPI1BUF;						//���ʃ��^�[��
}

uint32 spi2_read(uint16 data){   
	SPI2STATbits.SPIROV = 0;			//��M�I�[�o�[�t���[�t���O���Z�b�g
//	spi2_write(data);					//��������
	if(SPI2CON1bits.MODE16){       
        SPI2BUF = data;			//16bit���M���[�h
	}else{ 
        SPI2BUF = data & 0xff;  //8bit���M���[�h�@(default) 
	}
	while(!SPI2STATbits.SPIRBF);		//��M�҂�
	return SPI2BUF;						//���ʃ��^�[��
}

void spi2_write(uint16 data){
	spi2_read(data);
}

void spi_write(uint16 data){
	spi_read(data);
}


unsigned char spi_data_is_in(){
	return SPI1STATbits.SPIRBF;//�f�[�^�����Ă��邩���m�F�A���Ă���1��Ԃ�
}


unsigned char spi2_data_is_in(){
	return SPI2STATbits.SPIRBF;//�f�[�^�����Ă��邩���m�F�A���Ă���1��Ԃ�
}


//Watch dog timer �֘A�֐�
void enable_wdt(){
	RCONbits.SWDTEN = 1;
}
void disable_wdt(){
	RCONbits.SWDTEN = 0;
}
void restart_wdt(){
	ClrWdt();
}

/* set_tris�n��錾 */
void set_tris_a(uint16 Bits){TRISA=Bits;}
void set_tris_b(uint16 Bits){TRISB=Bits;}
void set_tris_c(uint16 Bits){TRISC=Bits;}
void set_tris_d(uint16 Bits){TRISD=Bits;}
void set_tris_e(uint16 Bits){TRISE=Bits;}
void set_tris_f(uint16 Bits){TRISF=Bits;}
void set_tris_g(uint16 Bits){TRISG=Bits;}


//change clock Freq
//�@Div�̒l�������C���N���b�N�̎��g���𕪊����Ă����
// ������s������C�^�C�}�[�n�̏�������Y�ꂸ�ɁE�E�E�E
// ini_clock, ini_UART*, ini_timer* setup_adc_time_auto set_up_spi*
void changeClock(uint8 mode){
	CLKDIVbits.ROI = 0; // ���荞�݂ŃN���b�N���߂�̂�j�~
	__asm__("NOP");
	// DOZE �ւ̏������݃t�F�[�Y
	CLKDIVbits.DOZEN = 0;
	__asm__("NOP");
	CLKDIVbits.DOZE =  mode; // set
	__asm__("NOP");
	CLKDIVbits.DOZEN = 1; // make DOZE enable
}

#
#endif
