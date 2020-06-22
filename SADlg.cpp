// SADlg.cpp : implementation file
//

#include "stdafx.h"
#include "SA.h"
#include "SADlg.h"
#include <math.h>

#include <conio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSADlg dialog

CSADlg::CSADlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSADlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSADlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSADlg)
	DDX_Control(pDX, IDC_CITY_FRAME, m_CityFrame);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSADlg, CDialog)
	//{{AFX_MSG_MAP(CSADlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, OnStart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSADlg message handlers

BOOL CSADlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	
	srand((unsigned)time(NULL));

	CRect r;
	m_CityFrame.GetClientRect(&r);

	m_memDC.CreateCompatibleDC(GetDC());
	m_bmp.CreateCompatibleBitmap(GetDC(), r.Width(), r.Height()); 
	m_font.CreatePointFont(110, L"Arial");
	m_pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_brush.CreateSolidBrush(RGB(255, 255, 255));
	
	m_memDC.SelectObject(&m_bmp);
	m_memDC.SelectObject(&m_font);
	m_memDC.SelectObject(&m_pen);
	m_memDC.SelectObject(&m_brush);

	m_CityFrame.GetClientRect(&r);
	m_memDC.Rectangle(&r);

	m_memDC.SetBkMode(TRANSPARENT); 
	
	COLORREF oldTxtCol = (m_memDC.SetTextColor(RGB (0, 100, 10)));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSADlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSADlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // device context for painting

		CRect r;
		m_CityFrame.GetWindowRect(&r);
		ScreenToClient(r);		

		dc.BitBlt(r.left, r.top, r.right - r.left, r.bottom - r.top, &m_memDC, 0, 0, SRCCOPY);
	
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSADlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



/**************************************************************************}
{ODCZYT MAPY Z PLIKU													   }
{**************************************************************************/
BOOL CSADlg::InitializeMap()
{

	int i, j, k;
	double lenergy;
	int stop;


	CFile f;
	if (!f.Open(L"tsp-c.dat", CFile::modeRead | CFile::shareDenyWrite))
	{
		MessageBox(L"Proszê umieœciæ plik tsp-c.dat w katalogu, w którym znajduje siê program.");
		return FALSE;
	}
	
	float fBuf;

	for (i = 0; i < NCITIES; i++)
	{
		f.Read(&fBuf, sizeof(float));
		loc[i][0] = (double)fBuf;
		f.Read(&fBuf, sizeof(float));
		loc[i][1] = (double)fBuf;
	};

	f.Close();

	for (i = 0; i < NCITIES; i++)
		for (j = 0; j < NCITIES; j++)
			distance[i][j]=sqrt((loc[i][0]-loc[j][0])*(loc[i][0]-loc[j][0])+(loc[i][1]-loc[j][1])*(loc[i][1]-loc[j][1]));
	
	for (j = 0; j < NCITIES; j++)
	{
		do
		{
			stop = 1;
			route[j] = rand() % NCITIES;
			k=0;
		 
			while (k<j)
			{
				if (route[k] == route[j])
				{
					stop = 0;
					break;
				}
				k++;
			};
		}
		while (!stop);
	 };

	lenergy=0;
	for (j = 0;j < NCITIES-1; j++)
		lenergy = lenergy + distance[route[j]][route[j+1]];
	lenergy=lenergy+distance[route[NCITIES-1]][route[0]];

	
	 length = lenergy;

	 return TRUE;
}

/**************************************************************************}
{RYSOWANIE AKTUALNEJ TRASY, WYŒWIETLENIE JEJ D£UGOŒCI 					   }
{**************************************************************************/
void CSADlg::Draw()
{

	int i, x, y, x1, y1;
	char s[10];
	CString sEn = "D³ugoœæ trasy: ";

	CRect r;
	m_CityFrame.GetClientRect(&r);
	m_memDC.Rectangle(&r);

	_gcvt_s(s, 10, length, 4);
	sEn += CString(s);

	m_memDC.TextOut(10, r.Height()-20, sEn);

	for (i = 0; i < NCITIES; i++)
	{
		x = 10 + (int)((r.Width()-20)*loc[i][0]);
		y = 10 + (int)((r.Height()-20)*loc[i][1]);
		m_memDC.Ellipse(x-5, y-5, x+5, y+5);
	}

	for (i = 0; i < NCITIES-1; i++)
	{
		x = 10 + (int)((r.Width()-20)*loc[route[i]][0]);
		y = 10 + (int)((r.Height()-20)*loc[route[i]][1]);
		x1 = 10 + (int)((r.Width()-20)*loc[route[i+1]][0]);
		y1 = 10 + (int)((r.Height()-20)*loc[route[i+1]][1]);
		m_memDC.MoveTo(x, y);
		m_memDC.LineTo(x1, y1);
	};

	x = 10 + (int)((r.Width()-20)*loc[route[NCITIES-1]][0]);
	y = 10 + (int)((r.Height()-20)*loc[route[NCITIES-1]][1]);
	x1 = 10 + (int)((r.Width()-20)*loc[route[0]][0]);
	y1 = 10 + (int)((r.Height()-20)*loc[route[0]][1]);
	m_memDC.MoveTo(x, y);
	m_memDC.LineTo(x1, y1);

	CDC* pDC = GetDC();
	m_CityFrame.GetWindowRect(&r);
	ScreenToClient(r);

	pDC->BitBlt(r.left, r.top, r.right - r.left, r.bottom - r.top, &m_memDC, 0, 0, SRCCOPY);

	ReleaseDC(pDC);
}

void CSADlg::InitPopulation(void)
{
	int i, j, k, stop;

	for (i = 0; i < NCREAT; i++)
	{

		for (j = 0; j < NCITIES; j++)
		{
			do
			{
				stop = 1;
				population[i][j] = rand() % NCITIES;
				k=0;
			 
				while (k<j)
				{
					if (population[i][k] == population[i][j])
					{
						stop = 0;
						break;
					}
					k++;
				};
			}
			while (!stop);
		 };
	}
}

BOOL CSADlg::Check(int city, int n, int* route)
{
	BOOL bRes = TRUE;
	int i;

	for (i = 0; i < n; i++)
	{
		if (route[i] == city)
		{
			bRes = FALSE;
			break;
		}
	}

	return bRes;
}

double* CSADlg::get_fitness() {

	double* oceny = new double[NCREAT];
	for (int i = 0; i < NCREAT; i++)
		oceny[i] = 0;

	for (int i = 0; i < NCREAT; i++) {
		for (int k = 0; k < NCITIES-1; k++) {
			int a = population[i][k];
			int b = population[i][k + 1];
			oceny[i] += distance[a][b];
		}
		int a = population[i][NCITIES - 1];
		int b = population[i][0];
		oceny[i] += distance[a][b];

		oceny[i] = 100 / oceny[i]; //teraz - im wiêksze wartoœæ, tym lepiej
	}
	

	/*for (int i = 0; i < NCREAT; i++)
		_cprintf("%f ", oceny[i]);*/

	return oceny;
}

int* CSADlg::crossover(int parentA, int parentB) {
	
	int geneA = rand() % NCITIES;
	int geneB = rand() % NCITIES;

	int start_gene = min(geneA, geneB);
	int end_gene = max(geneA, geneB);

	int *child = new int[NCITIES];
	//int child_second[NCITIES];
	for (int i = 0; i < NCITIES; i++) {
		child[i] = 0;
		//child_second[i] = 0;
	}

	for (int i = start_gene; i < end_gene; i++) {
		child[i] = population[parentA][i];
	}

	for (int i = 0; i < start_gene; i++) {
		for (int k = 0; k < NCITIES; k++) {
			if (Check(population[parentB][k], NCITIES, child)) {
				child[i] = population[parentB][k];
					break;
			}
		}
	}

	for (int i = end_gene; i < NCITIES; i++) {
		for (int k = 0; k < NCITIES; k++) {
			if (Check(population[parentB][k], NCITIES, child)) {
				child[i] = population[parentB][k];
				break;
			}
		}
	}

	/*for (int i = 0; i < NCITIES; i++) {
		_cprintf("%d ", population[parentA][i]);
	}
	_cprintf("\n");
	for (int i = 0; i < NCITIES; i++) {
		_cprintf("%d ", population[parentB][i]);
	}
	_cprintf("\n");
	for (int i = 0; i < NCITIES; i++) {
		_cprintf("%d ", child_first[i]);
	}
	_cprintf("\n");*/

	return child;
}

void reverse_array(int* arr, int start, int end) {
	while (start < end)
	{
		int tmp = arr[start];
		arr[start] = arr[end];
		arr[end] = tmp;
		start++;
		end--;
	}
}

void CSADlg::mutation(int* route)
{
	int start = 0;
	int end = NCITIES - 1;

	reverse_array(route, start, end);

	int a = 0;
	int b = 0;

	while (1) {
		a = rand() % NCITIES;
		b = rand() % NCITIES;

		if (a != b)
			break;
	}

	int tmp = route[a];
	route[a] = route[b];
	route[b] = tmp;

}

int CSADlg::get_alpha(double* oceny)
{
	double max_value = 0;
	int index = -1;

	for (int i = 0; i < NCREAT; i++) {
		if (oceny[i] > max_value) {
			max_value = oceny[i];
			index = i;
		}
	}

	return index;
}

void CSADlg::update_length()
{
	length = 0;
	for (int i = 0; i < NCITIES - 1; i++) {
		length += distance[route[i]][route[i + 1]];
	}
	length += distance[route[NCITIES - 1]][route[0]];
}

void CSADlg::OnStart() 
{

	if (!InitializeMap())
		return;
 
	Draw();

	InitPopulation();

	//AllocConsole();

	for (int iteration = 0; iteration < 10000; iteration++) {
		double* oceny = get_fitness();
		int old_alpha = get_alpha(oceny);

		double SUM = 0;
		for (int i = 0; i < NCREAT; i++)
			SUM += oceny[i];

		int** new_population = new int* [NCREAT];
		for (int i = 0; i < NCREAT; i++) {
			new_population[i] = new int[NCITIES];
		}

		// ruletka
		for (int counter = 0; counter < NCREAT-1; counter++) {

			int parents[2] = { -1, -1 };
			int k = 0;

			for (int i = 0; i < 2; i++) {
				int rand_num = rand() % (int)SUM;
				double sum = 0;
				for (int j = 0; j < NCREAT; j++) {
					sum += oceny[j];
					if (sum > rand_num) {
						parents[k] = j;
						k += 1;
						break;
					}
				}
			}
		// end ruletka

			//krzy¿owanie
			int** children = new int*[NCREAT];
			bool born = false;
			if (rand() % 10 < 5) {
				children[0] = crossover(parents[0], parents[1]);
				children[1] = crossover(parents[0], parents[1]);
				born = true;
			}
			
			for (int i = 0; i < NCITIES; i++) {
				if (born) {
					new_population[counter][i] = children[0][i];
					new_population[counter+1][i] = children[1][i];
				}
				else {
					new_population[counter][i] = population[parents[0]][i];
					new_population[counter + 1][i] = population[parents[1]][i];
				}
			}

			if (born) {
				for (int i = 0; i < 2; i++) {
					delete[] children[i];
				}
			}
			delete[] children;

			// end krzy¿owanie
		}

		// mutacja
		if (rand() % 10 < 2) { //changed 
			for (int i = 0; i < NCREAT - 1; i++) {
				mutation(new_population[i]);
			}
		}
		// end mutacja

		// transfer the best individual from the old generation to new generation
		for (int i = 0; i < NCITIES; i++) {
			new_population[NCREAT - 1][i] = population[old_alpha][i];
		}

		// transfer new generation to old one
		for (int i = 0; i < NCREAT; i++) {
			for (int k = 0; k < NCITIES; k++) {
				population[i][k] = new_population[i][k];
			}
		}

		//get the best individual
		double* new_oceny = get_fitness();
		int alpha = get_alpha(new_oceny);

		//update route array
		for (int i = 0; i < NCITIES; i++) {
			route[i] = population[alpha][i];
		}
		//update route length
		update_length();


		Draw();

		for (int i = 0; i < NCREAT; i++) {
			delete[] new_population[i];
		}
		delete[] new_population;

		delete[] oceny;
		delete[] new_oceny;
	}
	
  /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  TU WSTAWIC ALGORYTM GENETYCZNY                                             
  Odleg³oœci pomiêdzy miastami zawiera tablica 'distance' (plik SADlg.h).	 
  																			  
  Funkcja Draw rysuje trasê zawart¹ w tablicy 'route' i wyœwietla wartoœæ	  
  zmiennej 'length'.														  
  																			  	
  Populacja pocz¹tkowa jest ju¿ utworzona - zawiera j¹ tablica 'population'. 
  																			  
  Liczba osobników jest okreœlona przez sta³¹ NCREAT.						  
  																			  
  Funkcja Check(int city, int n, int* route) pozwala stwierdziæ, czy miasto	  
  'city' znajduje siê ju¿ wœród pierwszych n miast w trasie 'route'. Jeœli	  
  miasto ju¿ wystêpuje w trasie funkcja 'Check' zwraca wartoœæ FALSE.		  

  --------------------------------------------------------------------------
  Nale¿y utworzyæ 10000 populacji i za ka¿dym razem wyœwietliæ najlepszego
  osobnika przy u¿yciu funkcji Draw.

  Przyjmujemy prawdopodobieñstwo krzy¿owania 0.5 i prawdopodobieñstwo mutacji 0.2.

  Tworzenie populacji:
  1. Ocena stopnia dopasowania ('fitness') ka¿dego z osobników.
  2. Wybór pary osobników metod¹ ruletki - http://www.obitko.com/tutorials/genetic-algorithms/selection.php, sekcja "Roulette Wheel Selection".
  3. Krzy¿owanie wybranych osobników z zadanym prawdopodobieñstwem, tak by powsa³y dwa nowe osobniki
	 (w przeciwnym razie klonowanie do nastêpnej populacji) - http://www.obitko.com/tutorials/genetic-algorithms/crossover-mutation.php, sekcja "Permutation encoding". 
  3. Mutacja z zadanym prawdopodobieñstwem.

  Po utworzeniu nowej populacji nale¿y do niej sklonowaæ najlepszego ze starych 
  osobników (na miejsce dowolnego z nowych), a potem zast¹piæ star¹ populacjê 
  now¹.
  
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

  

}


