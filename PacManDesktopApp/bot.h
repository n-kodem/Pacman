// CLASS BOT
class c_bot{
private:
    float f_X=8;
    float f_Y=12;
    float f_predkosc = 1;
    int i_oldX;
    int i_oldY;
    //Buffs i debuffs
    int b_buff=false;
    int b_trap=false;
    public:
    // Movement
    void setY(float value){f_Y = value;}
    void addY(float value){f_Y = f_Y+value;}
    int getY(){return ((int) f_Y);}
    void setX(float value){f_X = value;}
    void addX(float value){f_X = f_X+value;}
    int getX(){return ((int) f_X);}
    int geti_oldX(){return i_oldX;}
    int geti_oldY(){return i_oldY;}
    void seti_oldX(float value){i_oldX=value;}
    void seti_oldY(float value){i_oldY=value;}
    // Speed
    float getbPredkosc(){return f_predkosc;}
    void setbPredkosc(float value){f_predkosc=value;}
    // Buffs
    int getBuff(){return b_buff;}
    void setBuff(bool newBuff){b_buff=newBuff;}
    // Traps
    int getTrap(){return b_trap;}
    void setTrap(bool newTrap){b_trap=newTrap;}
};
