#ifndef HMLIB_SIGNALS_INC
#define HMLIB_SIGNALS_INC 100
#
/*===signals===
boost::signals2を管理/補助するためのクラス群

提供機能
	unique_connections
		destructorでdisconnectを呼ぶconnection保持クラス
		複数のconnectionを保持可能
*/
#include"signals/connect.hpp"
#include"signals/unique_connections.hpp"
#
#endif
