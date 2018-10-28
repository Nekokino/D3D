#pragma once

class NTSplitWindow : public CSplitterWnd
{
private:
	bool ScrollLock;

public:
	void Lock() { ScrollLock = true; }
	void Unlock() { ScrollLock = false; }

	UINT GetBorderY() { return (UINT)m_cyBorder; }

private:
	BOOL PreCreateWindow(CREATESTRUCT&  cs);

public:
	DECLARE_MESSAGE_MAP();
	afx_msg void OnLButtonDown(UINT _nFlags, CPoint _Point);
	afx_msg void OnMouseMove(UINT _nFlags, CPoint _Point);
	afx_msg BOOL OnSetCursor(CWnd* _Wnd, UINT _nHitTest, UINT _Msg);
public:
	NTSplitWindow();
	~NTSplitWindow();
};

