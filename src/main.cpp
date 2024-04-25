#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <wx/wx.h>
#include <wx/editlbox.h>
#include <wx/listctrl.h>

#include "config.hpp"

const wxColour BACKGROUND_COLOR(20, 20, 20);
const wxColour FOREGROUND_COLOR(240, 240, 240);

class OPaL4cIDTrackerApp : public wxApp {
    public:
        virtual bool OnInit();
};

class MainFrame : public wxFrame{
    public:
        MainFrame();
};

class TrackerList : public wxListCtrl {
    public:
        TrackerList(wxWindow *parent, wxWindowID ID);

    private:
        size_t cursor = 0;
        size_t commandColumn;
        size_t argumentColumn;

        void HandleKeyPress(wxKeyEvent &evt);

        wxDECLARE_EVENT_TABLE();
};

class MainPanel : public wxPanel{
    public:
        MainPanel(wxWindow *parent);
};

enum {
  
};

TrackerList::TrackerList(wxWindow *parent, wxWindowID ID) : wxListCtrl(parent, ID, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxBORDER_SIMPLE){

    this->commandColumn = this->AppendColumn("Command");
    this->argumentColumn = this->AppendColumn("Argument");

    for (int i = 0; i < 10; ++i){
        wxListItem item;
        long itemCount = this->GetItemCount();

        item.SetId(itemCount);
        item.SetText("Test");
        
        this->InsertItem(item);
        this->SetItem(i, this->commandColumn, "Test entry");
        this->SetItem(i, this->argumentColumn, std::to_string(i));
    }

    this->SetBackgroundColour(BACKGROUND_COLOR);
    this->SetForegroundColour(FOREGROUND_COLOR);

    this->Update();
}

wxBEGIN_EVENT_TABLE(TrackerList, wxListCtrl)
wxEND_EVENT_TABLE()

void TrackerList::HandleKeyPress(wxKeyEvent &evt){

}

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "OPaL Tracker " + _(VERSION) + "-" + _(BUILD_TYPE)){
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(wxID_SAVE);
    menuFile->Append(wxID_EXIT);
    wxMenu* menuAbout = new wxMenu;
    menuAbout->Append(wxID_ABOUT);
    //wxMenu* menuEdit = new wxMenu;
    //menuEdit->Append(ID_Config, "&Configure V-HACD...");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    //menuBar->Append(menuEdit, "&Edit");
    menuBar->Append(menuAbout, "&About");

    Bind(wxEVT_MENU, [&](wxCommandEvent&){ Close(true); }, wxID_EXIT);

    SetMenuBar(menuBar);

    MainPanel* mainPanel = new MainPanel(this);

    SetInitialSize(wxSize(800,600));
}

MainPanel::MainPanel(wxWindow *parent) : wxPanel(parent, wxID_ANY){
    wxBoxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *instrument_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *pattern_main_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *pattern_menu_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *left_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sequence_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sequence_top_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *sequence_bottom_sizer = new wxBoxSizer(wxHORIZONTAL);

    TrackerList *instrumentList = new TrackerList(this, wxID_ANY);

    TrackerList *patternListCmds = new TrackerList(this, wxID_ANY);

    TrackerList *sequence1List = new TrackerList(this, wxID_ANY);
    TrackerList *sequence2List = new TrackerList(this, wxID_ANY);
    TrackerList *sequence3List = new TrackerList(this, wxID_ANY);
    TrackerList *sequence4List = new TrackerList(this, wxID_ANY);
    
    main_sizer->Add(instrument_sizer, wxSizerFlags(1).Expand());
    main_sizer->Add(pattern_main_sizer, wxSizerFlags(5).Expand());

    instrument_sizer->Add(instrumentList, wxSizerFlags(1).Top().Expand());

    pattern_main_sizer->Add(left_sizer, wxSizerFlags(4).Expand());
    pattern_main_sizer->Add(sequence_sizer, wxSizerFlags(3).Expand());

    sequence_sizer->Add(sequence_top_sizer, wxSizerFlags(1).Expand());
    sequence_sizer->Add(sequence_bottom_sizer, wxSizerFlags(1).Expand());

    sequence_top_sizer->Add(sequence1List, wxSizerFlags(1).Expand());
    sequence_top_sizer->Add(sequence2List, wxSizerFlags(1).Expand());

    sequence_bottom_sizer->Add(sequence3List, wxSizerFlags(1).Expand());
    sequence_bottom_sizer->Add(sequence4List, wxSizerFlags(1).Expand());

    left_sizer->Add(pattern_menu_sizer, wxSizerFlags().Top().Expand());
    left_sizer->Add(patternListCmds, wxSizerFlags(1).Top().Expand());

    this->SetBackgroundColour(BACKGROUND_COLOR);
    this->SetForegroundColour(FOREGROUND_COLOR);

    this->Update();

    SetSizerAndFit(main_sizer);
}

wxIMPLEMENT_APP(OPaL4cIDTrackerApp);

bool OPaL4cIDTrackerApp::OnInit(){
    MainFrame *frame = new MainFrame();
    frame->Show(true);
    return true;
}