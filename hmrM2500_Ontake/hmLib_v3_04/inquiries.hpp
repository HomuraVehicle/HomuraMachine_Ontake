#ifndef HMLIB_INQUIRIES_INC
#define HMLIB_INQUIRIES_INC 100
#
/*===inquiries::inquiry===
値を能動的に参照取得するための共通インターフェースを提供する
	安全のため、いずれのinquiryクラスパターンでもconstな制御しか許されていない。
	接続相手の値を能動的に制御するためには、signal/slotパターンの利用を推奨している。
	高頻度のsignal/slot発信を回避する目的であれば、inquiry_signalの利用を推奨している。
	
提供クラス
	inquiryクラス
		あらゆるオブジェクトと接続することができる。
		operator()(void)で、接続したオブジェクトをconstで取得することができる。
		lock/unlock/try_lock関数で、オブジェクトを一時的にスレッド排他的に利用することができる。
	range_inquiryクラス
		begin/end関数を利用できるオブジェクトと接続することができる。
		begin/end関数で、iterator_を取得できる。
		lock/unlock/try_lock関数で、オブジェクトを一時的にスレッド排他的に利用することができる。
	signal_inquiryクラス
		値を返さずにboost::signals2を発信するinquiry。
	unique_contactsクラス
		コンタクトを複数保持し、destructor呼び出し時にdisconnectを呼び出すようなクラス。

inquieies v1_00/130328 hmIto
	inquiryを継承、分割して成立。
inquiry:v1_00/130328 hmIto
	inquiryの定義を変更
		能動型signalとしてのinquiryから、const参照値取得クラスとしてのinquiryへ定義変更。
	inquiry追加
		operator()で値参照機能を提供するクラス
		mutexインターフェースを備える
	range_inquiry追加
		指定のiteratorのbegin/endを提供するクラス。
		mutexインターフェースを備える
	unique_connections追加
		connectionクラスを保持し、destructorでdisconnectを呼び出す。
inquiry:v0_03/130324 hmIto
	inquiryにdefault_valueを追加
inquiry:v0_02/130224 hmIto
	inquiry_astruct::connection(inquiry_abstruct*)の変数名が間違っていた致命的バグを修正
inquiry:v0_01/130224 hmIto
	inquiryについては提供開始
	range_inquiryは、iterator_proxy待ち
*/
#include "inquiries/inquiry.hpp"
#include "inquiries/range_inquiry.hpp"
#include "inquiries/signal_inquiry.hpp"
#include "inquiries/unique_connections.hpp"
#include "inquiries/connect.hpp"
#
#endif
