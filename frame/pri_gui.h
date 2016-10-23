int _GetUserInputKey_Real()
{
    return getch();///defined in conio.h
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

int GetUserInputKey()
{
    int a=_GetUserInputKey_Real();
    if(a==224||a==0)
    {
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
        switch(a)
        {
            case ' ':case '\r':case '\n':case '5':
            return KEY_CONFIRM;
            case 'w':case 'W':case '8':
            return KEY_UP;
            case 's':case 'S':case '2':
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

using color;
#define TAG(tagID,realID) do{if(realID==tagID)cprint(black,yellow);else cprint(white,black);}while(0)
#define TAGPRINT(tagID,realID,FormatString,Args...) do{TAG(tagID,realID);printf(FormatString,##Args);}while(0)
#define INFOPRINT(tagID,realID,FormatString,Args...) do{if(tagID==realID) printf(FormatString,##Args);}while(0)
#define INFODO(tagID,realID,USG) do{if(tagID==realID) {USG} }while(0)
//#define GetAction(realID,minVal,maxVal) GetAction_WS(&realID,minVal,maxVal)
#define GetAction(realID,minVal,maxVal) GetAction_WS_E(&realID,minVal,maxVal,maxVal)
#define ClearInput() fflush(stdin)
