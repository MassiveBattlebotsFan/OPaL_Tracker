#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <array>
#include <vector>
#include <format>
#include <wx/wx.h>
#include <wx/log.h> 
#include <wx/listctrl.h>

#include "config.hpp"
#include "o4s.hpp"

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
};

class InstrumentList : public wxListCtrl {
    public:
        InstrumentList(wxWindow *parent, wxWindowID ID);
        virtual ~InstrumentList();

    private:
        size_t cursor = 4;
        O4SInstrument *instrumentData;
        void HandleKeyPress(wxKeyEvent &evt);
        long InsertItemIntoAllColumns(wxListItem &item);
        void HandleItemSelected(wxListEvent &evt);
        void MoveCursorColumnLocation(wxListItem &item, size_t old, size_t current);

        wxDECLARE_EVENT_TABLE();
};

class MainPanel : public wxPanel{
    public:
        MainPanel(wxWindow *parent);
};

enum {
  ID_InstrumentList
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

long InstrumentList::InsertItemIntoAllColumns(wxListItem &item){
    long insertedId = this->InsertItem(item);

    byte *instrument = (byte *)item.GetData();

    if(instrument != nullptr){
        for (int i = 0; i < sizeof(O4SInstrument); ++i){
            this->SetItem(insertedId, i, std::format("{:02x}", *(instrument + i)));
        }
    }

    return insertedId;
}

InstrumentList::InstrumentList(wxWindow *parent, wxWindowID ID) : wxListCtrl(parent, ID, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxBORDER_SIMPLE){

    for (int i = 0; i < sizeof(O4SInstrument); ++i){
        this->AppendColumn(std::format("{:02x}", i));
        this->SetColumnWidth(i, 30);
    }

    this->instrumentData = new O4SInstrument[256];

    for (size_t i = 0; i < sizeof(O4SInstrument[256]); ++i){
        *((byte *)this->instrumentData + i) = 0;
    }

    wxListItem initialListItem;

    initialListItem.SetId(this->GetItemCount());
    initialListItem.SetData((void *)&this->instrumentData[0]);

    this->InsertItemIntoAllColumns(initialListItem);

    this->SetBackgroundColour(BACKGROUND_COLOR);
    this->SetForegroundColour(FOREGROUND_COLOR);

    this->Update();
}

InstrumentList::~InstrumentList(){
    delete[] this->instrumentData;
    this->instrumentData = nullptr;
}

wxBEGIN_EVENT_TABLE(InstrumentList, wxListCtrl)
    EVT_KEY_DOWN(InstrumentList::HandleKeyPress)
    EVT_LIST_ITEM_SELECTED(ID_InstrumentList, InstrumentList::HandleItemSelected)
wxEND_EVENT_TABLE()

void InstrumentList::HandleItemSelected(wxListEvent &evt){
    wxListItem item = evt.GetItem();
    wxLogInfo(std::to_string(evt.GetItem()).c_str());
    
    this->SetItem(item.GetId(), this->cursor % sizeof(O4SInstrument), std::format("*{:02x}", *(((byte *)item.GetData()) + this->cursor % sizeof(O4SInstrument))));

    //evt.Skip();
}

void InstrumentList::HandleKeyPress(wxKeyEvent &evt){
    if(evt.GetKeyCode() == WXK_RIGHT){
        size_t oldCursor = cursor;
        this->cursor = (this->cursor + 1) % (sizeof(O4SInstrument) - 1);
        this->MoveCursorColumnLocation(this->GetSelection(), oldCursor, this->cursor);
        return;
    }
    evt.Skip();
}

void InstrumentList::MoveCursorColumnLocation(wxListItem& item, size_t old, size_t current){
    this->SetItem(item.GetId(), old, std::format("{:02x}", *(((byte *)item.GetData()) + old)));
    this->SetItem(item.GetId(), current, std::format("*{:02x}", *(((byte *)item.GetData()) + current)));
}

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "OPaL Tracker " + _(VERSION) + "-" + _(BUILD_TYPE))
{
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(wxID_SAVE);
    menuFile->Append(wxID_EXIT, "&Quit\tCtrl+Q");
    wxMenu* menuAbout = new wxMenu;
    menuAbout->Append(wxID_ABOUT);
    //wxMenu* menuEdit = new wxMenu;
    //menuEdit->Append(ID_Config, "&Configure V-HACD...");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    //menuBar->Append(menuEdit, "&Edit");
    menuBar->Append(menuAbout, "&About");

    Bind(wxEVT_MENU, [&](wxCommandEvent&){ if(wxMessageBox("Are you sure you want to quit? All unsaved edits will be lost.", "Confirm exit", wxYES_NO, this) == wxYES) Close(true); }, wxID_EXIT);

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

    InstrumentList *instrumentList = new InstrumentList(this, ID_InstrumentList);

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