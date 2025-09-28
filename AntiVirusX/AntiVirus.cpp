#define NOMINMAX

#include "ThreatMonitoring.h"
#include "GUIMessenge.h"
#pragma comment(lib, "wbemuuid.lib")
#pragma comment (lib,"wintrust")
#pragma comment (lib,"Iphlpapi.lib")

wxIMPLEMENT_APP_NO_MAIN(MyApp);
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_CLOSE(MyFrame::OnClose)
wxEND_EVENT_TABLE()

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	if (!wxEntryStart(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
		return -1;
	if (!wxTheApp->CallOnInit())
		return -1;

	MyFrame* frame = dynamic_cast<MyFrame*>(wxTheApp->GetTopWindow());
	auto res = [&frame](std::string mes) {
		frame->ShowHiddenTabWithText(mes);
	};

	Global g(res);
	Global::objLog->WriteToLog(LogLevel::INFO, Global::AntiVirusX+ Global::Start+ Global::Scanning);
	ThreatMonitoring threatMonitoring;
	threatMonitoring.Start(hInstance, nCmdShow);

    int code = wxTheApp->OnRun();
	while (Global::running) {}

	wxTheApp->OnExit();
	wxEntryCleanup();
	return code;

}

