/** Kiritow's GUI(CUI) System.
*   Public Version
*   Created On: 20161109
*
*/
#ifndef _KIRITOW_CUI_PUBLIC_VERSION
#define _KIRITOW_CUI_PUBLIC_VERSION

#include <cpplib/cpplib#gcolor>
#include <cstdio>
#include <conio.h>


int _last_keyval;
int _GetUserInputKey_Real()
{
    return _last_keyval=getch();///defined in conio.h
}

#define KEY_UNDEF 0
#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_LEFT 3
#define KEY_RIGHT 4
#define KEY_CONFIRM 5
#define KEY_ESC 6
#define KEY_HOME 7
#define KEY_END 8

/** Update! Now you can change key in settings
*   Update! Key: Esc, Home, End added.
*/
int _last_isSpecial;

int GetUserInputKey()
{
    int a=_GetUserInputKey_Real();
    if(a==224||a==0)
    {
        _last_isSpecial=1;
        int b=_GetUserInputKey_Real();
        switch(b)
        {
            case 72:return KEY_UP;
            case 80:return KEY_DOWN;
            case 75:return KEY_LEFT;
            case 77:return KEY_RIGHT;
            case 71:return KEY_HOME;
            case 79:return KEY_END;
            default:return KEY_UNDEF;
        }
    }
    else
    {
        _last_isSpecial=0;
        switch(a)
        {
            case ' ':case '\r':case '\n':case '5':
            return KEY_CONFIRM;

            case 'w':case 'W':
            #ifdef __C4DROID__
            case '2':
            #else
            case '8':
            #endif /// End of KEY_UP Cross-Platform Definition
            return KEY_UP;

            case 's':case 'S':
            #ifdef __C4DROID__
            case '8':
            #else
            case '2':
            #endif /// End of KEY_DOWN Cross-Platform Definition
            return KEY_DOWN;

            case 'a':case 'A':case '4':
            return KEY_LEFT;

            case 'd':case 'D':case '6':
            return KEY_RIGHT;

            case 27:
            return KEY_ESC;

            default:
            return KEY_UNDEF;
        }
    }
}

void KeyCheckLoop()
{
    while(1)
    {
        int c=GetUserInputKey();
        switch(c)
        {
        case KEY_UNDEF:
            printf("Undefined.\n");
            break;
        case KEY_UP:
            printf("Up.\n");
            break;
        case KEY_DOWN:
            printf("Down.\n");
            break;
        case KEY_LEFT:
            printf("Left.\n");
            break;
        case KEY_RIGHT:
            printf("Right.\n");
            break;
        case KEY_CONFIRM:
            printf("Confirm.\n");
            break;
        case KEY_ESC:
            printf("Escape.\n");
            break;
        case KEY_HOME:
            printf("Home.\n");
            break;
        case KEY_END:
            printf("End.\n");
            break;
        }
    }
}

int GetAction_WS(int* p,int MinVal,int MaxVal)
{
    int t=GetUserInputKey();
    switch(t)
    {
    case KEY_UP:
        {
            --(*p);
            *p=*p<MinVal?MinVal:*p;
            *p=*p>MaxVal?MaxVal:*p;
            return 0;
        }
    case KEY_DOWN:
        {
            ++(*p);
            *p=*p<MinVal?MinVal:*p;
            *p=*p>MaxVal?MaxVal:*p;
            return 0;
        }
    case KEY_CONFIRM:
        return 1;
    default:
        return 0;
    }
}

int GetAction_WS_E(int* p,int MinVal,int MaxVal,int EscapeVal)
{
    int t=GetUserInputKey();
    switch(t)
    {
    case KEY_UP:
        {
            --(*p);
            *p=*p<MinVal?MinVal:*p;
            *p=*p>MaxVal?MaxVal:*p;
            return 0;
        }
    case KEY_DOWN:
        {
            ++(*p);
            *p=*p<MinVal?MinVal:*p;
            *p=*p>MaxVal?MaxVal:*p;
            return 0;
        }
    case KEY_ESC:
        {
            *p=EscapeVal;
            return 0;
        }
    case KEY_CONFIRM:
        return 1;
    default:
        return 0;
    }
}
typedef int (*IACTION)(int isControl,int Key,int* cid);
int GetAction_WS_E_X(int* p,int MinVal,int MaxVal,int EscapeVal,IACTION FunctionPointer)
{
    int t=GetUserInputKey();
    switch(t)
    {
    case KEY_UP:
        {
            --(*p);
            *p=*p<MinVal?MinVal:*p;
            *p=*p>MaxVal?MaxVal:*p;
            return 0;
        }
    case KEY_DOWN:
        {
            ++(*p);
            *p=*p<MinVal?MinVal:*p;
            *p=*p>MaxVal?MaxVal:*p;
            return 0;
        }
    case KEY_ESC:
        {
            *p=EscapeVal;
            return 0;
        }
    case KEY_CONFIRM:
        return 1;
    default:
        if(FunctionPointer)
        {
            return FunctionPointer(_last_isSpecial,t,p);
        }
        else
        {
            return 0;
        }
    }
}

#define TITLE(FormatString,Args...) do{cprint(color::black,color::lightblue);printf(FormatString,##Args);resetcolor();}while(0)
#define WARNING(FormatString,Args...) do{cprint(color::yellow,color::black);printf(FormatString,##Args);resetcolor();}while(0)
#define CONFIRM _ABOX_Confirm
#define TAG(cid,tagID) do{if(cid==tagID)cprint(color::black,color::yellow);else cprint(color::white,color::black);}while(0)
#define TAGPRINT(cid,tagID,FormatString,Args...) do{TAG(cid,tagID);printf(FormatString,##Args);}while(0)
#define INFOPRINT(cid,tagID,FormatString,Args...) do{if(tagID==cid) printf(FormatString,##Args);}while(0)
#define INFODO(cid,tagID,USG) do{if(tagID==cid) {USG} }while(0)
//#define GetAction(cid,minVal,maxVal) GetAction_WS(&cid,minVal,maxVal)
//#define GetAction(cid,minVal,maxVal) GetAction_WS_E(&cid,minVal,maxVal,maxVal)
#define GetAction(cid,minVal,maxVal) GetAction_WS_E_X(&cid,minVal,maxVal,maxVal,NULL)
#define GetActionX(minVal,maxVal,FunctionPointer) GetAction_WS_E_X(&cid,minVal,maxVal,maxVal,FunctionPointer)
#define GetActionEX(minVal,maxVal,escVal,FunctionPointer) GetAction_WS_E_X(&cid,minVal,maxVal,escVal,FunctionPointer)
void ClearInput();
void ClearScreen();
void resetcolor();

int _ABOX_Confirm(const char* fmt,...)
{
    cprint(color::yellow,color::black);
    va_list vlist;
    va_start(vlist,fmt);
    vprintf(fmt,vlist);
    va_end(vlist);
    ClearInput();
    return (GetUserInputKey()==KEY_CONFIRM);
}

#endif /// End of _KIRITOW_CUI_PUBLIC_VERSION
