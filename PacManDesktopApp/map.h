#include <wx/msgdlg.h>
#include <iostream>
#include <wx/timer.h>
#include <wx/vector.h>
#include <fstream>
#include <bits/stdc++.h>
#include "PacManDesktopAppMain.h"
using namespace std;
// Class MAP
class c_map
{
private:
    const unsigned int POINT_2=5;
    const unsigned int POINT_1=1;
    int i_poziom = 0;
    int i_points = 0;
    int i_allTime = 0;
    string MapData[20][20];
    string PrimaryMap[20][20];
    int i_RozmiarMapyX = 20;
    int i_RozmiarMapyY = 20;
    int i_pozostale_punkty =1;
public:
    wxStaticBitmap* Mapa[20][20];
    // LOADING FILE INTO MAP
    void wczytajPlikMapy(string NazwaPliku){
            string singleLine[20],line,text;
            short loop=0;
            ifstream myfile (NazwaPliku);
            int x;
            if (myfile.is_open())
            {
                while (! myfile.eof() )
                {
                    getline (myfile,line);
                    singleLine[loop] = line;
                    istringstream streamedString(singleLine[loop]);
                    x = 0;
                    while(streamedString >> text){
                        MapData[x][loop]=text;
                        PrimaryMap[x][loop]=text;
                        cout<<MapData[x][loop];
                        ++x;
                    };
                    cout<<"\n";
                    loop++;
                }
                myfile.close();
            }
            else cout << "Brak pliku";
    };
    // SAVING THE FILE TO THE MAP
    void zapiszPlikMapy(char* NazwaPliku){
        fstream plik;
        // Otwieranie wyznaczonego pliku
        plik.open( NazwaPliku, ios::out);
        //Jezeli plik istnieje, zapisuje plik
        if(plik.good())
        {
            plik << i_RozmiarMapyX<<" "<<endl;
            plik << i_RozmiarMapyY<<" "<<endl;
            for (int i=0; i<i_RozmiarMapyY;i++){
                for(int j=0; j<i_RozmiarMapyX; j++){
                    plik<<Mapa[i][j]<<" ";
                }
                plik<<endl;
            }
            plik.close();
        }
            //Jezeli plik nie istnieje
        else{
            cout<<"Brak pliku";
        }
    }
    // Loading a map based on data
    string loadMapData(int playerX, int playerY,int fastBotX, int fastBotY ,int slowBotX ,int slowBotY, int i, int j){
        if(i==playerX && j==playerY){
            return "pacman.png";
        }
        if(i==fastBotX && j==fastBotY){
            return "capman.png";
        }
        if(i==slowBotX && j==slowBotY){
            return "capman.png";
        }
        else if(MapData[i][j]=="WOL"){
            return "free.bmp";
        }
        else if(MapData[i][j]=="PK1"){
            return "pk1.bmp";
        }
        else if(MapData[i][j]=="PEF"){
            return "pef.bmp";
        }
        else if(MapData[i][j]=="PES"){
            return "pes.bmp";
        }
        else{
            return "wall.png";
        }
    }
    // Get Map Data
    string** getMapData(){
        string** table = new string*[20];
        for(int i = 0; i < 20; i++) {
            table[i] = new string[20];
            for(int j = 0; j < 20; j++){table[i][j] = MapData[i][j];}
        }
        return table;
    }
    void countPoints(){
        i_points=0;
        // Downloading wall informations
        for (int i = 0;i<20;i+=1){
            for(int j = 0;j<20;j+=1){
                if(MapData[i][j]=="PK1"){i_points+=1;}
            }
        }

    }
    // Simple sets / gets
    string getPrimData(int X, int Y){return PrimaryMap[X][Y];}
    void setLevel(int value){i_poziom=value;}
    int getLevel(){return i_poziom;}
    int getTime(){return i_allTime;}
    void setTime(int value){i_allTime=value;}
    int geti_RozmiarMapyX(){return i_allTime;}
    void seti_RozmiarMapyX(int value){i_allTime=value;}
    int getMapPoints(){return i_points;}
    void setMapPoints(int value){i_points = value;}
    void setMapData(string value, int x, int y){MapData[x][y]=value;}
};
