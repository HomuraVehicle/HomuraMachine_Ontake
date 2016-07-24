#ifndef HMLIB_INQUIRIES_INC
#define HMLIB_INQUIRIES_INC 100
#
/*===inquiries::inquiry===
�l��\���I�ɎQ�Ǝ擾���邽�߂̋��ʃC���^�[�t�F�[�X��񋟂���
	���S�̂��߁A�������inquiry�N���X�p�^�[���ł�const�Ȑ��䂵��������Ă��Ȃ��B
	�ڑ�����̒l��\���I�ɐ��䂷�邽�߂ɂ́Asignal/slot�p�^�[���̗��p�𐄏����Ă���B
	���p�x��signal/slot���M���������ړI�ł���΁Ainquiry_signal�̗��p�𐄏����Ă���B
	
�񋟃N���X
	inquiry�N���X
		������I�u�W�F�N�g�Ɛڑ����邱�Ƃ��ł���B
		operator()(void)�ŁA�ڑ������I�u�W�F�N�g��const�Ŏ擾���邱�Ƃ��ł���B
		lock/unlock/try_lock�֐��ŁA�I�u�W�F�N�g���ꎞ�I�ɃX���b�h�r���I�ɗ��p���邱�Ƃ��ł���B
	range_inquiry�N���X
		begin/end�֐��𗘗p�ł���I�u�W�F�N�g�Ɛڑ����邱�Ƃ��ł���B
		begin/end�֐��ŁAiterator_���擾�ł���B
		lock/unlock/try_lock�֐��ŁA�I�u�W�F�N�g���ꎞ�I�ɃX���b�h�r���I�ɗ��p���邱�Ƃ��ł���B
	signal_inquiry�N���X
		�l��Ԃ�����boost::signals2�𔭐M����inquiry�B
	unique_contacts�N���X
		�R���^�N�g�𕡐��ێ����Adestructor�Ăяo������disconnect���Ăяo���悤�ȃN���X�B

inquieies v1_00/130328 hmIto
	inquiry���p���A�������Đ����B
inquiry:v1_00/130328 hmIto
	inquiry�̒�`��ύX
		�\���^signal�Ƃ��Ă�inquiry����Aconst�Q�ƒl�擾�N���X�Ƃ��Ă�inquiry�֒�`�ύX�B
	inquiry�ǉ�
		operator()�Œl�Q�Ƌ@�\��񋟂���N���X
		mutex�C���^�[�t�F�[�X�������
	range_inquiry�ǉ�
		�w���iterator��begin/end��񋟂���N���X�B
		mutex�C���^�[�t�F�[�X�������
	unique_connections�ǉ�
		connection�N���X��ێ����Adestructor��disconnect���Ăяo���B
inquiry:v0_03/130324 hmIto
	inquiry��default_value��ǉ�
inquiry:v0_02/130224 hmIto
	inquiry_astruct::connection(inquiry_abstruct*)�̕ϐ������Ԉ���Ă����v���I�o�O���C��
inquiry:v0_01/130224 hmIto
	inquiry�ɂ��Ă͒񋟊J�n
	range_inquiry�́Aiterator_proxy�҂�
*/
#include "inquiries/inquiry.hpp"
#include "inquiries/range_inquiry.hpp"
#include "inquiries/signal_inquiry.hpp"
#include "inquiries/unique_connections.hpp"
#include "inquiries/connect.hpp"
#
#endif