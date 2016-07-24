#ifndef HMLIB_INQUIRIES_CONNECT_INC
#define HMLIB_INQUIRIES_CONNECT_INC 100
#
/*===inquiries::connect===
boost::signals2::signalのconnectionを管理し、destructorでdisconnectするクラス

signals::unique_connections v1_00/130328 hmIto
	signals名前空間ファイルに移動
signal_unique_connections v1_00/130328 hmIto
	作成
*/
#ifndef HMLIB_EXCEPTION_INC
#	include<hmLib_v3_04/exception.hpp>
#endif
#ifndef HMLIB_INQUIRIES_INQUIRYABSTRUCT_INC
#	include"inquiry_abstruct.hpp"
#endif
#ifndef HMLIB_INQUIRIES_INQUIRY_INC
#	include"inquiry_abstruct.hpp"
#endif
#ifndef HMLIB_INQUIRIES_RANGEINQUIRY_INC
#	include"inquiry_abstruct.hpp"
#endif
namespace hmLib{
	namespace inquiries{
		template<typename Value>
		inquiries::connection connect(inquiry<Value>& Inq_,const Value& Val_){
			return Inq_.connect(Val_);
		}
		template<typename Value,typename Mutex>
		inquiries::connection connect(inquiry<Value>& Inq_,const Value& Val_,Mutex& Mx_){
			return Inq_.connect(Val_,Mx_);
		}
		template<typename Iterator,typename Func>
		inquiries::connection connect(range_inquiry<Iterator>& Inq_,const Func& Fnc_){
			return Inq_.connect(Fnc_);
		}
		template<typename Iterator,typename Func,typename Mutex>
		inquiries::connection connect(range_inquiry<Iterator>& Inq_,const Func& Fnc_,Mutex& Mx_){
			return Inq_.connect(Fnc_,Mx_);
		}
	}
}
#
#endif
