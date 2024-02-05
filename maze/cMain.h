#pragma once
#include "wx/wx.h"
class cMain : public wxFrame
{
public:
	cMain();
	~cMain();
public:
	int nFieldWidth = 10;
	int nFieldHeight = 10;
	wxButton** btn;
	int* nWalls = nullptr;
	bool bSetStart = true;
	bool bSetEnd = false;
	bool bFirstClick = true;
	int nStartPos = 0;
	int nEndPos = 0;
	void onButtonClicked(wxCommandEvent& evt);
	wxColor noWallColor = wxColor(153, 148, 147);
	wxColor WallColor = wxColor(70, 56, 52);
	wxColor StartColor = wxColor(113, 241, 51);
	wxColor EndColor = wxColor(252, 37, 16);
	wxColor SolveColor = wxColor(250, 244, 49);

	wxMenuBar* mMenuBar = nullptr;
	wxToolBar* mToolBar = nullptr;
	void OnStart(wxCommandEvent& evt);
	void OnEnd(wxCommandEvent& evt);
	void Solve(wxCommandEvent& evt);
	void Reset(wxCommandEvent& evt);
	void bfs(int);
	wxDECLARE_EVENT_TABLE();
};

