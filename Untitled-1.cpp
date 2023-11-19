#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/button.h>

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    void OnRunMacro(wxCommandEvent& event);
    void OnLoopMacro(wxCommandEvent& event);

    wxTextCtrl *delayBeforeShotInput;
    wxTextCtrl *delayAfterShotInput;
    wxTextCtrl *flickDistanceInput;
    wxTextCtrl *flickSpeedInput;
    wxButton *runMacroButton;
    wxButton *loopMacroButton;
};

bool MyApp::OnInit() {
    MyFrame *frame = new MyFrame("Macro Configurator", wxPoint(50, 50), wxSize(350, 250));
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size) {
    
    wxPanel *panel = new wxPanel(this, wxID_ANY);

    delayBeforeShotInput = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(10,10), wxSize(150, 30));
    delayAfterShotInput = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(10,50), wxSize(150, 30));
    flickDistanceInput = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(10,90), wxSize(150, 30));
    flickSpeedInput = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(10,130), wxSize(150, 30));

    runMacroButton = new wxButton(panel, wxID_ANY, "Run Macro", wxPoint(180,10), wxSize(150, 30));
    loopMacroButton = new wxButton(panel, wxID_ANY, "Loop Macro", wxPoint(180,50), wxSize(150, 30));

    Connect(runMacroButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::OnRunMacro));
    Connect(loopMacroButton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::OnLoopMacro));
}

void MyFrame::OnRunMacro(wxCommandEvent& event) {
    // Access the values entered by the user using GetValue() and perform the macro operations
}

void MyFrame::OnLoopMacro(wxCommandEvent& event) {
    // Loop the macro using the user's last settings
}

wxIMPLEMENT_APP(MyApp);

#include <thread>

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    void OnSetHotkey(wxCommandEvent& event);
    void ListenForKeyPress(char hotkey);

    wxTextCtrl *delayBeforeShotInput;
    wxTextCtrl *delayAfterShotInput;
    wxTextCtrl *flickDistanceInput;
    wxTextCtrl *flickSpeedInput;
    wxTextCtrl *hotkeyInput;
    wxButton *setHotkeyButton;
};

// ... [MyApp::OnInit() and MyFrame constructor]

void MyFrame::OnSetHotkey(wxCommandEvent& event) {
    char hotkey = hotkeyInput->GetValue()[0]; // Get the first character of the input
    std::thread listenerThread(&MyFrame::ListenForKeyPress, this, hotkey);
    listenerThread.detach(); // Run the thread in the background
}

void MyFrame::ListenForKeyPress(char hotkey) {
    while (true) { // Endlessly check for the key
        if (GetAsyncKeyState(hotkey) & 0x8000) {
            // The key was pressed; execute the macro code
            // Make sure to add some delay to prevent repeated executions while the key is held down
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Sleep a bit to prevent maxing out the CPU
    }
}

// ... [wxIMPLEMENT_APP(MyApp)]