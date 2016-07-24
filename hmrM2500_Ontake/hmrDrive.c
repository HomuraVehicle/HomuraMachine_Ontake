#ifndef HMR_DRIVE_C_INC
#define HMR_DRIVE_C_INC 0
#
#define HMLIB_NOLIB
#include<stddef.h>
#ifndef HMR_DRIVE_INC
#	include"hmrDrive.h"
#endif
//*********************** Motor
//sint8 LBuffer=0;    //-100:100
//sint8 RBuffer=0;    //-100:100
unsigned char motor_wdt_count=0;
hmLib_boolian motor_listen(cstring Str){
	//データサイズ確認
	if(cstring_size(&Str)!=4)return hmLib_true;

        //????????????
        if(cstring_getc(&Str,1)!=0x00){
		motor_powerLA(1);
		motor_powerLB(1);
        }else if(cstring_getc(&Str,0)==0x64){
		motor_powerLA(1);
		motor_powerLB(0);
//		_LATB3=1;
//		_LATB2=0;
	}else if(cstring_getc(&Str,0)==0x9C){
		motor_powerLA(0);
		motor_powerLB(1);
//		_LATB3=0;
//		_LATB2=1;
	}else{
		motor_powerLA(0);
		motor_powerLB(0);
//		_LATB3=0;
//		_LATB2=0;
	}

	//????????????
        if(cstring_getc(&Str,3)!=0x00){
		motor_powerRA(1);
		motor_powerRB(1);
        }else if(cstring_getc(&Str,2)==0x64){
		motor_powerRA(1);
		motor_powerRB(0);
//		_LATB4=1;
//		_LATB5=0;
	}else if(cstring_getc(&Str,2)==0x9C){
		motor_powerRA(0);
		motor_powerRB(1);	
//		_LATB4=0;
//		_LATB5=1;
	}else{
		motor_powerRA(0);
		motor_powerRB(0);		
//		_LATB4=0;
//		_LATB5=0;
	}

        motor_wdt_count=0;
        
	return hmLib_false;
}
hmLib_boolian motor_talk(cstring* pStr){
	return hmLib_true;
}
void motor_setup_listen(void){return;}
void motor_setup_talk(void){return;}
sint16 motor_task_wdt(sint16 interval){
        ++motor_wdt_count;

        if(motor_wdt_count>=2){
                motor_wdt_count=0;
		motor_powerLA(0);
		motor_powerLB(0);
		motor_powerRA(0);
		motor_powerRB(0);		
        }
        
	return interval;
}

cstring motor_message2(cstring Str){
	cstring Ans={0,0};
	if(cstring_getc(&Str,0)==0x64){
//		motor_powerLA(1);
		_LATB3=1;
		_LATB2=0;
//		motor_powerLB(0);
	}else if(cstring_getc(&Str,0)==0x9C){
		_LATB3=0;
		_LATB2=1;
	}else{
		_LATB3=0;
		_LATB2=0;
	}
	if(cstring_getc(&Str,2)==0x64){
		_LATB4=1;
		_LATB5=0;
	}else if(cstring_getc(&Str,2)==0x9C){
		_LATB4=0;
		_LATB5=1;
	}else{
		_LATB4=0;
		_LATB5=0;
	}
/*	if((signed char)(cstring_getc(&Str,0))>0){
		motor_powerLA(1);
		motor_powerLB(0);
	}else if((signed char)(cstring_getc(&Str,0))<0){
		motor_powerLA(0);
		motor_powerLB(1);
	}else{
		motor_powerLA(0);
		motor_powerLB(0);
	}
	if((signed char)(cstring_getc(&Str,2))>0){
		motor_powerRA(1);
		motor_powerRB(0);
	}else if((signed char)(cstring_getc(&Str,2))<0){
		motor_powerRA(0);
		motor_powerRB(1);
	}else{
		motor_powerRA(0);
		motor_powerRB(0);
	}
*/
	return Ans;
}
/*
*********************** Manipulator
cstring manipu_message(cstring Str){
	cstring Ans={0,0};
	uint16 angle[3];
	uint8 i;

	for(i=0;i<3;++i){
		angle[i] = cstring_getc(&Str,2*i)+ cstring_getc(&Str,2*i+1) * 256;
	}
	MmSPI_manipu(angle[0],angle[1],angle[2]);

	return Ans;
}

*********************** MmSPI
uint8 MmSPI_read(uint8 u){
	delay_ms(1);
	return spi2_read(u);
}
void MmSPI_strt(uint8 cmd){
	delay_ms(1);
	spi_selectDrive(1);
	delay_ms(1);
	spi2_read('<');
	delay_ms(1);
	spi2_read(cmd);
	delay_ms(1);
}
void MmSPI_trmn(uint8 cmd){
	delay_ms(1);
	spi2_read('>');
	delay_ms(1);
	spi_selectDrive(0);
}
void MmSPI_motor(uint8 left,uint8 l_breake, uint8 right, uint8 r_breake){
	MmSPI_strt('m');

	//dBuf[2] = 
	MmSPI_read(left);
	MmSPI_read(l_breake);
	MmSPI_read(right);
	MmSPI_read(r_breake);

	MmSPI_trmn('m');
}
void MmSPI_manipu(uint16 arm1, uint16 arm2, uint16 arm3){
	MmSPI_strt('M');

	MmSPI_read( (uint8)(arm1 % 256) );
	MmSPI_read( (uint8)(arm1 / 256) );
	MmSPI_read( (uint8)(arm2 % 256) );
	MmSPI_read( (uint8)(arm2 / 256) );
	MmSPI_read( (uint8)(arm3 % 256) );
	MmSPI_read( (uint8)(arm3 / 256) );

	MmSPI_trmn('M');
}
void MmSPI_setLog(uint8 io_){
	MmSPI_strt('l');

	if(io_)MmSPI_read(1);
	else MmSPI_read(0);

	MmSPI_trmn('l');
}
uint8 MmSPI_getLog(uint8* ptr_){
	uint8 Num;
	uint8 Cnt=0;

	MmSPI_strt('L');

	Num=MmSPI_read(NULL);

	for(Cnt=0;Cnt<Num;++Cnt){
		ptr_[Cnt]=MmSPI_read(NULL);
	}

	MmSPI_trmn('L');

	return Num;
}
*/
#
#endif
