#ifndef HMR_DATA_C_INC
#define HMR_DATA_C_INC 101
#define HMLIB_NOLIB
#ifndef HMR_DATA_INC
#	include "hmrData.h"
#endif
#ifndef HMR_UTILITY_INC
#	include "hmrUtility.h"
#endif
#

void idata_format(idata* Dat){
	Dat->ID = 0;
	cstring_format(&(Dat->Str));
	return;
}
void odata_format(odata* Dat){
	Dat->ID = 0;
	Dat->DSRPAG=0;
	Dat->FpSendable = 0;
	Dat->FpInformDestruct = 0;
	cstring_format(&(Dat->Str));
	return;
}

hmLib_boolian idata_is_construct(idata* Dat){
	return Dat->ID != 0;
}
hmLib_boolian odata_is_construct(odata* Dat){
	return Dat->ID != 0;
}

void idata_construct(idata* Dat, data_did_t ID, cstring* mStr){
	Dat->ID = ID;
	cstring_move( mStr, &(Dat->Str) );
	return;
}
void odata_construct(odata* Dat, data_did_t ID, cstring* mStr, int DSRPAG,data_dsizeFp_v FpSendable, data_vFp_v FpInformDestruct){
	Dat->ID = ID;
	Dat->FpSendable = FpSendable;
	Dat->FpInformDestruct = FpInformDestruct;
	cstring_move( mStr, &(Dat->Str) );
	Dat->DSRPAG=DSRPAG;
	return;
}

void idata_destruct(idata* Dat){
	if(cstring_is_construct(&(Dat->Str))){
		cstring_destruct_safe(&(Dat->Str));
	}
	Dat->ID = 0;
}
void odata_destruct(odata* Dat){
	if(cstring_is_construct(&(Dat->Str))){
		cstring_destruct_safe(&(Dat->Str));
	}
	Dat->ID = 0;
	Dat->FpSendable = 0;
	if(Dat->FpInformDestruct)Dat->FpInformDestruct();
	Dat->FpInformDestruct = 0;
}

void idata_move(idata* From, idata* To){
	To->ID=From->ID;
	cstring_move(&(From->Str),&(To->Str));
	idata_format(From);
}
void odata_move(odata* From, odata* To){
	To->ID=From->ID;
	cstring_move(&(From->Str),&(To->Str));
	To->FpSendable = From->FpSendable;
	To->FpInformDestruct = From->FpInformDestruct;
	odata_format(From);
}

#
#endif
