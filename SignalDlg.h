
// SignalDlg.h : файл заголовка
//
#include <vector>
#pragma once

// диалоговое окно CSignalDlg
class CSignalDlg : public CDialogEx
{
// Создание
public:
	CSignalDlg(CWnd* pParent = NULL);	// стандартный конструктор

	//для чистого сигнала
	CWnd * PicWndCleanSign;
	CDC * PicDcCleanSign;
	CRect PicCleanSign;

	CWnd * PicWndNoise;
	CDC * PicDcNoise;
	CRect PicNoise;

	CWnd * PicWndMakeANoiseSignal;
	CDC * PicDcMakeANoiseSignal;
	CRect PicMakeANoiseSignal;

	CWnd * PicWnd;
	CDC * PicDc;
	CRect Pic;

	CPen osi_pen;		// для осей 
	CPen setka_pen;		// для сетки
	CPen graf_pen;		// для графика функции
	CPen graf_pen2;

	//Переменные для работы с масштабом
	double xp, yp,			//коэфициенты пересчета
		xmin, xmax,			//максисимальное и минимальное значение х 
		ymin, ymax;			//максисимальное и минимальное значение y

	double xpsh, ypsh,			//коэфициенты пересчета
		xminsh, xmaxsh,			//максисимальное и минимальное значение х 
		yminsh, ymaxsh;			//максисимальное и минимальное значение y
	double xpsp, ypsp,			//коэфициенты пересчета
		xminsp, xmaxsp,			//максисимальное и минимальное значение х 
		yminsp, ymaxsp;			//максисимальное и минимальное значение y


// Данные диалогового окна
	enum { IDD = IDD_SIGNAL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg double CleanSignal(double Ai, double Fi, double Pi);
	afx_msg void OnBnClickedMakeanoise();
	afx_msg void OnBnClickedButton1();
	double E0;//Полная энергия спектра
	afx_msg void OnBnClickedSignal1();
	afx_msg void OnBnClickedButton2();
	double Fd;
	double d;
	double t;
	double Es;
	double time;
	double t1, t2;
	double Signal(double tn);
	void Otrisovka1(double Ym1);
	void Otrisovka2(double Ym2);
	double shmax = 0;
	std::vector<double> rightFourea;
	double Ymax=0;
	typedef struct cmplx { float real; float image; } Cmplx;
//========================================================

void fourea(struct cmplx *data, int n, int is)
{
	int i, j, istep;
	int m, mmax;
	float r, r1, theta, w_r, w_i, temp_r, temp_i;
	float pi = 3.1415926f;

	r = pi*is;
	j = 0;
	for (i = 0; i<n; i++)
	{
		if (i<j)
		{
			temp_r = data[j].real;
			temp_i = data[j].image;
			data[j].real = data[i].real;
			data[j].image = data[i].image;
			data[i].real = temp_r;
			data[i].image = temp_i;
		}
		m = n >> 1;
		while (j >= m) { j -= m; m = (m + 1) / 2; }
		j += m;
	}
	mmax = 1;
	while (mmax<n)
	{
		istep = mmax << 1;
		r1 = r / (float)mmax;
		for (m = 0; m<mmax; m++)
		{
			theta = r1*m;
			w_r = (float)cos((double)theta);
			w_i = (float)sin((double)theta);
			for (i = m; i<n; i += istep)
			{
				j = i + mmax;
				temp_r = w_r*data[j].real - w_i*data[j].image;
				temp_i = w_r*data[j].image + w_i*data[j].real;
				data[j].real = data[i].real - temp_r;
				data[j].image = data[i].image - temp_i;
				data[i].real += temp_r;
				data[i].image += temp_i;
			}
		}
		mmax = istep;
	}
	if (is>0)
		for (i = 0; i<n; i++)
		{
			data[i].real /= (float)n;
			data[i].image /= (float)n;
		}

}

afx_msg void OnBnClickedButton3();
double n;
double Amplitude1;
double Amplitude2;
double Amplitude3;
double Frequency1;
double Frequency2;
double Frequency3;
double Phase1;
double Phase2;
double Phase3;
double nevuzki;
afx_msg void OnBnClickedSignal3();
afx_msg void OnEnChangeEdit13();
};
