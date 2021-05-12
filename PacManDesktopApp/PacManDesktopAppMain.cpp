/***************************************************************
 * Name:      PacManDesktopAppMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    n_kodem ()
 * Created:   2021-02-13
 * Copyright: n_kodem ()
 * License:
 **************************************************************/

#include "PacManDesktopAppMain.h"
#include "player.h"
#include "bot.h"
#include "map.h"
#include "pathfinding.h"
#include <wx/msgdlg.h>
#include <iostream>
#include <wx/timer.h>
#include <wx/vector.h>
#include <fstream>
#include <windows.h>
#include <stdlib.h>
#include <bits/stdc++.h>
// namespace
using namespace std;
//(*InternalHeaders(PacManDesktopAppFrame)
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}
// Ghost
c_bot bot_ghostSlow;
c_bot bot_ghostFast;
// Map
c_map map_Map;
// Player
c_player player_Player;

static wchar_t* charToWChar(const char* text)
{
    const size_t size = strlen(text) + 1;
    wchar_t* wText = new wchar_t[size];
    mbstowcs(wText, text, size);
    return wText;
}


char audiofile_char[50];
string audiofile_str;
fstream fp;
unsigned int i_timer = 0, i_fastGhostTimer=4,i_slowGhostTimer=14;
bool b_isClicked = false;
int dialogOutput;
bool modal = false;


//(*IdInit(PacManDesktopAppFrame)
const long PacManDesktopAppFrame::ID_STATICBITMAP1 = wxNewId();
const long PacManDesktopAppFrame::ID_PANEL1 = wxNewId();
const long PacManDesktopAppFrame::ID_STATICTEXT1 = wxNewId();
const long PacManDesktopAppFrame::ID_STATICTEXT2 = wxNewId();
const long PacManDesktopAppFrame::idMenuQuit = wxNewId();
const long PacManDesktopAppFrame::idMenuMusic = wxNewId();
const long PacManDesktopAppFrame::idMenuAbout = wxNewId();
//*)

BEGIN_EVENT_TABLE(PacManDesktopAppFrame,wxFrame)
    //(*EventTable(PacManDesktopAppFrame)
       EVT_KEY_DOWN(PacManDesktopAppFrame::OnKeyDown)
       EVT_ACTIVATE(PacManDesktopAppFrame::handlerFuncName)
       EVT_TIMER(-2, PacManDesktopAppFrame::OnMainTimer)
    //*)
END_EVENT_TABLE()

PacManDesktopAppFrame::PacManDesktopAppFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(PacManDesktopAppFrame)
    wxMenu* Menu1;
    wxMenu* Menu2;
    wxMenuBar* MenuBar1;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem2;

    Create(parent, wxID_ANY, _("Pacman"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX, _T("wxID_ANY"));
    SetClientSize(wxSize(800,836));
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(0,0), wxSize(800,800), 0, _T("ID_PANEL1"));
    Panel1->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_SCROLLBAR));
    StaticBitmap1 = new wxStaticBitmap(Panel1, ID_STATICBITMAP1, wxNullBitmap, wxPoint(232,264), wxDefaultSize, 0, _T("ID_STATICBITMAP1"));
    scoreText = new wxStaticText(this, ID_STATICTEXT1, _("Wynik: 0"), wxPoint(0,800), wxSize(88,40), 0, _T("ID_STATICTEXT1"));
    timeText = new wxStaticText(this, ID_STATICTEXT2, _("Twoj czas: 0s"), wxPoint(632,800), wxSize(168,40), 0, _T("ID_STATICTEXT2"));
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu3 = new wxMenu();
    MenuItem3 = new wxMenuItem(Menu3, idMenuMusic, _("Music"), _("Use to turn on/off music"), wxITEM_CHECK);
    Menu3->Append(MenuItem3);
    MenuItem3->Check(true);
    MenuBar1->Append(Menu3, _("Options"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);

    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PacManDesktopAppFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PacManDesktopAppFrame::OnAbout);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&PacManDesktopAppFrame::OnClose);
    //*)

    // Setting up bots pos
    bot_ghostFast.setX(7);
    bot_ghostFast.setY(7);
    bot_ghostSlow.setX(5);
    bot_ghostSlow.setY(5);
    //
    // ---Map Render Start
    map_Map.wczytajPlikMapy("mapa.txt");
    for(int i = 0; i<20; i++){
        for(int j = 0; j<20; j++){
            map_Map.Mapa[i][j] = new wxStaticBitmap(
                    PacManDesktopAppFrame::Panel1,
                        wxID_ANY,
                        wxBitmap(wxImage(map_Map.loadMapData(player_Player.getX(),player_Player.getY(),bot_ghostFast.getX() ,bot_ghostFast.getY() ,bot_ghostSlow.getX() ,bot_ghostSlow.getY() ,i ,j)),wxBITMAP_TYPE_PNG),
                        wxPoint(i*40,j*40),
                        wxSize(40,40),
                        0

            );}}
    map_Map.countPoints();
    // ---Map Render End
    // Timer Start
    m_timer = new wxTimer(this, -2);
    m_timer->Start(250);    // Interwal
    // Timer End


}

void PacManDesktopAppFrame::EndGame(){
    // ENDGAME BOX
        // Stops timer
        m_timer->Stop();
        // Message dialog with actual player points
        wxMessageDialog *dial = new wxMessageDialog(NULL,
        wxT("Twoj wynik: "+to_string(player_Player.GetScore())+".\nCzy chcesz wyjsc z gry?"), wxT("KONIEC GRY"),
        wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
        // Show dialog and get user choice to this variable
        dialogOutput=dial->ShowModal();
        // If User have choosen yes
        if(dialogOutput==5103){
            Destroy();
        }
        else{
            // Resets stats and loading first lvl map
            map_Map.setLevel(0);
            map_Map.wczytajPlikMapy("mapa.txt");
            player_Player.SetScore(0);
            map_Map.setTime(0);
            // Reset pos
            player_Player.setX(1);
            player_Player.setY(1);
            bot_ghostFast.setX(7);
            bot_ghostFast.setY(7);
            bot_ghostSlow.setX(5);
            bot_ghostSlow.setY(5);
            for(int i = 0; i<20; i++){
            for(int j = 0; j<20; j++){
                delete map_Map.Mapa[i][j];
                map_Map.Mapa[i][j] = new wxStaticBitmap(
                        PacManDesktopAppFrame::Panel1,
                            wxID_ANY,
                            wxBitmap(wxImage(map_Map.loadMapData(player_Player.getX(),player_Player.getY(),bot_ghostFast.getX() ,bot_ghostFast.getY() ,bot_ghostSlow.getX() ,bot_ghostSlow.getY() ,i ,j)),wxBITMAP_TYPE_PNG),
                            wxPoint(i*40,j*40),
                            wxSize(40,40),
                            0
                );
                map_Map.Mapa[i][j] -> Refresh();
            }
            }
            map_Map.countPoints();
            modal=false;
            // Start timer again
            m_timer->Start();
        }
    //
}

PacManDesktopAppFrame::~PacManDesktopAppFrame()
{
    //(*Destroy(PacManDesktopAppFrame)
    //*)
    delete m_timer;
}
void PacManDesktopAppFrame::OnQuit(wxCommandEvent& event){exit(0);}
void PacManDesktopAppFrame::OnAbout(wxCommandEvent& event)
{
    std::string data = "Gra autorstwa: Nikodem Reszka \nSterowanie:\nESC - Wyjscie z Gry\nW - Gora\nS - Dol\nA - Lewo\nD - Prawo";
    wxString msg(data.c_str(), wxConvUTF8);
    wxMessageBox(msg, _("informacje:"));
}
void PacManDesktopAppFrame::OnKeyDown(wxKeyEvent& event)
{
    // isClicked prevents from texture bugs and generally makes player move slower
    if(!b_isClicked){
    b_isClicked=true;
    const int mystring = event.GetKeyCode();
    if((mystring==87 || mystring==315)||(mystring==83 || mystring==317)||(mystring==65 || mystring==314)||(mystring==68 || mystring==316)){
        bool isPoint,isSpeedBoost,isSlowBoost;
        const int playerY = player_Player.getY(),playerX = player_Player.getX();
        // Change new and last player pos
        player_Player.setLastX(player_Player.getX());
        player_Player.setLastY(player_Player.getY());
        switch(mystring) {
            case 87: case 315:
                    if((player_Player.getY()-1!=-1)&&(map_Map.getMapData()[player_Player.getX()][player_Player.getY()-1]!="MUR")){
                        player_Player.setY(player_Player.getY()-1);
                        }break;
            case 83: case 317:
                    if((player_Player.getY()+1!=(sizeof map_Map.Mapa/sizeof map_Map.Mapa[0]))&&(map_Map.getMapData()[player_Player.getX()][player_Player.getY()+1]!="MUR")){
                        player_Player.setY(player_Player.getY()+1);
                    }break;
            case 65: case 314:
                    if ((player_Player.getX()-1!=-1)&&(map_Map.getMapData()[player_Player.getX()-1][player_Player.getY()]!="MUR")){
                        player_Player.setX(player_Player.getX()-1);
                    }break;
            case 68: case 316:
                    if((player_Player.getX()+1!=sizeof map_Map.Mapa[0]/sizeof(int))&&(map_Map.getMapData()[player_Player.getX()+1][player_Player.getY()]!="MUR")){
                        player_Player.setX(player_Player.getX()+1);
                    }break;
        }
        // Points and boost check
        isPoint = (map_Map.getMapData()[player_Player.getX()][player_Player.getY()]=="PK1")?true:false;
        isSpeedBoost = (map_Map.getMapData()[player_Player.getX()][player_Player.getY()]=="PEF")?true:false;
        isSlowBoost = (map_Map.getMapData()[player_Player.getX()][player_Player.getY()]=="PES")?true:false;
        // Movement
        if((playerX!=player_Player.getX())||(playerY!=player_Player.getY())){
            map_Map.Mapa[player_Player.getX()][player_Player.getY()]->Destroy();
            map_Map.Mapa[player_Player.getLastX()][player_Player.getLastY()]->Destroy();
            map_Map.Mapa[player_Player.getX()][player_Player.getY()] = new wxStaticBitmap(
                    Panel1,
                        wxID_ANY,
                        wxBitmap(wxImage(_T("pacman.png")),wxBITMAP_TYPE_PNG),
                        wxPoint(player_Player.getX()*40,player_Player.getY()*40),
                        wxSize(40,40)

            );
            map_Map.Mapa[player_Player.getLastX()][player_Player.getLastY()] = new wxStaticBitmap(
                    Panel1,
                        wxID_ANY,
                        wxBitmap(wxImage(_T("free.bmp")),wxBITMAP_TYPE_PNG),
                        wxPoint(player_Player.getLastX()*40,player_Player.getLastY()*40),
                        wxSize(40,40)
            );
            // Point/Boost check
            if(isPoint||isSpeedBoost||isSlowBoost){
                map_Map.setMapData("WOL", player_Player.getX(), player_Player.getY());
                if(MenuItem3->IsChecked()){
                    //(isPoint)?char* audiofile_str =(char*)"point.wav":(isSpeedBoost)?char* audiofile_str =(char*)"point.wav":(isSlowBoost)?char* audiofile_str =(char*)"point.wav":0;
                    char* audiofile_str =(char*)"point.wav";
                    PlaySound(charToWChar(audiofile_str), NULL, SND_ASYNC);
                }
                player_Player.SetScore(player_Player.GetScore()+1);
                (isPoint)?map_Map.setMapPoints(map_Map.getMapPoints()-1),0:true;
                scoreText ->SetLabel("Wynik: "+std::to_string(player_Player.GetScore()));
            }
            // Level change/ End Game
            if (map_Map.getMapPoints()==0){
                map_Map.setLevel(map_Map.getLevel()+1);
                if(map_Map.getLevel()==1){
                    map_Map.wczytajPlikMapy("mapa2.txt");
                    player_Player.setX(1);
                    player_Player.setY(1);
                    bot_ghostFast.setX(7);
                    bot_ghostFast.setY(7);
                    bot_ghostSlow.setX(5);
                    bot_ghostSlow.setY(5);
                    for(int i = 0; i<20; i++){
                    for(int j = 0; j<20; j++){
                        delete map_Map.Mapa[i][j];
                        map_Map.Mapa[i][j] = new wxStaticBitmap(
                                PacManDesktopAppFrame::Panel1,
                                    wxID_ANY,
                                    wxBitmap(wxImage(map_Map.loadMapData(player_Player.getX(),player_Player.getY(),bot_ghostFast.getX() ,bot_ghostFast.getY() ,bot_ghostSlow.getX() ,bot_ghostSlow.getY() ,i ,j)),wxBITMAP_TYPE_PNG),
                                    wxPoint(i*40,j*40),
                                    wxSize(40,40),
                                    0
                        );
                        map_Map.Mapa[i][j] -> Refresh();
                    }
                    }
                    map_Map.countPoints();
                }
                if(map_Map.getLevel()==2){
                    EndGame();
                }
            }
            // Pos Refresh
            map_Map.Mapa[player_Player.getX()][player_Player.getY()]->Refresh();
            map_Map.Mapa[player_Player.getLastX()][player_Player.getLastY()]->Refresh();
        }
    }
    // Game quit button
    if (mystring==27){
        Destroy();
    }
    }
}

// FOCUS
void PacManDesktopAppFrame::handlerFuncName(wxActivateEvent& event){if (event.GetActive() == true){this->SetFocus();}}
// OnClose Destroy Window
void PacManDesktopAppFrame::OnClose(wxCloseEvent& event){Destroy();}
void PacManDesktopAppFrame::OnMainTimer(wxTimerEvent& event)
{
    // If this window is not opened and player pos equals ghost pos show endgame dialog
    if( !modal && ((player_Player.getX()==bot_ghostFast.getX() && player_Player.getY()==bot_ghostFast.getY()) || (player_Player.getX()==bot_ghostSlow.getX() && player_Player.getY()==bot_ghostSlow.getY())) ){
        modal=true;
        EndGame();
    }
    // Checks that player can move
    if(i_timer%(int)player_Player.GetSpeed()==0){
        b_isClicked=false;
    }
    // Every second update time
    if(i_timer%4==0){
        map_Map.setTime(map_Map.getTime()+1);
        timeText ->SetLabel("Twoj czas: "+std::to_string(map_Map.getTime()) + "s");
    }
    // S L O W   G H O S T
    if(i_timer%i_slowGhostTimer==0){
        bool isntNearPlayer = true;
        // Checks that bot is near player
        if(((bot_ghostSlow.getX()-1==player_Player.getX())&&(bot_ghostSlow.getY()==player_Player.getY()))||((bot_ghostSlow.getX()+1==player_Player.getX())&&(bot_ghostSlow.getY()==player_Player.getY()))||((bot_ghostSlow.getX()==player_Player.getX())&&(bot_ghostSlow.getY()-1==player_Player.getY()))||((bot_ghostSlow.getX()==player_Player.getX()) && (bot_ghostSlow.getY()+1 == player_Player.getY()))){
            isntNearPlayer=false;
        }
        // If bot isnt near player then find path
        if (isntNearPlayer){
            Solver solve(Position(player_Player.getX(),player_Player.getY()),Position(bot_ghostSlow.getX(),bot_ghostSlow.getY()), 20,map_Map.getMapData());
            std::array<int,2> tempPosition;
            tempPosition=solve.aStar(20);
            bot_ghostSlow.seti_oldX(bot_ghostSlow.getX());
            bot_ghostSlow.seti_oldY(bot_ghostSlow.getY());
            bot_ghostSlow.setX(tempPosition[0]);
            bot_ghostSlow.setY(tempPosition[1]);
        }
        else{
            bot_ghostSlow.seti_oldX(bot_ghostSlow.getX());
            bot_ghostSlow.seti_oldY(bot_ghostSlow.getY());
            bot_ghostSlow.setX(player_Player.getX());
            bot_ghostSlow.setY(player_Player.getY());
        }
        // Deletes old data
        map_Map.Mapa[bot_ghostSlow.getX()][bot_ghostSlow.getY()]->Destroy();
        map_Map.Mapa[bot_ghostSlow.geti_oldX()][bot_ghostSlow.geti_oldY()]->Destroy();
        // Replaces with new data
        map_Map.Mapa[bot_ghostSlow.getX()][bot_ghostSlow.getY()] = new wxStaticBitmap(
                Panel1,wxID_ANY,
                    wxBitmap(wxImage(_T("capman.png")),wxBITMAP_TYPE_PNG),
                    wxPoint(bot_ghostSlow.getX()*40,bot_ghostSlow.getY()*40),
                    wxSize(40,40),
                    0
        );
        map_Map.Mapa[bot_ghostSlow.geti_oldX()][bot_ghostSlow.geti_oldY()] = new wxStaticBitmap(
                Panel1,wxID_ANY,
                    wxBitmap(wxImage(map_Map.loadMapData(player_Player.getX(),player_Player.getY(),bot_ghostSlow.getX() ,bot_ghostSlow.getY() ,bot_ghostSlow.getX() ,bot_ghostSlow.getY() ,bot_ghostSlow.geti_oldX() ,bot_ghostSlow.geti_oldY())),wxBITMAP_TYPE_BMP),
                    wxPoint(bot_ghostSlow.geti_oldX()*40,bot_ghostSlow.geti_oldY()*40),
                    wxSize(40,40),
                    0
        );
        // Images refresh
        map_Map.Mapa[bot_ghostSlow.getX()][bot_ghostSlow.getY()]->Refresh();
        map_Map.Mapa[bot_ghostSlow.geti_oldX()][bot_ghostSlow.geti_oldY()]->Refresh();
    }
    //F A S T   G H O S T
    if(i_timer%i_fastGhostTimer==0){
        bool isntNearPlayer = true;
        // Checks that bot is near player
        if( ( (bot_ghostFast.getX()-1 == player_Player.getX()) && (bot_ghostFast.getY() == player_Player.getY()) ) || ( (bot_ghostFast.getX()+1 == player_Player.getX()) && (bot_ghostFast.getY() == player_Player.getY()) ) || ( (bot_ghostFast.getX() == player_Player.getX()) && (bot_ghostFast.getY()-1 == player_Player.getY()) ) || ( (bot_ghostFast.getX() == player_Player.getX()) && (bot_ghostFast.getY()+1 == player_Player.getY()) ) ){
            isntNearPlayer=false;
        }
        // If bot isnt near player then find path
        if (isntNearPlayer){
            Solver solve(Position(player_Player.getX(),player_Player.getY()),Position(bot_ghostFast.getX(),bot_ghostFast.getY()), 20,map_Map.getMapData());
            std::array<int,2> tempPosition;
            tempPosition=solve.aStar(20);
            bot_ghostFast.seti_oldX(bot_ghostFast.getX());
            bot_ghostFast.seti_oldY(bot_ghostFast.getY());
            bot_ghostFast.setX(tempPosition[0]);
            bot_ghostFast.setY(tempPosition[1]);
        }
        else{
            bot_ghostFast.seti_oldX(bot_ghostFast.getX());
            bot_ghostFast.seti_oldY(bot_ghostFast.getY());
            bot_ghostFast.setX(player_Player.getX());
            bot_ghostFast.setY(player_Player.getY());
        }
        // Deletes old data
        map_Map.Mapa[bot_ghostFast.getX()][bot_ghostFast.getY()]->Destroy();
        map_Map.Mapa[bot_ghostFast.geti_oldX()][bot_ghostFast.geti_oldY()]->Destroy();
        // Replaces with new data
        map_Map.Mapa[bot_ghostFast.getX()][bot_ghostFast.getY()] = new wxStaticBitmap(
                Panel1,
                    wxID_ANY,
                    wxBitmap(wxImage(_T("capman.png")),wxBITMAP_TYPE_PNG),
                    wxPoint(bot_ghostFast.getX()*40,bot_ghostFast.getY()*40),
                    wxSize(40,40),
                    0

        );
        map_Map.Mapa[bot_ghostFast.geti_oldX()][bot_ghostFast.geti_oldY()] = new wxStaticBitmap(
                Panel1,
                    wxID_ANY,
                    wxBitmap(wxImage(map_Map.loadMapData(player_Player.getX(),player_Player.getY(),bot_ghostFast.getX() ,bot_ghostFast.getY() ,bot_ghostSlow.getX() ,bot_ghostSlow.getY() ,bot_ghostFast.geti_oldX() ,bot_ghostFast.geti_oldY())),wxBITMAP_TYPE_BMP)
                    ,wxPoint(bot_ghostFast.geti_oldX()*40,bot_ghostFast.geti_oldY()*40),
                    wxSize(40,40),
                    0
        );
        // Images refresh
        map_Map.Mapa[bot_ghostFast.getX()][bot_ghostFast.getY()]->Refresh();
        map_Map.Mapa[bot_ghostFast.geti_oldX()][bot_ghostFast.geti_oldY()]->Refresh();
    }
    //Prevents from overflow
    if(i_timer==4000){
        i_timer=0;
    }
    i_timer+=1;

}

