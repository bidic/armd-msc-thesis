//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "CGAUGES.h"
#include <jpeg.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TLabel *status;
        TImage *Obraz;
        TScrollBar *SerwoKat;
        TScrollBar *PrawySilnik;
        TScrollBar *LewySilnik;
        TButton *StopButton;
        TImage *pad;
        TButton *NapiecieCheck;
        TLabel *WartoscNapiecia;
        TButton *Preview3;
        TTimer *TimeOutTimer;
        TCGauge *PasekPostepu;
        TLabel *DownloadStatus;
        TLabel *DownloadErrors;
        TLabel *RXBuffer;
        TCGauge *RXPasek;
        TCGauge *EnergiaPasek;
        TButton *Preview1;
        TButton *Preview2;
        TButton *AutoWlacz;
        TButton *AutoWylacz;
        TButton *AutoPreviewOnButton;
        TButton *DiodaWlacz;
        TButton *DiodaWylacz;
        TButton *SerwoOn;
        TButton *SerwoOff;
        TLabel *LewyStatus;
        TLabel *PrawyStatus;
        TLabel *SerwoStatus;
        TComboBox *WyborPortu;
        TButton *ComConnect;
        TButton *ComDisconnect;
        TButton *Klawiatura;
        TTimer *AutoTimeOutTimer;
        TButton *AutoPreviewOffButton;
        TImage *Image2;
        TImage *Image3;
        TCheckBox *AutoPreviewCheck1;
        TCheckBox *AutoPreviewCheck2;
        TCheckBox *AutoPreviewCheck3;
        TButton *AutoPrevievDisable;
        TButton *PojedynczyPodglad;
        TImage *Image4;
        TImage *Image5;
        TImage *Image6;
        TImage *Image7;
        TMainMenu *MainMenu1;
        TMenuItem *Program1;
        TMenuItem *Informacje1;
        TMenuItem *Wylacz1;
        TTimer *SterowanieTimer;
        TImage *Image1;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall SerwoKatChange(TObject *Sender);
        void __fastcall PrawySilnikChange(TObject *Sender);
        void __fastcall LewySilnikChange(TObject *Sender);
        void __fastcall StopButtonClick(TObject *Sender);
        void __fastcall padMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall NapiecieCheckClick(TObject *Sender);
        void __fastcall Preview3Click(TObject *Sender);
        void __fastcall TimeOutTimerTimer(TObject *Sender);
        void __fastcall Preview1Click(TObject *Sender);
        void __fastcall Preview2Click(TObject *Sender);
        void __fastcall AutoPreviewOnButtonClick(TObject *Sender);
        void __fastcall AutoWlaczClick(TObject *Sender);
        void __fastcall AutoWylaczClick(TObject *Sender);
        void __fastcall SerwoOnClick(TObject *Sender);
        void __fastcall SerwoOffClick(TObject *Sender);
        void __fastcall DiodaWlaczClick(TObject *Sender);
        void __fastcall DiodaWylaczClick(TObject *Sender);
        void __fastcall ObrazMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall ObrazClick(TObject *Sender);
        void __fastcall ComConnectClick(TObject *Sender);
        void __fastcall ComDisconnectClick(TObject *Sender);
        void __fastcall KlawiaturaKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall KlawiaturaKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall AutoTimeOutTimerTimer(TObject *Sender);
        void __fastcall AutoPreviewOffButtonClick(TObject *Sender);
        void __fastcall Wylacz1Click(TObject *Sender);
        void __fastcall AutoPreviewCheck1Click(TObject *Sender);
        void __fastcall AutoPreviewCheck2Click(TObject *Sender);
        void __fastcall AutoPreviewCheck3Click(TObject *Sender);
        void __fastcall AutoPrevievDisableClick(TObject *Sender);
        void __fastcall PojedynczyPodgladClick(TObject *Sender);
        void __fastcall padMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall padMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall SterowanieTimerTimer(TObject *Sender);
        void __fastcall Informacje1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);


   DCB dcb1;
   DWORD dwErrors;
   COMSTAT comStat;
   HANDLE handlePort_;

   byte mem[64000]; //pamiec obrazu
   int TimeOut, AutoTimeOut;
   int lewo, prawo, przod, tyl;

   DWORD RS_ile;      //ilosc bitow wyslanych

void GetFrame(char komenda, int MaxCounter)
{
char data;
int counter=0, err=0, cleared=0;
PurgeComm(handlePort_, PURGE_RXCLEAR);
ClearCommError(handlePort_, &dwErrors, &comStat);

//wyslanie prosby o pierwszy blok danych (10 linii)
data=komenda;
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
data=counter;
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);

while (counter < MaxCounter && err < 10)
{
 //ustawienie TimeOutu oczekiwania (milisekundy)
 TimeOut=0;
 if(counter==0) { TimeOutTimer->Interval=1800; }
  else{ TimeOutTimer->Interval=400; }
 TimeOutTimer->Enabled=true;

 //oczekiwanie na blok danych
 while (comStat.cbInQue < 3200 && TimeOut==0)
 {
  ClearCommError(handlePort_, &dwErrors, &comStat);
  RXBuffer->Caption="RXBuffer: " + IntToStr(comStat.cbInQue);
  RXPasek->Progress=(comStat.cbInQue / 3200.0)*100;
  Application->ProcessMessages();
 }

 //zresetowanie timeoutu
 TimeOutTimer->Enabled=false;
 TimeOut=0;

 //jesli odebrano poprawna ilosc danych
 if (comStat.cbInQue == 3200)
 {
  //wysylamy prosbe o kolejny blok
   data=komenda;
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
   data=counter+1;
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);

  //odbieramy poprzednie dane z bufora
   ReadFile(handlePort_, &mem[3200*counter], 3200, &RS_ile, NULL);
   counter++;
 }
 else {
       //gdy blad - czyscimy zle dane
       err++;

       TimeOutTimer->Interval=1000;
       TimeOutTimer->Enabled=true;

       ClearCommError(handlePort_, &dwErrors, &comStat);
        //if(comStat.cbInQue > 3200)
        //{
         while (comStat.cbInQue > 0 || TimeOut==0)
         {
          if(comStat.cbInQue > 0)
          {
           PurgeComm(handlePort_, PURGE_RXCLEAR);
           TimeOutTimer->Enabled=false;
           TimeOutTimer->Interval=1000;
           TimeOutTimer->Enabled=true;
          }
          ClearCommError(handlePort_, &dwErrors, &comStat);
          RXBuffer->Caption="RXBuffer: " + IntToStr(comStat.cbInQue);
          RXPasek->Progress=(comStat.cbInQue / 3200.0)*100;
          Application->ProcessMessages();
         }
       //}
       TimeOutTimer->Enabled=false;
       TimeOut=0;

       //wysylamy ponowna prosbe o ten sam blok
       data=komenda;
       WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
       data=counter;
       WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
      }

  //statusy
  PasekPostepu->Progress=((float)counter/MaxCounter)*100;
  DownloadStatus->Caption="Odbieram pakiet nr: " + IntToStr(counter+1) + " z " + IntToStr(MaxCounter);
  DownloadErrors->Caption="B³êdy: " + IntToStr(err);
  ClearCommError(handlePort_, &dwErrors, &comStat);
  RXBuffer->Caption="RXBuffer: " + IntToStr(comStat.cbInQue);
  RXPasek->Progress=(comStat.cbInQue / 3200.0)*100;
  Application->ProcessMessages();

 //jesli nie odebrano poprawnie danych zostanie wyslana ponowna prosba
 //o przeslanie tego samego bloku
}

 if(err<50){ DownloadStatus->Caption="Transfer zakonczony sukcesem"; }
  else {DownloadStatus->Caption="B³¹d - zbyt du¿e zaklócenia transmisji";}
 DownloadErrors->Caption="B³êdy: " + IntToStr(err);
}

int GetPx2bit(int wsk)
{
  int pos_bajt, pos_bit;
  pos_bajt = wsk>>2;
  pos_bit = wsk%4;

  return ( mem[pos_bajt] >> (6-pos_bit-pos_bit) ) & 0x03;
}

void SilnikiKlawiatura()
{
  if(przod==0 && lewo==0 && tyl==0 && prawo==0)
  { LewySilnik->Position=0; PrawySilnik->Position=0; }
  if(przod==1 && lewo==0 && tyl==0 && prawo==0)
  { LewySilnik->Position=-255; PrawySilnik->Position=-255; }
  if(przod==0 && lewo==1 && tyl==0 && prawo==0)
  { LewySilnik->Position=255; PrawySilnik->Position=-255; }
  if(przod==0 && lewo==0 && tyl==1 && prawo==0)
  { LewySilnik->Position=255; PrawySilnik->Position=255; }
  if(przod==0 && lewo==0 && tyl==0 && prawo==1)
  { LewySilnik->Position=-255; PrawySilnik->Position=255; }
  if(przod==1 && lewo==1 && tyl==0 && prawo==0)
  { LewySilnik->Position=0; PrawySilnik->Position=-255; }
  if(przod==1 && lewo==0 && tyl==0 && prawo==1)
  { LewySilnik->Position=-255; PrawySilnik->Position=0; }
  if(przod==0 && lewo==0 && tyl==1 && prawo==1)
  { LewySilnik->Position=0; PrawySilnik->Position=255; }
  if(przod==0 && lewo==1 && tyl==1 && prawo==0)
  { LewySilnik->Position=255; PrawySilnik->Position=0; }
}

void AllControlsEnable()
{
 Preview1->Enabled=true;
 Preview2->Enabled=true;
 Preview3->Enabled=true;
 AutoPreviewCheck1->Enabled=true;
 AutoPreviewCheck2->Enabled=true;
 AutoPreviewCheck3->Enabled=true;
 AutoPrevievDisable->Enabled=false;
 AutoWlacz->Enabled=true;
 AutoWylacz->Enabled=false;
 AutoPreviewOnButton->Enabled=false;
 AutoPreviewOffButton->Enabled=false;
 PojedynczyPodglad->Enabled=true;
 NapiecieCheck->Enabled=true;
 DiodaWlacz->Enabled=true;
 DiodaWylacz->Enabled=true;
 SerwoOn->Enabled=true;
 SerwoOff->Enabled=true;
 StopButton->Enabled=true;
 SerwoKat->Enabled=true;
 LewySilnik->Enabled=true;
 PrawySilnik->Enabled=true;
 pad->Enabled=true;
 Obraz->Enabled=true;
}

void AllControlsDisable()
{
 Preview1->Enabled=false;
 Preview2->Enabled=false;
 Preview3->Enabled=false;
 AutoPreviewCheck1->Enabled=false;
 AutoPreviewCheck2->Enabled=false;
 AutoPreviewCheck3->Enabled=false;
 AutoPrevievDisable->Enabled=false;
 AutoWlacz->Enabled=false;
 AutoWylacz->Enabled=false;
 AutoPreviewOnButton->Enabled=false;
 AutoPreviewOffButton->Enabled=false;
 PojedynczyPodglad->Enabled=false;
 NapiecieCheck->Enabled=false;
 DiodaWlacz->Enabled=false;
 DiodaWylacz->Enabled=false;
 SerwoOn->Enabled=false;
 SerwoOff->Enabled=false;
 StopButton->Enabled=false;
 SerwoKat->Enabled=false;
 LewySilnik->Enabled=false;
 PrawySilnik->Enabled=false;
 pad->Enabled=false;
 Obraz->Enabled=false;
}

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
