#include <cpplib/cpplib#gsock>
#include <cpplib/cpplib#gcolor>
#include <vector>
#include <string>
using namespace std;

#include <conio.h>




/// GameHarbor Extends GUI API...
class GUI_DrawableBasic
{
public:
    virtual void draw()=0;
    virtual void drawat(int x,int y)=0;
};
class GUI_Drawable : public GUI_DrawableBasic
{
public:
    void drawat(int x,int y)
    {
        gotoxy(x,y);draw();
    }
};
class GUI_TEXT : public GUI_Drawable
{
public:
    string text;
    color front,back;
    void draw()
    {
        cprint(front,back);printf("%s",text.c_str());
    }
    void set(const string& inctext,color incfront,color incback)
    {
        text=inctext;
        front=incfront;
        back=incback;
    }
};

class GUI_CHOICE : public GUI_Drawable
{
public:
    GUI_TEXT text_active,text_inactive;
    bool isactive;
    void draw()
    {
        if(isactive) text_active.draw();
        else text_inactive.draw();
    }
    void activate()
    {
        isactive=true;
    }
    void inactivate()
    {
        isactive=false;
    }
};

class GUI_SLIDERBAR : public GUI_Drawable
{
public:
    int value,total;
    struct _color_pack
    {
        color color_value;
        color color_total;
    }active,inactive;
    bool isActive;
    void draw()
    {
        if(isActive)
        {
            cprint(color::black,active.color_value);
            for(int i=0;i<value;i++) printf(" ");
            cprint(color::black,active.color_total);
            for(int i=0;i<total-value;i++) printf(" ");
        }
        else
        {
            cprint(color::black,inactive.color_value);
            for(int i=0;i<value;i++) printf(" ");
            cprint(color::black,inactive.color_total);
            for(int i=0;i<total-value;i++) printf(" ");
        }
    }
};

class GUI_SCREEN : public GUI_Drawable
{
public:
    vector<GUI_DrawableBasic*> res;
    void draw()
    {
        for(auto& i:res)
        {
            i->draw();
        }
    }
    void add(GUI_DrawableBasic* pDrawableBasic)
    {
        res.push_back(pDrawableBasic);
    }
};

int Display(GUI_SCREEN* pScreen)
{
    pScreen->draw();
    return 0;
}

inline void ClearScreen()
{
    cprint();
    cprint(CCLRSCR);
}
inline int GetUserInputKeyImmediately()
{
    return getch();///defined in conio.h
}
#ifdef __C4DROID__
#define DEF_KEY_UP '2'
#define DEF_KEY_DOWN '8'
#define DEF_KEY_CONFIRM '5'
#define DEF_KEY_LEFT '4'
#define DEF_KEY_RIGHT '6'
#else
#define DEF_KEY_UP 56
#define DEF_KEY_DOWN 50
#define DEF_KEY_CONFIRM 53
#define DEF_KEY_LEFT 52
#define DEF_KEY_RIGHT 54
#endif
int GUI_GetUserInput(GUI_SCREEN* pScreen)
{
    /// Compile the resources
    vector<GUI_CHOICE*> choiceid;
    for(size_t i=0;i<pScreen->res.size();i++)
    {
        GUI_CHOICE* pChoice;
        if((pChoice=dynamic_cast<GUI_CHOICE*>(pScreen->res.at(i)))!=nullptr)
        {
            pChoice->inactivate();
            choiceid.push_back(pChoice);
            pChoice=nullptr;
        }
    }
    /// No Choice
    if(choiceid.size()<1) return -5;
    /// Active the first choice
    choiceid.at(0)->activate();
    int actived_id=0;
    while(1)
    {
        ClearScreen();
        pScreen->draw();
        int k=GetUserInputKeyImmediately();
        choiceid.at(actived_id)->inactivate();
        switch(k)
        {
        case DEF_KEY_UP:
            actived_id=(actived_id>0)?(actived_id-1):(0);
            break;
        case DEF_KEY_DOWN:
            actived_id=(actived_id<(int)choiceid.size()-1)?(actived_id+1):(choiceid.size()-1);
            break;
        case DEF_KEY_CONFIRM:
            return actived_id;
        }
        choiceid.at(actived_id)->activate();
    }
    return -1;
}

GUI_TEXT _br=[]()->GUI_TEXT{GUI_TEXT s;s.text="\n";s.front=color::black;s.back=color::black;return s;}();
GUI_TEXT* br=&_br;
