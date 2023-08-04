// alggen.h : main header file for the ALGGEN application
//

#if !defined(AFX_ALGGEN_H__77655A65_B4DE_11D8_BFD7_CDA578250C4D__INCLUDED_)
#define AFX_ALGGEN_H__77655A65_B4DE_11D8_BFD7_CDA578250C4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAlggenApp:
// See alggen.cpp for the implementation of this class
//

class CAlggenApp : public CWinApp
{
public:
	CAlggenApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlggenApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAlggenApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALGGEN_H__77655A65_B4DE_11D8_BFD7_CDA578250C4D__INCLUDED_)
