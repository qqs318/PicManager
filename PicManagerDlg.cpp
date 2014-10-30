
// PicManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PicManager.h"
#include "PicManagerDlg.h"
#include "afxdialogex.h"
#include "RenameDlg.h"
#include "resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// PicManagerDlg 对话框



CPicManager::CPicManager(CWnd* pParent /*=NULL*/)
	: CDialog(CPicManager::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPicManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILEJPG, m_cJpgList);
	DDX_Control(pDX, IDC_LIST_FILEPNG, m_cPngList);
	DDX_Control(pDX, IDC_LIST_FILEBMP, m_cBmpList);
	DDX_Control(pDX, IDC_LIST_FILEGIF, m_cGifList);
}

BEGIN_MESSAGE_MAP(CPicManager, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILEJPG, OnNMDblclkListFilejpg)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILEPNG, OnNMDblclkListFilepng)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILEGIF, &CPicManager::OnNMDblclkListFilegif)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILEBMP, &CPicManager::OnNMDblclkListFilebmp)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FILEJPG, &CPicManager::OnNMRClickListFilejpg)
	ON_COMMAND( ID_RENAME, OnPicRename)
	ON_COMMAND(ID_OPENPATH, OnOpenPath)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CPicManager::OnBnClickedButtonFile)
END_MESSAGE_MAP()


// PicManagerDlg 消息处理程序

void CPicManager::OnPicRename()
{
	if (m_pCurrentPic == NULL) return;
	CRenameDlg dlg;	
	//dlg.Setpos(m_sCurrentPos);
	dlg.Setpos(rect);
	dlg.DoModal();

	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE); 
	if (dlg.m_sNewName == "")
	{
		return;
	}	 

	CString tmp = m_pCurrentPic->m_sPicPath+m_pCurrentPic->m_sPicName;
	char* source = tmp.GetBuffer(tmp.GetLength());
	CString tmp1= m_pCurrentPic->m_sPicPath + dlg.m_sNewName + ".jpg";
	char* newname = tmp1.GetBuffer(tmp1.GetLength());
	int i = 1;
	while (rename(source,newname)&&(i<100))
	{
		CString num ;
		num.Format("(%d)", i);
		tmp1 = m_pCurrentPic->m_sPicPath + dlg.m_sNewName + num + ".jpg";
		newname = tmp1.GetBuffer(tmp1.GetLength());
		i++;
	}
}

void CPicManager::OnOpenPath()
{
	if (m_pCurrentPic == NULL) return;
	CString path = _T("explorer.exe ") + m_pCurrentPic->m_sPicPath;
	int length = path.GetLength();
	char* szCommandLine = path.GetBuffer(length);
	STARTUPINFOA si = {sizeof(si)};
	PROCESS_INFORMATION pi;
	::CreateProcessA(NULL,szCommandLine,NULL,NULL,FALSE,NULL,NULL,NULL,&si,&pi);
	m_pCurrentPic = NULL;
}

BOOL CPicManager::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_cJpgList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_SINGLESEL & ~LVS_EX_CHECKBOXES);
	m_cBmpList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_SINGLESEL & ~LVS_EX_CHECKBOXES);
	m_cPngList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_SINGLESEL & ~LVS_EX_CHECKBOXES);
	m_cGifList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_SINGLESEL & ~LVS_EX_CHECKBOXES);

	m_cJpgList.InsertColumn(0, _T("ID"),LVCFMT_LEFT,45,-1);
	m_cJpgList.InsertColumn(1, _T("名称"),LVCFMT_LEFT,330,-1);
	m_cJpgList.DeleteAllItems();

	m_cBmpList.InsertColumn(0, _T("ID"),LVCFMT_LEFT,45,-1);
	m_cBmpList.InsertColumn(1, _T("名称"),LVCFMT_LEFT,330,-1);
	m_cBmpList.DeleteAllItems();

	m_cPngList.InsertColumn(0, _T("ID"),LVCFMT_LEFT,45,-1);
	m_cPngList.InsertColumn(1, _T("名称"),LVCFMT_LEFT,330,-1);
	m_cPngList.DeleteAllItems();

	m_cGifList.InsertColumn(0, _T("ID"),LVCFMT_LEFT,45,-1);
	m_cGifList.InsertColumn(1, _T("名称"),LVCFMT_LEFT,330,-1);
	m_cGifList.DeleteAllItems();
	m_Num[0] = 0;
	m_Num[1] = 0;
	m_Num[2] = 0;
	m_Num[3] = 0;
	//CString strFilePath = _T("C:\\Users\\QQS\\Pictures\\");
	//int num[4] = {0,0,0,0};
	//ScanDiskFile(strFilePath,num);
	CString strFilePath = _T("F:\\我的图片");
	ScanDiskFile(strFilePath,m_Num);
	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CPicManager::ScanDiskFile(const CString& strPath, int num[4])
{
	CFileFind find;
	CString strTemp = strPath;
	CString strDirectory = strPath + _T("\\") + _T("\\*.*");
	CString strFileName, strFilePath;
	BOOL IsFind = find.FindFile(strDirectory);
	while(IsFind )
	{
		IsFind=find.FindNextFile();
		if(find.IsDots())
			continue;
		else if(find.IsDirectory())
		{
			strFileName = find.GetFileName();
			strTemp = strPath + _T("\\") + strFileName;
			ScanDiskFile(strTemp, num);
		}
		else
		{            
			strFileName = find.GetFileName();
			int pos = strFileName.ReverseFind('.') ;
			CString filename = strFileName.Mid(pos+1);
			filename.MakeLower();

			if (filename == "jpg")
			{
				PicStruckDef* picstruck = new PicStruckDef;
				strFilePath = strPath + _T("\\") + strFileName;
				picstruck->m_sPicPath = strPath;
				picstruck->m_sPicName = strFileName;

				m_cJpgList.InsertItem(num[0],_T(""),0);
				CString str;
				str.Format( _T("%d"), num[0]+1 ) ;
				m_cJpgList.SetItemText( num[0], 0, str ) ;
				m_cJpgList.SetItemText( num[0], 1,strFileName) ;
				m_cJpgList.SetItemData( num[0],(DWORD_PTR)picstruck );	
				num[0]++;
			}
			else if (filename == "png")
			{
// 				strFilePath = strPath + _T("\\") + strFileName;
// 				m_cPngList.InsertItem(num[1],_T(""),0);
// 				CString str;
// 				str.Format( _T("%d"), num[1]+1 ) ;
// 				m_cPngList.SetItemText( num[1], 0, str ) ;
// 				m_cPngList.SetItemText( num[1], 1,strFileName) ;
// 				num[1]++;		
			}
			else if (filename == "bmp")
			{
// 				strFilePath = strPath + _T("\\") + strFileName;
// 				m_cBmpList.InsertItem(num[2],_T(""),0);
// 				CString str;
// 				str.Format( _T("%d"), num[2]+1 ) ;
// 				m_cBmpList.SetItemText( num[2], 0, str ) ;
// 				m_cBmpList.SetItemText( num[2], 1,strFileName) ;
// 				num[2]++;		
				
				//OpenFile(strFilePath);
			}
			else if (filename == "gif")
			{
// 				strFilePath = strPath + _T("\\") + strFileName;
// 				m_cGifList.InsertItem(num[3],_T(""),0);
// 				CString str;
// 				str.Format( _T("%d"), num[3]+1 ) ;
// 				m_cGifList.SetItemText( num[3], 0, str ) ;
// 				m_cGifList.SetItemText( num[3], 1,strFileName) ;
// 				num[3]++;		
			}
			continue;
		}
	}

	find.Close();
}


void CPicManager::OnNMDblclkListFilejpg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_cJpgList.GetFirstSelectedItemPosition();
	while (pos)
	{
		int ID = m_cJpgList.GetNextSelectedItem(pos);
		PicStruckDef* tmp = (PicStruckDef*)m_cJpgList.GetItemData(ID);
// 		CString path = _T("mspaint.exe ") + tmp->m_sPicPath + tmp->m_sPicName;
// 		int length = path.GetLength();
// 		char* szCommandLine = path.GetBuffer(length);
// 		STARTUPINFOA si = {sizeof(si)};
// 		PROCESS_INFORMATION pi;
// 		::CreateProcessA(NULL,szCommandLine,NULL,NULL,FALSE,NULL,NULL,NULL,&si,&pi);
		int nRes = (int)ShellExecute(NULL, "open", tmp->m_sPicName, "", tmp->m_sPicPath, SW_SHOWDEFAULT);
		if(SE_ERR_NOASSOC == nRes)  
		{   
			CString strCmd;  
			strCmd.Format("rundll32 shell32, OpenAs_RunDLL  %s", tmp->m_sPicName);  
			WinExec(strCmd, SW_SHOWNORMAL);  
		}
	}
	*pResult = 0;
}


void CPicManager::OnNMDblclkListFilepng(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_cPngList.GetFirstSelectedItemPosition();
	while (pos)
	{
		int ID = m_cPngList.GetNextSelectedItem(pos);
		PicStruckDef* tmp = (PicStruckDef*)m_cPngList.GetItemData(ID);
		CString path = _T("mspaint.exe ") + tmp->m_sPicPath + tmp->m_sPicName;
		int length = path.GetLength();
		char* szCommandLine = path.GetBuffer(length);
		STARTUPINFOA si = {sizeof(si)};
		PROCESS_INFORMATION pi;
		//char* szCommandLine = "mspaint.exe C:\\Users\\QQS\\Pictures\\{2F8BCB31-EF96-47A2-8E2E-D2C9903B730A}.png";
		::CreateProcessA(NULL,szCommandLine,NULL,NULL,FALSE,NULL,NULL,NULL,&si,&pi);
	}
	*pResult = 0;
}


void CPicManager::OnNMDblclkListFilegif(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_cGifList.GetFirstSelectedItemPosition();
	  while (pos)
	  {
		  int ID = m_cGifList.GetNextSelectedItem(pos);
		  PicStruckDef* tmp = (PicStruckDef*)m_cGifList.GetItemData(ID);
		  CString path = _T("C:\\Program Files\\Internet Explorer\\iexplore.exe ") + tmp->m_sPicPath + tmp->m_sPicName;
		  int length = path.GetLength();
		  char* szCommandLine = path.GetBuffer(length);
		  STARTUPINFOA si = {sizeof(si)};
		  PROCESS_INFORMATION pi;
		  //char* szCommandLine = "mspaint.exe C:\\Users\\QQS\\Pictures\\{2F8BCB31-EF96-47A2-8E2E-D2C9903B730A}.png";
		  ::CreateProcessA(NULL,szCommandLine,NULL,NULL,FALSE,NULL,NULL,NULL,&si,&pi);
	  }
	*pResult = 0;
}


void CPicManager::OnNMDblclkListFilebmp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	POSITION pos = m_cBmpList.GetFirstSelectedItemPosition();
	while (pos)
	{
		int ID = m_cBmpList.GetNextSelectedItem(pos);
		PicStruckDef* tmp = (PicStruckDef*)m_cBmpList.GetItemData(ID);
		CString path = _T("mspaint.exe ") + tmp->m_sPicPath + tmp->m_sPicName;
		int length = path.GetLength();
		char* szCommandLine = path.GetBuffer(length);
		STARTUPINFOA si = {sizeof(si)};
		PROCESS_INFORMATION pi;
		::CreateProcessA(NULL,szCommandLine,NULL,NULL,FALSE,NULL,NULL,NULL,&si,&pi);
	}
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

 
 void CPicManager::OnNMRClickListFilejpg(NMHDR *pNMHDR, LRESULT *pResult)
 {
 	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
 //	CRect rect;
	UINT nTestFlags;	
	m_nCurrentID = m_cJpgList.HitTest(pNMItemActivate->ptAction, &nTestFlags);
	m_cJpgList.GetItemRect(m_nCurrentID, &rect,nTestFlags);
	m_pCurrentPic = ( PicStruckDef*)m_cJpgList.GetItemData( m_nCurrentID ) ;
	CMenu* pMenu = new CMenu;
	pMenu->LoadMenu(IDR_MENU_RBDOWN);	
	ClientToScreen(&pNMItemActivate->ptAction);
	m_sCurrentPos.x = pNMItemActivate->ptAction.x;
	m_sCurrentPos.y = pNMItemActivate->ptAction.y + 30;

	TrackPopupMenu(pMenu->GetSubMenu(0)->m_hMenu,0,pNMItemActivate->ptAction.x,pNMItemActivate->ptAction.y+40,0,this->GetSafeHwnd(),&rect);
 	// TODO: 在此添加控件通知处理程序代码
 	*pResult = 0;
 }

void CPicManager::OnOK()
{
	int nCount = m_cJpgList.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		PicStruckDef* tmp = ( PicStruckDef*)m_cJpgList.GetItemData(i);
		delete tmp;
		tmp = NULL;
	}

	CDialog::OnOK();
}

void CPicManager::OnCancel()
{
	int nCount = m_cJpgList.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		PicStruckDef* tmp = ( PicStruckDef*)m_cJpgList.GetItemData(i);
		delete tmp;
		tmp = NULL;
	}
	CDialog::OnCancel();
}


void CPicManager::OnBnClickedButtonFile()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: Add your control notification handler code here 
	UpdateData(TRUE); 
	char szDir[MAX_PATH]; 
	BROWSEINFO bi; 
	ITEMIDLIST *pidl;  
	bi.hwndOwner = this->m_hWnd; // 指定父窗口，在对话框显示期间，父窗口将被禁用   
	bi.pidlRoot = NULL; // 如果指定NULL，就以“桌面”为根     
	bi.pszDisplayName = szDir;  bi.lpszTitle = "请选择目录"; // 这一行将显示在对话框的顶端    
	bi.ulFlags = BIF_STATUSTEXT | BIF_USENEWUI | BIF_RETURNONLYFSDIRS; 
	bi.lpfn = NULL; 
	bi.lParam = 0; 
	bi.iImage = 0;  
	pidl = SHBrowseForFolder(&bi); 
	if(pidl == NULL) 
		return;  
	if(!SHGetPathFromIDList(pidl, szDir)) 
		return; 
	else m_strPath = szDir; 

	ScanDiskFile(m_strPath,m_Num);

	UpdateData(FALSE); 

}
