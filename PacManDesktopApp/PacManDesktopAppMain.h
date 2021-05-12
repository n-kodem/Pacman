/***************************************************************
 * Name:      PacManDesktopAppMain.h
 * Purpose:   Defines Application Frame
 * Author:    n_kodem ()
 * Created:   2021-02-13
 * Copyright: n_kodem ()
 * License:
 **************************************************************/

#ifndef PACMANDESKTOPAPPMAIN_H
#define PACMANDESKTOPAPPMAIN_H

//(*Headers(PacManDesktopAppFrame)
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
//*)
#include <wx/timer.h>
class PacManDesktopAppFrame: public wxFrame
{
    public:

        PacManDesktopAppFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~PacManDesktopAppFrame();
        void OnMainTimer(wxTimerEvent& event);
        void ShowMessage3(wxCommandEvent & event);
        void EndGame();
        //void SlowGhostTimer(wxTimerEvent& event);
        //void FastGhostTimer(wxTimerEvent& event);

    private:

        //(*Handlers(PacManDesktopAppFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnMusic(wxCommandEvent& event);
        //void OnButton1Click(wxCommandEvent& event);
        void OnKeyDown(wxKeyEvent& event);
        void handlerFuncName(wxActivateEvent& event);
        void OnClose(wxCloseEvent& event);
        //*)

        //(*Identifiers(PacManDesktopAppFrame)
        static const long ID_STATICBITMAP1;
        static const long ID_PANEL1;
        static const long ID_STATICTEXT1;
        static const long ID_STATICTEXT2;
        static const long idMenuQuit;
        static const long idMenuMusic;
        static const long idMenuAbout;
        //*)

        //(*Declarations(PacManDesktopAppFrame)
        wxMenu* Menu3;
        wxMenuItem* MenuItem3;
        wxPanel* Panel1;
        wxStaticBitmap* StaticBitmap1;
        wxStaticText* scoreText;
        wxStaticText* timeText;
        //*)
        wxTimer*m_timer;
        DECLARE_EVENT_TABLE()
};

#endif // PACMANDESKTOPAPPMAIN_H
