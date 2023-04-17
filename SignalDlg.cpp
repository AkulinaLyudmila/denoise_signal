
// SignalDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "Signal.h"
#include "SignalDlg.h"
#include "afxdialogex.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>



#define KOORD(x,y) (xp*((x)-xmin)),(yp*((y)-ymax)) // макрос перевода координат
#define KOORDSH(x,y) (xpsh*((x)-xminsh)),(ypsh*((y)-ymaxsh))
#define SPECTR(x,y) (xpsp*((x)-xminsp)),(ypsp*((y)-ymaxsp))

// диалоговое окно CSignalDlg

using namespace std;


CSignalDlg::CSignalDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSignalDlg::IDD, pParent)
	, d(10)
	, t(1024)
	, n(70)
	, Amplitude1(1)
	, Amplitude2(5)
	, Amplitude3(10)
	, Frequency1(0.01)
	, Frequency2(0.003)
	, Frequency3(0.007)
	, Phase1(2*M_PI)
	, Phase2(M_PI)
	, Phase3(0)
	, nevuzki(0)
	, Fd(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSignalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT7, d);
	DDX_Text(pDX, IDC_EDIT9, t);
	DDX_Text(pDX, IDC_EDIT5, n);
	DDX_Text(pDX, IDC_EDIT1, Amplitude1);
	DDX_Text(pDX, IDC_EDIT3, Amplitude2);
	DDX_Text(pDX, IDC_EDIT6, Amplitude3);
	DDX_Text(pDX, IDC_EDIT10, Frequency1);
	DDX_Text(pDX, IDC_EDIT8, Frequency2);
	DDX_Text(pDX, IDC_EDIT2, Frequency3);
	DDX_Text(pDX, IDC_EDIT12, Phase1);
	DDX_Text(pDX, IDC_EDIT11, Phase2);
	DDX_Text(pDX, IDC_EDIT4, Phase3);
	DDX_Text(pDX, IDC_EDIT13, nevuzki);
	DDX_Text(pDX, IDC_EDIT14, Fd);
}

BEGIN_MESSAGE_MAP(CSignalDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MakeANoise, &CSignalDlg::OnBnClickedMakeanoise)
	ON_BN_CLICKED(IDC_BUTTON1, &CSignalDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSignalDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSignalDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_SIGNAL3, &CSignalDlg::OnBnClickedSignal3)
	//ON_EN_CHANGE(IDC_EDIT13, &CSignalDlg::OnEnChangeEdit13)
END_MESSAGE_MAP()


// обработчики сообщений CSignalDlg

BOOL CSignalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок
	
	PicWndCleanSign = GetDlgItem(IDC_SIGNAL1);
	PicDcCleanSign = PicWndCleanSign->GetDC();
	PicWndCleanSign->GetClientRect(&PicCleanSign);

	PicWndNoise = GetDlgItem(IDC_SIGNAL2);
	PicDcNoise = PicWndNoise->GetDC();
	PicWndNoise->GetClientRect(&PicNoise);

	PicWndMakeANoiseSignal = GetDlgItem(IDC_SIGNAL3);
	PicDcMakeANoiseSignal = PicWndMakeANoiseSignal->GetDC();
	PicWndMakeANoiseSignal->GetClientRect(&PicMakeANoiseSignal);


	setka_pen.CreatePen(		//для сетки
		PS_DOT,					//пунктирная
		0.1,						//толщина 1 пиксель
		RGB(0, 0, 250));		//цвет  черный
	osi_pen.CreatePen(			//координатные оси
		PS_SOLID,				//сплошная линия
		3,						//толщина 2 пикселя
		RGB(0, 0, 0));			//цвет черный

	graf_pen.CreatePen(			//график
		PS_SOLID,				//сплошная линия
		2,						//толщина 2 пикселя
		RGB(0, 0, 255));			//цвет черный
	graf_pen2.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}
void CSignalDlg::OnBnClickedButton2()
{
	Invalidate();
	UpdateData(true);
	UpdateWindow();
	time = int(t / Fd);
	double r[13] = { 0 }, t1 = 0, Kol, Summ_Eps = 0;
	vector<double> Eps;
	vector<double> Shum;
	for (int j = 0; j < time; j++)
	{
		Kol = 0;
		for (int i = 0; i < 13; i++)
		{

			r[i] = 2 * (rand() % 11) - 10;
			Kol += r[i];
		}
		Eps.push_back(Kol);
		Summ_Eps += Kol*Kol;
	}
	Es = 0;
	double En=0;
	double Amplitude[] = { Amplitude1, Amplitude2, Amplitude3 };
	double Frequency[] = { Frequency1, Frequency2, Frequency3 };
	double Phase[] = { Phase1, Phase2, Phase3 };
	double alpha = d / 100;
	int k = 0;
	double s1=0, s2=0, s3=0, s=0;
	while (t1 < time)
	{
		s1 = Amplitude[0] * sin(2 * M_PI*Frequency[0] * t1 + Phase[0]);
		s2 = Amplitude[1] * sin(2 * M_PI*Frequency[1] * t1 + Phase[1]);
		s3 = Amplitude[2] * sin(2 * M_PI*Frequency[2] * t1 + Phase[2]);
		s = s1 + s2 + s3;
		t1 += 1;
		Es += s*s;
	}

	En = alpha*Es;
	double betta = sqrt(En / Summ_Eps);
	t1 = 0;
	Ymax = 0;
	for (int j = 0; t1<time;j++)
	{
		Shum.push_back(Eps[j] * betta);
		rightFourea.push_back(Signal(j) + Shum[j]);
		t1 += 1;
		if (abs(rightFourea.back()) > Ymax)
		{
			Ymax = abs(rightFourea.back());
		}
	}
	Otrisovka1(Ymax);
	PicDcMakeANoiseSignal->SelectObject(&graf_pen);
	PicDcMakeANoiseSignal->MoveTo(KOORDSH(0, rightFourea[0]));
	for (int j = 0; j < time; j++)
	{
		PicDcMakeANoiseSignal->LineTo(KOORDSH(j, rightFourea[j]));
	}
	Eps.clear();
	Shum.clear();
}


// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CSignalDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CSignalDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//Кнопка зашумления
void CSignalDlg::OnBnClickedMakeanoise()
{
	UpdateData(true);

	vector<double> backFourea;
	int is = -1;
	time = int(t / Fd);
	t1 = 0; t2 = 0; E0 = 0;
	double Ym2 = 0;
	cmplx *sp = new cmplx[(int)time];
	for (int i = 0; i < time; i++)
	{
		sp[i].real = rightFourea[i];
		sp[i].image = 0;
	}
	fourea(sp, int(time), is);
	double Ei = 0;

	for (int i = 0; i < time; i++)
	{
		backFourea.push_back(sqrt(sp[i].real*sp[i].real+sp[i].image*sp[i].image));
		E0 += backFourea[i] * backFourea[i];
		if (backFourea.back()>Ym2)
		{
			Ym2 = backFourea.back();
		}
	}	
	Otrisovka2(Ym2);
	PicDcNoise->SelectObject(&graf_pen);
	PicDcNoise->MoveTo(SPECTR(0, sqrt(sp[0].real*sp[0].real + sp[0].image*sp[0].image)));
	for (int i = 0; i < time; i++)
	{
		PicDcNoise->LineTo(SPECTR(i, backFourea[i]));
	}
	for (int i = 0; i < time; i++)
	{
		Ei += backFourea[i] * backFourea[i] + backFourea[int(time)-1 - i] * backFourea[int(time)-1 - i];
		if (Ei >= n*E0 / 100.0)
		{
			t1 = i;
			break;
		}
	}
	t2 = time - 1 - t1;
	delete[] sp;
	backFourea.clear();
}


void CSignalDlg::OnBnClickedButton3()
{
	UpdateData(true);
	UpdateWindow();
	PicDcMakeANoiseSignal->SelectObject(&graf_pen2);
	PicDcNoise->SelectObject(&graf_pen2);
	int is = -1;
	double *backFourea = new double[(int)time];
	cmplx *sp = new cmplx[(int)time];
	for (int i = 0; i < time; i++)
	{
		sp[i].real=rightFourea[i];
		sp[i].image = 0;
	}
	fourea(sp, int(time), is);
	backFourea[0] = sqrt(sp[0].real*sp[0].real + sp[0].image*sp[0].image);
	PicDcNoise->MoveTo(SPECTR(0, backFourea[0]));
	for (int i = 0; i < time; i++)
	{
		if (i > t1&&i < t2)
		{
			sp[i].real = 0;
			sp[i].image = 0;
			backFourea[i] = sqrt(sp[i].real*sp[i].real + sp[i].image*sp[i].image);
			PicDcNoise->LineTo(SPECTR(i, backFourea[i]));
		}
		else
		{
			sp[i].real = sp[i].real;
			sp[i].image = sp[i].image;
			backFourea[i] = sqrt(sp[i].real*sp[i].real + sp[i].image*sp[i].image);
			PicDcNoise->LineTo(SPECTR(i, backFourea[i]));
		}
	}
	is = 1;
	fourea(sp, int(time), is);
	double *four1 = new double[(int)time];
	four1[0] = sp[0].real;
	PicDcMakeANoiseSignal->MoveTo(KOORDSH(0, four1[0]));
	double nev = 0;
	for (int i = 0; i < time; i++)
	{
		four1[i] = sp[i].real;
		/*four1[i] = sqrt(sp[i].real*sp[i].real + sp[i].image*sp[i].image);*/
		PicDcMakeANoiseSignal->LineTo(KOORDSH(i, four1[i]));
		nev += (four1[i] - Signal(i))*(four1[i] - Signal(i));
	}
	nevuzki = round((nev/Es) * 1000) / 1000;
	UpdateData(false);
	delete[] four1;
	delete[] sp;
	delete[] backFourea;
	rightFourea.clear();
}


double CSignalDlg::Signal(double tn)
{
	double Amplitude[] = { Amplitude1, Amplitude2, Amplitude3 };
	double Frequency[] = { Frequency1, Frequency2, Frequency3 };
	double Phase[] = { Phase1, Phase2, Phase3 };
	double result = 0;
	for (int i = 0; i <= 2; i++)
	{
		result += Amplitude[i] * sin(2 * M_PI*Frequency[i]*tn + Phase[i]);
	}
	return result;
}


void CSignalDlg::Otrisovka1(double Ym1)
{
	CDialog::OnPaint();
	PicDcMakeANoiseSignal->FillSolidRect(&PicMakeANoiseSignal, RGB(250, 250, 250));			//закрашиваю фон 

	PicDcMakeANoiseSignal->SelectObject(&osi_pen);		//выбираем перо

	yminsh = -1-Ym1-0.1*(Ym1+1);
	ymaxsh = 1+Ym1+0.1*(Ym1+1);
	xminsh = 0;
	xmaxsh = time;

	xpsh = ((double)(PicMakeANoiseSignal.Width()) / (xmaxsh - xminsh));			//Коэффициенты пересчёта координат по Х
	ypsh = -((double)(PicMakeANoiseSignal.Height()) / (ymaxsh - yminsh));

	PicDcMakeANoiseSignal->FillSolidRect(&PicMakeANoiseSignal, RGB(250, 250, 250));			//закрашиваю фон 

	PicDcMakeANoiseSignal->SelectObject(&osi_pen);
	//создаём Ось Y
	PicDcMakeANoiseSignal->MoveTo(KOORDSH(0, ymaxsh));
	PicDcMakeANoiseSignal->LineTo(KOORDSH(0, yminsh));
	//создаём Ось Х
	PicDcMakeANoiseSignal->MoveTo(KOORDSH(xminsh, 0));
	PicDcMakeANoiseSignal->LineTo(KOORDSH(xmaxsh, 0));

	//подпись осей
	//PicDcMakeANoiseSignal->TextOutW(KOORDSH(0, ymaxsh - 0.2), _T("Y")); //Y
	//PicDcMakeANoiseSignal->TextOutW(KOORDSH(xmaxsh - 0.3, 0 - 0.2), _T("t")); //X

	PicDcMakeANoiseSignal->SelectObject(&setka_pen);

	//отрисовка сетки по y
	for (float x = xminsh + 0.1*xmaxsh; x <= xmaxsh; x += xmaxsh / 10)
	{
		PicDcMakeANoiseSignal->MoveTo(KOORDSH(x, ymaxsh));
		PicDcMakeANoiseSignal->LineTo(KOORDSH(x, yminsh));
	}
	//отрисовка сетки по x
	for (float y = yminsh + 0.1*ymaxsh; y <= ymaxsh; y += ymaxsh / 10)
	{
		PicDcMakeANoiseSignal->MoveTo(KOORDSH(xminsh, y));
		PicDcMakeANoiseSignal->LineTo(KOORDSH(xmaxsh, y));
	}


	/*подпись точек на оси*/
	CFont font1;
	font1.CreateFontW(12, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS || CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Times New Roman"));
	PicDcMakeANoiseSignal->SelectObject(font1);
	//по Y с шагом 5
	for (double i = yminsh + 0.1*yminsh; i <= ymaxsh; i += ymaxsh / 10)
	{
		CString str;
		str.Format(_T("%.1f"), i);
		PicDcMakeANoiseSignal->TextOutW(KOORDSH(0.2, i), str);
	}
	//по X с шагом 0.5
	for (double j = xminsh + 0.1*xminsh; j <= xmaxsh; j += xmaxsh / 10)
	{
		CString str;
		str.Format(_T("%.1f"), j);
		PicDcMakeANoiseSignal->TextOutW(KOORDSH(j - 0.25, -0.2), str);
	}

}

void CSignalDlg::Otrisovka2(double Ym2)
{
	yminsp = -0.1*Ym2;
	ymaxsp = Ym2+0.1*Ym2;
	xminsp = 0; //-3
	xmaxsp = time;

	xpsp = ((double)(PicNoise.Width()) / (xmaxsp - xminsp));			//Коэффициенты пересчёта координат по Х
	ypsp = -((double)(PicNoise.Height()) / (ymaxsp - yminsp));


	PicDcNoise->FillSolidRect(&PicNoise, RGB(250, 250, 250));			//закрашиваю фон 

	PicDcNoise->SelectObject(&osi_pen);
	//создаём Ось Y
	PicDcNoise->MoveTo(SPECTR(0, ymaxsp));
	PicDcNoise->LineTo(SPECTR(0, yminsp));
	//создаём Ось Х
	PicDcNoise->MoveTo(SPECTR(xminsp, 0));
	PicDcNoise->LineTo(SPECTR(xmaxsp, 0));

	//подпись осей
	PicDcNoise->TextOutW(SPECTR(0, ymaxsp - 0.2), _T("А")); //Y
	PicDcNoise->TextOutW(SPECTR(xmaxsp - 0.3, 0 - 0.2), _T("t")); //X

	PicDcNoise->SelectObject(&setka_pen);

	//отрисовка сетки по y
	for (float x = xminsp + 0.1*xminsp; x <= xmaxsp; x += xmaxsp / 10)
	{
		PicDcNoise->MoveTo(SPECTR(x, ymaxsp));
		PicDcNoise->LineTo(SPECTR(x, yminsp));
	}
	//отрисовка сетки по x
	for (float y = yminsp + 0.1*yminsp; y <= ymaxsp; y += ymaxsp / 10)
	{
		PicDcNoise->MoveTo(SPECTR(xminsp, y));
		PicDcNoise->LineTo(SPECTR(xmaxsp, y));
	}


	//подпись точек на оси
	CFont font2;
	font2.CreateFontW(12, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS || CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Times New Roman"));
	PicDcNoise->SelectObject(font2);
	//по Y с шагом 5
	for (double i = yminsp + 0.1*yminsp; i <= ymaxsp; i += ymaxsp / 10)
	{
		CString str;
		str.Format(_T("%.1f"), i);
		PicDcNoise->SelectObject(font2);
		PicDcNoise->TextOutW(SPECTR(0.2, i), str);
	}
	//по X с шагом 0.5
	for (double j = xminsp + 0.1*xminsp; j <= xmaxsp; j += xmaxsp / 10)
	{
		CString str;
		str.Format(_T("%.1f"), j);
		PicDcNoise->TextOutW(SPECTR(j - 0.25, -0.2), str);
	}
}


//кнопка для получения сигнала
void CSignalDlg::OnBnClickedButton1()
{
	Invalidate();
	UpdateData(true);
	UpdateWindow();
	PicDcMakeANoiseSignal->SelectObject(&graf_pen);
	PicDcMakeANoiseSignal->MoveTo(KOORD(xmin, Signal(xmin)));
	for (double i = xmin; i < xmax; i++)
	{
		PicDcMakeANoiseSignal->LineTo(KOORD(i, Signal(i)));
	}

}







void CSignalDlg::OnBnClickedSignal3()
{
	// TODO: добавьте свой код обработчика уведомлений
}


void CSignalDlg::OnEnChangeEdit13()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
}
