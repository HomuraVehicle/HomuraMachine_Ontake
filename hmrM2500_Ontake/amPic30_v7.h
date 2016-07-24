#ifndef AMPIC30_INC
#define AMPIC30_INC 701
#

/*
amPic30_v7_02/130112 hmIto
	hmLib_v3_03�ɑΉ�
amPic30_v7_01/121028 hmIto
	ini_timer4�ŁAtimer�����b�N�����������Ă����̂��C��
	ini_timer4�̉��Z����������������
	changeClock�ɒ��ӏ�����t��
		���̊֐���p���ăN���b�N����ύX���Ă��A�e�����󂯂�̂�CPU�̂�
		timer�Ȃǂ͉e�����󂯂Ȃ��\��
amPic30_v7_00/121027 iwahori
	clock���g���̕ύX�֐�changeClock()�ǉ�
	set_adc_sampleMode()�ǉ�

*/
//
//���������@dsPIC C30�R���p�C���p���C�u�����@����������
//�w�b�_�[�t�@�C���Ł@Ver. 3-4
//
//*******�@�f�[�^�^(bit)�@************
//  dsPic�^		bit		hmr�^
//				8		byte
//	int			16		short
//  long		32		int
//	long long	64		long
//  double		32		float
//  long double 64		double
//
//*******�@delay�֐��ɂ��ā@********�@
//�@delay�֐��̂��߂�timer2,3���g�����̂ŁAtimer2,3�͎g�p�ł��Ȃ��d�l�ɂȂ��Ă���I�I
//  
//  JPEG���荞�ݓ��ōăR�l�N�g�̍ۂ�delay���g���K�v���������̂ŁA��p��delay�֐�����邱�Ƃɂ���
//�@timer 6,7 ���g�p����@
//
//*******�@output_high()�֐��ɂ��ā@*********
//�@����͂��̂悤�ȏo�͊֐��Q�̎����͂�����߂�
//  ���̑���A#define p_LED PORTAbits.RA0 �̂悤�ɒ�`���邱�ƂŁA
//�@p_LED = 1 �̂悤�ɂ��̂܂ܕϐ��Ƃ��ďo�͂��������鎖�ɂȂ�B
//
//*******  AD converter *******************
//�@AD�ϊ��̏ꍇ�A�|�[�g�̓��o�͐ݒ�͎����ł͍s��Ȃ��̂Œ��ӂ���K�v������
//�@��₱�������ł���΁A�֐��ɍ����������
//  ���݂�ADC1�ɂ��Ă̂ݎ���(����ŏ\���Ǝv����)
//
//*******  SPI�֐�  ************************
//	�����ݒ肵�Ă���Aread,�@write�@���邾���AMASTER���[�h�ɂ��Ă͓���m�F�ς�
//	SLAVE���[�h�ɂ��Ă͓���m�F�͂��Ă��Ȃ��̂Œ��ӂ��Ăق����B�@
//
//*******  WDT�֌W�@*************************
//	WDT�̓\�t�g�E�F�A��ONOFF�ł���B�������A����̓R���t�B�M���ˑ��Ȃ̂Œ���
//�@(��̓I�ɂ�WDT���R���t�B�M����OFF�ɂ���K�v������)
//
//*******  Timer�֌W�@***********************
// Timer�P�́A16�r�b�g�^�C�}�@�N���b�N�T�OM��500ms�҂��͕s�\�ɂȂ�̂Œ���
//
// Timer�S�́A32�r�b�g�^�C�}�@�N���b�N�͂�����ł��A10s�҂��ł��܂��I�I�@�@
//   �����A���荞�݂̐���� timer5 �̃��W�X�^���g���B���荞�݊֐��̖��O�ɒ��ӁI�I
//
//*******  CONFIG�@�́@�Q�l��@**************
/*
//config(���C���ł���ăN��)�@�Q�l��(���̊֐��Q�̓���m�F�ςݐݒ��) 
//
//�@WDT�I�t�ɂ��Ă���̂́A�����Ń\�t�g�E�F�A�I��ON,OFF�ł��邽��
//  WDT�͓����̕ʂ̃N���b�N�Ō��܂�̂ŁA���C���N���b�N�̑����ɂ͈ˑ����Ȃ�
//  WDT��WINDOW���[�h�́AWDT������3/4�ȍ~�ɁAWDT�N���A���Ȃ��ƁA���Z�b�g��������Ƃ������҂Ȃ̂�OFF
//	�����N���b�N��PLL�ɂ���đ����ł���B 
//�@
#include <p33FJ256GP710.h>
_FOSCSEL( FNOSC_FRC);		//Internal FRC(7.37MHz)
_FOSC( FCKSM_CSDCMD & OSCIOFNC_ON & POSCMD_NONE ); //HS
_FWDT( FWDTEN_OFF & WINDIS_OFF & WDTPRE_PR128 & WDTPOST_PS512 );//WDT 2s
_FPOR( FPWRT_PWR64 );//power on timer 64ms 
*/

//::::::::::::::::::�@�X�V�����@:::::::::::::::::::::::::
//
//  100724	UART�ɂ��ă��C�u�������i�߂�
//	100807	UART�ɂ��ă��C�u���������@
//			�^�C�}�[�A���Ԋ֌W�̃��C�u����������
//	100808�@�`�c�ϊ��ɂ��Ă̊֐��Q���쐬
//			Vref�����܂������Ă��Ȃ�(��H�̖�肩�H)
//			SPI�͓���`�F�b�N�ς�
//

/* amPic30���g���Ă��邱�Ƃ������錾 */
#define HMLIB_NOLIB
#include"hmrType.h"

//��{�f�[�^�T�C�Y�^
#ifdef P33FJ256GP710_INC
#	define P33FJ256GP710_INC
#	include <p33FJ256GP710.h>
#endif
#include <timer.h>

#define AN0 0x01
#define AN1 0x02
#define AN2 0x04
#define AN3 0x08
#define AN4 0x10
#define AN5 0x20
#define AN6 0x40
#define AN7 0x80
#define AN8 0x0100
#define AN9 0x0200
#define AN10 0x0400
#define AN11 0x0800
#define AN12 0x1000
#define AN13 0x2000
#define AN14 0x4000
#define AN15 0x8000

#define AN16 0x010000
#define AN17 0x020000
#define AN18 0x040000
#define AN19 0x080000
#define AN20 0x100000
#define AN21 0x200000
#define AN22 0x400000
#define AN23 0x800000
#define AN24 0x01000000
#define AN25 0x02000000
#define AN26 0x04000000
#define AN27 0x08000000
#define AN28 0x10000000
#define AN29 0x20000000
#define AN30 0x40000000
#define AN31 0x80000000

//******* �K���K�v�ȏ������֐� ******
//��ԏ��߂ɕK�����̃N���b�N�w�肪�K�v
//�N���b�N���̂́A�R���t�B�M�����[�V�����r�b�g�Ŏw��
//�����܂ł��A�N���b�N��ݒ肷��̂ł͂Ȃ��A�����֐��ɃN���b�N�������Ă��邾���ł��邱�Ƃɒ��ӁI�I

void ini_clock(uint64 _clock);//�N���b�N���w�肵�Ă��

//******* �V���A���ʐM�n�֐� *******
//���������āAfputc,fgetc�Ŏg��
//���邢�́A���������Ċ��荞�݋����Ċ��荞�݂Ŏg��

void ini_UART1(uint32 baudrate);//�������{UART1�g�p����
void ini_UART2(uint32 baudrate);//�������{�Q�g�p����
//�V���A���ʐM�ɂ����銄�荞�݂̎g����
/*
�P�A���荞�݂������Ă��(�ȉ��̊֐��R)
�@�@�@���Ȃ݂Ɂ@���M���荞�݂́A���M�o�b�t�@�ɋ󂫂�����Ί��荞��ł���B
�@�@�@�@�@�@�@�@��M���荞�݂́A������M�����犄�荞��ł���B
�Q�A���C���֐��̎n�܂�O�̂Ƃ���ŁA�ȉ��̂悤�Ɋ��荞�ݏ����������Ă��Ηǂ�
�@�@���荞�݊֐����ł́A�t���O���������Ă��΁A�������Ă��悢�I�I
�@�@steam �̏����ԈႢ�ɒ��Ӂi�t���O�����āA�������邾���j

void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void){//UART1�ɂ�鑗�M���荞��
	//�K�{����
	clear_interrupt_fputflag(1);		//���荞�݃t���O�N���A(�K�{)
	//�Ǝ�����
	fputc(data,1);						//�f�[�^���M
}
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void){//UART2�ɂ���M���荞��
	clear_interrupt_fgetfalg(2); 		//���荞�݃t���O�N���A
	data = fgetc(2);					//�f�[�^��M
}
*/

/* ���荞�݊֐��̃R���p�C����`�^�u���}�N�� */
//stream1�̎�M������
#define interrupt_fget_rs1(void) __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
//stream1�̑��M������
#define interrupt_fput_rs1(void) __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void)
//stream2�̎�M������
#define interrupt_fget_rs2(void) __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void)
//stream2�̑��M������
#define interrupt_fput_rs2(void) __attribute__((interrupt, no_auto_psv)) _U2TXInterrupt(void)

//���荞�݃t���O������
void clear_interrupt_fgetflag(uint8 stream);
void clear_interrupt_fputflag(uint8 stream);
void set_interrupt_fgetflag(uint8 stream);
void set_interrupt_fputflag(uint8 stream);
//���荞�݂�disable/enable
void enable_interrupt_fget(uint8 stream);
void enable_interrupt_fput(uint8 stream);
void disable_interrupt_fget(uint8 stream);
void disable_interrupt_fput(uint8 stream);

//�ꎞ�I��status��ޔ����Ċ��荞�݂�disable�@�댯�g����
// ����́Astatus��ޔ��������ƂɁA���荞�܂��status�̏�Ԃ��ς��\��������@
// �댯������g���ȁI�I�I�I�I�I�I�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@
uint8 tmp_disable_interrupt_fget(uint8 stream);
uint8 tmp_disable_interrupt_fput(uint8 stream);
uint8 tmp_disable_interrupt_fgetput(uint8 stream);
//tmp�֐��őޔ�����status�����ɖ߂��@�댯�@�g����
void replace_interrupt_fget(uint8 c,uint8 stream);
void replace_interrupt_fput(uint8 c,uint8 stream);
void replace_interrupt_fgetput(uint8 c,uint8 stream);
//����M�֐�
void rs_fputc(uint8 data, uint8 stream);
uint8 rs_fgetc(uint8 stream);
//��M�`�F�b�N�֐�
uint8 rs_kbhit(uint8 stream);

//********* �֗��֐� **********
//���ԑ҂�(����)
void delay_ms(uint16 time);
void delay_us(uint16 time);//�N���b�N���ᑬ�Ȃ̂�10us���݂ł����҂ĂȂ����Ƃɒ���
void delay_ms2(uint16 time);//�\��

//********* timer1�p�֐� ***************
//���������āA���荞�݋�����ƃ^�C�}�[�X�^�[�g

//�������֐��@��ms�Ɉ�񊄂荞�ނ�(1�`500�Ŏw��\)
void ini_timer1(uint16 time);
/*
//���荞�݂̎g����
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void){//T1IF�ɂ�鑗�M���荞��
	//���܂�������
	IFS0bits.T1IF = 0;			//T1���荞�݃t���O���N���A
	TMR1 = 0;					//T1�J�E���g���N���A
	//�ȉ��������e
	PORTAbits.RA0 += 1;			//LED�̏�Ԃ�ω�(����)
}
*/
void reset_timer1();//�^�C�}�[�J�E���^���Z�b�g
void enable_interrupt_timer1();//�^�C�}�[���荞�݋���
void disable_interrupt_timer1();//�^�C�}�[���荞�݋֎~
uint8 can_interrupt_timer1();
uint8 tmp_disable_interrupt_timer1();
void replace_interrupt_timer1(uint8 c);

// ********** timer4�p�֐��@**************** //
//���������āA���荞�݋�����ƃ^�C�}�[�X�^�[�g//
//�������֐��@��ms�Ɉ�񊄂荞�ނ��i1�`10000ms�j�Ŏw��\//
void ini_timer4(uint16 time);

/*
//���荞�݂̎g����
void __attribute__((interrupt, no_auto_psv)) _T5Interrupt(void){//T5(4�ł͂Ȃ�����)IF�ɂ�鑗�M���荞��
	IFS1bits.T5IF = 0;			//T5���荞�݃t���O���N���A
	TMR5 = 0;
	TMR4 = 0;                   //T4�J�E���g���N���A
	PORTAbits.RA0 += 1;			//LED�̏�Ԃ�ω�(����)
}
*/
void enable_interrupt_timer4();
void disable_interrupt_timer4();
void reset_timer4();

//********** AD�ϊ��֘A ***************
//����			�F�|�[�g�ݒ肵�āA�Q�Ɠd�����߂āA12bit�ɂ��邩�ۂ����߂āA�N���b�N�ݒ������
//�f�[�^�Ƃ�Ƃ��F�������w�肵�āA�ǂݍ��ފ֐����g��

//Port�ݒ�
void setup_adc_ports(uint32 port);//AD�ϊ��Ɏg���|�[�g���߁i�� port = AN1|AN3|AN23)

//AD�ϊ��Ɏg���Q�Ɠd�����߁@Vref�̃|�[�g����͂ɂ���̂�Y���ȁI�I
//Vref�ݒ�@�{�@ �|
#define		Vin_GND 0x00
#define		Vref_GND 0x01
#define		Vin_Vref 0x02
#define		Vref_Vref 0x03
void setup_adc1_Vref(uint8 mode);// ex) setup_adc1_Vref(Vin_GND) (ADC1�ɂ��Ă̂�) 

//12bitAD�ϊ����邩�ۂ� 1�Ȃ�x�����A����ȊO�m���@
void setup_adc1_12bit(uint8 Y_N);

//AD�ϊ��̂��߂̃N���b�N�ݒ�֐�(����)�{AD�ϊ�ON
void setup_adc1_time_auto();

//AD�ϊ�����CH(0~31)�̎w��(���̌�̎��ԑ҂��͂���Ȃ�)
void set_adc_channel(uint8 ch);

//AD�ϊ������f�[�^��ǂݍ���(20us������܂�)
uint32 read_adc();
// AD�ϊ��̃��[�h�̐ݒ�C1�`�����l�����ƂɂP�X�L�������邩�C�܂Ƃ߂�16�X�L�������邩�I���ł���
// �܂Ƃ߂ăX�L�����́CAD�ϊ��֐��ɂ��ύX���K�v�Ȃ̂Ŗ�����
#define EVERY 0x00
void set_adc_sampleMode(uint32 mode);
//*********** SPI�֘A�֐� ***********************
//SPI�̏����ݒ���s������A��������ŁA�ǂݎ���Ċ֐����g��
// �I�����Ă��Ȃ�PIN�� high�ŕێ����Ă����K�v������ 
//

//�����ݒ�
//MASTER or SLAVE
#define SPI_MASTER 0x00
#define SPI_SLAVE  0x01

//�N���b�N�ǂރ^�C�~���O
#define SPI_L_TO_H 0x00
#define SPI_H_TO_L 0x02

//�N���b�N�����w��(F����/?�@= Fosc/(2*?)) MASTER�̂�
//�����P
#define SPI_CLK_DIV_1 0x00
#define SPI_CLK_DIV_4 0x04
#define SPI_CLK_DIV_16 0x08
#define SPI_CLK_DIV_64 0x10

//����2
#define SPI_CLK_DIV2_1 0x00
#define SPI_CLK_DIV2_2 0x20
#define SPI_CLK_DIV2_3 0x40
#define SPI_CLK_DIV2_4 0x60
#define SPI_CLK_DIV2_5 0x80
#define SPI_CLK_DIV2_6 0xA0
#define SPI_CLK_DIV2_7 0xC0
#define SPI_CLK_DIV2_8 0xE0

//SSpin�@�������@SLAVE�̂�
#define SPI_SS_DISABLED 0x10

void setup_spi(uint8 mode);//SPI1 �������֐�
void setup_spi2(uint8 mode);//SPI2 �������֐�
//ex) setup_spi1(SPI_MASTER | SPI_L_TO_H | SPI_CLK_DIV_16 )

//SPI�ɏ������݁A�ǂݎ̂�
void spi_write(uint16 data);
void spi2_write(uint16 data);

//SPI����ǂ�ŏ�������
uint32 spi_read(uint16 data);
uint32 spi2_read(uint16 data);

//SPI�Ƀf�[�^�����Ă��邩�ǂ����H���Ă��� 1
uint8 spi_data_is_in();
uint8 spi2_data_is_in();

//*********** WDT�֘A�֐� ***********************
//enbale disable ���\�t�g�E�F�A�Ō��߂邱�Ƃ��ł���(����disable)
////restart_wdt()�@�ŃE�H�b�`�h�b�O�N���A

//WDT����
void enable_wdt();
//WDT����
void disable_wdt();
//WDT ����I�N���A
void restart_wdt();

/* output_XXX���g����悤�Ƀ}�N���錾 */
#define output_low(x) (x=0)
#define output_high(x) (x=1)
#define output_bit(x,a) (x=a)

/* set_tris�n��錾 */
void set_tris_a(uint16 Bits);
void set_tris_b(uint16 Bits);
void set_tris_c(uint16 Bits);
void set_tris_d(uint16 Bits);
void set_tris_e(uint16 Bits);
void set_tris_f(uint16 Bits);
void set_tris_g(uint16 Bits);
//#endif

//change clock Freq
//�@Div�̒l�������C���N���b�N�̎��g���𕪊����Ă����
// ������s������C�^�C�}�[�n�̏�������Y�ꂸ�ɁE�E�E�E
// ini_clock, ini_UART*, ini_timer* setup_adc_time_auto set_up_spi*
// ��121028 hmIto ���̋@�\��CPU�̂݊Ԉ�������̃N���b�N���𗘗p����@�\�Ȃ̂ŁA
//   �ď������͕s�v�̉\��  ���Ȃ��Ƃ��Atimer�̓I���W�i���̑��x�𗘗p���Ă���
#define MAIN_CLK_Div_128 0x07
#define MAIN_CLK_Div_64 0x06
#define MAIN_CLK_Div_32 0x05
#define MAIN_CLK_Div_16 0x04
#define MAIN_CLK_Div_8 0x03
#define MAIN_CLK_Div_4 0x02
#define MAIN_CLK_Div_2 0x01
#define MAIN_CLK_Div_1 0x00

void changeClock(uint8 mode);

#
#endif
