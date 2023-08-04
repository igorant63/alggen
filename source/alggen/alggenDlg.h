// alggenDlg.h : header file
//

#if !defined(AFX_ALGGENDLG_H__77655A67_B4DE_11D8_BFD7_CDA578250C4D__INCLUDED_)
#define AFX_ALGGENDLG_H__77655A67_B4DE_11D8_BFD7_CDA578250C4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAlggenDlg dialog
#include "program.h"

class CAlggenDlg : public CDialog
{
// Construction
public:
	CAlggenDlg(CWnd* pParent = NULL);	// standard constructor
  void BeginTime();
  void EndTime(CString& str);
	void Step();
	void sort(vector<program>::iterator& ip, vector<program>::iterator& ip2);
	void merge(vector<program>& pop, vector<program>& pop2);
	int count_m;
	HANDLE hThread;
	CMenu* pMenu;
	CMenu* nullMenu;
	void runStep() {OnStep();}
  int opt; // Оптимизация: 0 - нет, 1 - скорость, 2 - размер
	BOOL rem; // Оптимизация:  true - удаление дублей
	BOOL labels; // Оптимизация:  true - корректировка меток
	BOOL array; // Обновление тестовых массивов
	HINSTANCE hDll;
	vector<HINSTANCE> dlls;
	CStringArray dlls_s;
	int dll_ind;
	void load_dlls();
	void scan_dlls();
	// Dialog Data
	//{{AFX_DATA(CAlggenDlg)
	enum { IDD = IDD_ALGGEN_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlggenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;


	// Generated message map functions
	//{{AFX_MSG(CAlggenDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	afx_msg void OnSample();
	afx_msg void OnMakePopulation();
	afx_msg void OnSavePopulation();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLoadPopulation();
	afx_msg void OnViewProgram();
	afx_msg void OnPorgramLength();
	afx_msg void OnAddCommand();
	afx_msg void OnMakeProgram();
	afx_msg void OnRemoveProgram();
	afx_msg void OnMergePopulations();
	afx_msg void OnRemoveCommand();
	afx_msg void OnTest();
	afx_msg void OnTestAll();
	afx_msg void OnProbSettings();
	afx_msg void OnViewCpu();
	afx_msg void OnListProgram();
	afx_msg void OnSortPopulation();
	afx_msg void OnClearPop();
	afx_msg void OnStep();
	afx_msg void OnOpt();
	afx_msg void OnCut();
	afx_msg void OnNull();
	afx_msg void OnMutation();
	afx_msg void OnSaveConfig();
	afx_msg void OnLoadConfig();
	afx_msg void OnHist();
	afx_msg void OnPrev();
	afx_msg void OnSelecttest();
	afx_msg void OnAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CAlggenDlg* mainDlg;
/////////////////////////////////////////////////////////////////////////////
// CProgramLengthDialog dialog

class CProgramLengthDialog : public CDialog
{
// Construction
public:
	CProgramLengthDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgramLengthDialog)
	enum { IDD = IDD_PROGRAM_LENGTH };
	int 	m_pl;
	int		m_popl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgramLengthDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgramLengthDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CEditProgramDialog dialog

class CEditProgramDialog : public CDialog
{
// Construction
public:
	CEditProgramDialog(CWnd* pParent = NULL);   // standard constructor
	int ind_comm;
	int ind_op1;
	int ind_op2;
// Dialog Data
	//{{AFX_DATA(CEditProgramDialog)
	enum { IDD = IDD_EDIT_PROGRAM };
	int		m_label;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditProgramDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditProgramDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList1();
	afx_msg void OnSelchangeList2();
	afx_msg void OnSelchangeList3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CProbSettingsDialog dialog

class CProbSettingsDialog : public CDialog
{
// Construction
public:
	CProbSettingsDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProbSettingsDialog)
	enum { IDD = IDD_PROB_SETTINGS };
	int		m_prob1;
	int		m_prob2;
	int		m_prob3;
	int		m_prob4;
	int		m_prob5;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProbSettingsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProbSettingsDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CViewCPUDialog dialog CListCtrl

class CViewCPUDialog : public CDialog
{
// Construction
public:
	CViewCPUDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CViewCPUDialog)
	enum { IDD = IDD_CPU_VIEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewCPUDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CViewCPUDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CListProgramDialog dialog

class CListProgramDialog : public CDialog
{
// Construction
public:
	CListProgramDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CListProgramDialog)
	enum { IDD = IDD_LIST_PROGRAM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListProgramDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CListProgramDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCombo1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// ClearPodDialog dialog

class ClearPodDialog : public CDialog
{
// Construction
public:
	ClearPodDialog(CWnd* pParent = NULL);   // standard constructor
	bool cut;
// Dialog Data
	//{{AFX_DATA(ClearPodDialog)
	enum { IDD = IDD_CLEAR_POP };
	int		m_qual;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ClearPodDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ClearPodDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CDialogOpt dialog

class CDialogOpt : public CDialog
{
// Construction
public:
	CDialogOpt(CWnd* pParent = NULL);   // standard constructor
	int opt;	
// Dialog Data
	//{{AFX_DATA(CDialogOpt)
	enum { IDD = IDD_OPT };
	BOOL	m_rem;
	BOOL	m_labels;
	BOOL	m_array;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogOpt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogOpt)
	afx_msg void OnRadio1();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CMutDialog dialog

class CMutDialog : public CDialog
{
// Construction
public:
	CMutDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMutDialog)
	enum { IDD = IDD_MUTATION };
	int		m_mut1;
	int		m_mut2;
	int		m_mut3;
	int		m_mut4;
	int		m_mut5;
	int		m_mut_x5;
	int		m_mut_x1;
	int		m_mut_x2;
	int		m_mut_x3;
	int		m_mut_x4;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMutDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMutDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CDLLDialog dialog

class CDLLDialog : public CDialog
{
// Construction
public:
	CDLLDialog(CWnd* pParent = NULL);   // standard constructor
	int n;
// Dialog Data
	//{{AFX_DATA(CDLLDialog)
	enum { IDD = IDD_DLL };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLLDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLLDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALGGENDLG_H__77655A67_B4DE_11D8_BFD7_CDA578250C4D__INCLUDED_)
