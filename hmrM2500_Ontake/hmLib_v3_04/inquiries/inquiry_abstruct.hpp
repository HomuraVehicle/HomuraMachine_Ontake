#ifndef HMLIB_INQUIRIES_INQUIRYABSTRUCT_INC
#define HMLIB_INQUIRIES_INQUIRYABSTRUCT_INC 100
#
/*===inquiries::inquiry_abstruct===
inquiry機能の基底クラス
connectionについてもここで管理している

inquiries::inquiry_abstruct:v1_00/130328 hmIto
	inquiryから分離
*/
#ifndef HMLIB_EXCEPTION_INC
#	include<hmLib_v3_04/exception.hpp>
#endif
namespace hmLib{
	namespace inquiries{
		//connectionシステムを提供する機能
		class inquiry_abstruct{
		public:
			class connection{
			private:
				inquiry_abstruct* ptr;
			private:
				connection(const connection& My_);
				const connection& operator=(const connection& My_);
			public:
				connection():ptr(nullptr){}
				connection(inquiry_abstruct* ptr_):ptr(ptr_){}
				connection(connection&& My_):ptr(nullptr){
					std::swap(ptr,My_.ptr);
					ptr->ptr=this;
				}
				const connection& operator=(connection&& My_){
					hmLib_assert(ptr==nullptr,"connection already have a pointer");
					ptr=My_.ptr;
					My_.ptr=nullptr;
					return *this;
				}
				~connection(){if(ptr)ptr->ptr=nullptr;}
				bool is_connect()const{return ptr!=nullptr;}
				void disconnect(){if(ptr)ptr->disconnect();}
				friend class inquiry_abstruct;
			};
		private:
			bool IsConnect;
			connection* ptr;
		protected:
			connection create_connection(void){
				hmLib_assert(!is_connect(),"inquiry_abstruct have already connected");
				IsConnect=true;
				return connection(this);
			}
		public:
			virtual void disconnect(void){
				hmLib_assert(is_connect(),"inquiry_abstruct have no connection");
				if(ptr)ptr->ptr=nullptr;
				ptr=nullptr;
				IsConnect=false;
			}
			bool is_connect(void)const{return IsConnect;}
		public:
			inquiry_abstruct():ptr(nullptr),IsConnect(false){}
			inquiry_abstruct(inquiry_abstruct&& My_):ptr(My_.ptr),IsConnect(My_.IsConnect){
				My_.ptr=nullptr;
				My_.IsConnect=false;
			}
			const inquiry_abstruct& operator=(inquiry_abstruct&& My_){
				IsConnect=My_.IsConnect;
				ptr=My_.ptr;
				My_.IsConnect=false;
				My_.ptr=nullptr;
			}
			virtual ~inquiry_abstruct(){if(is_connect())disconnect();}
		private:
			inquiry_abstruct(const inquiry_abstruct& My_);
			const inquiry_abstruct& operator=(const inquiry_abstruct& My_);
		};
		typedef inquiry_abstruct::connection connection;
	}
}
#
#endif
