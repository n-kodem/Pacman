/***************************************************************
 * Name:      PacManDesktopAppApp.cpp
 * Purpose:   Code for Application Class
 * Author:    n_kodem ()
 * Created:   2021-02-13
 * Copyright: n_kodem ()
 * License:
 **************************************************************/

#include "PacManDesktopAppApp.h"

//(*AppHeaders
#include "PacManDesktopAppMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(PacManDesktopAppApp);

bool PacManDesktopAppApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	PacManDesktopAppFrame* Frame = new PacManDesktopAppFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
