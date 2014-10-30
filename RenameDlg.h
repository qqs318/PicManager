#pragma once


// CRenameDlg 对话框

class CRenameDlg : public CDialog
{
	DECLARE_DYNAMIC(CRenameDlg)

public:
	CRenameDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRenameDlg();
	CString m_sNewName;
// 对话框数据
	enum { IDD = IDD_DIALOG_RENAME };
	CRect m_Rect;
	CPoint m_Point;
	void Setpos(CPoint &point);
	void Setpos(CRect &rect);
	void GetNewName(CString& newname);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
