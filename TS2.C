#include <dos.h >
#include <conio.h>
#include <graphics.h>
#include <bios.h>
#define MouseStatusMouseOn  1
/*#include <E:\Win-TC\projects\Desk.c>*/
/*#include <E:\Win-TC\projects\MouseR.c>*/         /*�����ǰ����ļ�ͷ*/
struct mouse
{
   int top;
   int botton;
   int left;
   int rigth;
}Smouse;
extern void Dosk();
  unsigned int size;
  void *ball;
int MouseX        =0;        /*�������ˮƽ�����ֵ*/
int MouseY        =0;
char MouseType    =1;


enum MOUSE_MESSAGES_CODE
{        NOTHING=0,
        LBUTTONCLK=1,

};
int arrowMousePixSave[16][10];
int arrowMouseDraw[16][10]={    /*���������״,����1:��ʾ���ı߽�,2:��ʾ���߽�����Χ������,3:��ʾ������������*/
 {1,1,3,3,3,3,3,3,3,3},{1,2,1,3,3,3,3,3,3,3},{1,2,2,1,3,3,3,3,3,3},{1,2,2,2,1,3,3,3,3,3},
 {1,2,2,2,2,1,3,3,3,3},{1,2,2,2,2,2,1,3,3,3},{1,2,2,2,2,2,2,1,3,3},{1,2,2,2,2,2,2,2,1,3},
 {1,2,2,2,2,2,2,2,2,1},{1,2,2,2,2,2,2,1,3,3},{1,2,2,2,2,2,1,3,3,3},{1,2,1,3,1,2,2,1,3,3},
 {1,1,3,3,1,2,2,1,3,3},{3,3,3,3,3,1,2,2,1,3},{3,3,3,3,3,1,2,2,1,3},{3,3,3,3,3,3,1,1,2,1} };
 int editMousePixSave[15][5];
int editMouseDraw[15][5]={
{1,1,3,1,1},{3,3,1,3,3},{3,3,1,3,3},{3,3,1,3,3},{3,3,1,3,3},{3,3,1,3,3},{3,3,1,3,3},{3,3,1,3,3},
{3,3,1,3,3},{3,3,1,3,3},{3,3,1,3,3},{3,3,1,3,3},{3,3,1,3,3},{3,3,1,3,3},{1,1,3,1,1} 
   };
 void GetMouseXY(); 
    void MouseOn(int mousex,int mousey)
{
    int i,j;
    int color;
    if(MouseType==1)
    {
        for(i=0;i<16;i++)
    for(j=0;j<10;j++)
    {
        arrowMousePixSave[i][j]=getpixel(mousex+j,mousey+i);/* ����ԭ������ɫ*/
        if(arrowMouseDraw[i][j]==1)
        putpixel(mousex+j,mousey+i,0);
        else if(arrowMouseDraw[i][j]==2)        /* �����*/
            putpixel(mousex+j,mousey+i,15);
    }
    }
    else if(MouseType==2)
    {
    for(i=0;i<15;i++)
            for(j=0;j<5;j++)
            {
            editMousePixSave[i][j]=getpixel(mousex-2+j,mousey-7+i);
        if(editMouseDraw[i][j]==1)            /*���ڱ༭�����ȵ�����������*/
            putpixel(mousex-2+j,mousey-7+i,0);
        }
    }
}
void SetMouseHori(int minX,int maxX)
 {
    _AX=0x07;
    _CX=minX;
    _DX=maxX;
    geninterrupt(0x33);
}

void SetMouseVert(int minY,int maxY)
 {
    _AX=0x08;
    _CX=minY;
    _DX=maxY;
    geninterrupt(0x33);
}
/*�������*/
void MouseOff(int mousex,int mousey)
{
   int i,j,x,y,color;
   x=mousex;
   y=mousey;
   if(MouseType==1)
   {
      for(i=0;i<16;i++)
      for(j=0;j<10;j++)
      {
        if(arrowMouseDraw[i][j]==3)
        continue;
        color=getpixel(x+j,y+i);
        putpixel(x+j,y+i,color^color);        /*ԭλ��������*/
        putpixel(x+j,y+i,arrowMousePixSave[i][j]);/* ��ԭԭ����*/
      }
   }
   else if(MouseType==2)
   {
        for(i=0;i<15;i++)
      for(j=0;j<5;j++)
        {
           if(editMouseDraw[i][j]==3)
            continue;
         color=getpixel(x-2+j,y-7+i);
         putpixel(x-2+j,y-7+i,color^color);    /*ԭλ��������*/
         putpixel(x-2+j,y-7+i,editMousePixSave[i][j]);/* ��ԭԭ����*/
        }

    }
}
void SetMouseXY(int x,int y)
 {
   _AX=0x04;
   _CX=x;
   _DX=y;
    geninterrupt(0x33);
 }
void GetMouseXY()
{
    _AX=0x03;
    geninterrupt(0x33);
    MouseX=_CX;        /*�����ˮƽ����λ�ô�ŵ�MouseX*/
    MouseY=_DX;        /*����괹ֱ����λ�ô�ŵ�MouseY*/

}
void initgr(void) /* BGI��ʼ�� */
{
  int gd = DETECT, gm = 0; /* ��gd = VGA,gm = VGAHI��ͬ��Ч�� */
  registerbgidriver(EGAVGA_driver);/* ע��BGI��������Բ���Ҫ.BGI�ļ���֧������ */
  initgraph(&gd, &gm, "");
  SetMouseXY(0,0);
  SetMouseHori(0,600);
  SetMouseVert(0,450);
  setfillstyle(SOLID_FILL,7);
           bar(100,280,140,300);

  size=imagesize(100,280,141,301);
  ball=malloc(size);
  getimage(100,280,141,301,ball);
}


void main()
{   int q=0;
   float p=0;
    int temp1=0;
    int x,y,MouseMSG,M;
  int tempx,tempy,mouseMsg;

  char ch1;
  tempx=tempy=100;
  initgr();
   GetMouseXY();
   x=MouseX;
   y=MouseY;
   putimage(100,280,ball,XOR_PUT);
   while(!kbhit())
    {

        GetMouseXY();/*get current mouse position */
        /*�������λ��*/
        if(tempx!=MouseX || tempy!=MouseY)
        {
            MouseOff(tempx,tempy);
            MouseOn(MouseX,MouseY);        /*��ʾ���,�����漴������긲�ǵ�����*/
            tempx=MouseX;                  /* cache current mouse pos */
            tempy=MouseY;
        }
        _AX=0x03;            /*��ȡ��갴ť״̬*/
       geninterrupt(0x33);        /*����33��������ж�*/

        if((_BX&1) && !(_BX&2))        /*������������ͬʱ�Ҽ�û�а���*/
            {
               MouseMSG=4;
               M=MouseMSG;
             }
         if((_BX&2) && !(_BX&1))        /*����Ҽ�������ͬʱ���û�а���*/
            {
              MouseMSG=5;
              M=MouseMSG;
            }
        if(_BX&1 && _BX&2)        /*���������Ҽ�ͬʱ������*/
           {
             MouseMSG=3;
             M=MouseMSG;
           }
        _AX=0x06;            /*��ȡ��갴ť�ͷ���Ϣ*/
        _BX=0x00;            /*ָ����ȡ���������ͷ���Ϣ*/
        geninterrupt(0x33);        /*����33��������ж�*/
        if(_BX==1)            /*������������ͷ���Ϊ1*/
           MouseMSG=2;            /*����һ�����������Ϣ*/
         if(_BX==0&&M==4)
             if( tempx>=100&&tempx<=140&& tempy>=280&&tempy<=300)
             {
               setcolor(RED);
               /*outtextxy(100,100,"111111111111111111111");*/
               GetMouseXY();
                if(tempx!=MouseX || tempy!=MouseY)
                  {
                         /*outtextxy(200,200,"2222222222222");*/
                         putimage(300,270,ball,XOR_PUT);
                         delay(100);

                    }
             }
        /* if(_BX==1&&M==4)   */

 

         _AX=0x06;            /*��ȡ��갴ť�ͷ���Ϣ*/
        _BX=0x01;            /*ָ����ȡ����Ҽ����ͷ���Ϣ*/
        geninterrupt(0x33);        /*����33��������ж�*/
        if(_BX==1)            /*������������ͷŴ���Ϊ1*/
          MouseMSG=1;            /*����һ�������Ҽ���Ϣ*/

 

   }

}