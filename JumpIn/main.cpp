#include <iostream>
#include <cstring>
#include <graphics.h>
#include <winbgim.h>
#include <MMsystem.h>
#include <fstream>
using namespace std;
ifstream fin_niveluri("niveluri.txt");
fstream fin_progres("progres.in");
fstream moves("miscari.in");
fstream salvate("creatie.txt");
bool selected=0;
int dimensiune=500,casute=5,startx=300,starty=100,windowx=1100,windowy=700,latura=dimensiune/casute;
int n=30,CREATIE,i,j,linie,coloana,lg=1,moved,audioOn=1,dimbutx=220,dimbuty=58,progB,progA,closed=0,fundal=1;
int butonx=windowx-dimensiune-150,butony=windowy-dimensiune-dimensiune/10+147;
int vect[100],ss,saltj[5];
struct nivel{int n;
             int iepuri[10];
             int tabla[8][8];}usor[62],sablon[62];

void PornireJoc();
bool sfarsitjoc(int k)///Gabi
{
    int i;
    int ok=1;
    for(i=1;i<=3;i++)
        if(usor[k].iepuri[i]>=1)ok=0;//verifica frecventa iepuri
   return ok;
}
void startGame(int k);
void shop(int &fundal);
void creareTabla()
{
    clearviewport();
    int latura=dimensiune/casute;
    if(fundal==1)
      readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\camp.jpg",0,0,windowx,windowy);
    else if(fundal==2)
           readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\camp2.jpg",0,0,windowx,windowy);
         else if(fundal==3)
                readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\camp3.jpg",0,0,windowx,windowy);
              else if(fundal==4)
                    readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\camp4.jpg",0,0,windowx,windowy);
    setcolor(WHITE);//bordura
    line(startx-30,starty-30,startx+dimensiune+30,starty-30);
    line(startx-30,starty+30+dimensiune,startx+dimensiune+30,starty+30+dimensiune);
    line(startx-30,starty-30,startx-30,starty+dimensiune+30);
    line(startx+dimensiune+30,starty-30,startx+dimensiune+30,starty+dimensiune+30);
    setfillstyle(SOLID_FILL,GREEN);
    bar(startx,starty,dimensiune+startx,dimensiune+starty);
    for (i=startx;i<=dimensiune+startx;i+=latura)
    {
        setcolor(BLACK);
        line(i,starty,i,starty+dimensiune);//verticala
    }
    for(i=starty;i<=dimensiune+starty;i+=latura)
    {
        setcolor(BLACK);
        line(startx,i,startx+dimensiune,i);//orizontala
    }
}
void citireNiveluri(int &n)
{
    int k=1;
    fin_niveluri>>n;
    while(k<=n)
    {
        for(i=1;i<=casute+2;i++)
            for(j=1;j<=casute+2;j++)
            {
                fin_niveluri>>usor[k].tabla[i][j];
                sablon[k].tabla[i][j]=usor[k].tabla[i][j];
                if (usor[k].tabla[i][j]<=3 and usor[k].tabla[i][j]>=1)
                   {
                       usor[k].iepuri[usor[k].tabla[i][j]]++;//creaza frecventa
                       sablon[k].iepuri[sablon[k].tabla[i][j]]=usor[k].iepuri[usor[k].tabla[i][j]];//se face o copie a tablei citite
                   }

            }
        moves>>vect[k];//scorul
        k++;
    }
    k=1;
    moves.close();
    fin_progres>>progB;
    progA=progB;
    fin_progres.close();
}
void citireCreatii(int &CREATIE)
{
    int k=1;
    salvate>>CREATIE;
    while(k<=CREATIE)
    {
        for(i=1;i<=casute+2;i++)
            for(j=1;j<=casute+2;j++)
            {
                salvate>>usor[k+n].tabla[i][j];
                sablon[k+n].tabla[i][j]=usor[k+n].tabla[i][j];
                if (usor[k+n].tabla[i][j]<=3 and usor[k+n].tabla[i][j]>=1)
                   {
                       usor[k+n].iepuri[usor[k+n].tabla[i][j]]++;
                       sablon[k+n].iepuri[sablon[k+n].tabla[i][j]]=usor[k+n].iepuri[usor[k+n].tabla[i][j]];
                   }
            }
        k++;
    }
    salvate.close();
}
void restaurare(int k) //pentru butonul de restart; reface tabla initiala
{
    for(i=1;i<=casute+2;i++)
            for(j=1;j<=casute+2;j++)
            {
                usor[k].tabla[i][j]=sablon[k].tabla[i][j];
            }
    for (i=1;i<=3;i++)
        usor[k].iepuri[i]=sablon[k].iepuri[i];
}
void mutare_coloana(int k,int n,int iepure,int col)//pune valoarea in matrice; elibereaza locul
{
    if(usor[n].tabla[k][col]==0)
        usor[n].tabla[k][col]=iepure;
    usor[n].tabla[linie][coloana]=0;
}
void mutare_linie(int k,int n,int iepure,int lin)
{
    if(usor[n].tabla[lin][k]==0)
        usor[n].tabla[lin][k]=iepure;
    usor[n].tabla[linie][coloana]=0;
}
void miscareVulpe(int n,int &ok)
{
    int	vx,vy,nouLinie,nouColoana,nouvx,nouvy;
    int latura=dimensiune/casute;
    if(ismouseclick(WM_LBUTTONDOWN)&&ok==0)
    {
         clearmouseclick(WM_LBUTTONDOWN);
         vx=mousex();
         vy=mousey();
         if(vx>startx&&vy>starty&&vx<=dimensiune+startx&&vy<=dimensiune+starty)
         {
             linie=(vy-starty)/latura+2;
             coloana=(vx-startx)/latura+2;
             if(usor[n].tabla[linie][coloana]==6&&usor[n].tabla[linie+1][coloana]==6&&usor[n].tabla[linie+1][coloana]!=9)//verticala
             {
                  setcolor(YELLOW);
                  rectangle(startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana-1)*latura-1,starty+linie*latura-1);
                  ok=2;
             }
             else if(usor[n].tabla[linie][coloana]==6&&usor[n].tabla[linie-1][coloana]==6&&usor[n].tabla[linie-1][coloana]!=9)//verticala
                  {
                      linie=linie-1;
                      setcolor(YELLOW);
                      rectangle(startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana-1)*latura-1,starty+linie*latura-1);
                      ok=2;
                  }
               else if(usor[n].tabla[linie][coloana]==5&&usor[n].tabla[linie][coloana+1]==5&&usor[n].tabla[linie][coloana+1]!=9)//verticala
                {
                  setcolor(YELLOW);
                  rectangle(startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+coloana*latura-1,starty+(linie-1)*latura-1);
                  ok=3;
                }
                    else if(usor[n].tabla[linie][coloana-1]==5&&usor[n].tabla[linie][coloana]==5&&usor[n].tabla[linie][coloana-1]!=9)//verticala
                  {
                      coloana=coloana-1;
                      setcolor(YELLOW);
                      rectangle(startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+coloana*latura-1,starty+(linie-1)*latura-1);
                      ok=3;
                  }
         }
    }
    if(ismouseclick(WM_LBUTTONDOWN) and ok==2)
    {
        clearmouseclick(WM_LBUTTONDOWN);
        nouvx=mousex();
        nouvy=mousey();
        if(nouvx>startx&&nouvy>starty&&nouvx<=dimensiune+startx&&nouvy<=dimensiune+starty)
        {
            nouLinie=(nouvy-starty)/latura+2;
            nouColoana=(nouvx-startx)/latura+2;
            if(nouLinie==linie+1&&coloana==nouColoana and ok==2)//o pune la loc
            {
                ok=0;
                readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\vulpeVerticala.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-3)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                setcolor(BLACK);
                rectangle(startx+(coloana-2)*latura,starty+(linie-2)*latura,startx+(coloana-1)*latura,starty+linie*latura);
            }
            else if(linie==nouLinie&&coloana==nouColoana and ok==2)//o pune la loc
                 {
                    ok=0;
                    readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\vulpeVerticala.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+nouLinie*latura-1);
                    setcolor(BLACK);
                    rectangle(startx+(coloana-2)*latura,starty+(linie-2)*latura,startx+(coloana-1)*latura,starty+linie*latura);
                 }
            if(nouColoana==coloana)
            {
                 int k=linie+1;
                 while((usor[n].tabla[k][coloana]==0||usor[n].tabla[k][coloana]==6)&&k!=nouLinie and ok==2)//in jos
                     k++;
                 if(k==nouLinie&&usor[n].tabla[k][coloana]==0 and ok==2)
                 {
                     setcolor(BLACK);
                     rectangle(startx+(coloana-2)*latura,starty+(linie-2)*latura,startx+(coloana-1)*latura,starty+linie*latura);
                     readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\verde.jpg",startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana-1)*latura-1,starty+linie*latura-1);
                     rectangle(startx+(coloana-2)*latura,starty+(linie-2)*latura,startx+(coloana-1)*latura,starty+(linie-1)*latura);
                     rectangle(startx+(coloana-2)*latura,starty+(linie-1)*latura,startx+(coloana-1)*latura,starty+linie*latura);
                     usor[n].tabla[linie][coloana]=usor[n].tabla[linie+1][coloana]=0;
                     usor[n].tabla[nouLinie][nouColoana]=usor[n].tabla[nouLinie-1][nouColoana]=6;
                     readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\vulpeVerticala.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-3)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                     ok=0; moved++;
                 }
                 k=linie;
                 while((usor[n].tabla[k][coloana]==0||usor[n].tabla[k][coloana]==6)&&k!=nouLinie&&ok==2)//in sus
                     k--;
                 if(k==nouLinie&&usor[n].tabla[k][coloana]==0 and ok==2)
                 {
                     setcolor(BLACK);
                     rectangle(startx+(coloana-2)*latura,starty+(linie-2)*latura,startx+(coloana-1)*latura,starty+linie*latura);
                     readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\verde.jpg",startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana-1)*latura-1,starty+linie*latura-1);
                     rectangle(startx+(coloana-2)*latura,starty+(linie-2)*latura,startx+(coloana-1)*latura,starty+(linie-1)*latura);
                     rectangle(startx+(coloana-2)*latura,starty+(linie)*latura,startx+(coloana-1)*latura,starty+linie*latura);
                     usor[n].tabla[linie][coloana]=usor[n].tabla[linie+1][coloana]=0;
                     usor[n].tabla[nouLinie][nouColoana]=usor[n].tabla[nouLinie+1][nouColoana]=6;
                     readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\vulpeVerticala.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+nouLinie*latura-1);
                     ok=0; moved++;
                 }
            }
       }
       char sirmoved[10];
       setbkcolor(GREEN);
       setcolor(WHITE);
       itoa(moved,sirmoved,10);
       settextstyle(6,HORIZ_DIR,6);
       outtextxy(startx+235,16,sirmoved);
    }
    if(ismouseclick(WM_LBUTTONDOWN) and ok==3)
    {
        clearmouseclick(WM_LBUTTONDOWN);
        nouvx=mousex();
        nouvy=mousey();
        if(nouvx>startx&&nouvy>starty&&nouvx<=dimensiune+startx&&nouvy<=dimensiune+starty )
        {
            nouLinie=(nouvy-starty)/latura+2;
            nouColoana=(nouvx-startx)/latura+2;
            if(nouLinie==linie&&nouColoana==1+coloana and ok==3)
            {
                ok=0;
                readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\vulpeOrizontala.jpg",startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana)*latura-1,starty+(linie-1)*latura-1);
                setcolor(BLACK);
                rectangle(startx+(coloana-2)*latura,starty+(linie-2)*latura,startx+coloana*latura,starty+(linie-1)*latura);
            }
            else if(linie==nouLinie&&coloana==nouColoana and ok==3)
                 {
                    ok=0;
                    readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\vulpeOrizontala.jpg",startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana)*latura-1,starty+(linie-1)*latura-1);
                    setcolor(BLACK);
                    rectangle(startx+(coloana-2)*latura,starty+(nouLinie-2)*latura,startx+(coloana)*latura,starty+(linie-1)*latura);
                 }
            if(nouLinie==linie)
            {
                int k=coloana;
                 while((usor[n].tabla[linie][k]==0||usor[n].tabla[linie][k]==5)&&k!=nouColoana and ok==3)//in stanga
                        k--;
                   if(k==nouColoana&&usor[n].tabla[linie][k]==0 and ok==3)
                   {
                     setcolor(BLACK);
                     rectangle(startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana)*latura-1,starty+(linie-1)*latura-1);
                     readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\verde.jpg",startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana)*latura-1,starty+(linie-1)*latura-1);
                     rectangle(startx+(coloana-2)*latura,starty+(linie-2)*latura,startx+(coloana-1)*latura,starty+(linie-1)*latura);
                     rectangle(startx+(coloana-1)*latura,starty+(linie-2)*latura,startx+(coloana)*latura,starty+(linie-1)*latura);
                     usor[n].tabla[linie][coloana]=usor[n].tabla[linie][coloana+1]=0;
                     usor[n].tabla[nouLinie][nouColoana]=usor[n].tabla[nouLinie][nouColoana+1]=5;
                     readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\vulpeOrizontala.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana)*latura-1,starty+(nouLinie-1)*latura-1);
                     ok=0; moved++;
                   }
                 k=coloana;
                 while((usor[n].tabla[linie][k]==0||usor[n].tabla[linie][k]==5)&&k!=nouColoana and ok==3)//in dreapta
                        k++;
                   if(k==nouColoana&&usor[n].tabla[linie][k]==0 and ok==3)
                 {
                     setcolor(BLACK);
                     rectangle(startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana)*latura-1,starty+(linie-1)*latura-1);
                     readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\verde.jpg",startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana)*latura-1,starty+(linie-1)*latura-1);
                     rectangle(startx+(coloana-2)*latura,starty+(linie-2)*latura,startx+(coloana-1)*latura,starty+(linie-1)*latura);
                     rectangle(startx+(coloana-1)*latura,starty+(linie-2)*latura,startx+(coloana)*latura,starty+(linie-1)*latura);
                     usor[n].tabla[linie][coloana]=usor[n].tabla[linie][coloana+1]=0;
                     usor[n].tabla[nouLinie][nouColoana]=usor[n].tabla[nouLinie][nouColoana-1]=5;
                     readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\vulpeOrizontala.jpg",startx+(nouColoana-3)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                     ok=0; moved++;
                 }
            }
        }
         char sirmoved[10];
         setbkcolor(GREEN);
         setcolor(WHITE);
         itoa(moved,sirmoved,10);
         settextstyle(6,HORIZ_DIR,6);
         outtextxy(startx+235,16,sirmoved);
    }
}
void miscareIepure(int n,int &iepure,int &ok,int &okv)
{
    char sirmoved[10];
    int	vx,vy,nouLinie,nouColoana;
    int latura=dimensiune/casute;
    if (!closed)
    {
    if(ismouseclick(WM_LBUTTONDOWN)&&ok==0)
    {
         clearmouseclick(WM_LBUTTONDOWN);
         vx=mousex();
         vy=mousey();
         if(vx>startx&&vy>starty&&vx<=dimensiune+startx&&vy<=dimensiune+starty)
         {
             linie=(vy-starty)/latura+2;
             coloana=(vx-startx)/latura+2;
             if((usor[n].tabla[linie][coloana]==1||usor[n].tabla[linie][coloana]==2||usor[n].tabla[linie][coloana]==3))
             {
                 iepure=usor[n].tabla[linie][coloana];
                 setcolor(YELLOW);
                 rectangle(startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana-1)*latura-1,starty+(linie-1)*latura-1);
                 ok=1;//selectez iepure
             }
         }
         else
         {
             if(vx<=100 and vy<=70)
               moved=0, restaurare(n),PornireJoc();//buton back to menu
             clearmouseclick(WM_LBUTTONDOWN);
             if(vx>=1025 and vy<=65)//buton restart
             {
                 moved=0;
                 restaurare(n);
                 startGame(n);
             }
             if(vx>=1040&&vy>=600)//buton instructiuni
             {
                 okv=1;
                 if(lg==1)
                   readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\infoENG.jpg",startx+300,starty+100,1050,600);
                 else if(lg==2)
                        readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\infoROM.jpg",startx+300,starty+100,1050,600);
                      else if(lg==3)
                             readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\infoFRE.jpg",startx+300,starty+100,1050,600);
             }
         }
    }
    else if(ismouseclick(WM_LBUTTONDOWN)&&okv==1)//stergere instructiuni
    {
        clearmouseclick(WM_LBUTTONDOWN);
        vx=mousex();
        vy=mousey();
        if(vx>=1040&&vy>=600)
        {
            restaurare(n);
            startGame(n);
        }
        okv=0;
    }
    else if(ismouseclick(WM_LBUTTONDOWN)&&ok==1)
    {
         clearmouseclick(WM_LBUTTONDOWN);
         int iep2;
         vx=mousex();
         vy=mousey();
         if(vx>startx&&vy>starty&&vx<=dimensiune+startx&&vy<=dimensiune+starty)
         {
             nouLinie=(vy-starty)/latura+2;
             nouColoana=(vx-startx)/latura+2;
             iep2=usor[n].tabla[nouLinie][nouColoana];
             if(nouLinie==linie&&nouColoana==coloana)//revine poza la al doilea click pe iepure
             {
                 setcolor(GREEN);
                 rectangle(startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana-1)*latura-1,starty+(linie-1)*latura-1);
                 readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\verde.jpg",startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana-1)*latura-1,starty+(linie-1)*latura-1);
                 if(iepure==1)
                    readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure1.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                 else if(iepure==2)
                      readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure2.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                     else if(iepure==3)
                          readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure3.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                 ok=0;
             }
             else if(iepure!=iep2&&(iep2<1||iep2>3))
             {
                 if(usor[n].tabla[nouLinie][nouColoana]==0||usor[n].tabla[nouLinie][nouColoana]==-1)
                 {
                     int k=coloana;
                     while(usor[n].tabla[linie][k]!=-1&&usor[n].tabla[linie][k]!=0&&usor[n].tabla[linie][k]!=9)//verific pe rand in dreapta
                        k++;
                     if(k!=coloana+1&&k==nouColoana&&linie==nouLinie)
                     {
                         setcolor(GREEN);
                         rectangle(startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana-1)*latura-1,starty+(linie-1)*latura-1);
                         readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\verde.jpg",startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana-1)*latura-1,starty+(linie-1)*latura-1);
                         if(iepure==1)
                           readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure1.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                         else if(iepure==2)
                                readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure2.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                             else if(iepure==3)
                                    readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure3.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                         mutare_linie(k,n,iepure,nouLinie);//modifica valori in matrice
                         moved++;
                         ok=0;
                         if(usor[n].tabla[nouLinie][nouColoana]==-1)
                         {
                             readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\verde.jpg",startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana-1)*latura-1,starty+(linie-1)*latura-1);
                             if(iepure==1)
                               readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure1Groapa.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                             else if(iepure==2)
                                   readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure2Groapa.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                                 else if(iepure==3)
                                       readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure3Groapa.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                             usor[n].tabla[nouLinie][nouColoana]=10;
                             usor[n].iepuri[iepure]--;
                         }
                     }
                     k=linie;
                     while(usor[n].tabla[k][coloana]!=-1&&usor[n].tabla[k][coloana]!=0&&usor[n].tabla[k][coloana]!=9 and ok)//verific pe coloana in jos
                        k++;
                     if(k!=linie+1&&k==nouLinie&&coloana==nouColoana and ok)
                     {
                         setcolor(GREEN);
                         rectangle(startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana-1)*latura-1,starty+(linie-1)*latura-1);
                         readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\verde.jpg",startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana-1)*latura-1,starty+(linie-1)*latura-1);
                         if(iepure==1)
                           readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure1.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                         else if(iepure==2)
                                readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure2.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                             else if(iepure==3)
                                   readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure3.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                         mutare_coloana(k,n,iepure,nouColoana);
                         moved++;
                         ok=0;
                         if(usor[n].tabla[nouLinie][nouColoana]==-1)
                         {
                             readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\verde.jpg",startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana-1)*latura-1,starty+(linie-1)*latura-1);
                             if(iepure==1)
                               readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure1Groapa.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                             else if(iepure==2)
                                   readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure2Groapa.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                                 else if(iepure==3)
                                       readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure3Groapa.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                             ok=0;
                             usor[n].tabla[nouLinie][nouColoana]=10;
                             usor[n].iepuri[iepure]--;
                         }
                     }
                     k=linie;
                     while(usor[n].tabla[k][coloana]!=-1&&usor[n].tabla[k][coloana]!=0&&usor[n].tabla[k][coloana]!=9 and ok)//verific pe coloana in sus
                        k--;
                     if(k!=linie-1&&k==nouLinie&&coloana==nouColoana and ok)
                     {
                         setcolor(GREEN);
                         rectangle(startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana-1)*latura-1,starty+(linie-1)*latura-1);
                         readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\verde.jpg",startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana-1)*latura-1,starty+(linie-1)*latura-1);
                         if(iepure==1)
                          readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure1.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                         else if(iepure==2)
                               readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure2.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                             else if(iepure==3)
                                   readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure3.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                         mutare_coloana(k,n,iepure,nouColoana);
                         moved++;
                         ok=0;
                         if(usor[n].tabla[nouLinie][nouColoana]==-1)
                         {
                             usor[n].tabla[nouLinie][nouColoana]=10;
                             if(iepure==1)
                               readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure1Groapa.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                             else if(iepure==2)
                                   readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure2Groapa.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                                 else if(iepure==3)
                                       readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure3Groapa.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                            usor[n].iepuri[iepure]--;
                         }
                     }
                     k=coloana;
                     while(usor[n].tabla[linie][k]!=-1&&usor[n].tabla[linie][k]!=0&&usor[n].tabla[linie][k]!=9 and ok)//verific pe rand in stanga
                        k--;
                     if(k!=coloana-1&&k==nouColoana&&linie==nouLinie and ok)
                     {
                         setcolor(GREEN);
                         rectangle(startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana-1)*latura-1,starty+(linie-1)*latura-1);
                         readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\verde.jpg",startx+(coloana-2)*latura+1,starty+(linie-2)*latura+1,startx+(coloana-1)*latura-1,starty+(linie-1)*latura-1);
                         if(iepure==1)
                          readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure1.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                         else if(iepure==2)
                               readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure2.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                             else if(iepure==3)
                                   readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure3.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                         mutare_linie(k,n,iepure,nouLinie);
                         moved++;
                         ok=0;
                         if(usor[n].tabla[nouLinie][nouColoana]==-1)
                         {
                             usor[n].tabla[nouLinie][nouColoana]=10;
                             if(iepure==1)
                               readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure1Groapa.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                             else if(iepure==2)
                                   readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure2Groapa.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                                 else if(iepure==3)
                                       readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure3Groapa.jpg",startx+(nouColoana-2)*latura+1,starty+(nouLinie-2)*latura+1,startx+(nouColoana-1)*latura-1,starty+(nouLinie-1)*latura-1);
                             ok=0;
                             usor[n].iepuri[iepure]--;
                         }
                     }
                 }
             }
         }
         setbkcolor(GREEN);
         setcolor(WHITE);
         itoa(moved,sirmoved,10);
         settextstyle(6,HORIZ_DIR,6);
         outtextxy(startx+235,16,sirmoved);
   }
   else miscareVulpe(n,ok);
   }
}
void creareImagini(int k)
{
    for(i=2;i<=casute+1;i++)
    {
        for(j=2;j<=casute+1;j++)
            {
                //if (i==salt and usor[k].tabla[i-1][j]==6 and usor[k].tabla[i+1][j]==6 )salt=0;
                while(usor[k].tabla[i][j]==6 and usor[k].tabla[i-1][j]==6 and usor[k].tabla[i-2][j]!=6 )j++;
                if(usor[k].tabla[i][j]==1)//iepure1
                readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure1.jpg",startx+(j-2)*latura+1,starty+(i-2)*latura+1,startx+(j-1)*latura-1,starty+(i-1)*latura-1);
                else if(usor[k].tabla[i][j]==2)//iepure2
                     readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure2.jpg",startx+(j-2)*latura+1,starty+(i-2)*latura+1,startx+(j-1)*latura-1,starty+(i-1)*latura-1);
                    else if(usor[k].tabla[i][j]==3)//iepure3
                         readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\iepure3.jpg",startx+(j-2)*latura+1,starty+(i-2)*latura+1,startx+(j-1)*latura-1,starty+(i-1)*latura-1);
                        else if(usor[k].tabla[i][j]==4)//ciuperca
                             readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\ciuperca.jpg",startx+(j-2)*latura+1,starty+(i-2)*latura+1,startx+(j-1)*latura-1,starty+(i-1)*latura-1);
                            else if(usor[k].tabla[i][j]==-1)//gaura
                                 readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\gaura.jpg",startx+(j-2)*latura+1,starty+(i-2)*latura+1,startx+(j-1)*latura-1,starty+(i-1)*latura-1);
                                else if (usor[k].tabla[i][j]==0)//verdeata
                                {
                                    readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\verde.jpg",startx+(j-2)*latura+1,starty+(i-2)*latura+1,startx+(j-1)*latura-1,starty+(i-1)*latura-1);
                                    setcolor(BLACK);
                                    rectangle(startx+(j-2)*latura,starty+(i-2)*latura,startx+(j-1)*latura,starty+(i-1)*latura);
                                }
                                else if(usor[k].tabla[i][j]!=0&&usor[k].tabla[i][j]!=9)
                                {
                                    int oksalt=1;
                                    for (int RAN=1;RAN<=ss and oksalt;RAN++) if (saltj[ss]==j)oksalt=0;
                                    if((usor[k].tabla[i-2][j]==6 and usor[k].tabla[i-1][j]==6)|| usor[k].tabla[i-1][j]!=6) oksalt=1;
                                    if(usor[k].tabla[i][j]==usor[k].tabla[i+1][j] and usor[k].tabla[i][j]==6 and oksalt)
                                    //((j!=saltj and ((usor[k].tabla[i-1][j]==6 and usor[k].tabla[i-2][j]==6 )||usor[k].tabla[i-1][j]!=6  ) )))//vulpe pe verticala
                                    //if(usor[k].tabla[i][j]==usor[k].tabla[i+1][j] and usor[k].tabla[i][j]==6 )
                                    {
                                        saltj[++ss]=j;
                                        readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\vulpeVerticala.jpg",startx+(j-2)*latura+1,starty+(i-2)*latura+1,startx+(j-1)*latura-1,starty+i*latura-1);
                                    }
                                    else if(usor[k].tabla[i][j]==usor[k].tabla[i][j+1] and usor[k].tabla[i][j]==5 )//vulpe pe orizontala
                                       readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\vulpeOrizontala.jpg",startx+(j-2)*latura+1,starty+(i-2)*latura+1,startx+j*latura-1,starty+(i-1)*latura-1),j++;
                                }
            }
    }
    for(int RAN=1;RAN<=ss;RAN++)
        saltj[ss]=0;
    ss=1;
}
void startGame(int k)//permite jucarea nivelului
{
    int oki=0,okv=0;
    creareTabla();
    if (k<=n)
   {
    char MISCARI[3];
    if (lg==2)
        readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\scor.jpg",startx+dimensiune+60,starty,startx+dimensiune+250,starty+60);
    else
        readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\score.jpg",startx+dimensiune+60,starty,startx+dimensiune+250,starty+60);
    if(vect[k]==99)
    {
        settextstyle(10,HORIZ_DIR,3);
        setcolor(WHITE);
        setbkcolor(BROWN);
        if(lg==1)
           outtextxy(startx+dimensiune+60,starty+65,"Not completed");
            else if(lg==2)
                    outtextxy(startx+dimensiune+60,starty+65,"Incomplet");
                 else outtextxy(startx+dimensiune+60,starty+65,"Pas Achevé");
    }
    else
        {
            settextstyle(10,HORIZ_DIR,5);
            itoa(vect[k],MISCARI,10);
            setcolor(WHITE);
            setbkcolor(BROWN);
            outtextxy(startx+dimensiune+130,starty+65,MISCARI);}
        }
    creareImagini(k);
    while(1)
    {
        int iep=0;
        while(!sfarsitjoc(k))//cat timp mai sunt iepuri pe tabla
        {
            miscareIepure(k,iep,oki,okv);
        }
        delay(100);
        if(moved<vect[k])//afiseaza scorul cel mai mic
        {
            vect[k]=moved;
            moves.open("miscari.in",ios::out);
            for(int ll=1;ll<=n;ll++)
                moves<<vect[ll]<<" ";
            moves.close();
        }
        if(lg==1)
           readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\CasetaENG.jpg",startx+50,starty+130,startx+450,starty+370);
        else if(lg==2)
              readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\CasetaROM.jpg",startx+50,starty+130,startx+450,starty+370);
             else if(lg==3)
                   readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\CasetaFRE.jpg",startx+50,starty+130,startx+450,starty+370);
        if(k==progB and progB<30)
            progB++;
        if(progB>0)
        {
            fin_progres.open("progres.in",ios::out);
            fin_progres<<progB;
            fin_progres.close();
        }
        if((progB==6 and k==5)||(progB==16 and k==15)||(progB==31 and k==30))
        {
            if(lg==1)
              readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\shopnotificationENG.jpg",startx+50,starty,startx+450,starty+120);
            else if(lg==2)
                   readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\shopnotificationROM.jpg",startx+50,starty,startx+450,starty+120);
                 else if(lg==3)
                        readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\shopnotificationFRE.jpg",startx+50,starty,startx+450,starty+120);
        }
        while(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int x=mousex(),y=mousey();
            if(x>=startx+300&&y>=starty+300&&x<=startx+420&&y<=starty+320 and k!=30 and k<n+CREATIE)
            {
                restaurare(k);
                moved=0;
                startGame(k+1);
            }
            else if(x>=startx+100&&y>=starty+300&&x<=startx+200&&y<=starty+320)
                 {
                     restaurare(k);
                     moved=0;
                     PornireJoc();
                 }
        }
    }
    restaurare(k);
}
void selectLevel()
{
      int x,y,i,j,k=0;
      while (ismouseclick(WM_LBUTTONDOWN))
      {
          x=mousex(),y=mousey();
          clearmouseclick(WM_LBUTTONDOWN);
          if(x<=100 and y<=70)
             PornireJoc();
          else
            for(j=85;j<=windowy-100;j+=86)
                for(i=75;i<=windowx-200;i+=200)
                {
                    clearmouseclick(WM_LBUTTONDOWN);
                    k++;
                    if(x>=i&&x<=i+150&&y>=j&&y<=j+60 )//desenButon(i,j);
                    {
                        moved=0;
                        if(k<=progB)
                            selected=1,startGame(k);
                    }
                }
      }
}
void imagesShop()
{
      if(lg==1)
        readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\campShopENG.jpg",0,0,windowx,windowy);
      else if(lg==2)
             readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\campShopROM.jpg",0,0,windowx,windowy);
           else if(lg==3)
                  readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\campShopFRE.jpg",0,0,windowx,windowy);
      setcolor(BLACK);
      rectangle(startx-21,starty+149,startx+151,starty+321);
      readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\camp2L.jpg",startx-20,starty+150,startx+150,starty+320);
      rectangle(startx+179,starty+149,startx+351,starty+321);
      readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\camp3L.jpg",startx+180,starty+150,startx+350,starty+320);
      rectangle(startx+379,starty+149,startx+551,starty+321);
      readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\camp4L.jpg",startx+380,starty+150,startx+550,starty+320);
      rectangle(startx+179,starty+349,startx+351,starty+521);
      readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\camp1R.jpg",startx+180,starty+350,startx+350,starty+520);
}
void shop(int &fundal)
{
      int unlocked2,unlocked3,unlocked4;
      clearmouseclick(WM_LBUTTONDOWN);
      clearviewport();
      imagesShop();
      if(progB>5)
      {
          readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\camp2R.jpg",startx-20,starty+150,startx+150,starty+320);
          unlocked2=1;
      }
      if(progB>10)
      {
          readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\camp3R.jpg",startx+180,starty+150,startx+350,starty+320);
          unlocked3=1;
      }
      if(progB>15)
      {
          readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\camp4R.jpg",startx+380,starty+150,startx+550,starty+320);
          unlocked4=1;
      }
      while(1)
      {
          if(ismouseclick(WM_LBUTTONDOWN))
          {
              clearmouseclick(WM_LBUTTONDOWN);
              int vx=mousex(),vy=mousey();
              if(vx<=100 and vy<=70)
                 PornireJoc(),selected=0;
              else if(vx>=startx-20&&vy>=starty+150&&vx<=startx+150&&vy<=starty+320&&unlocked2==1)
               {
                   setcolor(BLACK);
                   rectangle(startx+179,starty+149,startx+351,starty+321);
                   rectangle(startx+379,starty+149,startx+551,starty+321);
                   rectangle(startx+179,starty+349,startx+351,starty+521);
                   fundal=2;
                   setcolor(YELLOW);
                   rectangle(startx-21,starty+149,startx+151,starty+321);
               }
               else if(vx>=startx+180&&vy>=starty+150&&vx<=startx+350&&vy<=starty+320&&unlocked3==1)
                    {
                        setcolor(BLACK);
                        rectangle(startx-21,starty+149,startx+151,starty+321);
                        rectangle(startx+379,starty+149,startx+551,starty+321);
                        rectangle(startx+179,starty+349,startx+351,starty+521);
                        fundal=3;
                        setcolor(YELLOW);
                        rectangle(startx+179,starty+149,startx+351,starty+321);
                    }
                    else if(vx>=startx+380&&vy>=starty+150&&vx<=startx+510&&vy<=starty+320&&unlocked4==1)
                         {
                             setcolor(BLACK);
                             rectangle(startx+179,starty+149,startx+351,starty+321);
                             rectangle(startx-21,starty+149,startx+151,starty+321);
                             rectangle(startx+179,starty+349,startx+351,starty+521);
                             fundal=4;
                             setcolor(YELLOW);
                             rectangle(startx+379,starty+149,startx+551,starty+321);
                         }
                         else if(vx>=startx+180&&vy>=starty+350&&vx<=startx+350&&vy<=starty+520)
                              {
                                  setcolor(BLACK);
                                  rectangle(startx-21,starty+149,startx+151,starty+321);
                                  rectangle(startx+179,starty+149,startx+351,starty+321);
                                  rectangle(startx+379,starty+149,startx+551,starty+321);
                                  fundal=1;
                                  setcolor(YELLOW);
                                  rectangle(startx+179,starty+349,startx+351,starty+521);
                              }
          }
      }
}
void imagesOptions()
{
    readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\campOps.jpg",0,0,windowx,windowy);
    readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\ROM.jpg",startx+50,starty+50,startx+150,starty+150);
    readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\ENG.jpg",startx+200,starty+50,startx+300,starty+150);
    readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\FRE.jpg",startx+350,starty+50,startx+450,starty+150);
    readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\audio.jpg",startx+110,starty+210,startx+400,starty+270);
    readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\soundOn.jpg",startx+200,starty+300,startx+300,starty+400);
}
void options(int &okp,int &lg)
{
    clearmouseclick(WM_LBUTTONDOWN);
    clearviewport();
    imagesOptions();
    if(lg==1)
        readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\lbENG.jpg",startx+110,starty-50,startx+400,starty+10);
    else if(lg==2)
           readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\lbROM.jpg",startx+110,starty-50,startx+400,starty+10);
         else if(lg==3)
               readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\lbFRE.jpg",startx+110,starty-50,startx+400,starty+10);
    while(1)
    {
      if(ismouseclick(WM_LBUTTONDOWN))
      {
         clearmouseclick(WM_LBUTTONDOWN);
         int vx=mousex(),vy=mousey();
         if(vx<=100 and vy<=70)
            PornireJoc(),selected=0;
         else if(vx>=startx+200&&vy>=starty+300&&vx<=startx+300&&vy<=starty+400)//opreste muzica
              {
                 okp=1;
                 audioOn=0;
                 PlaySound(NULL, NULL, 0);
                 readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\soundOFF.jpg",startx+200,starty+300,startx+300,starty+400);
              }
              else if(vx>=startx+200&&vy>=starty+50&&vx<=startx+300&&vy<=starty+150)
                   {
                       readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\lbENG.jpg",startx+110,starty-50,startx+400,starty+10);
                       lg=1;
                   }
                   else if(vx>=startx+50&&vy>=starty+50&&vx<=startx+150&&vy<=starty+150)
                        {
                          lg=2;
                          readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\lbROM.jpg",startx+110,starty-50,startx+400,starty+10);
                        }
                        else if(vx>=startx+350&&vy>=starty+50&&vx<=startx+450&&vy<=starty+150)
                        {
                            lg=3;
                            readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\lbFRE.jpg",startx+110,starty-50,startx+400,starty+10);
                        }
                            else if(vx>=1040&&vy>=600)
                            {
                               okp=2;
                               if(lg==1)
                                  readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\infoENG.jpg",startx+300,starty+100,1050,600);
                               else if(lg==2)
                                      readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\infoROM.jpg",startx+300,starty+100,1050,600);
                                    else if(lg==3)
                                          readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\infoFRE.jpg",startx+300,starty+100,1050,600);
                            }
      }
      else
      {
        if(ismouseclick(WM_LBUTTONDOWN)&&okp==1)//reporneste muzica
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int vx=mousex(),vy=mousey();
            if(vx>=startx+200&&vy>=starty+300&&vx<=startx+300&&vy<=starty+400)
            {
                if(audioOn==0)
                    PlaySound(TEXT("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\Dreams-2.wav"),NULL, SND_ASYNC);
                audioOn=1;
                readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\soundON.jpg",startx+200,starty+300,startx+300,starty+400);
            }
            okp=0;
        }
        else if(ismouseclick(WM_LBUTTONDOWN)&&okp==2)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int vx=mousex(),vy=mousey();
            if(vx>=1040&&vy>=600&&vx<=1100&&vy<=700)
                options(okp,lg);
        }
      }
    }
}
void creatormode();//GABI
void generare(int &CREATIE)//GABI
{
    CREATIE+=n;
    CREATIE++;
    int VALOARE;
    int terminat=0;
    setcolor(WHITE);
    setbkcolor(BROWN);
    outtextxy(900,100,"STERGE");
    outtextxy(900,200,"TERMINAT");
    outtextxy(20,100,"IEPURE");
    outtextxy(50,150,"1");
    outtextxy(90,150,"2");
    outtextxy(130,150,"3");
    outtextxy(20,200,"GROAPA");
    outtextxy(20,250,"CIUPERCUTA");
    outtextxy(40,300,"VULPE");
    outtextxy(20,350,"ORIZ");
    outtextxy(130,350,"VERT");
    int i,j;
        for(i=1;i<=casute+2;i++)
            for(j=1;j<=casute+2;j++)
            {
                if (i==1||i==casute+2||j==1||j==casute+2)
                    usor[CREATIE].tabla[i][j]=9;
                else
                    usor[CREATIE].tabla[i][j]=0;
                sablon[CREATIE].tabla[i][j]=usor[CREATIE].tabla[i][j];
            }
    while(!terminat)
{
 while (ismouseclick(WM_LBUTTONDOWN))
     {
    int x=mousex(),y=mousey();
    clearmouseclick(WM_LBUTTONDOWN);
            if(x<=100 and y<=70)
                selected=0,CREATIE-=n,restaurare(CREATIE),CREATIE--,PornireJoc();
            else if(x>=1000 and y<=70)
                restaurare(CREATIE);
            else if(x>=900 and y>=100 and x<=1100 and y<=140)
                VALOARE=0;
            else if (x>=50 and y>=150 and y<=190 and x<=80)
                VALOARE=1;
            else if (x>=90 and y>=150 and y<=190 and x<=120)
                VALOARE=2;
            else if (x>=130 and y>=150 and y<=190 and x<=160)
                VALOARE=3;
            else if (x>=20 and y>=200 and y<=240 and x<=160)
                VALOARE=-1;
            else if (x>=20 and y>=250 and y<=290 and x<=280)
                VALOARE=4;
            else if(x>=20 and y>=350 and y<=390 and x<=120)
                VALOARE=5;
            else if(x>=130 and y>=350 and y<=390 and x<=230)
                VALOARE=6;
      else if(x>=900 and y>=200 and x<=1100 and y<=240)
        terminat=1;
else if (x>=startx+1 and x<=startx+dimensiune-1 and y>=starty+1 and y<=starty+dimensiune-1)
{
    int linie,coloana;
    linie=(y-starty)/latura+2;
    coloana=(x-startx)/latura+2;
   if (VALOARE!=5 and VALOARE!=6 )
   {if (usor[CREATIE].tabla[linie][coloana]!=5 and usor[CREATIE].tabla[linie][coloana]!=6)
       usor[CREATIE].tabla[linie][coloana]=VALOARE;
    else if (usor[CREATIE].tabla[linie][coloana]==5)
    { if (!(usor[CREATIE].tabla[linie][coloana-1]==5 and usor[CREATIE].tabla[linie][coloana+1]==5))
        {
            if (usor[CREATIE].tabla[linie][coloana+1]==5)
            usor[CREATIE].tabla[linie][coloana+1]=0,usor[CREATIE].tabla[linie][coloana]=VALOARE;
        else if (usor[CREATIE].tabla[linie][coloana-1]==5)usor[CREATIE].tabla[linie][coloana-1]=0,usor[CREATIE].tabla[linie][coloana]=VALOARE;
        }
     else if (usor[CREATIE].tabla[linie][coloana-1]==5 and usor[CREATIE].tabla[linie][coloana-2]==5)
     {
          usor[CREATIE].tabla[linie][coloana+1]=0,usor[CREATIE].tabla[linie][coloana]=VALOARE;
     }
     else if(usor[CREATIE].tabla[linie][coloana+1]==5 and usor[CREATIE].tabla[linie][coloana+2]==5)
         usor[CREATIE].tabla[linie][coloana-1]=0,usor[CREATIE].tabla[linie][coloana]=VALOARE;
    }
    else if (usor[CREATIE].tabla[linie][coloana]==6)
    {
        if (!(usor[CREATIE].tabla[linie-1][coloana]==6 and usor[CREATIE].tabla[linie+1][coloana]==6))
        {if (usor[CREATIE].tabla[linie+1][coloana]==6)
            usor[CREATIE].tabla[linie+1][coloana]=0,usor[CREATIE].tabla[linie][coloana]=VALOARE;
        else if (usor[CREATIE].tabla[linie-1][coloana]==6)  usor[CREATIE].tabla[linie-1][coloana]=0,usor[CREATIE].tabla[linie][coloana]=VALOARE;
        }
        else if (usor[CREATIE].tabla[linie-1][coloana]==6 and usor[CREATIE].tabla[linie-2][coloana]==6)
            usor[CREATIE].tabla[linie+1][coloana]=0,usor[CREATIE].tabla[linie][coloana]=VALOARE;
        else if (usor[CREATIE].tabla[linie+1][coloana]==6 and usor[CREATIE].tabla[linie+2][coloana]==6)
             usor[CREATIE].tabla[linie-1][coloana]=0,usor[CREATIE].tabla[linie][coloana]=VALOARE;
    }
   }
   else if (VALOARE==5)
    {if (usor[CREATIE].tabla[linie][coloana+1]==0  and  usor[CREATIE].tabla[linie][coloana]!=6 and usor[CREATIE].tabla[linie][coloana+1]==0)
        {usor[CREATIE].tabla[linie][coloana]=VALOARE;
            usor[CREATIE].tabla[linie][coloana+1]=VALOARE;
        }
    if (usor[CREATIE].tabla[linie][coloana+1]==0 and usor[CREATIE].tabla[linie][coloana]==0 )
         {usor[CREATIE].tabla[linie][coloana]=VALOARE;
            usor[CREATIE].tabla[linie][coloana+1]=VALOARE;
        }
    }
    else if (VALOARE==6 and usor[CREATIE].tabla[linie][coloana]!=5 and usor[CREATIE].tabla[linie][coloana]==0)
    {
        if (usor[CREATIE].tabla[linie+1][coloana]==0)
        {usor[CREATIE].tabla[linie][coloana]=VALOARE;
            usor[CREATIE].tabla[linie+1][coloana]=VALOARE;
        }
    }
}
       creareImagini(CREATIE);
     }
}
    for (i=2;i<=casute+1;i++)
     for (j=2;j<=casute+1;j++)
            usor[CREATIE].iepuri[usor[CREATIE].tabla[i][j]]++;
if (!sfarsitjoc(CREATIE))
{
    salvate.open("creatie.txt",ios::out);
    CREATIE-=n;
    salvate<<CREATIE;
    salvate<<endl;
    for (int k=n+1;k<=CREATIE+n;k++)
    {
    for (i=1;i<=casute+2;i++)
        {for (j=1;j<=casute+2;j++)
            salvate<<usor[k].tabla[i][j]<<' ';
        salvate<<endl;
        }
        salvate<<endl;
}
salvate.close();
}
else restaurare(CREATIE);
PornireJoc();
}
void creatormode()//GABI
{
      int x,y,i,j,k=n;
      //if(fundal!=5)prefundal=fundal;
      setcolor(WHITE);
      setbkcolor(BROWN);
      outtextxy(100,0,"CREATE");
      char level[10];
      itoa(CREATIE,level,10);
      settextstyle(10,HORIZ_DIR,5);
      outtextxy(windowx-230,19,level);
      while (ismouseclick(WM_LBUTTONDOWN))
      {
          x=mousex(),y=mousey();
          clearmouseclick(WM_LBUTTONDOWN);
          if(x<=100 and y<=70)
             PornireJoc();
          else if(x>=110 and y<=70 and x<=250)
            {
                clearmouseclick(WM_LBUTTONDOWN);
                creareTabla();
                generare(CREATIE);
            }
                for(j=85;j<=windowy-100;j+=86)
                for(i=75;i<=windowx-200;i+=200)
                {
                    clearmouseclick(WM_LBUTTONDOWN);
                    k++;
                    if(x>=i&&x<=i+150&&y>=j&&y<=j+60 )//desenButon(i,j);
                    {
                        //fundal=prefundal;
                        moved=0;
                        if(k<=CREATIE+n)
                            selected=1,startGame(k);
                    }
                }
      }
}
void PornireJoc()
{
    clearviewport();
    selected=0;
    clearmouseclick(WM_LBUTTONDOWN);
    if(lg==1)
       readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\fundal.jpg",0,0,windowx,windowy);
    else if(lg==2)
           readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\fundalROM.jpg",0,0,windowx,windowy);
         else if(lg==3)
            readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\fundalFRE.jpg",0,0,windowx,windowy);
    settextstyle(10,HORIZ_DIR,5);
    setcolor(WHITE);
    setbkcolor(BROWN);
    outtextxy(700,300,"CM");
    setcolor(BLUE);
    while(1 and !closed)
    {
        while(ismouseclick(WM_LBUTTONDOWN) and !closed)
        {
            int x=mousex(),y=mousey();
            int okp=0;
            if (x>=700 and y>=300 and x<=750 and y<=340)
            {
                clearviewport();
                setbkcolor(BLACK);
                setfillstyle(SOLID_FILL,BLACK);
                floodfill(0,0,0);
                if(lg==1)
                  readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\selectNiv.jpg",0,0,windowx,windowy);
                else if(lg==2)
                       readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\selectNivROM.jpg",0,0,windowx,windowy);
                     else if(lg==3)
                         readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\selectNivFRE.jpg",0,0,windowx,windowy);
                clearmouseclick(WM_LBUTTONDOWN);
                while(!selected)
                creatormode();
            }
            else if(x>=butonx&&x<=butonx+dimbutx&&y>=butony&&y<=butony+dimbuty&&ismouseclick(WM_LBUTTONDOWN))
            {
                clearviewport();
                setbkcolor(BLACK);
                setfillstyle(SOLID_FILL,BLACK);
                floodfill(0,0,0);
                if(lg==1)
                  readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\selectNiv.jpg",0,0,windowx,windowy);
                else if(lg==2)
                       readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\selectNivROM.jpg",0,0,windowx,windowy);
                     else if(lg==3)
                         readimagefile("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\selectNivFRE.jpg",0,0,windowx,windowy);
                clearmouseclick(WM_LBUTTONDOWN);
                setcolor(WHITE);
                setbkcolor(BROWN);
                char level[10];
                itoa(progB,level,10);
                settextstyle(10,HORIZ_DIR,5);
                outtextxy(windowx-230,19,level);
                if(progB>=10)
                {
                    outtextxy(windowx-180,19,"/30");
                    rectangle(windowx-232,18,windowx-104,60);
                    rectangle(windowx-233,17,windowx-103,61);
                }
                else
                {
                    outtextxy(windowx-205,19,"/30");
                    rectangle(windowx-232,18,windowx-129,60);
                    rectangle(windowx-233,17,windowx-128,61);
                }
                while(!selected)
                    selectLevel();
            }
            else
                if(x>=butonx&&x<=butonx+dimbutx&&y>=butony+dimbuty&&y<=butony+2*dimbuty&&ismouseclick(WM_LBUTTONDOWN))
                {
                    shop(fundal);
                }
                else if(x>=butonx&&x<=butonx+dimbutx&&y>=butony+2*dimbuty&&y<=butony+3*dimbuty&&ismouseclick(WM_LBUTTONDOWN))
                     {
                         options(okp,lg);
                     }
                     else if(x>butonx&&x<=butonx+dimbutx&&y>=butony+3*dimbuty&&y<=10+butony+4*dimbuty&&ismouseclick(WM_LBUTTONDOWN))
                          {
                              PlaySound(NULL, 0, 0);
                              clearviewport();
                              if(progB>0)
                              {
                                  fin_progres.open("progres.in",ios::out);
                                  fin_progres<<progB;
                                  fin_progres.close();
                              }
                              clearmouseclick(WM_LBUTTONDOWN);
                              closed=1;
                              exit(0);
                          }
        clearmouseclick(WM_LBUTTONDOWN);
        }
    }
}
int main()
{
    initwindow(windowx,windowy,"Jump In- Andra Andrici & Capsa Gabriel ™");
    //PlaySound(TEXT("C:\\Users\\andra\\OneDrive\\Documenti\\INFO-exercitii\\JumpIn_test\\Dreams-2.wav"),NULL, SND_ASYNC);
    citireNiveluri(n);
    citireCreatii(CREATIE);
    PornireJoc();
    closegraph();
    return 0;
}
