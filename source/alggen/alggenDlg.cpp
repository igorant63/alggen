// alggenDlg.cpp : implementation file
//

#include "stdafx.h"
#include <afxtempl.h>
#include "alggen.h"
#include "alggenDlg.h"
#include <algorithm>
#include "dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int (__stdcall *check_alg)(base_check *);
int (__stdcall *get_max_qual)();
int (__stdcall *get_base_qual)();
int* (__stdcall *get_reg_one)();
int* (__stdcall *get_mem_one)();
void (__stdcall *init_array)();
const char* (__stdcall *about)();
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
	virtual BOOL OnInitDialog();
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



BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if(about)
		SetDlgItemText(IDC_ABOUT, about());
	return TRUE; 
}



BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlggenDlg dialog

CAlggenDlg::CAlggenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlggenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlggenDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAlggenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlggenDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAlggenDlg, CDialog)
	//{{AFX_MSG_MAP(CAlggenDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnStart)
	ON_BN_CLICKED(IDC_BUTTON2, OnSample)
	ON_COMMAND(IDM_MAKE_POPULATION, OnMakePopulation)
	ON_COMMAND(IDM_SAVE_POPULATION, OnSavePopulation)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_LOAD_POPULATION, OnLoadPopulation)
	ON_BN_CLICKED(IDC_BUTTON4, OnViewProgram)
	ON_COMMAND(IDM_PORGRAM_LENGTH, OnPorgramLength)
	ON_COMMAND(IDM_ADD_COMMAND, OnAddCommand)
	ON_COMMAND(IDM_MAKE_PROGRAM, OnMakeProgram)
	ON_COMMAND(IDM_REMOVE_PROGRAM, OnRemoveProgram)
	ON_COMMAND(IDM_MERGE_POPULATIONS, OnMergePopulations)
	ON_COMMAND(IDM_REMOVE_COMMAND, OnRemoveCommand)
	ON_BN_CLICKED(IDC_TEST, OnTest)
	ON_BN_CLICKED(IDC_TEST_ALL, OnTestAll)
	ON_COMMAND(IDM_PROB_SETTINGS, OnProbSettings)
	ON_BN_CLICKED(IDC_VIEW_CPU, OnViewCpu)
	ON_BN_CLICKED(IDC_LIST, OnListProgram)
	ON_COMMAND(IDM_SORT_POPULATION, OnSortPopulation)
	ON_COMMAND(IDM_CLEAR_POP, OnClearPop)
	ON_BN_CLICKED(IDC_STEP, OnStep)
	ON_COMMAND(IDM_OPT, OnOpt)
	ON_COMMAND(IDM_CUT, OnCut)
	ON_COMMAND(IDM_NULL, OnNull)
	ON_COMMAND(IDM_MUTATION, OnMutation)
	ON_COMMAND(IDM_SAVE_CONFIG, OnSaveConfig)
	ON_COMMAND(IDM_LOAD_CONFIG, OnLoadConfig)
	ON_COMMAND(IDM_HIST, OnHist)
	ON_BN_CLICKED(IDC_BUTTON5, OnPrev)
	ON_COMMAND(IDM_SELECTTEST, OnSelecttest)
	ON_COMMAND(IDM_ABOUT, OnAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlggenDlg message handlers

CAlggenDlg* mainDlg;

BOOL CAlggenDlg::OnInitDialog()
{	CDialog::OnInitDialog();
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
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	pMenu=new CMenu; 
	nullMenu=new CMenu; 
	pMenu->LoadMenu(IDR_MENU1);
	nullMenu->LoadMenu(IDR_MENU2);
	SetMenu(pMenu);
	maps::init_array(sample_prc);
	mainDlg=this;
	hDll=0;
	dll_ind=0;
	load_dlls();
	if(dlls.size() > 0) rule.init_rules();
	hThread=0;
	CFile f;
	if(f.Open("default.set",CFile::modeRead))
		{CArchive ar(&f,CArchive::load);
		ar >>	maps::program_length >> maps::pop_length; 
		ar >> maps::prob[0] >>  maps::prob[1] >> maps::prob[2] >> maps::prob[3] >> maps::prob[4];
		ar >> maps::mut[0] >> maps::mut[1] >> maps::mut[2] >> maps::mut[3] >> maps::mut[4]; 
		ar >> maps::mut_x[0] >> maps::mut_x[1] >> maps::mut_x[2] >> maps::mut_x[3] >> maps::mut_x[4]; 
		ar >> opt >> rem >> labels>>array;
		ar.Flush();
		f.Close();
		}
	else
		{opt=0;	rem=FALSE; labels=FALSE; array=FALSE; }
	CStatic* st=(CStatic*)GetDlgItem(IDC_STATUS);
	return TRUE; 
}

void CAlggenDlg::scan_dlls()
{			dll_ind=0;
			WIN32_FIND_DATA fd;
			HANDLE h=FindFirstFile("*.dll", &fd);
			if (h != INVALID_HANDLE_VALUE)
				{
				 	HINSTANCE hD=LoadLibrary(fd.cFileName);						
					char* (__stdcall *ab)();
					ab=(char* (__stdcall *)(void))GetProcAddress(hD,MAKEINTRESOURCE(1));
					if(ab) 
					{	dlls.push_back(hD);
						CString s; s.Format("%-15s",fd.cFileName);
						s+="  ";s+=ab();
						dlls_s.Add(s);	
					}
					while (FindNextFile(h,&fd)) 
						{
							 	HINSTANCE hD=LoadLibrary(fd.cFileName);						
								ab=(char* (__stdcall *)(void))GetProcAddress(hD,MAKEINTRESOURCE(1));
								if(ab) 
								{ CString s;
								   s.Format("%-15s",fd.cFileName);
								   if(!strcmp(fd.cFileName,"dll.dll")) dll_ind=dlls.size();
								   s+="  ";s+=ab();
								   dlls_s.Add(s);								
								   dlls.push_back(hD);
								}
						}
					FindClose(h);
			}
}

void CAlggenDlg::OnAbout() 
{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
}

void CAlggenDlg::load_dlls()
{	
				scan_dlls();
				if(dlls.size())
					{
						hDll=dlls[dll_ind];
						check_alg=(int (__stdcall *)(struct base_check *))GetProcAddress(hDll, MAKEINTRESOURCE(2));
						get_max_qual=(int (__stdcall *)(void))GetProcAddress(hDll,MAKEINTRESOURCE(4));
						get_base_qual=(int (__stdcall *)(void))GetProcAddress(hDll,MAKEINTRESOURCE(3));
						get_reg_one=(int* (__stdcall *)(void))GetProcAddress(hDll,MAKEINTRESOURCE(6));
						get_mem_one=(int* (__stdcall *)(void))GetProcAddress(hDll,MAKEINTRESOURCE(5));
						init_array=(void (__stdcall *)(void))GetProcAddress(hDll,MAKEINTRESOURCE(7));
						about=(const char* (__stdcall *)(void))GetProcAddress(hDll,MAKEINTRESOURCE(1));						
					}
}

void CAlggenDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAlggenDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

HCURSOR CAlggenDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int tm1;
int tm2;
void CAlggenDlg::BeginTime() 
{
	tm1=timeGetTime(); 
}

void CAlggenDlg::EndTime(CString& str) 
{ 
	tm2=timeGetTime(); 
	CString s;
	s.Format("%6i",tm2-tm1);
	str+=s;
}


bool FlagRun=false;
DWORD WINAPI ThreadFunc( LPVOID lpParam ) 
{ 
	while(1)
	{ if(FlagRun)
		{
			mainDlg->Step();				
		}
		else 
		{ 
			CWnd* b1=mainDlg->GetDlgItem(IDC_LIST);
			CButton* b=(CButton*)mainDlg->GetDlgItem(IDC_BUTTON1);
			b->SetWindowText("Start");
			b1->EnableWindow();
			b1=mainDlg->GetDlgItem(IDC_BUTTON4);
			b1->EnableWindow();	
			b1=mainDlg->GetDlgItem(IDC_BUTTON5);
			b1->EnableWindow();	
			b1=mainDlg->GetDlgItem(IDC_STEP);
			b1->EnableWindow();	
			b1=mainDlg->GetDlgItem(IDC_VIEW_CPU);
			b1->EnableWindow();
			b1=mainDlg->GetDlgItem(IDC_BUTTON2);
			b1->EnableWindow();
			b1=mainDlg->GetDlgItem(IDC_TEST);
			b1->EnableWindow();
			b1=mainDlg->GetDlgItem(IDC_TEST_ALL);
			b1->EnableWindow();
			mainDlg->SetMenu(mainDlg->pMenu);
			pop.pp=0;
			return 0; 
		}
	}
} 

void CAlggenDlg::OnStep() 
{ 
	if(dlls.size()==0) 
	{ 	
		SetDlgItemText(IDC_STATUS, "Status: Missing test module");
		return;		
	}
	if(pop.prg.size()==0) 
	{ 	
		SetDlgItemText(IDC_STATUS, "Status: No programs to run");
		return;		
	}
	count_m=0;
	char buf[10];
	int i=pop.prg.size();  
	vector<program>::iterator ip,ip2;
	ip2=pop.prg.end();   	  	
	for(ip=pop.prg.begin();ip!=ip2;ip++) 	(*ip).test();
	ip=pop.prg.begin();	ip2=pop.prg.end();   
	sort(ip,ip2);
	if(rem)
		{
			vector<program>::iterator end;
			end=unique(pop.prg.begin(),pop.prg.end(),pr_eq<program>());
			pop.prg.erase(end,pop.prg.end());
		}
	if(pop.prg.size()> 0) 
		{	if(hist.size() >0)
			{ 
				program p=hist.back();
				if(p == *pop.prg.begin());
				else hist.push_back(*pop.prg.begin());
			}
			else hist.push_back(*pop.prg.begin());
		}
	Step();
}

int time_out=0;

void CAlggenDlg::Step()
{

char buf[10];
	vector<program>::iterator ip,ip2;
	vector<program> mts;
	BeginTime();	
	if(array) // For mutable test array
	{	init_array(); // Из DLL 
		rule.init_rules();
		ip2=pop.prg.end();   	  	                           
		for(ip=pop.prg.begin();ip!=ip2;ip++) 	(*ip).test(); 
		ip=pop.prg.begin();	ip2=pop.prg.end();   
		sort(ip,ip2);		
	 }	
	ip2=pop.prg.end();
	for(ip=pop.prg.begin();ip!=ip2;ip++) 
		{		  program mutant(*ip);
					mutant.make_mutation(labels);
					mutant.test();
					if(mutant.status == 0) mts.push_back(mutant);
		}
	count_m+=pop.prg.size();
	ip=mts.begin();ip2=mts.end();
	sort(ip,ip2);
	merge(pop.prg, mts);
	ip=pop.prg.begin();ip2=pop.prg.end();
	sort(ip,ip2);
	if(rem)
		{ vector<program>::iterator end;
		  end=unique(pop.prg.begin(),pop.prg.end(),pr_eq<program>());
			pop.prg.erase(end,pop.prg.end());
		}
	int size=pop.prg.size();
	if(size > maps::pop_length)   // cut
			{	
				pop.prg.erase(pop.prg.begin()+maps::pop_length,pop.prg.end());
			}
 //   ip=pop.prg.begin();ip2=pop.prg.end();
//	sort(ip,ip2);
	if(rem)
		{ vector<program>::iterator end;
		  end=unique(pop.prg.begin(),pop.prg.end(),pr_eq<program>());
			pop.prg.erase(end,pop.prg.end());
		}
	ip = pop.prg.begin(); ip2 = pop.prg.end();
	sort(ip, ip2);
	if(hist.back() == *pop.prg.begin());
	else
		{ hist.push_back(*pop.prg.begin());
			CEdit* ed=(CEdit*)GetDlgItem(IDC_EDIT2);
			ed->SetSel(0, -1);
			ed->Clear();
			(*pop.prg.begin()).listing(ed);
		}
	if(timeGetTime() - time_out > 500) 
	{	time_out=timeGetTime();
		CString er="time="; 
		EndTime(er);
		er+="   tot_mutation="; 
		CString s;
		s.Format("%10i",count_m);
		er+=s;
		er+="   max_qual="; 
		s.Format("%6i",(*pop.prg.begin()).qual);
		er+=s;
		SetDlgItemText(IDC_STATUS, er);	
	}
}

void CAlggenDlg::OnStart() // Mutations
{ 
   if(dlls.size()==0) 
	{
	  SetDlgItemText(IDC_STATUS, "Status: Missing test module");
	  return;		
	}

   if(pop.prg.size()==0) 
	{
	   SetDlgItemText(IDC_STATUS, "Status: No programs to run");
	   return;		
	}
   if(!FlagRun)
   {	
	time_out=timeGetTime(); 
	count_m=0;
	vector<program>::iterator ip,ip2;
	ip2=pop.prg.end(); 
	for(ip=pop.prg.begin();ip!=ip2;ip++) 	(*ip).test();
	ip=pop.prg.begin();	ip2=pop.prg.end();   
	sort(ip,ip2);
	if(rem)
		{ 
			vector<program>::iterator end;
			end=unique(pop.prg.begin(),pop.prg.end(),pr_eq<program>());
			pop.prg.erase(end,pop.prg.end());
		}
	if(pop.prg.size()> 0) 
	{	
		if(hist.size() >0)
		{ 
			program p=hist.back();
			if(p == *pop.prg.begin());
			else hist.push_back(*pop.prg.begin());
		}
		else hist.push_back(*pop.prg.begin());
	}
	CButton* b=(CButton*)GetDlgItem(IDC_BUTTON1);
	DWORD dwThreadId;
	FlagRun=true;
	b->SetWindowText("Stop");
	CWnd* b1=GetDlgItem(IDC_LIST);
	b1->EnableWindow(FALSE);	
	b1=GetDlgItem(IDC_BUTTON4);
	b1->EnableWindow(FALSE);	
	b1=GetDlgItem(IDC_BUTTON5);
	b1->EnableWindow(FALSE);	
	b1=GetDlgItem(IDC_STEP);
	b1->EnableWindow(FALSE);	
	b1=GetDlgItem(IDC_VIEW_CPU);
	b1->EnableWindow(FALSE);
	b1=GetDlgItem(IDC_BUTTON2);
	b1->EnableWindow(FALSE);
	b1=GetDlgItem(IDC_TEST);
	b1->EnableWindow(FALSE);
	b1=GetDlgItem(IDC_TEST_ALL);
	b1->EnableWindow(FALSE);
	SetMenu(nullMenu);
	if(hThread) CloseHandle(hThread);
	hThread=CreateThread(NULL,0, ThreadFunc, 0, 0, &dwThreadId);  
 }
 else 
 {
	FlagRun=false;
 }
}

void CAlggenDlg::OnSample() 
{ 
	char buf[10];                    
	program pr;
	pr.gen_sample();
	pop.prg.push_back(pr);
	pop.pp=pop.prg.size()-1;
	CEdit* ed=(CEdit*)GetDlgItem(IDC_EDIT2);
	ed->SetSel(0, -1);
	ed->Clear();
	pop.prg[pop.pp].listing(ed);
	CString str;
	str+=(char)0x0d; str+=(char)0x0a;
	ed->ReplaceSel(str);
	CString er= "Status: Program Listing #"; 	
	er+=_itoa(pop.pp, buf,10);
	SetDlgItemText(IDC_STATUS, er);
}

void CAlggenDlg::OnMakePopulation() 
{ 
	char buf[10];
	CString er="Please wait..."; 
	SetDlgItemText(IDC_STATUS, er);	
	DWORD tm1=timeGetTime();  
	pop.make_population(maps::pop_length);
	DWORD tm2=timeGetTime();  
    CEdit* ed=(CEdit*)GetDlgItem(IDC_EDIT2);
	ed->SetSel(0, -1);
	ed->Clear();
	pop.prg[pop.pp].listing(ed);
	er="Status: Program Listing #"; 	
	er+=_itoa(pop.pp, buf,10);
	er+="  Time, ms "; 	
	er+=_itoa(tm2-tm1, buf,10);
	SetDlgItemText(IDC_STATUS, er);	
	hist.clear();
}

void CAlggenDlg::OnSavePopulation() 
{ 
	char buf[256];
	CFileDialog fd(FALSE,"pop","*.pop");
	GetCurrentDirectory(256,buf);
	fd.m_ofn.lpstrInitialDir=buf;
	CString File;
	int res=fd.DoModal();
	if(res==IDOK)
	{
		File=fd.GetPathName();
	}
	else return;
	CFile f(File,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	CArchive ar(&f,CArchive::store);
	pop.save_population(ar);
	ar.Close();
	f.Close();
	CString er="СStatus: Population saved";	SetDlgItemText(IDC_STATUS, er);
}

void CAlggenDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
 
}

void CAlggenDlg::OnLoadPopulation() 
{ 
	char buf[256];
	CFileDialog fd(TRUE,"pop","*.pop");
	GetCurrentDirectory(256,buf);
	fd.m_ofn.lpstrInitialDir=buf;
	CString File;
	int res=fd.DoModal();
	if(res==IDOK)
	{
		File=fd.GetPathName();
	}
	else return;
	CFile f(File, CFile::modeRead | CFile::typeBinary);
	CArchive ar(&f,CArchive::load);
	pop.load_population(ar);
	ar.Close();
	f.Close();
    CEdit* ed=(CEdit*)GetDlgItem(IDC_EDIT2);
	ed->SetSel(0, -1);
	ed->Clear();
	pop.prg[pop.pp].listing(ed);
	CString er="Status: Program Listing #"; 	
	er+=_itoa(pop.pp, buf,10);
	SetDlgItemText(IDC_STATUS, er);	
	hist.clear();
}

void CAlggenDlg::OnViewProgram() 
{ 
	char buf[10];  
    CEdit* ed=(CEdit*)GetDlgItem(IDC_EDIT2);
	ed->SetSel(0, -1);
	ed->Clear();
	if(pop.prg.size()==0) 
			{ 	SetDlgItemText(IDC_STATUS, "Status: No programs to display");
					return;		
			}
	pop.pp++;
	if(pop.pp >= pop.prg.size()) 
	{ 
		pop.pp=0; 
	}
	program& p=	pop.prg[pop.pp];
	p.listing(ed);
	CString str;
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
	CString er="Status: Program Listing #"; 	
	er+=_itoa(pop.pp, buf,10);
	SetDlgItemText(IDC_STATUS, er);	
	hist.clear();
}

void CAlggenDlg::OnPorgramLength() 
{
  CProgramLengthDialog d;
	if(d.DoModal()==IDOK)
	{ 
		maps::program_length=d.m_pl;
		maps::pop_length=d.m_popl;
	}
}

void CAlggenDlg::OnAddCommand() 
{
 CEditProgramDialog d;
	if(d.DoModal()==IDOK)
	{ int n=pop.prg.size();
		if(n==0) { 	MessageBox("Status: No program to edit");	return;}	
		program& p=pop.prg[pop.pp];
		if(p.prog_len>=MAX_PROGRAM_LENGTH) {	MessageBox("Program length limit reached");return;}	
		p.sign_user();	
		maps::make_command2(p, d.ind_comm,d.ind_op1,d.ind_op2,d.m_label);
		p.prog_len++;
		CEdit* ed=(CEdit*)GetDlgItem(IDC_EDIT2);
		ed->SetSel(0, -1);
		ed->Clear();
		pop.prg[pop.pp].listing(ed);
	}
		
}

void CAlggenDlg::OnMakeProgram() 
{   
	char buf[10];
	program pr;
	pop.prg.push_back(pr);
	pop.pp=pop.prg.size()-1;
	CEdit* ed=(CEdit*)GetDlgItem(IDC_EDIT2);
	ed->SetSel(0, -1);
	ed->Clear();
	CString str;
	str+=(char)0x0d; str+=(char)0x0a;
	ed->ReplaceSel(str);
	CString er= "Status: Program Listing #"; 	
	er+=_itoa(pop.pp, buf,10);
	SetDlgItemText(IDC_STATUS, er);	
}

void CAlggenDlg::OnRemoveProgram() 
{
	if(pop.prg.size()<=0) return;
	char buf[10];
	pop.prg.erase(pop.prg.begin()+pop.pp);
	if(pop.pp>=pop.prg.size()) pop.pp=pop.prg.size()-1;
	if(pop.pp <0) pop.pp=0;
	CEdit* ed=(CEdit*)GetDlgItem(IDC_EDIT2);
	ed->SetSel(0, -1);
	ed->Clear();
	CString er="Status:";
	if(pop.prg.size() > 0) 
			{pop.prg[pop.pp].listing(ed);
				 er+=" Status: Program Listing #"; 	
				er+=_itoa(pop.pp, buf,10);
			}
	CString str;
    str+=(char)0x0d; str+=(char)0x0a;
    ed->ReplaceSel(str);
	SetDlgItemText(IDC_STATUS, er);
}



void CAlggenDlg::OnMergePopulations() 
{ int length0=pop.prg.size();
	char buf[256];
	CFileDialog fd(TRUE,"pop","*.pop");
	GetCurrentDirectory(256,buf);
	fd.m_ofn.lpstrInitialDir=buf;
	CString File;
	int res=fd.DoModal();
	if(res==IDOK)
	{
		File=fd.GetPathName();
	}
	else return;
	CFile f(File, CFile::modeRead | CFile::typeBinary);
	CArchive ar(&f,CArchive::load);
	population pop_add; 
	pop_add.load_population(ar);
	int length1=pop_add.prg.size();
	ar.Close();
	f.Close();
//  pop.prg.merge(pop_add.prg);
//	vector<program> tmp(pop.prg.size()+pop_add.prg.size());
//	std::merge(pop.prg.begin(),pop.prg.end(),pop_add.prg.begin(),pop_add.prg.end(),tmp.begin(),less_sz<program>());
// pop.prg.resize(tmp.size());
// pop.prg.assign(tmp.begin(),tmp.end());
	merge(pop.prg, pop_add.prg);
  CString msg="The populations are merged.";msg+=(char)0x0d; msg+=(char)0x0a;
	msg+="Original length  "; 	msg+=_itoa(length0, buf,10);msg+=(char)0x0d; msg+=(char)0x0a;
	msg+="Addition  "; 	msg+=_itoa(length1, buf,10);msg+=(char)0x0d; msg+=(char)0x0a;
	msg+="Result  "; 	msg+=_itoa(pop.prg.size(), buf,10);msg+=(char)0x0d; msg+=(char)0x0a;
	MessageBox(msg);
  CEdit* ed=(CEdit*)GetDlgItem(IDC_EDIT2);
	ed->SetSel(0, -1);
	ed->Clear();
	pop.prg[pop.pp].listing(ed);
	CString str;
  str+=(char)0x0d; str+=(char)0x0a;
  ed->ReplaceSel(str);
	CString er="Status: Program Listing #"; 	
	er+=_itoa(pop.pp, buf,10);
	SetDlgItemText(IDC_STATUS, er);	
	hist.clear();	
}

void CAlggenDlg::OnRemoveCommand() 
{
	  if(pop.prg.size()==0) { 	MessageBox("No program to edit");	return;}	
		program& p=pop.prg[pop.pp];
		if(p.prog_len==0) {	MessageBox("No commands to remove");return;}	
		p.sign_user();	
		p.prog_len--;
		CEdit* ed=(CEdit*)GetDlgItem(IDC_EDIT2);
		ed->SetSel(0, -1);
		ed->Clear();
		pop.prg[pop.pp].listing(ed);
		CString str;
		str+=(char)0x0d; str+=(char)0x0a;
		ed->ReplaceSel(str);
}


void CAlggenDlg::OnTest() 
{ if(pop.prg.size()==0) {MessageBox("No program to test");	return;}	
	char buf[10];
	program& p=pop.prg[pop.pp];
	p.test();
	CString er="Status:  " ; 	
	if(p.status==0) er+="GOOD, ";
		else if(p.status==1) er+="MEM.ERROR, ";
				else if(p.status==2) er+="CYCLE ERROR, ";	
	er+="QUALITY=";er+=_itoa(p.qual, buf,10);
	er+=", ORIG.QUAL=";er+=_itoa(rule.test_qual, buf,10);
	er+=", MAX.QUAL=";er+=_itoa(rule.max_qual, buf,10);
	SetDlgItemText(IDC_STATUS, er);			
}

void CAlggenDlg::OnTestAll() 
{ char buf[10];
	int size=pop.prg.size(); 	
	vector<program>::iterator ip,ip2;
	ip=pop.prg.begin();	ip2=pop.prg.end();
	int good=0,cycle=0,mem=0;
	DWORD tm1=timeGetTime();
	for(;ip<ip2;ip++) 
	{ 	
		program& p=*ip;
		p.test();
		if(p.status==2) cycle++;
		else if(p.status==0) good++;
	   		 else  mem++;
	}
	DWORD tm2=timeGetTime();  
	CString er="Status:  Time, ms "; 	
	er+=_itoa(tm2-tm1, buf,10);
	er+="  GOOD ";er+=_itoa(good, buf,10);
	er+="  MEM.ERROR ";er+=_itoa(mem, buf,10);
	er+="  CYCLE ERROR ";	er+=_itoa(cycle, buf,10);
	SetDlgItemText(IDC_STATUS, er);	
}


void CAlggenDlg::OnProbSettings() 
{
   CProbSettingsDialog d;
	if(d.DoModal()==IDOK)
	{ int summ=d.m_prob1+d.m_prob2+d.m_prob3+d.m_prob4+d.m_prob5;
		if(summ != 100) 
			{ float k =100./summ;
				d.m_prob1*=k;
				d.m_prob2*=k;
				d.m_prob3*=k;
				d.m_prob4*=k;
				d.m_prob5*=k;
				summ=d.m_prob1+d.m_prob2+d.m_prob3+d.m_prob4+d.m_prob5;
				if(summ != 100) 
					{ d.m_prob5+=100-summ;
					  if(d.m_prob5 < 0) {d.m_prob1=d.m_prob2=d.m_prob3=d.m_prob4=d.m_prob5=20;}
					}	 
		}
		maps::prob[0]=d.m_prob1;maps::prob[1]=d.m_prob2;maps::prob[2]=d.m_prob3;maps::prob[3]=d.m_prob4;maps::prob[4]=d.m_prob5;
		maps::init_map();
	}

}


void CAlggenDlg::OnViewCpu() 
{
	if(pop.prg.size()==0) {MessageBox("Missing programs");	return;}	
	CViewCPUDialog d;	
	d.DoModal();
}


void CAlggenDlg::OnListProgram() 
{	if(pop.prg.size()==0) {MessageBox("Missing programs");	return;}	
	CListProgramDialog d;	
	d.DoModal();
}


void CAlggenDlg::OnSortPopulation() 
{  	char buf[16];
		CString er="Please wait..."; 
		SetDlgItemText(IDC_STATUS, er);	
		vector<program>::iterator ip,ip2;
		DWORD tm1=timeGetTime();   
		ip=pop.prg.begin();	ip2=pop.prg.end();   
		sort(ip,ip2);
		if(rem)
		{ vector<program>::iterator end;
		  end=unique(pop.prg.begin(),pop.prg.end(),pr_eq<program>());
			pop.prg.erase(end,pop.prg.end());
		}
		DWORD tm2=timeGetTime();  
		er="Status: Population sorted. Time, ms "; 	
		er+=_itoa(tm2-tm1, buf,10);
		SetDlgItemText(IDC_STATUS, er);	
		pop.pp=0;
}


void CAlggenDlg::OnClearPop() 
{   char buf[10]; 
		ClearPodDialog d;
		if(d.DoModal()==IDOK)
			{	DWORD tm1=timeGetTime();  
				vector<program>::iterator ip=pop.prg.begin();
				while(ip != pop.prg.end())
					{program& p=*ip;
						if(p.status< 3 && p.qual< d.m_qual) ip=pop.prg.erase(ip);
						else ++ip;
					}
				DWORD tm2=timeGetTime(); 
				CString er="Status: Deletion completed. Time, ms "; 	
				er+=_itoa(tm2-tm1, buf,10);
				SetDlgItemText(IDC_STATUS, er);	
//				 pop.ip=pop.prg.begin();
				 pop.pp=0;
			}
}


void CAlggenDlg::OnOpt() 
{
  CDialogOpt d;
	if(d.DoModal()==IDOK)
	{ 
		opt=d.opt;
		rem=d.m_rem;
		labels=d.m_labels;
		array=d.m_array;
	}	
}


void CAlggenDlg::sort(vector<program>::iterator& ip, vector<program>::iterator& ip2)
{ 
	if(rem)
		{
			if(opt==1)	stable_sort(ip,ip2, less_sz_un<program>());
			else if(opt==2) stable_sort(ip,ip2, great_sp_un<program>());
		 		 else stable_sort(ip,ip2, great_qu_un<program>());
		}
	else
		{	if(opt==1)	stable_sort(ip,ip2, less_sz<program>());
			else if(opt==2) stable_sort(ip,ip2, great_sp<program>());
				 else stable_sort(ip,ip2, great_qu<program>());
		}
}

void CAlggenDlg::merge(vector<program>& pop, vector<program>& pop2)
{
	vector<program> tmp(pop.size()+pop2.size());
	if(opt==1) std::merge(pop.begin(),pop.end(),pop2.begin(),pop2.end(),tmp.begin(),less_sz<program>());
	else if(opt==2) std::merge(pop.begin(),pop.end(),pop2.begin(),pop2.end(),tmp.begin(),great_sp<program>());
		 else std::merge(pop.begin(),pop.end(),pop2.begin(),pop2.end(),tmp.begin(),great_qu<program>());
	pop.resize(tmp.size());
	pop.assign(tmp.begin(),tmp.end());
}


void CAlggenDlg::OnCut() 
{
	char buf[10]; 
	ClearPodDialog d;
	d.cut=true;
	if(d.DoModal()==IDOK)
			{	
				DWORD tm1=timeGetTime();  
				pop.prg.erase(pop.prg.begin()+d.m_qual,pop.prg.end());									
				DWORD tm2=timeGetTime(); 
				CString er="Status: Deletion completed. Time, ms "; 	
				er+=_itoa(tm2-tm1, buf,10);
				SetDlgItemText(IDC_STATUS, er);	
				pop.pp=0;
			}
}



void CAlggenDlg::OnNull() 
{		
	char buf[10]; 
	DWORD tm1=timeGetTime();  
	pop.prg.clear();									
	DWORD tm2=timeGetTime(); 
	CString er="Status: Clearing completed. Time, ms "; 	
	er+=_itoa(tm2-tm1, buf,10);
	SetDlgItemText(IDC_STATUS, er);	
	CEdit* ed=(CEdit*)GetDlgItem(IDC_EDIT2);ed->SetSel(0, -1);ed->Clear();
	CString str;str+=(char)0x0d; str+=(char)0x0a;ed->ReplaceSel(str);
	pop.pp=0;
	hist.clear();
}


void CAlggenDlg::OnMutation() 
{		CMutDialog d;
		if(d.DoModal()==IDOK)
		{int summ=d.m_mut1+d.m_mut2+d.m_mut3+d.m_mut4+d.m_mut5;
			if(summ != 100) 
			{ float k =100./summ;
				d.m_mut1*=k;
				d.m_mut2*=k;
				d.m_mut3*=k;
				d.m_mut4*=k;
				d.m_mut5*=k;
				summ=d.m_mut1+d.m_mut2+d.m_mut3+d.m_mut4+d.m_mut5;
				if(summ != 100) 
					{ d.m_mut5+=100-summ;
					  if(d.m_mut5 < 0) {d.m_mut1=d.m_mut2=d.m_mut3=d.m_mut4=d.m_mut5=20;}
					}	 
			}
			maps::mut[0]=d.m_mut1;maps::mut[1]=d.m_mut2;maps::mut[2]=d.m_mut3;maps::mut[3]=d.m_mut4;maps::mut[4]=d.m_mut5;
		
			summ=d.m_mut_x1+d.m_mut_x2+d.m_mut_x3+d.m_mut_x4+d.m_mut_x5;	
			if(summ != 100) 
			{ float k =100./summ;
				d.m_mut_x1*=k;
				d.m_mut_x2*=k;
				d.m_mut_x3*=k;
				d.m_mut_x4*=k;
				d.m_mut_x5*=k;
				summ=d.m_mut_x1+d.m_mut_x2+d.m_mut_x3+d.m_mut_x4+d.m_mut_x5;
				if(summ != 100) 
					{ d.m_mut_x5+=100-summ;
					  if(d.m_mut_x5 < 0) {d.m_mut_x1=d.m_mut_x2=d.m_mut_x3=d.m_mut_x4=d.m_mut_x5=20;}
					}	 
			}
			maps::mut_x[0]=d.m_mut_x1;maps::mut_x[1]=d.m_mut_x2;maps::mut_x[2]=d.m_mut_x3;maps::mut_x[3]=d.m_mut_x4;maps::mut_x[4]=d.m_mut_x5;
		
		maps::init_map();
		}
}


void CAlggenDlg::OnSaveConfig() 
{
	char buf[256];
	CFileDialog fd(FALSE,"set","*.set");
	GetCurrentDirectory(256,buf);
	fd.m_ofn.lpstrInitialDir=buf;
	CString File;
	if(fd.DoModal()==IDOK)
	{
		File=fd.GetPathName();
	}
	else return;
	CFile mf(File,CFile::modeCreate | CFile::modeWrite);
	CArchive ar(&mf,CArchive::store);
	ar<<	maps::program_length << maps::pop_length; 
	ar << maps::prob[0] <<  maps::prob[1] << maps::prob[2] << maps::prob[3] << maps::prob[4];
	ar << maps::mut[0] << maps::mut[1] << maps::mut[2] << maps::mut[3] << maps::mut[4]; 
	ar << maps::mut_x[0] << maps::mut_x[1] << maps::mut_x[2] << maps::mut_x[3] << maps::mut_x[4]; 
	ar << opt << rem << labels<<array;
	ar.Flush();
	mf.Close();
}


void CAlggenDlg::OnLoadConfig() 
{	char buf[256];
	CFileDialog fd(TRUE,"set","*.set");
	GetCurrentDirectory(256,buf);
	fd.m_ofn.lpstrInitialDir=buf;
	CString File;
	if(fd.DoModal()==IDOK)
	{
		File=fd.GetPathName();
	}
	else return;
	CFile mf;
	if(!mf.Open(File,CFile::modeRead)) return;
	CArchive ar(&mf,CArchive::load);
	ar >>	maps::program_length >> maps::pop_length; 
	ar >> maps::prob[0] >>  maps::prob[1] >> maps::prob[2] >> maps::prob[3] >> maps::prob[4];
	ar >> maps::mut[0] >> maps::mut[1] >> maps::mut[2] >> maps::mut[3] >> maps::mut[4]; 
	ar >> maps::mut_x[0] >> maps::mut_x[1] >> maps::mut_x[2] >> maps::mut_x[3] >> maps::mut_x[4]; 
	ar >> opt >> rem >> labels>>array;
	ar.Flush();
	mf.Close();
}

void CAlggenDlg::OnHist() 
{ char buf[16];
	if(hist.size()==0) 
			{ 	SetDlgItemText(IDC_STATUS, "Status: Report is empty");
					return;		
			}
	pop.prg.resize(hist.size());
	pop.prg.assign(hist.begin(),hist.end());
	hist.clear();
  CEdit* ed=(CEdit*)GetDlgItem(IDC_EDIT2);
	ed->SetSel(0, -1);
	ed->Clear();
	pop.pp=0;
	pop.prg[pop.pp].listing(ed);
	CString er="Status: Program Listing #"; 	
	er+=_itoa(pop.pp, buf,10);
	SetDlgItemText(IDC_STATUS, er);	
}

void CAlggenDlg::OnPrev() 
{
	char buf[10];  
  CEdit* ed=(CEdit*)GetDlgItem(IDC_EDIT2);
	ed->SetSel(0, -1);
	ed->Clear();
	if(pop.prg.size()==0) 
			{ 	SetDlgItemText(IDC_STATUS, "Status: No programs");
					return;		
			}
	pop.pp--;
	if(pop.pp < 0)  pop.pp= pop.prg.size()-1; 
	program& p=	pop.prg[pop.pp];
	p.listing(ed);
	CString str;
	 str+=(char)0x0d; str+=(char)0x0a;
	 ed->ReplaceSel(str);
	CString er="Status: Program Listing #"; 	
	er+=_itoa(pop.pp, buf,10);
	SetDlgItemText(IDC_STATUS, er);	

}


void CAlggenDlg::OnSelecttest() 
{
	CDLLDialog d;
	if(d.DoModal()==IDOK)
	{	
		dll_ind=d.n;
		hDll=dlls[dll_ind];
		check_alg=(int (__stdcall *)(struct base_check *))GetProcAddress(hDll, MAKEINTRESOURCE(2));
		get_max_qual=(int (__stdcall *)(void))GetProcAddress(hDll,MAKEINTRESOURCE(4));
		get_base_qual=(int (__stdcall *)(void))GetProcAddress(hDll,MAKEINTRESOURCE(3));
		get_reg_one=(int* (__stdcall *)(void))GetProcAddress(hDll,MAKEINTRESOURCE(6));
		get_mem_one=(int* (__stdcall *)(void))GetProcAddress(hDll,MAKEINTRESOURCE(5));
	    init_array=(void (__stdcall *)(void))GetProcAddress(hDll,MAKEINTRESOURCE(7));
		about=(const char* (__stdcall *)(void))GetProcAddress(hDll,MAKEINTRESOURCE(1));
		rule.init_rules();
	}
}


/////////////////////////////////////////////////////////////////////////////
// CProgramLengthDialog dialog

CProgramLengthDialog::CProgramLengthDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CProgramLengthDialog::IDD, pParent)
{ 
	//{{AFX_DATA_INIT(CProgramLengthDialog)
	m_pl = maps::program_length;
	m_popl = maps::pop_length;
	//}}AFX_DATA_INIT
}


void CProgramLengthDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgramLengthDialog)
	DDX_Text(pDX, IDC_EDIT1, m_pl);
	DDV_MinMaxInt(pDX, m_pl, 1, MAX_PROGRAM_LENGTH);
	DDX_Text(pDX, IDC_EDIT2, m_popl);
	DDV_MinMaxInt(pDX, m_popl, 1, 1000000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgramLengthDialog, CDialog)
	//{{AFX_MSG_MAP(CProgramLengthDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgramLengthDialog message handlers
/////////////////////////////////////////////////////////////////////////////
// CEditProgramDialog dialog


CEditProgramDialog::CEditProgramDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEditProgramDialog::IDD, pParent)
{ 
	ind_comm=0;
	ind_op1=0;
	ind_op2=0;
	//{{AFX_DATA_INIT(CEditProgramDialog)
	m_label = 0;
	//}}AFX_DATA_INIT
}


void CEditProgramDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditProgramDialog)
	DDX_Text(pDX, IDC_EDIT1, m_label);
	DDV_MinMaxInt(pDX, m_label, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditProgramDialog, CDialog)
	//{{AFX_MSG_MAP(CEditProgramDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_LBN_SELCHANGE(IDC_LIST2, OnSelchangeList2)
	ON_LBN_SELCHANGE(IDC_LIST3, OnSelchangeList3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditProgramDialog message handlers

BOOL CEditProgramDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CListBox* lb=(CListBox*)GetDlgItem(IDC_LIST1);
	lb->AddString("op1=op2"); // mov
	lb->AddString("op1=op1+op2"); // add
	lb->AddString("op1=op1-op2"); // sub
	lb->AddString("xchg(op1,op2)"); //xchg
	lb->AddString("if(op1<op2)"); //  jl
	lb->AddString("if(op1>op2)");	// jg	
	lb->AddString("if(op1<=op2)"); // jle
	lb->AddString("if(op1>=op2)"); //  jge
	lb->AddString("nop");	//		
	lb->SetCurSel(0);
	lb=(CListBox*)GetDlgItem(IDC_LIST2);
	lb->AddString("A");lb->AddString("B");lb->AddString("C");lb->AddString("D");lb->AddString("E");					
	lb->AddString("array[A]");lb->AddString("array[B]");lb->AddString("array[C]");lb->AddString("array[D]");	lb->AddString("array[E]");
	lb->SetCurSel(0);
	lb=(CListBox*)GetDlgItem(IDC_LIST3);
	lb->AddString("A");lb->AddString("B");lb->AddString("C");lb->AddString("D");lb->AddString("E");					
	lb->AddString("array[A]");lb->AddString("array[B]");lb->AddString("array[C]");lb->AddString("array[D]");lb->AddString("array[E]");lb->AddString("0");lb->AddString("1");
	lb->SetCurSel(0);
	return TRUE; 
}


void CEditProgramDialog::OnSelchangeList1() 
{
	CListBox* lb=(CListBox*)GetDlgItem(IDC_LIST1);
	int n=lb->GetCurSel();
    ind_comm=n;
	switch(n)
		{ case 0: //MOV
			lb=(CListBox*)GetDlgItem(IDC_LIST2); lb->ResetContent();
			lb->AddString("A");lb->AddString("B");lb->AddString("C");lb->AddString("D");lb->AddString("E");					
			lb->AddString("array[A]");lb->AddString("array[B]");lb->AddString("array[C]");lb->AddString("array[D]");	lb->AddString("array[E]");
			lb=(CListBox*)GetDlgItem(IDC_LIST3);lb->ResetContent();
			lb->AddString("A");lb->AddString("B");lb->AddString("C");lb->AddString("D");lb->AddString("E");					
			lb->AddString("array[A]");lb->AddString("array[B]");lb->AddString("array[C]");lb->AddString("array[D]");	lb->AddString("array[E]");lb->AddString("0");lb->AddString("1");
			break;
			case 1: //ADD
			case 2: //SUB
			lb=(CListBox*)GetDlgItem(IDC_LIST2); lb->ResetContent();
			lb->AddString("A");lb->AddString("B");lb->AddString("C");lb->AddString("D");lb->AddString("E");					
			lb=(CListBox*)GetDlgItem(IDC_LIST3); lb->ResetContent(); 
			lb->AddString("A");lb->AddString("B");lb->AddString("C");lb->AddString("D");lb->AddString("E");					
			lb->AddString("1");
			break;
			case 3: //XCHG
			lb=(CListBox*)GetDlgItem(IDC_LIST2); lb->ResetContent();
			lb->AddString("A");lb->AddString("B");lb->AddString("C");lb->AddString("D");lb->AddString("E");					
			lb->AddString("array[A]");lb->AddString("array[B]");lb->AddString("array[C]");lb->AddString("array[D]");	lb->AddString("array[E]");
			lb=(CListBox*)GetDlgItem(IDC_LIST3);lb->ResetContent();
			lb->AddString("A");lb->AddString("B");lb->AddString("C");lb->AddString("D");lb->AddString("E");					
			lb->AddString("array[A]");lb->AddString("array[B]");lb->AddString("array[C]");lb->AddString("array[D]");lb->AddString("array[E]");
		  break;
			case 4: //J...
			case 5:
			case 6:
			case 7:
			lb=(CListBox*)GetDlgItem(IDC_LIST2); lb->ResetContent();
			lb->AddString("A");lb->AddString("B");lb->AddString("C");lb->AddString("D");lb->AddString("E");					
			lb->AddString("array[A]");lb->AddString("array[B]");lb->AddString("array[C]");lb->AddString("array[D]");	lb->AddString("array[E]");
			lb=(CListBox*)GetDlgItem(IDC_LIST3);lb->ResetContent();
			lb->AddString("A");lb->AddString("B");lb->AddString("C");lb->AddString("D");lb->AddString("E");					
			lb->AddString("array[A]");lb->AddString("array[B]");lb->AddString("array[C]");lb->AddString("array[D]");lb->AddString("array[E]");
			lb->AddString("0");lb->AddString("1");
			break;		
			case 8: //NOP
			lb=(CListBox*)GetDlgItem(IDC_LIST2); lb->ResetContent();
			lb=(CListBox*)GetDlgItem(IDC_LIST3);lb->ResetContent();
		}
		lb=(CListBox*)GetDlgItem(IDC_LIST2); 
	 	lb->SetCurSel(0);
		lb=(CListBox*)GetDlgItem(IDC_LIST3); 
		lb->SetCurSel(0);
}

void CEditProgramDialog::OnSelchangeList2() 
{	CListBox* lb=(CListBox*)GetDlgItem(IDC_LIST2);
	ind_op1=lb->GetCurSel();
	if(ind_comm==0) // MOV
		{ lb=(CListBox*)GetDlgItem(IDC_LIST3);
			lb->ResetContent();
			if(ind_op1<5)
				{
				lb->AddString("A");lb->AddString("B");lb->AddString("C");lb->AddString("D");lb->AddString("E");					
				lb->AddString("array[A]");lb->AddString("array[B]");lb->AddString("array[C]");lb->AddString("array[D]");	lb->AddString("array[E]");
				lb->AddString("0");lb->AddString("1");
				}
			else
				{
				lb->AddString("A");lb->AddString("B");lb->AddString("C");lb->AddString("D");lb->AddString("E");					
				lb->AddString("array[A]");lb->AddString("array[B]");lb->AddString("array[C]");lb->AddString("array[D]");	lb->AddString("array[E]");

				}

		}
	else if(ind_comm>3 && ind_comm<8)  // J...
		{ lb=(CListBox*)GetDlgItem(IDC_LIST3);
			lb->ResetContent();
			if(ind_op1<5)
				{
				lb->AddString("A");lb->AddString("B");lb->AddString("C");lb->AddString("D");lb->AddString("E");					
				lb->AddString("array[A]");lb->AddString("array[B]");lb->AddString("array[C]");lb->AddString("array[D]");	lb->AddString("array[E]");
				lb->AddString("0");lb->AddString("1");
				}
			else
				{
				lb->AddString("A");lb->AddString("B");lb->AddString("C");lb->AddString("D");lb->AddString("E");					
				lb->AddString("array[A]");lb->AddString("array[B]");lb->AddString("array[C]");lb->AddString("array[D]");	lb->AddString("array[E]");

				}

		}
		lb=(CListBox*)GetDlgItem(IDC_LIST3); 
		lb->SetCurSel(0);
}

void CEditProgramDialog::OnSelchangeList3() 
{
	CListBox* lb=(CListBox*)GetDlgItem(IDC_LIST3);
	ind_op2=lb->GetCurSel();
	
}

/////////////////////////////////////////////////////////////////////////////
// CProbSettingsDialog dialog


CProbSettingsDialog::CProbSettingsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CProbSettingsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProbSettingsDialog)
	m_prob1 = maps::prob[0];
	m_prob2 = maps::prob[1];
	m_prob3 = maps::prob[2];
	m_prob4 = maps::prob[3];
	m_prob5 = maps::prob[4];
	//}}AFX_DATA_INIT
}


void CProbSettingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProbSettingsDialog)
	DDX_Text(pDX, IDC_EDIT1, m_prob1);
	DDV_MinMaxInt(pDX, m_prob1, 0, 100);
	DDX_Text(pDX, IDC_EDIT3, m_prob2);
	DDV_MinMaxInt(pDX, m_prob2, 0, 100);
	DDX_Text(pDX, IDC_EDIT4, m_prob3);
	DDV_MinMaxInt(pDX, m_prob3, 0, 100);
	DDX_Text(pDX, IDC_EDIT5, m_prob4);
	DDV_MinMaxInt(pDX, m_prob4, 0, 100);
	DDX_Text(pDX, IDC_EDIT6, m_prob5);
	DDV_MinMaxInt(pDX, m_prob5, 0, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProbSettingsDialog, CDialog)
	//{{AFX_MSG_MAP(CProbSettingsDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProbSettingsDialog message handlers
/////////////////////////////////////////////////////////////////////////////
// CViewCPUDialog dialog


CViewCPUDialog::CViewCPUDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CViewCPUDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewCPUDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CViewCPUDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewCPUDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewCPUDialog, CDialog)
	//{{AFX_MSG_MAP(CViewCPUDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewCPUDialog message handlers

BOOL CViewCPUDialog::OnInitDialog() 
{
	int *a=get_mem_one();
	int *r=get_reg_one();
	program& p=pop.prg[pop.pp];
	p.test();
	CDialog::OnInitDialog();
	CString er="  0-7";
  CString buf;
	for(int i=0;i<8;i++)
		{ er+="   ";
			buf.Format("%05i",a[i]);
			er+=buf;
		}
	SetDlgItemText(IDC_STATIC_MEM1, er);
	er="8-15";
	for(int i=8;i<16;i++)
		{ er+="   ";
			buf.Format("%05i",a[i]);
			er+=buf;
		}
	SetDlgItemText(IDC_STATIC_MEM2, er);	
	er="Registers:";
	for(int i=0;i<5;i++)
		{ er+="   ";
			buf.Format("%c",'A'+i);	er+=buf;er+='=';
			buf.Format("%05i",r[i]);er+=buf;
		}
	SetDlgItemText(IDC_STATIC_REG1, er);
	er="  0-7";
	for(int i=0;i<8;i++)
		{ er+="   ";
			buf.Format("%05i",prc.a[i]);
			er+=buf;
		}
		SetDlgItemText(IDC_STATIC_MEM3, er);
		er="8-15";
	for(int i=8;i<16;i++)
		{ er+="   ";
			buf.Format("%05i",prc.a[i]);
			er+=buf;
		}
	SetDlgItemText(IDC_STATIC_MEM4, er);	
	er="Registers:";
	for(int i=0;i<5;i++)
		{ er+="   ";
			buf.Format("%c",'A'+i);	er+=buf;er+='=';
			buf.Format("%05i",prc.r[i]);er+=buf;
		}
	SetDlgItemText(IDC_STATIC_REG2, er);
	char buf2[10];
	er="Status:  " ; 	
	if(p.status==0) er+="GOOD, ";
		else if(p.status==1) er+="MEM.ERROR, ";
				else if(p.status==2) er+="CYCLE ERROR, ";	
	er+="QUALITY=";er+=_itoa(p.qual, buf2,10);
	er+=", ORIG.QUAL=";er+=_itoa(rule.test_qual, buf2,10);
	er+=", MAX.QUAL=";er+=_itoa(rule.max_qual, buf2,10);	
	SetDlgItemText(IDC_STATIC_STATUS, er);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
// CListProgramDialog dialog


CListProgramDialog::CListProgramDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CListProgramDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListProgramDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CListProgramDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListProgramDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListProgramDialog, CDialog)
	//{{AFX_MSG_MAP(CListProgramDialog)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, OnKeydownList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListProgramDialog message handlers

BOOL CListProgramDialog::OnInitDialog() 
{ vector<program>::iterator ip;
	char buf[10];
	CDialog::OnInitDialog();
	CListCtrl* lc=(CListCtrl*)GetDlgItem(IDC_LIST1);
	lc->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	lc->InsertColumn(0,"№", LVCFMT_LEFT,60);
	lc->InsertColumn(1,"Signature", LVCFMT_LEFT,80);
	lc->InsertColumn(2,"Age", LVCFMT_LEFT,60);
	lc->InsertColumn(3,"Status", LVCFMT_LEFT,60);
	lc->InsertColumn(4,"Quality", LVCFMT_LEFT,60);
	lc->InsertColumn(5,"Delta Q", LVCFMT_LEFT,50);
	lc->InsertColumn(6,"Length", LVCFMT_LEFT,50);
	lc->InsertColumn(7,"Takts", LVCFMT_LEFT,50);
	int size=pop.prg.size(); 	
	ip=pop.prg.begin();	
	int good=0,cycle=0,mem=0;
	int n;
	if(size <1000) n =size;
	else n=1000;
	for(int i=0;i<n;i++,ip++)  // Cycle of 1000 programs
	{ 	
		program& p=*ip;
		lc->InsertItem(i,0,0);
		lc->SetItemText(i,0,_itoa(i,buf,10));	
		lc->SetItemText(i,1,p.sign);
		lc->SetItemText(i,2,_itoa(p.age,buf,10));
		lc->SetItemText(i,6,_itoa(p.prog_len,buf,10));
		if(p.status==2) lc->SetItemText(i,3,"Cycle");
		else if(p.status==0) lc->SetItemText(i,3,"Good");
	  		 else  if(p.status==1) lc->SetItemText(i,3,"Memory");
			   	   else lc->SetItemText(i,3,"n/a");
		if(p.status<3) 
		{
			lc->SetItemText(i,4, _itoa(p.qual,buf,10));
			lc->SetItemText(i,5, _itoa(p.qual-rule.test_qual,buf,10)); 
			lc->SetItemText(i,7,_itoa(p.takt,buf,10));
		}
		else 
		{
			lc->SetItemText(i,4,"n/a");
			 lc->SetItemText(i,5,"n/a");
			 lc->SetItemText(i,7,"n/a");
		}
	}
	CComboBox* cb=(CComboBox*)GetDlgItem(IDC_COMBO1);
	CString buf2; int i=0;
	while(i < size)
		{	
			buf2.Format("%04u", i); 
			cb->AddString(buf2);
			i+=1000;		
		}
	cb->SetCurSel(0);
	return TRUE;  
}


void CListProgramDialog::OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult) 
{ 
	char buf[10];
	vector<program>::iterator ip;
	CListCtrl* lc=(CListCtrl*)GetDlgItem(IDC_LIST1);
    int n;
	int k=lc->GetSelectionMark();
	if(k==-1) return;
	CComboBox* cb=(CComboBox*)GetDlgItem(IDC_COMBO1);
	int m=cb->GetCurSel() * 1000;
	n=m+k;  
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	if(pLVKeyDow->wVKey == VK_DELETE)
	{
		ip=pop.prg.erase(pop.prg.begin()+n);
		pop.pp=n;
		if(ip==pop.prg.end()) 
		{
			pop.pp--;if(pop.pp<0) pop.pp=0;ip=pop.prg.begin()+pop.pp;
		}
		lc->DeleteAllItems();
		n=cb->GetCurSel() * 1000; // Базовый номер для отображения
		int sz=pop.prg.size(); 	
		if(n+1000 < sz) m=n+1000;   // Финальный номер ++
		else m=sz;
    	ip=pop.prg.begin()+n;	
		for(int i=n,j=0;i<m;i++,ip++,j++)  //Цикл по 1000 программ
		{ 	
			program& p=*ip;
			lc->InsertItem(j,0,0);
			lc->SetItemText(j,0,_itoa(i,buf,10));	
			lc->SetItemText(j,1,p.sign);
			lc->SetItemText(j,2,_itoa(p.age,buf,10));
			lc->SetItemText(j,6,_itoa(p.prog_len,buf,10));
			if(p.status==2) lc->SetItemText(j,3,"Cycle");
			else if(p.status==0) lc->SetItemText(j,3,"Good");
				 else  if(p.status==1) lc->SetItemText(j,3,"Memory");
				       else lc->SetItemText(j,3,"n/a");
			if(p.status<3) 
			{
				lc->SetItemText(j,4, _itoa(p.qual,buf,10));
				lc->SetItemText(j,5, _itoa(p.qual-rule.test_qual,buf,10)); 
				lc->SetItemText(j,7,_itoa(p.takt,buf,10));
			}
			else 
			{
				lc->SetItemText(j,4,"n/a");
				lc->SetItemText(j,5,"n/a");
				lc->SetItemText(j,7,"n/a");
			}
		}
	 }
	*pResult = 0;
}

void CListProgramDialog::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{	char buf[10];
	vector<program>::iterator ip;
	CListCtrl* lc=(CListCtrl*)GetDlgItem(IDC_LIST1);
    int n=lc->GetSelectionMark();
	if(n==-1) return;
	CComboBox* cb=(CComboBox*)GetDlgItem(IDC_COMBO1);
    CEdit* ed=(CEdit*)(mainDlg->GetDlgItem(IDC_EDIT2));
	ed->SetSel(0, -1);
	ed->Clear();
	int m=cb->GetCurSel() * 1000;
	n+=m; 
	pop.pp=n;
	ip=pop.prg.begin()+n;
	program& p=	*ip;
	p.listing(ed);
	CString str;
	str+=(char)0x0d; str+=(char)0x0a;
	ed->ReplaceSel(str);
	CString er="Status: Program Listing #"; 	
	er+=_itoa(pop.pp, buf,10);
	mainDlg->SetDlgItemText(IDC_STATUS, er);		
	EndDialog(0);
	*pResult = 0;
}

void CListProgramDialog::OnSelchangeCombo1() 
{ char buf[10];
	CListCtrl* lc=(CListCtrl*)GetDlgItem(IDC_LIST1);
	lc->DeleteAllItems();
	CComboBox* cb=(CComboBox*)GetDlgItem(IDC_COMBO1);
	int n=cb->GetCurSel() * 1000; // Base number to display
	int m; 
	int sz=pop.prg.size(); 	
	if(n+1000 < sz) m=n+1000;   // Final number 
	else m=sz;           
	vector<program>::iterator ip;
	ip=pop.prg.begin()+n;	
	for(int i=n,j=0;i<m;i++,ip++,j++)  // Cycle of 1000 programs
		{ 	program& p=*ip;
			lc->InsertItem(j,NULL,0);
			lc->SetItemText(j,0,_itoa(i,buf,10));	
			lc->SetItemText(j,1,p.sign);
			lc->SetItemText(j,2,_itoa(p.age,buf,10));
			lc->SetItemText(j,6,_itoa(p.prog_len,buf,10));
			if(p.status==2) lc->SetItemText(j,3,"Cycle");
			else if(p.status==0) lc->SetItemText(j,3,"Good");
			 	 else  if(p.status==1) lc->SetItemText(j,3,"Memory");
					   else lc->SetItemText(j,3,"n/a");
			if(p.status<3) 
			{
				lc->SetItemText(j,4, _itoa(p.qual,buf,10));
				lc->SetItemText(j,5, _itoa(p.qual-rule.test_qual,buf,10)); 
				lc->SetItemText(j,7,_itoa(p.takt,buf,10));
			}
			else 
			{
				lc->SetItemText(j,4,"n/a");
				lc->SetItemText(j,5,"n/a");
				lc->SetItemText(j,7,"n/a");
			}
		}
	
}

/////////////////////////////////////////////////////////////////////////////
// ClearPodDialog dialog


ClearPodDialog::ClearPodDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ClearPodDialog::IDD, pParent)
{	cut=false;
	//{{AFX_DATA_INIT(ClearPodDialog)
	m_qual = rule.test_qual;
	//}}AFX_DATA_INIT
}


void ClearPodDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ClearPodDialog)
	DDX_Text(pDX, IDC_EDIT1, m_qual);
	//}}AFX_DATA_MAP
}

BOOL ClearPodDialog::OnInitDialog() 
{	char buf[10];
	CDialog::OnInitDialog();
	if(cut) 
		{		SetDlgItemText(IDC_EDIT1, _itoa(pop.prg.size(),buf,10));
				SetDlgItemText(IDC_STATIC1, "Delete instances with number above");	
		}
	return TRUE;  
}


BEGIN_MESSAGE_MAP(ClearPodDialog, CDialog)
	//{{AFX_MSG_MAP(ClearPodDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ClearPodDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// CDialogOpt dialog


CDialogOpt::CDialogOpt(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogOpt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogOpt)
	m_rem = mainDlg->rem;
	m_labels = mainDlg->labels;
	m_array = mainDlg->array;
	//}}AFX_DATA_INIT
}


void CDialogOpt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogOpt)
	DDX_Check(pDX, IDC_CHECK1, m_rem);
	DDX_Check(pDX, IDC_LABEL, m_labels);
	DDX_Check(pDX, IDC_ARRAY, m_array);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogOpt, CDialog)
	//{{AFX_MSG_MAP(CDialogOpt)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogOpt message handlers

void CDialogOpt::OnRadio1() 
{
	opt=0;
}


void CDialogOpt::OnRadio2() 
{
 opt=1;	
}

void CDialogOpt::OnRadio3() 
{
	opt=2;	
}
BOOL CDialogOpt::OnInitDialog() 
{
	CDialog::OnInitDialog();
	opt=mainDlg->opt;
	CheckRadioButton( IDC_RADIO1, IDC_RADIO3, IDC_RADIO1+opt );
	return TRUE; 
}


/////////////////////////////////////////////////////////////////////////////
// CMutDialog dialog


CMutDialog::CMutDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMutDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMutDialog)
	m_mut1 = maps::mut[0];
	m_mut2 = maps::mut[1];
	m_mut3 = maps::mut[2];
	m_mut4 = maps::mut[3];
	m_mut5 = maps::mut[4];
	m_mut_x5 = maps::mut_x[4];
	m_mut_x1 = maps::mut_x[0];
	m_mut_x2 = maps::mut_x[1];
	m_mut_x3 = maps::mut_x[2];
	m_mut_x4 = maps::mut_x[3];
	//}}AFX_DATA_INIT
}


void CMutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMutDialog)
	DDX_Text(pDX, IDC_EDIT1, m_mut1);
	DDV_MinMaxInt(pDX, m_mut1, 0, 100);
	DDX_Text(pDX, IDC_EDIT2, m_mut2);
	DDV_MinMaxInt(pDX, m_mut2, 0, 100);
	DDX_Text(pDX, IDC_EDIT3, m_mut3);
	DDV_MinMaxInt(pDX, m_mut3, 0, 100);
	DDX_Text(pDX, IDC_EDIT4, m_mut4);
	DDV_MinMaxInt(pDX, m_mut4, 0, 100);
	DDX_Text(pDX, IDC_EDIT5, m_mut5);
	DDV_MinMaxInt(pDX, m_mut5, 0, 100);
	DDX_Text(pDX, IDC_EDIT10, m_mut_x5);
	DDV_MinMaxInt(pDX, m_mut_x5, 0, 100);
	DDX_Text(pDX, IDC_EDIT6, m_mut_x1);
	DDV_MinMaxInt(pDX, m_mut_x1, 0, 100);
	DDX_Text(pDX, IDC_EDIT7, m_mut_x2);
	DDV_MinMaxInt(pDX, m_mut_x2, 0, 100);
	DDX_Text(pDX, IDC_EDIT8, m_mut_x3);
	DDV_MinMaxInt(pDX, m_mut_x3, 0, 100);
	DDX_Text(pDX, IDC_EDIT9, m_mut_x4);
	DDV_MinMaxInt(pDX, m_mut_x4, 0, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMutDialog, CDialog)
	//{{AFX_MSG_MAP(CMutDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMutDialog message handlers


/////////////////////////////////////////////////////////////////////////////
// CDLLDialog dialog


CDLLDialog::CDLLDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDLLDialog::IDD, pParent)
{		n=0;
	//{{AFX_DATA_INIT(CDLLDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDLLDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLLDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLLDialog, CDialog)
	//{{AFX_MSG_MAP(CDLLDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLLDialog message handlers

BOOL CDLLDialog::OnInitDialog() 
{	CDialog::OnInitDialog();
	CListBox* lb=(CListBox*)GetDlgItem(IDC_LIST1);
	for(int i=0;i< mainDlg->dlls.size();i++) lb->AddString(mainDlg->dlls_s[i]);
	if(mainDlg->dlls.size()) lb->SetCurSel(mainDlg->dll_ind);
	return TRUE; 
}

void CDLLDialog::OnSelchangeList1() 
{	CListBox* lb=(CListBox*)GetDlgItem(IDC_LIST1);
	n=lb->GetCurSel();	
}

