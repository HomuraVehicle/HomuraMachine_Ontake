#ifndef HMLIB_CDEQUE_C_INC
#define HMLIB_CDEQUE_C_INC 103
#
#ifndef HMLIB_CDEQUE_INC
#	include <hmLib_v3_04/cdeque.h>
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
//メモリの初期化
void cdeque_format(cdeque* ptr){
	ptr->ElemSize=0;
	ptr->BufBegin=0;
	ptr->BufEnd=0;
	ptr->Begin=0;
	ptr->End=0;
	ptr->DynamicFlag=0;
}
//すでに初期化されたかどうかの確認
hmLib_boolian cdeque_is_construct(cdeque* ptr){
	return ptr->BufBegin==0;
}
//静的確保によってバッファ確保
void cdeque_construct_static(cdeque* pDque,void* Begin,void* End,cdeque_size_t ElemSize){
	pDque->ElemSize=ElemSize;
	pDque->BufBegin=Begin;
	pDque->BufEnd=(hmLib_uint8*)(Begin)+ElemSize*(((hmLib_uint8*)(End)-(hmLib_uint8*)(Begin))/ElemSize);
	pDque->Begin=pDque->BufBegin;
	pDque->End=pDque->BufBegin;
	pDque->DynamicFlag=hmLib_false;
}
//動的確保によってバッファ確保
void cdeque_construct_dynamic(cdeque* pDque,cdeque_size_t Size,cdeque_size_t ElemSize){
	pDque->ElemSize=ElemSize;
	pDque->BufBegin=malloc(ElemSize*(Size+1));
	pDque->BufEnd=(hmLib_uint8*)(pDque->BufBegin)+ElemSize*(Size+1);
	pDque->Begin=pDque->BufBegin;
	pDque->End=pDque->BufBegin;
	pDque->DynamicFlag=hmLib_true;
}
//バッファ解放
void cdeque_destruct(cdeque* pDque){
	if(pDque->DynamicFlag){
		free(pDque->BufBegin);
	}
	pDque->BufBegin=0;
	pDque->BufEnd=0;
	pDque->Begin=0;
	pDque->End=0;
}
//cdequeを作成(dynamic_constructを使用)
cdeque* cdeque_create(cdeque_size_t Size,cdeque_size_t ElemSize){
	cdeque* ans;
	ans=(cdeque*)malloc(sizeof(cdeque));
	cdeque_construct_dynamic(ans,Size,ElemSize);
	return ans;
}
//cdequeを破棄(dynamic_destructを使用)
void cdeque_release(cdeque* pDque){
	cdeque_destruct(pDque);
	free(pDque);
}
//move
void cdeque_move(cdeque* from, cdeque* to){
	to->DynamicFlag=from->DynamicFlag;
	to->ElemSize=from->ElemSize;
	to->Begin=from->Begin;
	to->End=from->End;
	to->BufBegin=from->BufBegin;
	to->BufEnd=from->BufEnd;
	cdeque_format(from);
}
//copy
void cdeque_swap(cdeque* ptr1,cdeque* ptr2){
	cdeque tmp;
	tmp.DynamicFlag=ptr1->DynamicFlag;
	tmp.ElemSize=ptr1->ElemSize;
	tmp.Begin=ptr1->Begin;
	tmp.End=ptr1->End;
	tmp.BufBegin=ptr1->BufBegin;
	tmp.BufEnd=ptr1->BufEnd;
	ptr1->DynamicFlag=ptr2->DynamicFlag;
	ptr1->ElemSize=ptr2->ElemSize;
	ptr1->Begin=ptr2->Begin;
	ptr1->End=ptr2->End;
	ptr1->BufBegin=ptr2->BufBegin;
	ptr1->BufEnd=ptr2->BufEnd;
	ptr2->DynamicFlag=tmp.DynamicFlag;
	ptr2->ElemSize=tmp.ElemSize;
	ptr2->Begin=tmp.Begin;
	ptr2->End=tmp.End;
	ptr2->BufBegin=tmp.BufBegin;
	ptr2->BufEnd=tmp.BufEnd;
}
//バッファの先頭の要素アドレスを取得
void* cdeque_frontptr(cdeque* pDque){return pDque->Begin;}
//バッファの末尾の要素アドレスを取得
void* cdeque_backptr(cdeque* pDque){return cdeque_prev(pDque,pDque->End);}
//バッファにデータを挿入
hmLib_boolian cdeque_push_front(cdeque* pDque,const void* Ptr){
	cdeque_size_t cnt=0;

	if(cdeque_full(pDque))return 1;

	for(cnt=0;cnt<pDque->ElemSize;++cnt){
		*((hmLib_uint8*)(pDque->Begin)+cnt)=*((hmLib_uint8*)(Ptr)+cnt);
	}

	pDque->Begin=cdeque_prev(pDque,pDque->Begin);

	return 0;
}
//バッファの先頭のデータ削除
hmLib_boolian cdeque_pop_front(cdeque* pDque){
	if(cdeque_empty(pDque))return 1;

	pDque->Begin=cdeque_next(pDque,pDque->Begin);

	return 0;
}
//バッファの末尾にデータを挿入
hmLib_boolian cdeque_push_back(cdeque* pDque,const void* Ptr){
	cdeque_size_t cnt=0;

	if(cdeque_full(pDque))return 1;

	for(cnt=0;cnt<pDque->ElemSize;++cnt){
		*((hmLib_uint8*)(pDque->End)+cnt)=*((hmLib_uint8*)(Ptr)+cnt);
	}

	pDque->End=cdeque_next(pDque,pDque->End);

	return 0;
}
//バッファの末尾のデータ削除
hmLib_boolian cdeque_pop_back(cdeque* pDque){
	if(cdeque_empty(pDque))return 1;

	pDque->End=cdeque_prev(pDque,pDque->End);

	return 0;
}
//バッファに入っているサイズ
cdeque_size_t cdeque_size(cdeque* pDque){return (((hmLib_uint8*)(pDque->End)-(hmLib_uint8*)(pDque->Begin)+(hmLib_uint8*)(pDque->BufEnd)-(hmLib_uint8*)(pDque->BufBegin))%((hmLib_uint8*)(pDque->BufEnd)-(hmLib_uint8*)(pDque->BufBegin)))/pDque->ElemSize;}
//バッファに入っているサイズ
cdeque_size_t cdeque_rest(cdeque* pDque){return ((hmLib_uint8*)(pDque->BufEnd)-(hmLib_uint8*)(pDque->BufBegin))/pDque->ElemSize-cdeque_size(pDque)-1;}
//バッファが空かどうか確認
hmLib_boolian cdeque_empty(cdeque* pDque){return (pDque->Begin==pDque->End);}
//バッファがいっぱいかどうか確認
hmLib_boolian cdeque_full(cdeque* pDque){return (pDque->Begin==cdeque_next(pDque,pDque->End));}
//バッファのbegin関数
cdeque_iterator cdeque_begin(cdeque* pDque){return pDque->Begin;}
//バッファのend関数
cdeque_iterator cdeque_end(cdeque* pDque){return pDque->End;}
//cdeque_iteratorの次のcdeque_iteratorを返す
cdeque_iterator cdeque_next(cdeque* pDque,cdeque_iterator itr){
	itr=(hmLib_uint8*)(itr)+pDque->ElemSize;
	if(pDque->BufEnd == itr)itr=pDque->BufBegin;
	return itr;
}
//cdeque_iteratorの前のcdeque_iteratorを返す
cdeque_iterator cdeque_prev(cdeque* pDque,cdeque_iterator itr){
	if(pDque->BufBegin == itr)itr=pDque->BufEnd;
	itr=(hmLib_uint8*)(itr)-pDque->ElemSize;
	return itr;
}
#ifdef __cplusplus
}	//extern "C"
}	//namespace hmLib
#endif
#
#endif
