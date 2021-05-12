// Class Player
class c_player
{
private:
    // Player default position
    float f_X=1;
    float f_Y=1;
    // Previous player position
    int lastGraczPosX = 5;
    int lastGraczPosY = 5;
    //Data
    int i_wynik = 0;
    float f_predkosc = 1;
    // Buffs and debuffs W.I.P
    int i_buffTime=0;
    int i_trapTime=0;
public:
    void ZmienPozycje(int pozX, int pozY){
        if (pozX>=0)
            f_X=pozX;
        if (pozY>=0)
            f_Y=pozY;
    }
    // Score
    void SetScore(int wartosc){i_wynik = wartosc;}
    int GetScore(){return i_wynik;};
    // Pos
    void setY(float value){f_Y = value;}
    void addY(float value){f_Y = f_Y+value;}
    int getY(){return ((int) f_Y);}
    void setX(float value){f_X = value;}
    void addX(float value){f_X = f_X+value;}
    int getX(){return ((int) f_X);}
    float GetSpeed(){return f_predkosc;}
    void SetSpeed(float value){f_predkosc=value;}
    // Last pos
    void setLastY(float value){lastGraczPosY = value;}
    void setLastX(float value){lastGraczPosX = value;}
    int getLastX(){return ((int) lastGraczPosX);}
    int getLastY(){return ((int) lastGraczPosY);}
    //Buffs W.I.P
    int getBuff(){return i_buffTime;}
    void substractBuff(){i_buffTime-=1;}
    void activateBuff(){i_buffTime+=10;}
    //Traps W.I.P
    int getTrap(){return i_trapTime;}
    void substractTrap(){i_trapTime-=1;}
    void activateTrap(){i_trapTime+=10;}

};
