#ifndef HMR_TASK_C_INC
#define HMR_TASK_C_INC 106
#
#ifndef HMR_TASK_INC
#	include"hmrTask.h"
#endif
//���ԗݐϗp�ϐ�
uint16 _task_msCounter;
//�^�X�N�̉ғ��̕K�v���̗L���̒ʒm�p�֐��|�C���^
task_vFp_v _task_Fp_informStart=0;
task_vFp_v _task_Fp_informStop=0;
void task_resgist_informStart(task_vFp_v Fp_){_task_Fp_informStart=Fp;}
void task_resgist_informStop(task_vFp_v Fp_){_task_Fp_informStop=Fp;}
//===�^�X�N�o�b�t�@�p�֐�===
//�^�X�N�p�֐��z��
task x_task_buf_Buf[HMR_TASK_MAXNUM];
//���݂̃^�X�N�̐�
uint8 x_task_buf_BufSize=0;
//�^�X�N�o�b�t�@������
void _task_buf_initialize(void){
	x_task_buf_BufSize=0;
}
//�^�X�N�o�b�t�@�I�[��
void _task_buf_finalize(void){
	x_task_buf_BufSize=0;
	if(_task_Fp_informStop)_task_Fp_informStop();
}
//�^�X�N�o�b�t�@�̃T�C�Y���擾����
uint8 _task_buf_size(void){return x_task_buf_BufSize;}
//�^�X�N�o�b�t�@���󂩊m�F����
boolian _task_buf_empty(void){return x_task_buf_BufSize==0;}
//�^�X�N�o�b�t�@����t���m�F����
boolian _task_buf_full(void){return x_task_buf_BufSize==HMR_TASK_MAXNUM;}
//�^�X�N�o�b�t�@�̐擪�A�h���X���擾����
task* _task_buf_begin(void){return x_task_buf_Buf;}
//�^�X�N�o�b�t�@�̖����A�h���X���擾����
task* _task_buf_end(void){return x_task_buf_Buf+x_task_buf_BufSize;}
//�^�X�N�o�b�t�@����A�^�X�N��T���Ă���
task* _task_buf_find(task_function Fp){
	task* itr;

	//�I�[�܂Ŋm�F���āA����^�X�N�̗L�����m�F����
	itr=_task_buf_begin();
	while(itr!=_task_buf_end()){
		if(itr->Fp==Fp)break;
		++itr;
	}

	return itr;
}
//�^�X�N�o�b�t�@�ɗv�f��ǉ�����
boolian _task_buf_insert(task_function Fp,sint16 msInterval){
	//task�o�b�t�@�������ς��Ȃ�G���[��Ԃ�
	if(_task_buf_full())return 1;

	//�o�^���e�𖖔��ɏ������݁A�T�C�Y�𑝂₷
	x_task_buf_Buf[x_task_buf_BufSize].Fp=Fp;
	x_task_buf_Buf[x_task_buf_BufSize].msInterval=msInterval;
	++x_task_buf_BufSize;

	//task�����߂ēo�^���ꂽ�ꍇ�́Ainform�֐����Ăяo��
	if(x_task_buf_BufSize==1 && _task_Fp_informStart)_task_Fp_informStart();

	return 0;
}
//�^�X�N�o�b�t�@�̗v�f���폜����
boolian _task_buf_erase(task* itr){
	task* pre;

	//�w��itr�Ȍ��
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
//===�^�X�N�p���J�֐�===
//�^�X�N�������֐�
void task_initialize(void){
	_task_Fp_informStart=0;
	_task_Fp_informStop=0;
	_task_msCounter=0;
	_task_buf_initialize();
}
//�^�X�N�I�[���֐�	
void task_finalize(void){_task_buf_finalize();}
//�^�X�N�ɓo�^�ł���m�F����
boolian task_can_start(void){return !_task_buf_full();}
//�^�X�N��o�^�i��d�o�^�ł��Ă��܂��̂ŁA�댯�j
boolian task_quick_start(task_function TaskFp,sint16 msInterval){
	//�^�X�N�̎w�莞�Ԃ����������ꍇ�̓G���[��߂�
	if(msInterval<=0)return 1;

	//�o�^�ł��Ȃ��ꍇ�̓G���[��߂�
	if(!task_can_start())return 1;

	//�^�X�N��ǉ�
	_task_buf_insert(TaskFp,(uint16)msInterval);

	return 0;
}
//�^�X�N��o�^(��d�o�^�͉��)
boolian task_start(task_function TaskFp,sint16 msInterval){
	task* itr;

	//�^�X�N��T���ɍs��
	itr=_task_buf_find(TaskFp);

	//�V�K�̏ꍇ
	if(itr==_task_buf_end()){
		//�w���Ԃ����̒l�Ȃ�A�G���[��߂�
		if(msInterval<0)return 1;

		//�o�^�ł��Ȃ��ꍇ�́A�G���[��߂�
		if(_task_buf_full())return 1;

		//�^�X�N��o�^
		return _task_buf_insert(TaskFp,msInterval);
	}//�����̏ꍇ
	else{
		//��~�v���Ȃ�A�폜����
		if(msInterval<0)return _task_buf_erase(itr);
		
		//�o�^�C���Ȃ�A�w�莞�Ԃ�������
		itr->msInterval=(uint16)(msInterval);
		return 0;
	}
}
//�^�X�N��ݒ�ύX/�폜
boolian task_restart(task_function TaskFp,sint16 msInterval){
	task* itr;

	//�^�X�N��T���ɍs��
	itr=_task_buf_find(TaskFp);

	//�V�K�̏ꍇ�A�G���[��Ԃ�
	if(itr==_task_buf_end())return 1;
	
	//��~�v���Ȃ�A�폜����
	if(msInterval<0){
		_task_buf_erase(itr);
	}
	
	//�o�^�C���Ȃ�A�w�莞�Ԃ�������
	itr->msInterval=(uint16)(msInterval);
	return 0;
}
//�^�X�N���J��
boolian task_stop(task_function TaskFp){
	return task_restart(TaskFp,-1);
}
//�^�X�N�̎��s
void task_interrupt_function(taskmaster* TaskMaster,sint16 Interval){
	task* itr;
	uint16 msNewCounter;

	//�V���ȃJ�E���^�[���Ԃ��v�Z����
	msNewCounter=(TaskMaster->Counter+Interval)%0x8000;

	//�^�X�N�o�b�t�@�����Ɋm�F����
	itr=_task_buf_begin();
	while(itr<_task_buf_end()){
		//�O��̃J�E���^�[�ƍ���̃J�E���^�[�̊Ԃ�Interval���ׂ��ł���΁A���s
		if(itr->Interval>0 && _task_msCounter/itr->Interval != msNewCounter/itr->Interval){
			//�^�X�N�����s
			Interval=itr->Fp(itr->Interval);
			//�I���v�����������ꍇ
			if(Interval<0){
				_task_buf_erase(itr);
				continue;
			}
			//�v���ʂ�̃C���^�[�o���ɃZ�b�g���Ȃ���
			itr->Interval=Interval;
		}
		++itr;
	}

	//�J�E���^�[��V�������Ă���
	_task_msCounter=msNewCounter;
}
#
#endif
