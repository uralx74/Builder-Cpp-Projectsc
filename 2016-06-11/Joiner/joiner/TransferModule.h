//---------------------------------------------------------------------------
#ifndef TransferModuleH
#define TransferModuleH

#include "c:\PROGRS\current\util\taskutils.h"
#include "c:\PROGRS\current\util\appver.h"
#include "c:\PROGRS\current\util\MSExcelWorks.h"
#include "c:\PROGRS\current\util\MSXMLWorks.h"
#include "c:\PROGRS\current\util\appver.h"
#include "c:\PROGRS\current\util\CommandLine.h"
#include <ComCtrls.hpp>
#include "Storage.h"
#include "Logger.h"

//---------------------------------------------------------------------------
class TTransferModule
{
private:	// User declarations
public:		// User declarations
    __fastcall TTransferModule();

    bool __fastcall LoadParameters();
    void __fastcall Start();
    void __fastcall Transfer(TStorage* Src, TStorage* Dst);


    // �������� � ��������
    TStorage* SrcStor;
    TStorage* DstStor;




    //Singleton& Logger;
    Singleton* Logger;
};

//---------------------------------------------------------------------------
#endif
