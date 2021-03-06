//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MonthPicker.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
//
//---------------------------------------------------------------------------
//
static inline void ValidCtrCheck(TMonthPicker *)
{
    new TMonthPicker(NULL);
}
//---------------------------------------------------------------------------
__fastcall TMonthPicker::TMonthPicker(TComponent* Owner)
    : TPanel(Owner)
{
    ShortDateFormat = "dd.mm.yyyy";
    DateSeparator = '.';
    this->Caption = " ";
    Create(this);
    SetDate(Now());
    FEnabled = true;
}

//---------------------------------------------------------------------------
namespace Monthpicker
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TMonthPicker)};
         RegisterComponents("Alt", classes, 0);
    }
}

//---------------------------------------------------------------------------
//
__fastcall TMonthPicker::~TMonthPicker(void)
{

    //delete shape;
    //delete edit;
    //delete button1;
    //delete button2;
    //delete panel;
}

//---------------------------------------------------------------------------
// �������� ��������� ����������
__fastcall TMonthPicker::Create(TWinControl *Owner)
{
    int left = 0;
    int top = 0;
    int colors[5] = {RGB(53,194,255), RGB(171,220,24), RGB(255,210,64), RGB(200,170,120), RGB(53,194,255)};
    int color;

    this->Width = 156;
    this->Height = 115;

    edit = new TEdit(Owner);
    edit->Parent = this;
    edit->Width = 50;
    edit->Height = 22;
    edit->Top = 1;
    edit->Left = this->Width / 2 - edit->Width / 2;
    edit->MaxLength = 4;

    edit->OnKeyPress = EditKeyPress;
    edit->OnChange = EditChange;

    button1 = new TSpeedButton(Owner);
    button1->Parent = this;
    button1->Width = 23;
    button1->Top = 1;
    button1->Left = edit->Left - button1->Width - 1;
    button1->Tag = -1;
    button1->OnClick = SpeedButtonClick;
    button1->Caption = "<";
    button1->Flat = true;
    button1->BringToFront();

    button2 = new TSpeedButton(Owner);
    button2->Parent = this;
    button2->Width = 23;
    button2->Top = 1;
    button2->Left = edit->Left + edit->Width + 1;
    button2->Tag = 1;
    button2->OnClick = SpeedButtonClick;
    button2->Caption = ">";
    button2->Flat = true;


    shape = new TShape(Owner);
    shape->Parent = this;
    shape->Width = this->Width - 2;
    shape->Height = this->Height - edit->Height - 4;
    shape->Top = edit->Height + 3;
    shape->Left = 1;
    shape->Brush->Style = bsClear;
    shape->Pen->Style = psSolid;
    shape->Pen->Color = clSilver;
    shape->Pen->Width = 1;

    for(int i=0; i<=11; i++)
    {
        if (i%4 == 0) {
            top += 28;
            left = 4;
        }

        TSpeedButton* button = new TSpeedButton(Owner);
        //l->Caption = ShortMonthNames[i];
        button->Caption = AnsiUpperCase(ShortMonthNames[i][1])+ShortMonthNames[i].SubString(2, ShortMonthNames[i].Length()-1);
        button->Parent = this;
        button->Name = "Label_text" + String(i);
        button->Width = 28;
        button->Left = left;
        button->Top = top;
        button->Font->Size = 8;
        button->GroupIndex = 1;
        button->Flat = true;
        button->OnClick = MonthButtonClick;
        button->Tag = i;
        btnMonthsList[i] = button;

        color = colors[(i+1)/3];

        TLabel* l_c = new TLabel(Owner);
        l_c->Name = "Label_color" + String(i);
        l_c->Parent = this;
        l_c->Width = 20;
        l_c->Height = 2;
        l_c->Left = button->Left + button->Width / 2 - l_c->Width / 2;
        l_c->Top = button->Top+button->Height+1;
        l_c->Color = color;
        labelColorLines[i] = l_c;

        left += 40;
    }           
}

//---------------------------------------------------------------------------
// �������� ������ ������� ����
void __fastcall TMonthPicker::SetMinDate(TDateTime dt)
{
    unsigned short year, month, day;
    dt.DecodeDate(&year, &month, &day);
    dt = EncodeDate(year, month, 1);

    if (!CheckBoundMax(dt))
        throw "Min date must be less than the max date";

    FMinDate = dt;
    FixDate();
    UpdateControl();
}

//---------------------------------------------------------------------------
// �������� ������� ������� ����
void __fastcall TMonthPicker::SetMaxDate(TDateTime dt)
{
    unsigned short year, month, day;
    dt.DecodeDate(&year, &month, &day);
    dt = IncMonth(EncodeDate(year, month, 1), 1)-1;

    if (!CheckBoundMin(dt))
        throw "Max date must be greater than the min date";

    FMaxDate = dt;
    FixDate();
    UpdateControl();
}

//---------------------------------------------------------------------------
// �������� ������ ���� �� ������� � ������ �������
bool __fastcall TMonthPicker::CheckBounds(TDateTime dt)
{
    return CheckBoundMin(dt) && CheckBoundMax(dt);
}

//---------------------------------------------------------------------------
// �������� ������ �� ������ �������
inline bool __fastcall TMonthPicker::CheckBoundMin(TDateTime dt)
{
    return (int(FMinDate) == 0 || dt >= FMinDate);
}

//---------------------------------------------------------------------------
// �������� ������ �� ������� �������
inline bool __fastcall TMonthPicker::CheckBoundMax(TDateTime dt)
{
    return (int(FMaxDate) == 0 || dt <= FMaxDate);
}

//---------------------------------------------------------------------------
//  ��������� ����
void __fastcall TMonthPicker::FixDate()
{
    TDateTime seldate = GetDate();
    // ��������� ���������� ������
    if (!CheckBoundMax(seldate))
    {
        SetDate(FMaxDate);
    }
    else if (!CheckBoundMin(seldate))
    {
        SetDate(FMinDate);
    }
}

//---------------------------------------------------------------------------
//  ��������� ����
void __fastcall TMonthPicker::FixMonth()
{
    TDateTime seldate = GetDate();
    // ��������� ���������� ������
    if (!CheckBoundMax(seldate))
    {
        SetMonth(FMaxDate);
    }
    else if (!CheckBoundMin(seldate))
    {
        SetMonth(FMinDate);
    }
}

//---------------------------------------------------------------------------
// ���������� ��������� ����������
void __fastcall TMonthPicker::UpdateControl()
{
    TDateTime curyear = EncodeDate(FYear, 1, 1);
    //TDateTime seldate = GetDate();

    // ����������� ����� ����
    button1->Enabled = int(FMinDate) == 0 || FMinDate < curyear;
    button2->Enabled = int(FMaxDate) == 0 || FMaxDate > IncMonth(curyear,12);
    edit->Enabled = button1->Enabled || button2->Enabled;

    // ��������� ������
    TDateTime tmpdt;
    for (unsigned short i=0; i<12; i++) {
        tmpdt = IncMonth(curyear,i);
        btnMonthsList[i]->Enabled = CheckBounds(tmpdt);
    }
}


//---------------------------------------------------------------------------
// ��������� �������� Enabled
void __fastcall TMonthPicker::SetFEnabled(bool Value)
{
    FEnabled = Value;
    button1->Enabled = Value;
    button2->Enabled = Value;
    for (unsigned short i=0; i<12; i++) {
        btnMonthsList[i]->Enabled = Value;
    }
    btnMonthsList[this->FMonth]->Enabled = true;
}

//---------------------------------------------------------------------------
// ��������� ����
void __fastcall TMonthPicker::SetYear(unsigned short year)
{
    edit->OnChange = NULL;

    if (year > 0 && CheckBoundMin(EncodeDate(year, 12, 31)) && CheckBoundMax(EncodeDate(year, 1, 1))) {
        this->FYear = year;
        edit->Text = year;
    } else {
        edit->Text = this->FYear;
    }

    // ��������� ���������� ������
    FixMonth();

    edit->OnChange = EditChange;
    if (FOnChange != NULL)
        FOnChange(this);

    UpdateControl();
}

//---------------------------------------------------------------------------
// ���������� ����� short
void __fastcall TMonthPicker::SetMonth(unsigned short month)
{
    //btnMonthsList[this->pMonth]->Font->Style = btnMonthsList[this->pMonth]->Font->Style >> fsBold;
    this->FMonth = month-1;
    //btnMonthsList[this->pMonth]->Font->Style = btnMonthsList[this->pMonth]->Font->Style << fsBold;
    btnMonthsList[this->FMonth]->Down = true;
    if (FOnChange != NULL)
        FOnChange(this);
}

//---------------------------------------------------------------------------
// ���������� ����� �� TDateTime
void __fastcall TMonthPicker::SetMonth(TDateTime dt)
{
    unsigned short year, month, day;
    dt.DecodeDate(&year, &month, &day);
    SetMonth(month);
}

//---------------------------------------------------------------------------
// ���������� ��� �� TDateTime
void __fastcall TMonthPicker::SetYear(TDateTime dt)
{
    unsigned short year, month, day;
    dt.DecodeDate(&year, &month, &day);
    SetYear(year);
}

//---------------------------------------------------------------------------
// ���������� ���� �� TDateTime
void __fastcall TMonthPicker::SetDate(TDateTime dt)
{
    unsigned short year, month, day;
    dt.DecodeDate(&year, &month, &day);
    SetMonth(month);
    SetYear(year);
}

//---------------------------------------------------------------------------
// ������� ���� � TDateTime
TDateTime __fastcall TMonthPicker::GetDate()
{
    return EncodeDate(this->FYear, this->FMonth + 1, 1);
}

//---------------------------------------------------------------------------
// ������� ���� � String
String __fastcall TMonthPicker::GetDate(String format)
{
    return FormatDateTime(format, GetDate());
}

//---------------------------------------------------------------------------
// ������� �������� Year
unsigned short  __fastcall TMonthPicker::GetYear()
{
    return this->FYear;
}

//---------------------------------------------------------------------------
// ������� �������� Month
unsigned short __fastcall TMonthPicker::GetMonth()
{
    return this->FMonth + 1;
}

//---------------------------------------------------------------------------
// ������� �������� LastDay (��������� ���� ������)
unsigned short __fastcall TMonthPicker::GetLastDay()
{
    return MonthDays[IsLeapYear(this->FYear)][this->FMonth];
}

//---------------------------------------------------------------------------
// ���������� ������ ������ ������
void __fastcall TMonthPicker::MonthButtonClick(TObject *Sender)
{
    TSpeedButton* b = ((TSpeedButton*) Sender);
    SetMonth(b->Tag + 1);
}

//---------------------------------------------------------------------------
// ���������� ������ < � > ��� ����������/���������� ���� ���
void __fastcall TMonthPicker::SpeedButtonClick(TObject *Sender)
{
    SetYear(FYear + ((TSpeedButton*)Sender)->Tag);
}

//---------------------------------------------------------------------------
// ���������� KeyPress � ���� ���
void __fastcall TMonthPicker::EditKeyPress(TObject *Sender, char &Key)
{
    if ((Key >= '0') && (Key <= '9') || Key == VK_BACK) {}  // ����� � 'Key == '-'' - �� ������ ������ ��� ������������� �����, �� ���������� ����� ������.
    else
    if (Key == 8)
        {}  // <-
    else Key = 0;

    edit->SelLength = 1;
}

//---------------------------------------------------------------------------
// ���������� Ctrl-V � ���� ���
void __fastcall TMonthPicker::EditChange(TObject *Sender)
{
    if (!IsNumber(edit->Text))
        edit->Text = FYear;
    else
        SetYear(StrToInt(edit->Text));

    edit->SelLength = 1;
}

//------------------------------------------------------------------------------
// ��������� �������� �� ������ ����� ������
bool __fastcall TMonthPicker::IsNumber(String str)
{
    int l = str.Length();
    if ( l <= 0)
        return false;

    for (int i = 1; i <= l; i++) {
        char a = str[i];
        if (a == '1' || a== '2' || a == '3' || a == '4' || a== '5' ||
            a== '6' || a == '7' || a == '8' || a== '9' || a== '0') {
            continue;
        };
        return false;
    }
    return true;
}

