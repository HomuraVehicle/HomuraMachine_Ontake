#ifndef HMLIB_INQUIRIES_RANGEINQUIRY_INC
#define HMLIB_INQUIRIES_RANGEINQUIRY_INC 100
#
/*===inquiries::range_inquiry===
�R���e�i�^�C�v�̕ϐ��ɑ΂��āAiterator��begin/end�֐��ɂ���ĊԐړI�Ɏ擾����N���X�A
inquiries::range_inquiry��񋟁B

inquiries::range_inquiry:v1_00/130328 hmIto
	inquiry���番��
*/
#include<memory>
#include<functional>
#ifndef HMLIB_MUTEXPROXY_INC
#	include<hmLib_v3_04/mutex_proxy.hpp>
#endif
#ifndef HMLIB_EXCEPTION_INC
#	include<hmLib_v3_04/exception.hpp>
#endif
#ifndef HMLIB_INQUIRIES_INQUIRYABSTRUCT_INC
#	include"inquiry_abstruct.hpp"
#endif
namespace hmLib{
	namespace inquiries{
		//�R���e�i�ւ̎Q��inquiry
		template<typename iterator_>
		class range_inquiry:public inquiry_abstruct{
		private:
			std::function<iterator_(void)> FnBegin;
			std::function<iterator_(void)> FnEnd;
			bool locked;
			std::unique_ptr<mutex_proxy> pmx;
		public:
			template<typename Containor_>
			connection connect(const Containor_& t_){
				hmLib_assert(!is_connect(),"have connected yet");
				FnBegin=[&](void)->iterator_{return std::begin(t_);};
				FnEnd=[&](void)->iterator_{return std::end(t_);};
				return create_connection();
			}
			template<typename Containor_,typename Mutex_>
			connection connect(const Containor_& t_,Mutex_& mx_){
				hmLib_assert(!is_connect(),"have connected yet");
				FnBegin=[&](void)->iterator_{return std::begin(t_);};
				FnEnd=[&](void)->iterator_{return std::end(t_);};
				pmx.reset(new mutex_proxy(mx_));
				return create_connection();
			}
			iterator_ begin()const{
				hmLib_assert(is_connect(),"have not connected yet");
				return FnBegin();
			}
			iterator_ end()const{
				hmLib_assert(is_connect(),"have not connected yet");
				return FnEnd();
			}
			void lock(){
				hmLib_assert(is_connect(),"have not connected yet");
				if(pmx)	pmx->lock();
				locked=true;
			}
			void unlock(){
				hmLib_assert(is_connect(),"have not connected yet");
				if(pmx)pmx->unlock();
				locked=false;
			}
			bool try_lock(){
				hmLib_assert(is_connect(),"have not connected yet");
				if(pmx && pmx->try_lcok())locked=true;
				return locked;
			}
		};
	}
}
#
#endif
