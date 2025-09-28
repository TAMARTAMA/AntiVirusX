#pragma once



//class MessageFrame : public wxFrame {
//public:
//    MessageFrame(const wxString& message, bool* F)
//        : wxFrame(nullptr, wxID_ANY, "Message Window", wxDefaultPosition, wxSize(400, 200)), m_F(F) {
//        wxPanel* panel = new wxPanel(this, wxID_ANY);
//        wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
//
//        wxStaticText* msgText = new wxStaticText(panel, wxID_ANY, message, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
//        vbox->Add(msgText, 1, wxEXPAND | wxALL, 10);
//
//        wxButton* button = new wxButton(panel, wxID_OK, "OK", wxDefaultPosition, wxDefaultSize);
//        vbox->Add(button, 0, wxALIGN_CENTER | wxALL, 10);
//
//        panel->SetSizer(vbox);
//
//        button->Bind(wxEVT_BUTTON, &MessageFrame::OnButtonClick, this);
//    }
//
//private:
//    bool* m_F;
//
//    void OnButtonClick(wxCommandEvent& event) {
//        if (m_F) {
//            *m_F = true;
//        }
//        Close();
//    }
//};
//
//class MessageApp : public wxApp {
//public:
//    bool OnInit() override {
//        return true;
//    }
//};
//
//wxIMPLEMENT_APP_NO_MAIN(MessageApp);
//
//GUIMessenge::GUIMessenge() {
//    int argc = 0;
//    char** argv = nullptr;
//    wxEntryStart(argc, argv);
//    wxTheApp->CallOnInit();
//}
//
//bool GUIMessenge::GUIShowMessenge(std::wstring messenge) {
//    MessageFrame* frame = new MessageFrame(messenge, nullptr);
//    //frame->Show();
//    return true;
//}
//
//bool GUIMessenge::AddButton(std::wstring messenge, bool* F) {
//    MessageFrame* frame = new MessageFrame(messenge, F);
//    //frame->Show();
//    return true;
//}
//
//class GUIMessenge
//{
//private:
//	/*std:: wstring messenge;*/
//public:
//	GUIMessenge();//אתחול חלון ממשק גרפי והפעלת פו
//	bool GUIShowMessenge(std::wstring messenge);//הוספת הודעה לחלון הקיים ונקוי החלון לפני
//	bool AddButton(std::wstring messenge, bool* F);//הוספת כפתור  לחלון הקים ובעת לחיצה יופעל הפונקציה F
//};
// מחלקה שמנהלת את האפליקציה


#ifndef GUI_Messenge_H
#define GUI_Messenge_H
#include <string>
#include <wx/wx.h>
#include "ThreatMonitoring.h"
#include <wx/notebook.h>
#include "Global.h"
#include "Registery.h"
class MyFrame : public wxFrame {
private:
	wxNotebook* notebook;
	wxPanel* hiddenTab;
	wxPanel* scanTab;
	wxTextCtrl* inputText;
	wxStaticText* outputMessage;
	wxCheckBox* autoRunCheckBox;

	wxPanel* helpTab;
	wxStaticText* hiddenTabLabel;
	bool ValidateInput(const wxString& filePath) {

		DWORD attributes = GetFileAttributesW(filePath.c_str());

		// אם הפונקציה מחזירה INVALID_FILE_ATTRIBUTES, הנתיב לא תקין או לא קיים
		if (attributes == INVALID_FILE_ATTRIBUTES)
			return false;

		// נוודא שזה קובץ ולא תיקייה
		if (attributes & FILE_ATTRIBUTE_DIRECTORY)
			return false;

		// ננסה גם לפתוח את הקובץ לבדיקה
		HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_READ,
			FILE_SHARE_READ, nullptr, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, nullptr);

		if (hFile == INVALID_HANDLE_VALUE)
			return false;

		CloseHandle(hFile);
		return true;
	}
public:
	MyFrame();
	bool IsAutoRunChecked() const;
	void ShowHiddenTab(const wxString& text);
	void InitScanTab();
	void SetInstructionsText(const wxString& text);
	void OnHelpButtonClicked(wxCommandEvent& event);
	void InitHelpButton(wxWindow* parentPanel);
	void ShowHiddenTabWithText(const wxString& text);
	void SetWindowIcon(); 
	void OnClose(wxCloseEvent& event); 

	wxDECLARE_EVENT_TABLE();
};

class MyApp : public wxApp {
public:
	virtual bool OnInit() override {
		MyFrame* frame = new MyFrame();
		frame->Show(true);
		SetTopWindow(frame);
		return true;
	}
};
#endif // MYGUIWINDOW_H



