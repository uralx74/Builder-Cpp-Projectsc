//---------------------------------------------------------------------------
#ifndef XmlLoader_H
#define XmlLoader_H

#include "c:\PROGRS\current\util\CommandLine.h"
#include "c:\PROGRS\current\util\MSXMLWorks.h"
#include "Storage.h"
#include "StorageDbf.h"
#include "StorageOra.h"
#include "StorageText.h"
#include "StorageExcel.h"
#include "Logger.h"
#include "encoder.h"

//---------------------------------------------------------------------------
class TXmlLoader
{
private:	// User declarations
    AnsiString ExpandFileNameCustom(AnsiString FileName, AnsiString FilePath);
    TCommandLine* CommandLine;
    TLogger* Logger;

public:
    TXmlLoader();
    ~TXmlLoader();
    bool __fastcall LoadParameters();

    // �������� � ��������
    TStorage* SrcStor;
    TStorage* DstStor;

};





//---------------------------------------------------------------------------
#endif
