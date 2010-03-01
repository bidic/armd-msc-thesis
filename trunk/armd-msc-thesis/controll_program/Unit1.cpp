//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include <Windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <math.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CGAUGES"
#pragma resource "*.dfm"
TForm1 *Form1;

   int i=0, x=0, y=0, r, g, b;
   int AutoPreview=0, AutoPreview1=0, AutoPreview2=0, AutoPreview3=0;
   int LButtonDown=0, RButtonDown=0;
   int pwm_left, pwm_right, kierunek_left, kierunek_right, kat, kat255;

   Graphics::TBitmap *Bmp = new Graphics::TBitmap;  //obraz
   Graphics::TBitmap *rysunek = new Graphics::TBitmap;  //sterowanie
   float PI=3.14159265;
   int xwzorzec, ywzorzec;

 float fi_rad(float x, float y)
 {
  if(x>0 && y>=0) {return atan(y/x);}
  if(x>0 && y<0) {return atan(y/x) + (2.0*PI);}
  if(x<0) {return atan(y/x) + PI;}
  if(x==0 && y>0) {return PI/2.0;}
  if(x==0 && y<0) {return (3.0*PI)/2.0;}
 }

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
 Form1->DoubleBuffered = true;
 
 rysunek->Width = 300;
 rysunek->Height = 300;
 Bmp->Width = 640;
 Bmp->Height = 400;
 Bmp->PixelFormat=pf32bit;

 rysunek->Canvas->Brush->Color = clBlack;
 rysunek->Canvas->FillRect(Rect(0,0,300,300));
 rysunek->Canvas->Pen->Color=clRed;
 rysunek->Canvas->Ellipse(100,100,200,200);
 pad->Picture->Bitmap->Assign(rysunek);

 Form1->Color=RGB(224,223,227);

 lewo=prawo=przod=tyl=0;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
 //zamkniecie polaczenia jesli otwarte
 if(ComDisconnect->Enabled==true){ CloseHandle(handlePort_); }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::SerwoKatChange(TObject *Sender)
{
char data;
data='s';
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
data=255-SerwoKat->Position;
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
SerwoStatus->Caption="K¹t: "+IntToStr( (int)(((float)(255-SerwoKat->Position)/255)*145) );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PrawySilnikChange(TObject *Sender)
{
//r - silniki prawe kierunek
//b - silniki prawe pwm

  char data;

  if(PrawySilnik->Position >= -2 && PrawySilnik->Position <= 2)
  {
   //stop silniki
   data='r';
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
   data=0;
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
   PrawyStatus->Caption="STOP";
  }

  if(PrawySilnik->Position < -2)
  {
   //prawe silniki do przodu
   data='r';
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
   data=1;
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);

   data='b';
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
   data=(PrawySilnik->Position)*-1;
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
   PrawyStatus->Caption="/|\\ "+ IntToStr(PrawySilnik->Position*-1);
  }

  if(PrawySilnik->Position > 2)
  {
   //prawe silniki do tylu
   data='r';
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
   data=2;
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);

   data='b';
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
   data=(PrawySilnik->Position);
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
   PrawyStatus->Caption="\\|/ "+ IntToStr(PrawySilnik->Position);
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::LewySilnikChange(TObject *Sender)
{
//l - silniki lewe kierunek
//a - silniki lewe pwm

  char data;

  if(LewySilnik->Position >= -2 && LewySilnik->Position <= 2)
  {
   //stop silniki
   data='l';
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
   data=0;
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
   LewyStatus->Caption="STOP";

  }

  if(LewySilnik->Position < -2)
  {
   //lewe silniki do przodu
   data='l';
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
   data=1;
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);

   data='a';
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
   data=(LewySilnik->Position)*-1;
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
   LewyStatus->Caption="/|\\ "+ IntToStr(LewySilnik->Position*-1);
  }

  if(LewySilnik->Position > 2)
  {
   //lewe silniki do tylu
   data='l';
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
   data=2;
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);

   data='a';
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
   data=(LewySilnik->Position);
   WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
   LewyStatus->Caption="\\|/ "+ IntToStr(LewySilnik->Position);
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::StopButtonClick(TObject *Sender)
{
char data;
data='l';
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
data=0;
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
LewySilnik->Position=0;

data='r';
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
data=0;
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
PrawySilnik->Position=0;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::padMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
int x, y, promien;
float r, fi;
char data;
rysunek->Canvas->Brush->Color = clBlack;
rysunek->Canvas->FillRect(Rect(0,0,300,300));
rysunek->Canvas->Pen->Color=clRed;
rysunek->Canvas->Ellipse(100,100,200,200);

x = -1*(150 - X);
y = (150 - Y);
r = sqrt((x*x) + (y*y));
fi = fi_rad(x,y);
if (r>50) {r=50;}

//promien = r;
promien = 255.0 * (r/50.0);
kat=fi * (180.0 / PI);

if (!(X==150 && Y==150))
{
 rysunek->Canvas->MoveTo(150,150);
 rysunek->Canvas->LineTo( ((r*cos(fi))+150), -1*(r*sin(fi))+150);
}

if(kat>=0 && kat <= 45)
{
  kierunek_left = 1; //przod
  kierunek_right = 2; //tyl
  pwm_left = promien;
  pwm_right = promien - ( promien *  ((float)kat/45.0) );
}

if(kat>45 && kat <= 90)
{
  kierunek_left = 1; //przod
  kierunek_right = 1; //przod
  pwm_left = promien;
  pwm_right = promien *  ((float)(kat-45)/45.0);
}

if(kat>90 && kat <= 135)
{
  kierunek_left = 1; //przod
  kierunek_right = 1; //przod
  pwm_left = promien - ( promien *  ((float)(kat-90)/45.0) );
  pwm_right = promien;
}

if(kat>135 && kat <= 180)
{
  kierunek_left = 2; //tyl
  kierunek_right = 1; //przod
  pwm_left = promien *  ((float)(kat-135)/45.0);
  pwm_right = promien;
}

if(kat>180 && kat <= 225)
{
  kierunek_left = 2; //tyl
  kierunek_right = 1; //przod
  pwm_left = promien;
  pwm_right = promien - ( promien *  ((float)(kat-180)/45.0) );
}

if(kat>225 && kat <= 270)
{
  kierunek_left = 2; //tyl
  kierunek_right = 2; //tyl
  pwm_left = promien;
  pwm_right = promien *  ((float)(kat-225)/45.0);
}

if(kat>270 && kat <= 315)
{
  kierunek_left = 2; //tyl
  kierunek_right = 2; //tyl
  pwm_left = promien - ( promien *  ((float)(kat-270)/45.0) );
  pwm_right = promien;
}

if(kat>315 && kat <= 360)
{
  kierunek_left = 1; //przod
  kierunek_right = 2; //tyl
  pwm_left = promien *  ((float)(kat-315)/45.0);
  pwm_right = promien;
}

if (pwm_left<2) {kierunek_left=0; pwm_left=2;}
if (pwm_right<2) {kierunek_right=0; pwm_right=2;}

pad->Picture->Bitmap->Assign(rysunek);
}
//---------------------------------------------------------------------------



void __fastcall TForm1::NapiecieCheckClick(TObject *Sender)
{
 char data;
 byte val[2];
 int val_rx, energia;
 float vref=3.3, napiecie;

 // czyszczenie bufora
 ClearCommError(handlePort_, &dwErrors, &comStat);
 PurgeComm(handlePort_, PURGE_RXCLEAR);

 data='t';
 WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
 data=0;
 WriteFile(handlePort_, &data, 1, &RS_ile, NULL);

 ReadFile(handlePort_, &val[0], 2, &RS_ile, NULL);

 val_rx = (val[0]<<8) + val[1];
 napiecie = vref / 1023;
 napiecie = napiecie * val_rx;
 napiecie = napiecie * 3.0;

 //przyblizona pozostala energia w akumulatorach w %
 //napiecie max = 8,5v (4.25 na cele)
 //napiecie min = 6v (3.0 na cele)
 //max - min = 2.5v
 energia = ((napiecie-6.0)/2.5) * 100;
 EnergiaPasek->Progress=energia;

 WartoscNapiecia->Caption="Napiecie = " + FormatFloat("0.00", napiecie);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Preview3Click(TObject *Sender)
{
 Preview1->Enabled=false;
 Preview2->Enabled=false;
 Preview3->Enabled=false;
 AutoPreviewCheck1->Enabled=false;
 AutoPreviewCheck2->Enabled=false;
 AutoPreviewCheck3->Enabled=false;
 AutoWlacz->Enabled=false;
 PojedynczyPodglad->Enabled=false;
 NapiecieCheck->Enabled=false;
 if(AutoPreview3==1) { AutoPrevievDisable->Enabled=true; }
 
 do
 {
  //odebranie klatki 320x200 przez bluetootha (20 pakietow)
  GetFrame('f',20);

  int counter=0;
  int *linia = new int[400];
  int *linia2 = new int[400];

   for(y=199; y>=0; y--)
   {
    linia = (int*)Bmp->ScanLine[y*2];
    linia2 = (int*)Bmp->ScanLine[(y*2)+1];
      for(x=319; x>=0; x--)
      {
          linia[x*2] = RGB(mem[counter],mem[counter],mem[counter]);
          linia[(x*2)+1] = RGB(mem[counter],mem[counter],mem[counter]);
          linia2[x*2] = RGB(mem[counter],mem[counter],mem[counter]);
          linia2[(x*2)+1] = RGB(mem[counter],mem[counter],mem[counter]);
          counter++;
          Application->ProcessMessages();
      }
   }
   Obraz->Picture->Bitmap->Assign(Bmp);

  }
  while(AutoPreview3==1);

 // czyszczenie bufora
 ClearCommError(handlePort_, &dwErrors, &comStat);
 PurgeComm(handlePort_, PURGE_RXCLEAR);
 
 Preview1->Enabled=true;
 Preview2->Enabled=true;
 Preview3->Enabled=true;
 AutoPreviewCheck1->Enabled=true;
 AutoPreviewCheck2->Enabled=true;
 AutoPreviewCheck3->Enabled=true;
 AutoWlacz->Enabled=true;
 PojedynczyPodglad->Enabled=true;
 NapiecieCheck->Enabled=true;
 AutoPrevievDisable->Enabled=false;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::TimeOutTimerTimer(TObject *Sender)
{
 TimeOut=1;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Preview1Click(TObject *Sender)
{
 Preview1->Enabled=false;
 Preview2->Enabled=false;
 Preview3->Enabled=false;
 AutoPreviewCheck1->Enabled=false;
 AutoPreviewCheck2->Enabled=false;
 AutoPreviewCheck3->Enabled=false;
 AutoWlacz->Enabled=false;
 PojedynczyPodglad->Enabled=false;
 NapiecieCheck->Enabled=false;
 if(AutoPreview1==1) { AutoPrevievDisable->Enabled=true; }

 do
 {
  //odebranie klatki 160x100 przez bluetootha (5 pakietow)
  GetFrame('p',5);
  
  int counter=0;
  int *linia1 = new int[400];
  int *linia2 = new int[400];
  int *linia3 = new int[400];
  int *linia4 = new int[400];

   for(y=99; y>=0; y--)
   {
    linia1 = (int*)Bmp->ScanLine[y*4];
    linia2 = (int*)Bmp->ScanLine[(y*4)+1];
    linia3 = (int*)Bmp->ScanLine[(y*4)+2];
    linia4 = (int*)Bmp->ScanLine[(y*4)+3];
      for(x=159; x>=0; x--)
      {
          linia1[x*4] = RGB(mem[counter],mem[counter],mem[counter]);
          linia1[(x*4)+1] = RGB(mem[counter],mem[counter],mem[counter]);
          linia1[(x*4)+2] = RGB(mem[counter],mem[counter],mem[counter]);
          linia1[(x*4)+3] = RGB(mem[counter],mem[counter],mem[counter]);
          linia2[x*4] = RGB(mem[counter],mem[counter],mem[counter]);
          linia2[(x*4)+1] = RGB(mem[counter],mem[counter],mem[counter]);
          linia2[(x*4)+2] = RGB(mem[counter],mem[counter],mem[counter]);
          linia2[(x*4)+3] = RGB(mem[counter],mem[counter],mem[counter]);
          linia3[x*4] = RGB(mem[counter],mem[counter],mem[counter]);
          linia3[(x*4)+1] = RGB(mem[counter],mem[counter],mem[counter]);
          linia3[(x*4)+2] = RGB(mem[counter],mem[counter],mem[counter]);
          linia3[(x*4)+3] = RGB(mem[counter],mem[counter],mem[counter]);
          linia4[x*4] = RGB(mem[counter],mem[counter],mem[counter]);
          linia4[(x*4)+1] = RGB(mem[counter],mem[counter],mem[counter]);
          linia4[(x*4)+2] = RGB(mem[counter],mem[counter],mem[counter]);
          linia4[(x*4)+3] = RGB(mem[counter],mem[counter],mem[counter]);
          counter++;
          Application->ProcessMessages();
      }
   }
   Obraz->Picture->Bitmap->Assign(Bmp);


  }
  while(AutoPreview1==1);

 // czyszczenie bufora
 ClearCommError(handlePort_, &dwErrors, &comStat);
 PurgeComm(handlePort_, PURGE_RXCLEAR);
 
 Preview1->Enabled=true;
 Preview2->Enabled=true;
 Preview3->Enabled=true;
 AutoPreviewCheck1->Enabled=true;
 AutoPreviewCheck2->Enabled=true;
 AutoPreviewCheck3->Enabled=true;
 AutoWlacz->Enabled=true;
 PojedynczyPodglad->Enabled=true;
 NapiecieCheck->Enabled=true;
 AutoPrevievDisable->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Preview2Click(TObject *Sender)
{
 Preview1->Enabled=false;
 Preview2->Enabled=false;
 Preview3->Enabled=false;
 AutoPreviewCheck1->Enabled=false;
 AutoPreviewCheck2->Enabled=false;
 AutoPreviewCheck3->Enabled=false;
 AutoWlacz->Enabled=false;
 PojedynczyPodglad->Enabled=false;
 NapiecieCheck->Enabled=false;
 if(AutoPreview2==1) { AutoPrevievDisable->Enabled=true; }
 
 do
 {
  //odebranie klatki 160x100 przez bluetootha (10 pakietow)
  GetFrame('x',10);

  int counter=0, cb, cr, Y, r, g, b, cbcrchange;
  int *linia1 = new int[400];
  int *linia2 = new int[400];
  int *linia3 = new int[400];
  int *linia4 = new int[400];

   for(y=99; y>=0; y--)
   {
    linia1 = (int*)Bmp->ScanLine[y*4];
    linia2 = (int*)Bmp->ScanLine[(y*4)+1];
    linia3 = (int*)Bmp->ScanLine[(y*4)+2];
    linia4 = (int*)Bmp->ScanLine[(y*4)+3];
      for(x=159; x>=0; x--)
      {
          if(cbcrchange==1) {cbcrchange=0;}
            else {cbcrchange=1;}

          if(cbcrchange==1){cb = (mem[counter]/1.15)+15;}
           else {cr =  (mem[counter]/1.15)+15;}

          Y = (mem[counter+1]/1.15)+15;

          r=((1.403*(float)cr)-179.5+(float)Y);
	  g=((-0.3443*(float)cb)+135.5+(float)Y-(0.7144*(float)cr));
	  b=(-227.0+(float)Y+(1.773*(float)cb));
          
          linia1[x*4] = RGB(b,g,r);
          linia1[(x*4)+1] = RGB(b,g,r);
          linia1[(x*4)+2] = RGB(b,g,r);
          linia1[(x*4)+3] = RGB(b,g,r);
          linia2[x*4] = RGB(b,g,r);
          linia2[(x*4)+1] = RGB(b,g,r);
          linia2[(x*4)+2] = RGB(b,g,r);
          linia2[(x*4)+3] = RGB(b,g,r);
          linia3[x*4] = RGB(b,g,r);
          linia3[(x*4)+1] = RGB(b,g,r);
          linia3[(x*4)+2] = RGB(b,g,r);
          linia3[(x*4)+3] = RGB(b,g,r);
          linia4[x*4] = RGB(b,g,r);
          linia4[(x*4)+1] = RGB(b,g,r);
          linia4[(x*4)+2] = RGB(b,g,r);
          linia4[(x*4)+3] = RGB(b,g,r);
          counter=counter+2;
          Application->ProcessMessages();
      }
   }
   Obraz->Picture->Bitmap->Assign(Bmp);

  }
  while(AutoPreview2==1);

 // czyszczenie bufora
 ClearCommError(handlePort_, &dwErrors, &comStat);
 PurgeComm(handlePort_, PURGE_RXCLEAR);\
 
 Preview1->Enabled=true;
 Preview2->Enabled=true;
 Preview3->Enabled=true;
 AutoPreviewCheck1->Enabled=true;
 AutoPreviewCheck2->Enabled=true;
 AutoPreviewCheck3->Enabled=true;
 AutoWlacz->Enabled=true;
 PojedynczyPodglad->Enabled=true;
 NapiecieCheck->Enabled=true;
 AutoPrevievDisable->Enabled=false;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::AutoPreviewOnButtonClick(TObject *Sender)
{
 AutoPreviewOnButton->Enabled=false;
 AutoPreviewOffButton->Enabled=true;
 PojedynczyPodglad->Enabled=false;
 NapiecieCheck->Enabled=false;

 int r,g,b;
 int counter=0;
 int *linia = new int[400];
 int *linia2 = new int[400];
 int xmin, xmax, ymin, ymax;
 char data;

 AutoPreview=1;

 AutoTimeOut=0;
 AutoTimeOutTimer->Interval=5000;
 AutoTimeOutTimer->Enabled=true;

 //uruchomienie wysylania informacji synchronizacyjnych
 //po stronie robota
 data='i';
 WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
 data=12;
 WriteFile(handlePort_, &data, 1, &RS_ile, NULL);

 while(AutoPreview==1 && AutoTimeOut==0)
 {
   Application->ProcessMessages();
   ClearCommError(handlePort_, &dwErrors, &comStat);
   if (comStat.cbInQue >= 8)
   {
    ReadFile(handlePort_, &mem[60000], 8, &RS_ile, NULL);
    PurgeComm(handlePort_, PURGE_RXCLEAR);
    if(mem[60000]=='p' && mem[60001]==2)
    {
     xmin=mem[60002]; xmin = xmin<<8; xmin+=mem[60003];
     xmax=mem[60004]; xmax = xmax<<8; xmax+=mem[60005];
     ymin=mem[60006];
     ymax=mem[60007];

     xmin = (319-xmin)*2;
     xmax = (319-xmax)*2;
     ymin = (199-ymin)*2;
     ymax = (199-ymax)*2;

     //rysowanie zaznaczenia rozpoznanego obiektu
     Bmp->Canvas->Brush->Color = clRed;
     Bmp->Canvas->FrameRect(Rect(xmax, ymax, xmin, ymin));
     Bmp->Canvas->FrameRect(Rect(xmax+1, ymax+1, xmin-1, ymin-1));
     Bmp->Canvas->FrameRect(Rect(xmax+2, ymax+2, xmin-2, ymin-2));
     Bmp->Canvas->FrameRect(Rect(xmax+3, ymax+3, xmin-3, ymin-3));
     Obraz->Picture->Bitmap->Assign(Bmp);
     //Label6->Caption="  "+ IntToStr(xmax)+"  "+ IntToStr(ymax)+"  "+ IntToStr(xmin)+"  "+ IntToStr(ymin);
    }
    if(mem[60000]=='p' && mem[60001]==1)
    {
     //zresetowanie timera
     AutoTimeOut=0;
     AutoTimeOutTimer->Enabled=false;
     AutoTimeOutTimer->Interval=5000;
     AutoTimeOutTimer->Enabled=true;

     //odbieranie podgladu
     GetFrame('i',5);
     
     counter=0;
     for(y=199; y>=0; y--)
     {
        linia = (int*)Bmp->ScanLine[y*2];
        linia2 = (int*)Bmp->ScanLine[(y*2)+1];

        for(x=319; x>=0; x--)
        {
           r=g=b=150;
           if (GetPx2bit(counter)==0) {r=g=b=0;}
           if (GetPx2bit(counter)==1) {r=g=b=255;}
           if (GetPx2bit(counter)==2) {r=g=b=150;}
           if (GetPx2bit(counter)==3) {r=200; g=b=0;}

           linia[x*2] = RGB(b,g,r);
           linia[(x*2)+1] = RGB(b,g,r);
           linia2[x*2] = RGB(b,g,r);
           linia2[(x*2)+1] = RGB(b,g,r);
           counter++;
           Application->ProcessMessages();
        }
     }
     Obraz->Picture->Bitmap->Assign(Bmp);

    }// end odbieranie podgladu
   }// end if (comStat.cbInQue >= 8)
 }// end while

 //wylaczenie wysylania informacji synchronizacyjnych
 //po stronie robota
 data='i';
 WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
 data=13;
 WriteFile(handlePort_, &data, 1, &RS_ile, NULL);

 // czyszczenie bufora
 ClearCommError(handlePort_, &dwErrors, &comStat);
 PurgeComm(handlePort_, PURGE_RXCLEAR);

 if( AutoWlacz->Enabled==false) { AutoPreviewOnButton->Enabled=true; }
  else { AutoPreviewOnButton->Enabled=false; }
 AutoPreviewOffButton->Enabled=false;
 PojedynczyPodglad->Enabled=true;
 NapiecieCheck->Enabled=true;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::AutoWlaczClick(TObject *Sender)
{
 char data;

 Preview1->Enabled=false;
 Preview2->Enabled=false;
 Preview3->Enabled=false;
 AutoPreviewCheck1->Enabled=false;
 AutoPreviewCheck2->Enabled=false;
 AutoPreviewCheck3->Enabled=false;
 AutoWlacz->Enabled=false;
 AutoWylacz->Enabled=true;
 AutoPreviewOnButton->Enabled=true;
 AutoPreviewOffButton->Enabled=false;

 data='i';
 WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
 data=10;
 WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AutoWylaczClick(TObject *Sender)
{
 char data;

 Preview1->Enabled=true;
 Preview2->Enabled=true;
 Preview3->Enabled=true;
 AutoPreviewCheck1->Enabled=true;
 AutoPreviewCheck2->Enabled=true;
 AutoPreviewCheck3->Enabled=true;
 AutoWlacz->Enabled=true;
 AutoWylacz->Enabled=false;
 AutoPreviewOnButton->Enabled=false;
 AutoPreviewOffButton->Enabled=false;

 data='i';
 WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
 data=11;
 WriteFile(handlePort_, &data, 1, &RS_ile, NULL);

 AutoPreviewOffButton->Click();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SerwoOnClick(TObject *Sender)
{
char data;
data='w';
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
data=1;
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SerwoOffClick(TObject *Sender)
{
char data;
data='w';
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
data=0;
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DiodaWlaczClick(TObject *Sender)
{
char data;
data='d';
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
data=1;
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DiodaWylaczClick(TObject *Sender)
{
char data;
data='d';
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
data=0;
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ObrazMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
xwzorzec=319-(X/2);
ywzorzec=199-(Y/2);
}
//---------------------------------------------------------------------------



void __fastcall TForm1::ObrazClick(TObject *Sender)
{
char data;

data='m';
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
data=(xwzorzec/1.3);
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);

data='n';
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
data=ywzorzec;
WriteFile(handlePort_, &data, 1, &RS_ile, NULL);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ComConnectClick(TObject *Sender)
{
 char port[15][5]={
 {"COM1"},{"COM2"},{"COM3"},{"COM4"},{"COM5"},{"COM6"},{"COM7"},{"COM8"},{"COM9"},
 {"COM10"},{"COM11"},{"COM12"},{"COM13"},{"COM14"},{"COM15"} };
 handlePort_ = CreateFile(port[WyborPortu->ItemIndex],  // Specify port device: default "COM1"
 GENERIC_READ | GENERIC_WRITE,       // Specify mode that open device.
 0,                                  // the devide isn't shared.
 NULL,                               // the object gets a default security.
 OPEN_EXISTING,                      // Specify which action to take on file.
 FILE_ATTRIBUTE_NORMAL,              // default.
 NULL);                              // default.

 // Assign user parameter.
 //dcb1.BaudRate = CBR_115200;  // Specify buad rate of communicaiton.
 //dcb1.BaudRate = 230400;  // Specify buad rate of communicaiton.
 dcb1.BaudRate = 460800;  // Specify buad rate of communicaiton.
 dcb1.StopBits = ONESTOPBIT;  // Specify stopbit of communication.
 dcb1.Parity = NOPARITY;      // Specify parity of communication.
 dcb1.ByteSize = 8;  // Specify  byte of size of communication.

 if ( SetCommState(handlePort_,&dcb1) == 0)
 {
  status->Caption="Status: blad inicjalizacji portu COM" + IntToStr(WyborPortu->ItemIndex+1);
 }  else {
           status->Caption="Status: Po³¹czono via COM" + IntToStr(WyborPortu->ItemIndex+1);
           ComConnect->Enabled=false;
           ComDisconnect->Enabled=true;
           WyborPortu->Enabled=false;
           AllControlsEnable();
         }

    // instance an object of COMMTIMEOUTS.
    COMMTIMEOUTS comTimeOut;
    // Specify time-out between charactor for receiving.
    comTimeOut.ReadIntervalTimeout = 200;
    // Specify value that is multiplied
    // by the requested number of bytes to be read.
    comTimeOut.ReadTotalTimeoutMultiplier = 100;
    // Specify value is added to the product of the
    // ReadTotalTimeoutMultiplier member
    comTimeOut.ReadTotalTimeoutConstant = 200;
    // Specify value that is multiplied
    // by the requested number of bytes to be sent.
    comTimeOut.WriteTotalTimeoutMultiplier = 300;
    // Specify value is added to the product of the
    // WriteTotalTimeoutMultiplier member
    comTimeOut.WriteTotalTimeoutConstant = 200;
    // set the time-out parameter into device control.
    SetCommTimeouts(handlePort_,&comTimeOut);

    ClearCommError(handlePort_, &dwErrors, &comStat);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComDisconnectClick(TObject *Sender)
{
 ClearCommError(handlePort_, &dwErrors, &comStat);
 CloseHandle(handlePort_);
 status->Caption="Status: Roz³¹czono - COM" + IntToStr(WyborPortu->ItemIndex+1);
 ComConnect->Enabled=true;
 ComDisconnect->Enabled=false;
 WyborPortu->Enabled=true;
 AllControlsDisable();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::KlawiaturaKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if(Key=='W'){ przod=1; }
 if(Key=='A'){ lewo=1; }
 if(Key=='S'){ tyl=1; }
 if(Key=='D'){ prawo=1; }

 SilnikiKlawiatura();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::KlawiaturaKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if(Key=='W'){ przod=0; }
 if(Key=='A'){ lewo=0; }
 if(Key=='S'){ tyl=0; }
 if(Key=='D'){ prawo=0; }

 SilnikiKlawiatura();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AutoTimeOutTimerTimer(TObject *Sender)
{
 AutoTimeOut=1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AutoPreviewOffButtonClick(TObject *Sender)
{
 AutoPreview=0;
 AutoTimeOutTimer->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Wylacz1Click(TObject *Sender)
{
 exit(0);        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AutoPreviewCheck1Click(TObject *Sender)
{
 if (AutoPreviewCheck1->Checked==true) { AutoPreview1=1; }
  else { AutoPreview1=0; }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AutoPreviewCheck2Click(TObject *Sender)
{
 if (AutoPreviewCheck2->Checked==true) { AutoPreview2=1; }
  else { AutoPreview2=0; }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AutoPreviewCheck3Click(TObject *Sender)
{
 if (AutoPreviewCheck3->Checked==true) { AutoPreview3=1; }
  else { AutoPreview3=0; }        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AutoPrevievDisableClick(TObject *Sender)
{
 AutoPreview1=0;
 AutoPreviewCheck1->Checked=false;
 AutoPreview2=0;
 AutoPreviewCheck2->Checked=false;
 AutoPreview3=0;
 AutoPreviewCheck3->Checked=false;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::PojedynczyPodgladClick(TObject *Sender)
{
 Preview1->Enabled=false;
 Preview2->Enabled=false;
 Preview3->Enabled=false;
 AutoPreviewCheck1->Enabled=false;
 AutoPreviewCheck2->Enabled=false;
 AutoPreviewCheck3->Enabled=false;
 PojedynczyPodglad->Enabled=false;
 NapiecieCheck->Enabled=false;
 if(AutoWylacz->Enabled==true) { AutoPreviewOnButton->Enabled=false; }

 int r,g,b;
 int counter=0;
 int *linia = new int[400];
 int *linia2 = new int[400];

     //odbieranie podgladu
     GetFrame('j',5);
     
     counter=0;
     for(y=199; y>=0; y--)
     {
        linia = (int*)Bmp->ScanLine[y*2];
        linia2 = (int*)Bmp->ScanLine[(y*2)+1];

        for(x=319; x>=0; x--)
        {
           r=g=b=150;
           if (GetPx2bit(counter)==0) {r=g=b=0;}
           if (GetPx2bit(counter)==1) {r=g=b=255;}
           if (GetPx2bit(counter)==2) {r=g=b=150;}
           if (GetPx2bit(counter)==3) {r=200; g=b=0;}

           linia[x*2] = RGB(b,g,r);
           linia[(x*2)+1] = RGB(b,g,r);
           linia2[x*2] = RGB(b,g,r);
           linia2[(x*2)+1] = RGB(b,g,r);
           counter++;
           Application->ProcessMessages();
        }
     }
     Obraz->Picture->Bitmap->Assign(Bmp);

 ClearCommError(handlePort_, &dwErrors, &comStat);
 PurgeComm(handlePort_, PURGE_RXCLEAR);

 if(AutoWlacz->Enabled==true)
 {
 Preview1->Enabled=true;
 Preview2->Enabled=true;
 Preview3->Enabled=true;
 AutoPreviewCheck1->Enabled=true;
 AutoPreviewCheck2->Enabled=true;
 AutoPreviewCheck3->Enabled=true;
 NapiecieCheck->Enabled=true;
 }
 else { AutoPreviewOnButton->Enabled=true; }
 PojedynczyPodglad->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::padMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 if (Button==0){ LButtonDown=1; SterowanieTimer->Enabled=true;}
 if (Button==1){ RButtonDown=1; SterowanieTimer->Enabled=true;}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::padMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 if (Button==0)
 {
  LButtonDown=0;
  if( Preview1->Enabled==true ){ Preview1->Click(); }
 }
 if (Button==1){ RButtonDown=0; }       
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SterowanieTimerTimer(TObject *Sender)
{
 if(LButtonDown==1)
 {
  //silniki lewe
  if(kierunek_left==0){ LewySilnik->Position=0; }
  if(kierunek_left==1){ LewySilnik->Position=pwm_left*-1; }
  if(kierunek_left==2){ LewySilnik->Position=pwm_left; }

  //silniki prawe
  if(kierunek_right==0){ PrawySilnik->Position=0; }
  if(kierunek_right==1){ PrawySilnik->Position=pwm_right*-1; }
  if(kierunek_right==2){ PrawySilnik->Position=pwm_right; }
 }
 else {
       //silniki lewe
       LewySilnik->Position=0;

       //silniki prawe
       PrawySilnik->Position=0;
      }

 if (RButtonDown==1)
 {
  kat255 = kat;
  if(kat255>145 && kat255<=270) { kat255=145; }
  if(kat255>270) { kat255=0; }
  kat255 = ( ((float)kat255)/145.0 ) * 255;
  SerwoKat->Position=255-kat255;
 }

 if(LButtonDown==0 && RButtonDown==0)
 {
  //autowylaczenie
  SterowanieTimer->Enabled=false;
 }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Informacje1Click(TObject *Sender)
{
Form2->ShowModal();
}
//---------------------------------------------------------------------------



