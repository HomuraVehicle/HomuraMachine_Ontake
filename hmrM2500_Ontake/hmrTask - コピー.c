#ifndef HMR_TASK_C_INC
#define HMR_TASK_C_INC 106
#
#ifndef HMR_TASK_INC
#	include"hmrTask.h"
#endif
//時間累積用変数
uint16 _task_msCounter;
//タスクの稼働の必要性の有無の通知用関数ポインタ
task_vFp_v _task_Fp_informStart=0;
task_vFp_v _task_Fp_informStop=0;
void task_resgist_informStart(task_vFp_v Fp_){_task_Fp_informStart=Fp;}
void task_resgist_informStop(task_vFp_v Fp_){_task_Fp_informStop=Fp;}
//===タスクバッファ用関数===
//タスク用関数配列
task x_task_buf_Buf[HMR_TASK_MAXNUM];
//現在のタスクの数
uint8 x_task_buf_BufSize=0;
//タスクバッファ初期化
void _task_buf_initialize(void){
	x_task_buf_BufSize=0;
}
//タスクバッファ終端化
void _task_buf_finalize(void){
	x_task_buf_BufSize=0;
	if(_task_Fp_informStop)_task_Fp_informStop();
}
//タスクバッファのサイズを取得する
uint8 _task_buf_size(void){return x_task_buf_BufSize;}
//タスクバッファが空か確認する
boolian _task_buf_empty(void){return x_task_buf_BufSize==0;}
//タスクバッファが一杯か確認する
boolian _task_buf_full(void){return x_task_buf_BufSize==HMR_TASK_MAXNUM;}
//タスクバッファの先頭アドレスを取得する
task* _task_buf_begin(void){return x_task_buf_Buf;}
//タスクバッファの末尾アドレスを取得する
task* _task_buf_end(void){return x_task_buf_Buf+x_task_buf_BufSize;}
//タスクバッファから、タスクを探してくる
task* _task_buf_find(task_function Fp){
	task* itr;

	//終端まで確認して、同一タスクの有無を確認する
	itr=_task_buf_begin();
	while(itr!=_task_buf_end()){
		if(itr->Fp==Fp)break;
		++itr;
	}

	return itr;
}
//タスクバッファに要素を追加する
boolian _task_buf_insert(task_function Fp,sint16 msInterval){
	//taskバッファがいっぱいならエラーを返す
	if(_task_buf_full())return 1;

	//登録内容を末尾に書き込み、サイズを増やす
	x_task_buf_Buf[x_task_buf_BufSize].Fp=Fp;
	x_task_buf_Buf[x_task_buf_BufSize].msInterval=msInterval;
	++x_task_buf_BufSize;

	//taskが初めて登録された場合は、inform関数を呼び出す
	if(x_task_buf_BufSize==1 && _task_Fp_informStart)_task_Fp_informStart();

	return 0;
}
//タスクバッファの要素を削除する
boolian _task_buf_erase(task* itr){
	task* pre;

	//指定itr以後の
	while(1){
		pre=itr++;
		if(itr==_task_buf_end())break;
		pre->Fp=itr->Fp;
		pre->msInterval=itr->msInterval;
	}

	--x_task_buf_BufSize;
	if(x_task_buf_BufSize==0 && _task_Fp_informStop)_task_Fp_informStop();

	return 0;
}
//===タスク用公開関数===
//タスク初期化関数
void task_initialize(void){
	_task_Fp_informStart=0;
	_task_Fp_informStop=0;
	_task_msCounter=0;
	_task_buf_initialize();
}
//タスク終端化関数	
void task_finalize(void){_task_buf_finalize();}
//タスクに登録できる確認する
boolian task_can_start(void){return !_task_buf_full();}
//タスクを登録（二重登録できてしまうので、危険）
boolian task_quick_start(task_function TaskFp,sint16 msInterval){
	//タスクの指定時間がおかしい場合はエラーを戻す
	if(msInterval<=0)return 1;

	//登録できない場合はエラーを戻す
	if(!task_can_start())return 1;

	//タスクを追加
	_task_buf_insert(TaskFp,(uint16)msInterval);

	return 0;
}
//タスクを登録(二重登録は回避)
boolian task_start(task_function TaskFp,sint16 msInterval){
	task* itr;

	//タスクを探しに行く
	itr=_task_buf_find(TaskFp);

	//新規の場合
	if(itr==_task_buf_end()){
		//指定状態が負の値なら、エラーを戻す
		if(msInterval<0)return 1;

		//登録できない場合は、エラーを戻す
		if(_task_buf_full())return 1;

		//タスクを登録
		return _task_buf_insert(TaskFp,msInterval);
	}//既存の場合
	else{
		//停止要請なら、削除する
		if(msInterval<0)return _task_buf_erase(itr);
		
		//登録修正なら、指定時間を代入する
		itr->msInterval=(uint16)(msInterval);
		return 0;
	}
}
//タスクを設定変更/削除
boolian task_restart(task_function TaskFp,sint16 msInterval){
	task* itr;

	//タスクを探しに行く
	itr=_task_buf_find(TaskFp);

	//新規の場合、エラーを返す
	if(itr==_task_buf_end())return 1;
	
	//停止要請なら、削除する
	if(msInterval<0){
		_task_buf_erase(itr);
	}
	
	//登録修正なら、指定時間を代入する
	itr->msInterval=(uint16)(msInterval);
	return 0;
}
//タスクを開放
boolian task_stop(task_function TaskFp){
	return task_restart(TaskFp,-1);
}
//タスクの実行
void task_interrupt_function(taskmaster* TaskMaster,sint16 Interval){
	task* itr;
	uint16 msNewCounter;

	//新たなカウンター時間を計算する
	msNewCounter=(TaskMaster->Counter+Interval)%0x8000;

	//タスクバッファを順に確認する
	itr=_task_buf_begin();
	while(itr<_task_buf_end()){
		//前回のカウンターと今回のカウンターの間にIntervalを跨いでいれば、実行
		if(itr->Interval>0 && _task_msCounter/itr->Interval != msNewCounter/itr->Interval){
			//タスクを実行
			Interval=itr->Fp(itr->Interval);
			//終了要求があった場合
			if(Interval<0){
				_task_buf_erase(itr);
				continue;
			}
			//要求通りのインターバルにセットしなおす
			itr->Interval=Interval;
		}
		++itr;
	}

	//カウンターを新しくしておく
	_task_msCounter=msNewCounter;
}
#
#endif
