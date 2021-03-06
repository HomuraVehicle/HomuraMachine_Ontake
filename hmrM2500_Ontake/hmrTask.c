#ifndef HMR_TASK_C_INC
#define HMR_TASK_C_INC 106
#
#ifndef HMR_TASK_INC
#	include"hmrTask.h"
#endif
#include <stdlib.h>
//===タスクバッファ用関数===
void _task_buf_initialize(taskmaster* TaskMaster);
//タスクバッファのサイズを取得する
uint8 _task_buf_size(taskmaster* TaskMaster);
//タスクバッファが空か確認する
boolian _task_buf_empty(taskmaster* TaskMaster);
//タスクバッファが一杯か確認する
boolian _task_buf_full(taskmaster* TaskMaster);
//タスクバッファの先頭アドレスを取得する
task* _task_buf_begin(taskmaster* TaskMaster);
//タスクバッファの末尾アドレスを取得する
task* _task_buf_end(taskmaster* TaskMaster);
//タスクバッファから、タスクを探してくる
task* _task_buf_find(taskmaster* TaskMaster,task_function Fp);
//タスクバッファに要素を追加する
boolian _task_buf_insert(taskmaster* TaskMaster,task_function Fp,sint16 Interval,task_inform TaskInformStopFp);
//タスクバッファの要素を削除する
boolian _task_buf_erase(taskmaster* TaskMaster,task* itr);


//===タスクバッファ用関数===
void _task_buf_initialize(taskmaster* TaskMaster){
	if(TaskMaster==0)return;
	task* itr=_task_buf_begin(TaskMaster);
	while(itr!=_task_buf_end(TaskMaster)){
		itr->Fp=0;
		itr->InformStop=0;
		itr->Interval=0;
		++itr;
	}
}
//タスクバッファのサイズを取得する
uint8 _task_buf_size(taskmaster* TaskMaster){
	if(TaskMaster==0)return 0;
	return TaskMaster->Size;
}
//タスクバッファが空か確認する
boolian _task_buf_empty(taskmaster* TaskMaster){
	if(TaskMaster==0)return 0;
	return TaskMaster->Size==0;
}
//タスクバッファが一杯か確認する
boolian _task_buf_full(taskmaster* TaskMaster){
	if(TaskMaster==0)return 1;
	return TaskMaster->Size==TaskMaster->MaxSize;
}
//タスクバッファの先頭アドレスを取得する
task* _task_buf_begin(taskmaster* TaskMaster){
	if(TaskMaster==0)return 0;
	return TaskMaster->Buf;
}
//タスクバッファの末尾アドレスを取得する
task* _task_buf_end(taskmaster* TaskMaster){
	if(TaskMaster==0)return 0;
	return TaskMaster->Buf+TaskMaster->Size;
}
//タスクバッファから、タスクを探してくる
task* _task_buf_find(taskmaster* TaskMaster,task_function Fp){
	if(TaskMaster==0)return 0;
	if(Fp==0)return 0;
	task* itr;

	//終端まで確認して、同一タスクの有無を確認する
	itr=_task_buf_begin(TaskMaster);
	while(itr!=_task_buf_end(TaskMaster)){
		if(itr->Fp==Fp)break;
		++itr;
	}

	if(itr==_task_buf_end(TaskMaster))return 0;
	return itr;
}
//タスクバッファに要素を追加する
boolian _task_buf_insert(taskmaster* TaskMaster,task_function Fp,sint16 Interval,task_inform TaskInformStopFp){
	if(TaskMaster==0)return 1;
	if(Fp==0)return 1;
	//taskバッファがいっぱいならエラーを返す
	if(_task_buf_full(TaskMaster))return 1;

	//登録内容を末尾に書き込み、サイズを増やす
	TaskMaster->Buf[TaskMaster->Size].Fp=Fp;
	TaskMaster->Buf[TaskMaster->Size].Interval=Interval;
	TaskMaster->Buf[TaskMaster->Size].InformStop=TaskInformStopFp;
	++TaskMaster->Size;

	//taskが初めて登録された場合は、inform関数を呼び出す
	if(TaskMaster->Size==1 && TaskMaster->Fp_informStart)TaskMaster->Fp_informStart();

	return 0;
}
//タスクバッファの要素を削除する
boolian _task_buf_erase(taskmaster* TaskMaster,task* itr){
	task* pre=0;
	if(TaskMaster==0 || itr==0)return 1;
	if(itr<_task_buf_begin(TaskMaster) || _task_buf_end(TaskMaster)<=itr)return 1;

	//指定itr以後の
	while(1){
		pre=itr++;
		if(itr==_task_buf_end(TaskMaster))break;
		pre->Fp=itr->Fp;
		pre->Interval=itr->Interval;
		pre->InformStop=itr->InformStop;
		itr->Fp=0;
		itr->InformStop=0;
		itr->Interval=0;
	}

	--TaskMaster->Size;
	if(TaskMaster->Size==0 && TaskMaster->Fp_informStop)TaskMaster->Fp_informStop();

	return 0;
}
//===タスク用公開関数===
//タスクシステムフォーマット
void task_format(taskmaster* TaskMaster){
	if(TaskMaster==0)return;
	TaskMaster->Buf=0;
	TaskMaster->Counter=0;
	TaskMaster->DynamicFlag=hmLib_false;
	TaskMaster->Fp_informStart=0;
	TaskMaster->Fp_informStop=0;
	TaskMaster->MaxSize=0;
	TaskMaster->Size=0;
}
//タスクシステムが初期化されているか
boolian task_is_constructed(taskmaster* TaskMaster){
	if(TaskMaster==0)return 0;
	return TaskMaster->Buf!=0;
}
//タスクシステム初期化関数
void task_construct_dynamic(taskmaster* TaskMaster,uint8 MaxSize){
	if(TaskMaster==0)return;
	TaskMaster->Buf=(task*)malloc(sizeof(task)*MaxSize);
	TaskMaster->Counter=0;
	TaskMaster->DynamicFlag=hmLib_true;
	TaskMaster->Fp_informStart=0;
	TaskMaster->Fp_informStop=0;
	TaskMaster->MaxSize=MaxSize;
	TaskMaster->Size=0;
	_task_buf_initialize(TaskMaster);
}
//タスクシステム初期化関数
void task_construct_static(taskmaster* TaskMaster,task* Begin,task* End){
	if(TaskMaster==0 || Begin==0 || End==0)return;
	TaskMaster->Buf=Begin;
	TaskMaster->Counter=0;
	TaskMaster->DynamicFlag=hmLib_false;
	TaskMaster->Fp_informStart=0;
	TaskMaster->Fp_informStop=0;
	TaskMaster->MaxSize=(uint8)(End-Begin);
	TaskMaster->Size=0;
	_task_buf_initialize(TaskMaster);
}
//タスクシステム終端化関数	
void task_destruct(taskmaster* TaskMaster){
	task* itr=0;
	if(TaskMaster==0)return;
	itr=_task_buf_begin(TaskMaster);
	while(itr!=_task_buf_end(TaskMaster)){
		if(itr->InformStop)itr->InformStop();
		++itr;
	}
	if(TaskMaster->Fp_informStop)TaskMaster->Fp_informStop();
	if(TaskMaster->DynamicFlag)free(TaskMaster->Buf);
	TaskMaster->Buf=0;
	TaskMaster->Counter=0;
	TaskMaster->DynamicFlag=hmLib_false;
	TaskMaster->Fp_informStart=0;
	TaskMaster->Fp_informStop=0;
	TaskMaster->MaxSize=0;;
	TaskMaster->Size=0;
}
//タスクに登録できる確認する
boolian task_can_start(taskmaster* TaskMaster){
	if(TaskMaster==0)return 0;
	return !_task_buf_full(TaskMaster);
}
//タスクを登録(二重登録は回避)
boolian task_start(taskmaster* TaskMaster,sint16 Interval,task_function TaskFp,task_inform TaskInformStopFp){
	task* itr=0;
	if(TaskMaster==0 || TaskFp==0)return 1;

	//タスクを探しに行く
	itr=_task_buf_find(TaskMaster,TaskFp);

	//新規の場合
	if(itr==_task_buf_end(TaskMaster)){
		//指定状態が負の値なら、エラーを戻す
		if(Interval<0)return 1;

		//登録できない場合は、エラーを戻す
		if(_task_buf_full(TaskMaster))return 1;

		//タスクを登録
		return _task_buf_insert(TaskMaster,TaskFp,Interval,TaskInformStopFp);
	}//既存の場合
	else{
		//停止要請なら、削除する
		if(Interval<0)return _task_buf_erase(TaskMaster,itr);
		
		//登録修正なら、指定時間を代入する
		itr->Interval=(uint16)(Interval);
		itr->Fp=TaskFp;
		itr->InformStop=TaskInformStopFp;
		return 0;
	}
}
//タスクを登録（二重登録できてしまうので、危険）
boolian task_quick_start(taskmaster* TaskMaster,sint16 Interval,task_function TaskFp,task_inform TaskInformStopFp){
	if(TaskMaster==0 || TaskFp==0)return 1;
	//タスクの指定時間がおかしい場合はエラーを戻す
	if(Interval<=0)return 1;

	//登録できない場合はエラーを戻す
	if(!task_can_start(TaskMaster))return 1;

	//タスクを追加
	_task_buf_insert(TaskMaster,TaskFp,(uint16)Interval,TaskInformStopFp);

	return 0;
}
//タスクを設定変更/削除
boolian task_restart(taskmaster* TaskMaster,task_function TaskFp,sint16 Interval){
	task* itr=0;
	if(TaskMaster==0 || TaskFp==0)return 1;

	//タスクを探しに行く
	itr=_task_buf_find(TaskMaster,TaskFp);

	//新規の場合、エラーを返す
	if(itr==_task_buf_end(TaskMaster))return 1;
	
	//停止要請なら、削除する
	if(Interval<0){
		_task_buf_erase(TaskMaster,itr);
	}
	
	//登録修正なら、指定時間を代入する
	itr->Interval=(uint16)(Interval);
	return 0;
}
//タスクを開放
boolian task_stop(taskmaster* TaskMaster,task_function TaskFp){
	if(TaskMaster==0 || TaskFp==0)return 1;
	return task_restart(TaskMaster,TaskFp,-1);
}
//タスクの実行
void task_interrupt_function(taskmaster* TaskMaster,sint16 Interval){
	task* itr=0;
	uint16 NewCounter;
	if(TaskMaster==0)return;

	//新たなカウンター時間を計算する
	NewCounter=((uint16)(TaskMaster->Counter)+(uint16)(Interval))%0x8000;

	//タスクバッファを順に確認する
	itr=_task_buf_begin(TaskMaster);
	while(itr<_task_buf_end(TaskMaster)){
		//前回のカウンターと今回のカウンターの間にIntervalを跨いでいれば、実行
		if(itr->Interval>0 && TaskMaster->Counter/itr->Interval != NewCounter/itr->Interval){
			//タスクを実行
			Interval=itr->Fp(itr->Interval);
			//終了要求があった場合
			if(Interval<0){
				_task_buf_erase(TaskMaster,itr);
				continue;
			}
			//要求通りのインターバルにセットしなおす
			itr->Interval=Interval;
		}
		++itr;
	}

	//カウンターを新しくしておく
	TaskMaster->Counter=NewCounter;
}
//タスクシステムの登録がゼロでなくなった場合に通知
void task_resgist_informStart(taskmaster* TaskMaster,task_vFp_v Fp_){
	if(TaskMaster==0 || Fp_==0)return;
	TaskMaster->Fp_informStart=Fp_;
}
//タスクシステムからすべての登録が消えたと場合に通知
void task_resgist_informStop(taskmaster* TaskMaster,task_vFp_v Fp_){
	if(TaskMaster==0 || Fp_==0)return;
	TaskMaster->Fp_informStop=Fp_;
}
#
#endif
