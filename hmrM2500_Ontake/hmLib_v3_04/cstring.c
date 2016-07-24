#ifndef HMLIB_CSTRING_C_INC
#define HMLIB_CSTRING_C_INC 103
#ifndef HMLIB_CSTRING_INC
#	include <hmLib_v3_04/cstring.h>
#endif
#ifdef __cplusplus
extern "C"{
#endif
#include <stdlib.h>
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
namespace hmLib{
extern "C"{
#endif
	
unsigned char _cstring_debug_CreateCnt=0;
unsigned char _cstring_debug_DynamicCnt=0;
unsigned char _cstring_debug_StaticCnt=0;
unsigned char _cstring_debug_ErrorPre=0;
unsigned char _cstring_debug_ErrorDynamic=0;
unsigned char _cstring_debug_ErrorStatic=0;
unsigned char _cstring_debug_ErrorDestruct=0;
void cstring_format(cstring* str){
	str->DynamicFlag=0;
	str->Begin=0;
	str->End=0;
}
hmLib_boolian cstring_is_construct(cstring* str){
	return str->Begin!=0;
}
cstring* cstring_create(cstring_size_t size){
	cstring* str;
	str=(cstring*)malloc(sizeof(cstring));
	cstring_construct_dynamic(str,size);
/*DEBUG*/++_cstring_debug_CreateCnt;
	return str;
}
void cstring_release(cstring* str){
	cstring_destruct(str);
/*DEBUG*/--_cstring_debug_CreateCnt;
	free(str);
}
void cstring_construct_dynamic(cstring* str,cstring_size_t size){
/*DEBUG*/if(cstring_is_construct(str)){++_cstring_debug_ErrorPre;return;}
	if(size==0){
		cstring_format(str);
		return;
	}

	str->Begin=(unsigned char*)malloc(size);
/*DEBUG*/if(str->Begin==0){++_cstring_debug_ErrorDynamic;return;}


	str->End=str->Begin+size;
	str->DynamicFlag=hmLib_true;
/*DEBUG*/++_cstring_debug_DynamicCnt;
}
void cstring_construct_static(cstring* str,unsigned char* Begin,unsigned char* End){
/*DEBUG*/if(cstring_is_construct(str)){++_cstring_debug_ErrorPre;return;}
	if(Begin==0){
		cstring_format(str);
		return;
	}

	str->Begin=Begin;
	str->End=End;
	str->DynamicFlag=hmLib_false;
/*DEBUG*/++_cstring_debug_StaticCnt;
/*DEBUG*/if(!cstring_is_construct(str))++_cstring_debug_ErrorStatic;
}
void cstring_destruct(cstring* str){
/*DEBUG*/if(!cstring_is_construct(str)){++_cstring_debug_ErrorDestruct;return;}
	if(str->DynamicFlag){
		free((void*)str->Begin);
/*DEBUG*/--_cstring_debug_DynamicCnt;
	}else{
/*DEBUG*/--_cstring_debug_StaticCnt;
	}
	str->Begin=0;
	str->End=0;
}
void cstring_move(cstring* from, cstring* to){
	to->DynamicFlag=from->DynamicFlag;
	to->Begin=from->Begin;
	to->End=from->End;
	cstring_format(from);
}
void cstring_swap(cstring* str1, cstring* str2){
	cstring tmp;
	tmp.DynamicFlag=str1->DynamicFlag;
	tmp.Begin=str1->Begin;
	tmp.End=str1->End;
	str1->DynamicFlag=str2->DynamicFlag;
	str1->Begin=str2->Begin;
	str1->End=str2->End;
	str2->DynamicFlag=tmp.DynamicFlag;
	str2->Begin=tmp.Begin;
	str2->End=tmp.End;
}
unsigned char* cstring_ptr(const cstring* str){return str->Begin;}
cstring_size_t cstring_size(const cstring* str){return str->End - str->Begin;}
cstring_iterator cstring_begin(const cstring* str){return str->Begin;}
cstring_iterator cstring_end(const cstring* str){return str->End;}
cstring_iterator cstring_next(const cstring* str,cstring_iterator itr){return ++itr;}
cstring_iterator cstring_prev(const cstring* str,cstring_iterator itr){return --itr;}
void cstring_putc(cstring* str,cstring_size_t pos,unsigned char c){
	if(str->Begin+pos<str->End)str->Begin[pos]=c;
}
void cstring_puts(cstring* str,cstring_size_t pos,const unsigned char* p,cstring_size_t size){
	cstring_size_t cnt;
	for(cnt=0;cnt<size;++cnt){
		if(pos+cnt+str->Begin>=str->End)break;
		str->Begin[pos+cnt]=p[cnt];
	}
}
unsigned char cstring_getc(cstring* str,cstring_size_t pos){
	return str->Begin+pos<str->End ? str->Begin[pos] : 0;
}
unsigned char* cstring_gets(cstring* str, cstring_size_t pos){
	return str->Begin+pos;
}
#ifdef __cplusplus
}	//extern "C"
}	//namespace hmLib
#endif
#
#endif
