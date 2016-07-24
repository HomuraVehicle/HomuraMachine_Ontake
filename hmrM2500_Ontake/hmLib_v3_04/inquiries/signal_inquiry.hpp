#ifndef HMLIB_INQUIRIES_SIGNALINQUIRY_INC
#define HMLIB_INQUIRIES_SIGNALINQUIRY_INC 100
#
/*===inquiries::signal_inquiry===
operator()(void)で、取得した値を返さずに、Signalを発信するinquiry

inquiries::signal_inquiry:v1_00/130328 hmIto
	新規作成
*/
#include<boost/signals2.hpp>
#ifndef HMLIB_EXCEPTION_INC
#	include<hmLib_v3_04/exception.hpp>
#endif
#ifndef HMLIB_INQUIRIES_INQUIRYABSTRUCT_INC
#	include"inquiry_abstruct.hpp"
#endif
namespace hmLib{
	namespace inquiries{
		template<typename T>
		class signal_inquiry:public inquiry_abstruct{
			typedef boost::signals2::signal<void(const T&)> signal;
		public:
			signal Signal;
		private:
			const T* ptr;
			bool locked;
			std::unique_ptr<mutex_proxy> pmx;
		public:
			connection connect(const T& t_){
				hmLib_assert(!is_connect(),"have connected yet");
				ptr=&t_;
				return create_connection();
			}
			template<typename Mutex_>
			connection connect(const T& t_,Mutex_& mx_){
				hmLib_assert(!is_connect(),"have connected yet");
				ptr=&t_;
				pmx.reset(new mutex_proxy(mx_));
				return create_connection();
			}
			void operator()(void)const{
				hmLib_assert(is_connect(),"have not connected yet");
				if(pmx){
					std::lock_guard<mutex_proxy> Lock(*pmx);
					Signal(*ptr);
				}else{
					Signal(*ptr);
				}
			}
		};
	}
}
#
#endif
