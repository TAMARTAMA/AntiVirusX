#include "GuiMessenge.h"
void MyFrame::SetInstructionsText(const wxString& text) {
	wxTextCtrl* instructionsTextCtrl = new wxTextCtrl(helpTab, wxID_ANY, text,
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

	autoRunCheckBox = new wxCheckBox(helpTab, wxID_ANY, "הפעל אוטומטית עם אתחול המחשב");

	std::wstring appName = L"MyAntivirus";
	autoRunCheckBox->SetValue(Registery::IsAutoRunEnabled(appName));

	autoRunCheckBox->Bind(wxEVT_CHECKBOX, [=](wxCommandEvent& evt) {
		wxString filePath = inputText->GetValue();
		std::wstring appPath = L"C:\\Users\\USER\\Desktop\\tichnut\\B\\Poject\\AntiVirus\\x64\\Debug\\AntiVirus.exe";
		if (autoRunCheckBox->GetValue()) {
			Registery::EnableAutoRun(appName, appPath);
		}
		else {
			Registery::DisableAutoRun(appName);
		}
		});
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(instructionsTextCtrl, 1, wxEXPAND | wxALL, 10);
	sizer->AddStretchSpacer();
	sizer->Add(autoRunCheckBox, 0, wxALL | wxEXPAND, 5);
	helpTab->SetSizer(sizer);
	helpTab->Layout();
}
void MyFrame::OnHelpButtonClicked(wxCommandEvent& event) {
	wxLaunchDefaultBrowser("https://example.com/support");
}

void MyFrame::InitHelpButton(wxWindow* parentPanel) {
	wxButton* helpButton = new wxButton(parentPanel, wxID_ANY, "עזרה");

	helpButton->Bind(wxEVT_BUTTON, &MyFrame::OnHelpButtonClicked, this);

	wxBoxSizer* sizer = dynamic_cast<wxBoxSizer*>(parentPanel->GetSizer());
	if (sizer) {
		sizer->Add(helpButton, 0, wxALIGN_RIGHT | wxALL, 10);
		parentPanel->Layout();
	}
}

void MyFrame::ShowHiddenTabWithText(const wxString& text) {
	hiddenTab = new wxPanel(notebook, wxID_ANY);
	hiddenTabLabel = new wxStaticText(hiddenTab, wxID_ANY, "סריקת קובץ הסתימה", wxPoint(10, 10));
	int hiddenIndex = notebook->FindPage(hiddenTab);
	if (hiddenIndex == wxNOT_FOUND) {
		notebook->AddPage(hiddenTab, "סריקת קובץ הסתימה");
	}

	hiddenTab->DestroyChildren();
	wxTextCtrl* textCtrl = new wxTextCtrl(hiddenTab, wxID_ANY, text,
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxBORDER_NONE);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(textCtrl, 1, wxEXPAND | wxALL, 10);
	hiddenTab->SetSizer(sizer);
	hiddenTab->Layout();

	hiddenTab->SetBackgroundColour(wxColour(255, 230, 230));
	hiddenTab->Refresh();

	notebook->ChangeSelection(notebook->GetPageCount() - 1);
}

void MyFrame::SetWindowIcon() {
	wxIcon icon;
	if (icon.LoadFile("C:\\Users\\USER\\Desktop\\tichnut\\B\\Poject\\ImageToStl.com_antivirus.ico", wxBITMAP_TYPE_ICO)) {
		SetIcon(icon);
	}
	else {
		wxLogWarning("לא נמצא קובץ אייקון: myicon.ico");
	}
}

void MyFrame::OnClose(wxCloseEvent& event) {
	wxMessageBox("היישום נסגר. להתראות!", "סגירה", wxOK | wxICON_INFORMATION);

	event.Skip();
	Global::SetRunning();
}

MyFrame::MyFrame()
	: wxFrame(nullptr, wxID_ANY, "חלון עם Tabs", wxDefaultPosition, wxSize(500, 400))
{
	notebook = new wxNotebook(this, wxID_ANY);

	scanTab = new wxPanel(notebook, wxID_ANY);
	notebook->AddPage(scanTab, "סריקת קובץ");

	helpTab = new wxPanel(notebook, wxID_ANY);
	new wxStaticText(helpTab, wxID_ANY, "כאן יהיו הוראות שימוש", wxPoint(10, 10));
	notebook->AddPage(helpTab, "הוראות");

	SetInstructionsText("ברוכים הבאים לתוכנת הסריקה שלנו!\n\nכדי להתחיל, הזן את הנתיב של הקובץ שברצונך לסרוק בטאב סריקת קובץ.\n\nאם ברצונך שהתוכנה תופעל אוטומטית עם אתחול המחשב, סמן את התיבה המתאימה בטאב 'הוראות.");
	InitHelpButton(helpTab);

	InitScanTab();
	SetWindowIcon();
	Centre();
}
bool MyFrame::IsAutoRunChecked() const {
	return autoRunCheckBox && autoRunCheckBox->IsChecked();
}

void MyFrame::InitScanTab() {
	helpTab->SetBackgroundColour(wxColour(224, 240, 255)); 
	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

	inputText = new wxTextCtrl(scanTab, wxID_ANY, "", wxDefaultPosition, wxSize(300, -1));
	vbox->Add(inputText, 0, wxALL | wxEXPAND, 10);

	wxButton* submitBtn = new wxButton(scanTab, wxID_ANY, "שלח");
	vbox->Add(submitBtn, 0, wxALL | wxCENTER, 5);

	outputMessage = new wxStaticText(scanTab, wxID_ANY, "");
	vbox->Add(outputMessage, 0, wxALL | wxEXPAND, 10);

	scanTab->SetSizer(vbox);

	submitBtn->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) {
		wxString userInput = inputText->GetValue();

		if (ValidateInput(userInput)) {
			outputMessage->SetLabel("קלט תקין: " + userInput);

			bool runOnStartup = autoRunCheckBox->GetValue();

			ThreatMonitoring::ScanFilePath(userInput);

		}
		else {
			outputMessage->SetLabel("קלט שגוי. אנא נסה שוב.");
		}
		});
}

void MyFrame::ShowHiddenTab(const wxString& text) {
	if (hiddenTabLabel) {
		hiddenTabLabel->SetLabel(text);
		hiddenTabLabel->Wrap(300); 
	}
}

