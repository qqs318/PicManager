// RenameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PicManager.h"
#include "RenameDlg.h"
#include "afxdialogex.h"


// CRenameDlg 对话框

IMPLEMENT_DYNAMIC(CRenameDlg, CDialog)

CRenameDlg::CRenameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRenameDlg::IDD, pParent)
{

}

CRenameDlg::~CRenameDlg()
{
}

void CRenameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RENAME, m_sNewName);
}


BEGIN_MESSAGE_MAP(CRenameDlg, CDialog)
END_MESSAGE_MAP()

BOOL CRenameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//MoveWindow(m_Point.x, m_Point.y+15,90,20);
	MoveWindow(m_Rect);
	UpdateData(FALSE);
	return TRUE;
}
void CRenameDlg::Setpos(CPoint &point)
{
	m_Point.x = point.x;
	m_Point.y = point.y;
}
void CRenameDlg::Setpos(CRect &rect)
{
	m_Rect.CopyRect(rect);

}

void CRenameDlg::OnOK()
{
	UpdateData(TRUE);
	CDialog::OnOK();
}


// CRenameDlg 消息处理程序
