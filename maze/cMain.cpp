#include "cMain.h"
#include <list>
#include <unordered_map>
#include <string>
using namespace std;
wxBEGIN_EVENT_TABLE(cMain,wxFrame)
	//EVT_MENU(100,cMain::OnStart)
	//EVT_MENU(200, cMain::OnEnd)
	EVT_BUTTON(300,cMain::OnStart)
	EVT_BUTTON(350, cMain::OnEnd)
	EVT_BUTTON(400, cMain::Solve)
	EVT_BUTTON(450, cMain::Reset)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr,wxID_ANY,"Maze title",wxPoint(30,30),wxSize(800,600))
{
	//Menu
	mMenuBar = new wxMenuBar();
	this->SetMenuBar(mMenuBar);
	wxMenu* menuTile = new wxMenu;
	menuTile->Append(100, "Start");
	menuTile->Append(200, "End");
	mMenuBar->Append(menuTile, "Tiles");

	//Toolbar
	mToolBar = this->CreateToolBar(wxTB_HORIZONTAL, wxID_ANY);
	//Add buttons to toolbar
	wxButton* start = new wxButton(mToolBar, 300, "Start", wxDefaultPosition, wxSize(40, 24), 0);
	start->SetBackgroundColour(StartColor);
	mToolBar->AddControl(start);
	wxButton* end = new wxButton(mToolBar, 350, "End", wxDefaultPosition, wxSize(40, 24), 0);
	end->SetBackgroundColour(EndColor);
	mToolBar->AddControl(end);
	wxButton* solve = new wxButton(mToolBar, 400, "Solve", wxDefaultPosition, wxSize(40, 24), 0);
	solve->SetBackgroundColour(SolveColor);
	mToolBar->AddControl(solve);
	wxButton* reset = new wxButton(mToolBar, 450, "Reset", wxDefaultPosition, wxSize(40, 24), 0);
	reset->SetBackgroundColour(noWallColor);
	mToolBar->AddControl(reset);

	mToolBar->Realize();

	//Create grid and sizer
	btn = new wxButton * [nFieldWidth * nFieldHeight];
	wxGridSizer* grid = new wxGridSizer(nFieldWidth, nFieldHeight, 0, 0);
	nWalls = new int[nFieldWidth * nFieldHeight];

	for (int x = 0; x < nFieldWidth; x++) {
		for (int y = 0; y < nFieldHeight; y++) {
			btn[y * nFieldWidth + x] = new wxButton(this, 10000 + (y * nFieldWidth + x));
			grid->Add(btn[y * nFieldWidth + x], 1, wxEXPAND | wxALL);
			btn[y * nFieldWidth + x]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::onButtonClicked, this);
			btn[y * nFieldWidth + x]->SetBackgroundColour(noWallColor);
			nWalls[y * nFieldWidth + x] = 0;
		}
	}
	this->SetSizer(grid);
	grid->Layout();
}
cMain::~cMain()
{
	delete []btn;
	delete []nWalls;
	
}

void cMain::onButtonClicked(wxCommandEvent& evt)
{
	
	int x = (evt.GetId() - 10000) % nFieldWidth;
	int y = (evt.GetId() - 10000) / nFieldWidth;
	int pos = y * nFieldWidth + x;
	if (bSetStart) {
		nStartPos = evt.GetId();// -10000;
		bSetStart = false;
		if (bFirstClick) {
			bSetEnd = true;
		}
		nWalls[pos] = 0;
		btn[pos]->Enable(false);
		btn[pos]->SetBackgroundColour(StartColor);
	}
	else if (bSetEnd) {
		nEndPos = evt.GetId();// -10000;
		bSetEnd = false;
		nWalls[pos] = 0;
		btn[pos]->Enable(false);
		btn[pos]->SetBackgroundColour(EndColor);
	}
	else {
		wxString w = "Wall at pos= " + to_string(pos);
		wxLogDebug(w);
		if (nWalls[pos] == 0){
			nWalls[pos] = 1;
			btn[pos]->SetBackgroundColour(WallColor);
		}
		else {
			nWalls[pos] = 0;
			btn[pos]->SetBackgroundColour(noWallColor);
		}
	}
	bFirstClick = false;
	evt.Skip();
}

void cMain::OnStart(wxCommandEvent& evt)
{
	bSetStart = true;
	bSetEnd = false;
	if (nStartPos == 0) {
		return;
	}
	btn[nStartPos - 10000]->SetBackgroundColour(noWallColor);
	btn[nStartPos - 10000]->Enable(true);
	evt.Skip();
}

void cMain::OnEnd(wxCommandEvent& evt)
{
	bSetEnd = true;
	bFirstClick = false;
	bSetStart = false;
	if (nEndPos == 0) {
		return;
	}
	btn[nEndPos-10000]->SetBackgroundColour(noWallColor);
	btn[nEndPos-10000]->Enable(true);
	evt.Skip();
}

void cMain::bfs(int pos)
{
	unordered_map<int,bool> visited;
	unordered_map<int, int> prev;
	//visited.resize(nFieldWidth * nFieldHeight, false);

	// Create a queue for BFS
	list<int> queue;
	/*wxString p = to_string(pos);
	wxLogDebug(p);*/
	// Mark the current node as visited and enqueue it
	visited[pos] = true;
	prev[pos] = 0;
	queue.push_back(pos);
	int adj[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };
	while (!queue.empty())
	{
		// Dequeue a vertex from queue and print it
		pos = queue.front();
		wxString pos2 = "pos= " + to_string(pos);
		wxLogDebug(pos2);
		if (pos == nEndPos) {
			string short_path = to_string(pos);
			reverse(short_path.begin(), short_path.end());
			int cur = pos;
			while (cur != nStartPos) {
				string nxt = to_string(prev[cur]);
				reverse(nxt.begin(), nxt.end());
				btn[prev[cur]-10000]->SetBackgroundColour(SolveColor);
				short_path = short_path + " >- " + nxt;
				cur = prev[cur];
			}
			btn[nStartPos-10000]->SetBackgroundColour(StartColor);
			reverse(short_path.begin(), short_path.end());
			wxString ans = short_path;
			wxLogDebug(ans);
			return;
		}
		//cout << pos << " ";
		
		queue.pop_front();

		// Get all adjacent vertices of the dequeued
		// vertex s. If a adjacent has not been visited,
		// vertex s. If a adjacent has not been visited,
		// then mark it visited and enqueue it
		int x = (pos - 10000) % nFieldWidth;
		int y = (pos - 10000) / nFieldWidth;
		int old_pos = pos;
		for (auto adjecent : adj)
		{
			/*wxString a = to_string(adjecent[0]) +","+ to_string(adjecent[1]);
			wxLogDebug(a);*/
			int nx = x + adjecent[0];
			int ny = y + adjecent[1];
			
			if (nx < 0 or nx >= nFieldWidth or ny < 0 or ny >= nFieldHeight) {
				continue;
			}
			pos = 10000 + (ny * nFieldWidth + nx);
			
			if (nWalls[pos-10000] == 1) {
				continue;
			}
			wxString pos2 = "x: " + to_string(nx) + " y: " + to_string(ny);
			wxLogDebug(pos2);
			if (!prev[pos]) {
				prev[pos] = old_pos;
			}
			if (!visited[pos])
			{
				visited[pos] = true;
				/*wxString w = "pushing= " + to_string(pos);
				wxLogDebug(w);*/
				queue.push_back(pos);
			}
		}
	}
}

void cMain::Solve(wxCommandEvent& evt)
{
	if (nStartPos == nEndPos) {
		return;
	}
	for (int x = 0; x < nFieldWidth; x++) {
		for (int y = 0; y < nFieldHeight; y++) {
			if (y * nFieldWidth + x == nStartPos - 10000) { continue; };
			if (y * nFieldWidth + x == nEndPos-10000) { continue; };
			if (nWalls[y * nFieldWidth + x] == 1) { continue; };
			btn[y * nFieldWidth + x]->SetBackgroundColour(noWallColor);
		}
	}
	bfs(nStartPos);
	
}

void cMain::Reset(wxCommandEvent& evt)
{
	if (nStartPos != 0) {
		btn[nStartPos - 10000]->Enable(true);
	}
	if (nEndPos != 0) {
		btn[nEndPos - 10000]->Enable(true);
	}
	nStartPos = 0;
	nEndPos = 0;
	bFirstClick = true;
	bSetStart = true;
	for (int x = 0; x < nFieldWidth; x++) {
		for (int y = 0; y < nFieldHeight; y++) {
			btn[y * nFieldWidth + x]->SetBackgroundColour(noWallColor);
			nWalls[y * nFieldWidth + x] = 0;
		}
	}

	
}
