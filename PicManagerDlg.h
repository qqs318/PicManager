
// PicManagerDlg.h : 头文件
//

#pragma once
#include "vector"
using namespace std;
struct PicStruckDef
{
	CString m_sPicPath;      		//文件所在文件夹
	CString m_sPicName;
} ;
// PicManagerDlg 对话框
class CPicManager : public CDialog
{
// 构造
public:
	CPicManager(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PICMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual void OnOK();
	virtual void OnCancel();
	void ScanDiskFile(const CString& strPath, int num[4]);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();	
	CRect rect;
	CListCtrl m_cJpgList;
	CListCtrl m_cPngList;
	CListCtrl m_cBmpList;
	CListCtrl m_cGifList;
	int m_Num[4];
	CString m_strPath;
	int m_nCurrentID;
	CPoint m_sCurrentPos;
	PicStruckDef* m_pCurrentPic;
public:
	afx_msg void OnNMDblclkListFilejpg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListFilepng(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListFilegif(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListFilebmp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListFilejpg(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnPicRename();
	afx_msg void OnOpenPath();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonFile();
};
