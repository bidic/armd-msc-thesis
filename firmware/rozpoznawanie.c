/**
 * Projekt    : Dark Explorer firmware
 * Plik       : rozpoznawanie.c
 * Zawartosc  : Analiza i rozpoznawanie obrazu
 * Autor      : Pawe� Kmak
 * Data       : 1.11.2009
 **/

//#include <peripherals/pio_helper.h>
#include <pio/pio.h>

#include <de_board.h>
#include <rozpoznawanie.h>
#include <utils.h>
#include <peripherals.h>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////
// Globalne zmienne zewnetrzne
////////////////////////////////////////////////////////////////////////////////
extern float wzorzec1, wzorzec2, wzorzec3, wzorzec4, wzorzec5, wzorzec6,
		wzorzec7, wzorzec8, wzorzec9;
extern int xwzorzec, ywzorzec, WzorzecCount, RozpoznanyX, RozpoznanyY;
extern volatile int SerwoAngleBuffer;
extern char SilnikiEnable;
extern char AutoPreview;
extern char mem[];
extern volatile int xmin, xmax, ymin, ymax;

extern Pin pin_led_power;

////////////////////////////////////////////////////////////////////////////////
// Analiza i rozpoznawanie
////////////////////////////////////////////////////////////////////////////////
inline int Rozpoznaj() {
	register long int wsk;
	register int x, y, i, j;
	long int srednia, ObszarCounter, pole, mx, my;
	char srednia_255, ZnalezionoObszar;
	float odleglosc, tmp, obwod, rzut1, rzut2, rzut3, rzut4, roznica;
	float wspolczynnik1, wspolczynnik2, wspolczynnik3;

	roznica = 100000;

	//wlaczenie diody
	//  AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, LED_POWER );
	PIO_Clear(&pin_led_power);

	//odebranie ramki 320x200
	GetFrame(2, 2, 0);

	//wylaczenie diody
//	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, LED_POWER);
	PIO_Set(&pin_led_power);

	//zbadanie sredniej jasnosci
	srednia = 0;
	for (wsk = 0; wsk < 64000; ++wsk) {
		srednia += mem[wsk];
	}
	srednia = srednia / 64000;
	srednia_255 = srednia;

	//progowanie
	for (wsk = 0; wsk < 64000; ++wsk) {
		if (mem[wsk] > srednia_255) {
			mem[wsk] = 255;
		} else {
			mem[wsk] = 0;
		}
	}

	//zapis w postaci 2-bitowej
	for (wsk = 0; wsk < 64000; ++wsk) {
		if (mem[wsk] > 0) {
			SetPx2bit(wsk, 1);
		} else {
			SetPx2bit(wsk, 0);
		}
	}

	//skopiowanie podgladu na koniec tablicy mem (autopodgl�d)
	if (AutoPreview) {
		for (wsk = 0; wsk < 16000; ++wsk) {
			mem[wsk + 48000] = mem[wsk];
		}
		//wyslanie info ze gotowe (synchronizacja)
		mem[47992] = 'p';
		mem[47993] = 1;
		UART0_DMA_Write(&mem[47992], 8);//wypelniacz - dane bez sensu
	}

	x = 0;
	y = 0;
	ObszarCounter = 0;
	ZnalezionoObszar = 0;
	while (y < 200) {
		//zamalowywanie obszaru zawierajacego piksel (x, y)
		if (GetPx2bit(ToWsk(x, y)) == 0) {
			//sprawdzenie czy obszar ma wiecej jak 100 pikseli
			//jesli tak - zamaina wartosci pikseli obszaru na 2
			if (SmithFill(x, y, 0, 3) > 100) {
				SmithFill(x, y, 3, 2);
				ZnalezionoObszar = 1;
				++ObszarCounter;
			} else {
				SmithFill(x, y, 3, 1);
			}
		}

		//Dla kazdego znalezionego obszaru sp�jnego analiza i rozpoznawanie
		if (ZnalezionoObszar) {
			//wyliczenie pola obszaru
			//oraz momentow 1szego rzedu (srodek ciezkosci)
			pole = 0;
			mx = 0;
			my = 0;
			for (i = xmin; i <= xmax; ++i) {
				for (j = ymin; j <= ymax; ++j) {
					if (GetPx2bit(ToWsk(i, j)) == 2) {
						++pole;
						mx = mx + i;
						my = my + j;
					}
				}
			}

			mx = mx / pole;
			my = my / pole;

			//suma odleglosci od srodka ciezkosci
			//dla Blaira Blissa
			odleglosc = 0;
			for (i = xmin; i <= xmax; ++i) {
				for (j = ymin; j <= ymax; ++j) {
					if (GetPx2bit(ToWsk(i, j)) == 2) {
						tmp = sqrt(((i - mx) * (i - mx))
								+ ((j - my) * (j - my)));
						odleglosc = odleglosc + (tmp * tmp);
					}
				}
			}

			odleglosc = sqrt(odleglosc * 6.28);
			wspolczynnik1 = 0;
			if (odleglosc != 0) {
				wspolczynnik1 = ((float) pole / odleglosc) * 2.0;
			}

			//obwod figury
			rzut1 = 0, rzut2 = 0, rzut3 = 0, rzut4 = 0;
			if (xmin > 1 && ymin > 1 && xmax < 318 && ymax < 198) {
				for (i = xmin - 1; i < xmax + 1; ++i) {
					for (j = ymin - 1; j < ymax + 1; ++j) {
						//dlugosc rzutu 0stopni
						if (GetPx2bit(ToWsk(i, j)) == 2 && GetPx2bit(ToWsk(i
								+ 1, j)) != 2) {
							++rzut1;
						}

						//dlugosc rzutu 45stopni
						if (GetPx2bit(ToWsk(i, j)) == 2 && GetPx2bit(ToWsk(i
								+ 1, j - 1)) != 2) {
							++rzut2;
						}

						//dlugosc rzutu 90stopni
						if (GetPx2bit(ToWsk(i, j)) == 2 && GetPx2bit(ToWsk(i, j
								- 1)) != 2) {
							++rzut3;
						}

						//dlugosc rzutu 135stopni
						if (GetPx2bit(ToWsk(i, j)) == 2 && GetPx2bit(ToWsk(i
								- 1, j - 1)) != 2) {
							++rzut4;
						}
					}
				}
			}

			obwod = 0.785 * ((rzut1 + rzut3) + (1.414 * (rzut2 + rzut4)));
			//obwod = 0.785 * ( (rzut1 + rzut3) + (0.707*(rzut2 + rzut4)) );
			wspolczynnik2 = (obwod * obwod) / (float) (12.56 * pole);
			wspolczynnik3 = 1;

			//jesli brak wzorca, to przypisac
			if (xwzorzec > 0 && ywzorzec > 0 && GetPx2bit(ToWsk(xwzorzec,
					ywzorzec)) == 2) {
				if (WzorzecCount == 0) {
					wzorzec1 = wspolczynnik1;
					wzorzec2 = wspolczynnik2;
					wzorzec3 = wspolczynnik3;
				}

				if (WzorzecCount == 1) {
					wzorzec4 = wspolczynnik1;
					wzorzec5 = wspolczynnik2;
					wzorzec6 = wspolczynnik3;
				}

				if (WzorzecCount == 2) {
					wzorzec7 = wspolczynnik1;
					wzorzec8 = wspolczynnik2;
					wzorzec9 = wspolczynnik3;
				}
				xwzorzec = 0;
				ywzorzec = 0;
				++WzorzecCount;

				if (WzorzecCount == 3) {
					WzorzecCount = 0;
				}
			}

			//sprawdzanie podobienstwa obiektu ze wzorcem (jesli jest wzorzec)
			if (wzorzec1 > 0 && wzorzec2 > 0 && wzorzec3 > 0) {
				tmp = sqrt(((wspolczynnik1 - wzorzec1) * (wspolczynnik1
						- wzorzec1)) + ((wspolczynnik2 - wzorzec2)
						* (wspolczynnik2 - wzorzec2)) + ((wspolczynnik3
						- wzorzec3) * (wspolczynnik3 - wzorzec3)));
				if (tmp < roznica) {
					roznica = tmp;
					RozpoznanyX = mx;
					RozpoznanyY = my;
				}
			}

			if (wzorzec4 > 0 && wzorzec5 > 0 && wzorzec6 > 0) {
				tmp = sqrt(((wspolczynnik1 - wzorzec4) * (wspolczynnik1
						- wzorzec4)) + ((wspolczynnik2 - wzorzec5)
						* (wspolczynnik2 - wzorzec5)) + ((wspolczynnik3
						- wzorzec6) * (wspolczynnik3 - wzorzec6)));
				if (tmp < roznica) {
					roznica = tmp;
					RozpoznanyX = mx;
					RozpoznanyY = my;
				}
			}

			if (wzorzec7 > 0 && wzorzec8 > 0 && wzorzec9 > 0) {
				tmp = sqrt(((wspolczynnik1 - wzorzec7) * (wspolczynnik1
						- wzorzec7)) + ((wspolczynnik2 - wzorzec8)
						* (wspolczynnik2 - wzorzec8)) + ((wspolczynnik3
						- wzorzec9) * (wspolczynnik3 - wzorzec9)));
				if (tmp < roznica) {
					roznica = tmp;
					RozpoznanyX = mx;
					RozpoznanyY = my;
				}
			}

			//zapamietanie wymiarow rozpoznanego obiektu w mem
			if (RozpoznanyX == mx && RozpoznanyY == my) {
				mem[47994] = xmin >> 8;
				mem[47995] = xmin & 0xFF;
				mem[47996] = xmax >> 8;
				mem[47997] = xmax & 0xFF;
				;
				mem[47998] = ymin;
				mem[47999] = ymax;
			}

			//zmiana wartosci obszaru na 3
			for (i = xmin; i <= xmax; ++i) {
				for (j = ymin; j <= ymax; ++j) {
					if (GetPx2bit(ToWsk(i, j)) == 2) //gdy piksel nalezy do obszaru
					{
						SetPx2bit(ToWsk(i, j), 3);
					}
				}
			}
			ZnalezionoObszar = 0;
		} //end znaleziono obszar

		++x;
		if (x > 319) {
			x = 0;
			++y;
		}

	} // end while (y<200)

	//Zaznaczenie rozpoznanego obszaru na 2
	SetPx2bit(ToWsk(RozpoznanyX, RozpoznanyY), 2);
	SetPx2bit(ToWsk(RozpoznanyX, RozpoznanyY + 1), 2);
	SetPx2bit(ToWsk(RozpoznanyX, RozpoznanyY + 2), 2);
	SetPx2bit(ToWsk(RozpoznanyX, RozpoznanyY + 3), 2);
	SetPx2bit(ToWsk(RozpoznanyX + 1, RozpoznanyY), 2);
	SetPx2bit(ToWsk(RozpoznanyX + 1, RozpoznanyY + 1), 2);
	SetPx2bit(ToWsk(RozpoznanyX + 1, RozpoznanyY + 2), 2);
	SetPx2bit(ToWsk(RozpoznanyX + 1, RozpoznanyY + 3), 2);
	SetPx2bit(ToWsk(RozpoznanyX + 2, RozpoznanyY), 2);
	SetPx2bit(ToWsk(RozpoznanyX + 2, RozpoznanyY + 1), 2);
	SetPx2bit(ToWsk(RozpoznanyX + 2, RozpoznanyY + 2), 2);
	SetPx2bit(ToWsk(RozpoznanyX + 2, RozpoznanyY + 3), 2);
	SetPx2bit(ToWsk(RozpoznanyX + 3, RozpoznanyY), 2);
	SetPx2bit(ToWsk(RozpoznanyX + 3, RozpoznanyY + 1), 2);
	SetPx2bit(ToWsk(RozpoznanyX + 3, RozpoznanyY + 2), 2);
	SetPx2bit(ToWsk(RozpoznanyX + 3, RozpoznanyY + 3), 2);

	//wyslanie lokalizacji rozpoznanego obiektu
	if (AutoPreview) {
		mem[47992] = 'p';
		mem[47993] = 2;
		UART0_DMA_Write(&mem[47992], 8);
	}

	//sterowanie silnikami
	if (SilnikiEnable) {
		if (RozpoznanyX > 120 && RozpoznanyX < 200) {
			//obiekt na srodku - jazda na wprost
			//srodek = 320/2 = 160
			go(2, 215, 225);
		}

		if (RozpoznanyX >= 200) {
			//obiekt po lewej
			//ostry zakret w lewo przez odpowiedni czas
			go(1, 255, 255);
			waitms(250);
			//potem stop do nastepnej klatki
			go(0, 255, 255);
		}

		if (RozpoznanyX <= 120) {
			//obiekt po prawej
			//ostry zakret w prawo przez odpowiedni czas
			go(3, 255, 255);
			waitms(250);
			//potem stop do nastepnej klatki
			go(0, 255, 255);
		}
	}

	//sterowanie serwem
	if (RozpoznanyY > 120) {
		SerwoAngleBuffer = SerwoAngleBuffer + 10;
	}
	if (RozpoznanyY < 80) {
		SerwoAngleBuffer = SerwoAngleBuffer - 10;
	}

	return ObszarCounter;
}
