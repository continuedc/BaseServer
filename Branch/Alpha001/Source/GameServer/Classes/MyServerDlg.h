
// MyServerDlg.h : ͷ�ļ�
//

#pragma once


// CMyServerDlg �Ի���
class CMyServerDlg : public CDialog
{
// ����
public:
	CMyServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MYSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnnotify();
	afx_msg void OnBnClickedBtnRestart();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedChkLua();
	afx_msg void OnBnClickedChkGm();
	afx_msg void OnBnClickedChkGmtool();
	afx_msg void OnBnClickedBtnUpdateLua();
	afx_msg void OnBnClickedBtnUpdateTable();
};