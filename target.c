#include<stdio.h>
int gcd(int a,int b)
{
if(b==(0))
{
return (a);
}
else
{
return (gcd(b,a%(b)));
}
}
int main(int argc,char** argv)
{
//input,output
int x,y;
<<<<<<< HEAD
=======
int get(int a,int bnt b))
{
get(int a,int bnt b))=(b + (a));
}
>>>>>>> 8bacc4d3d8db95744841722753f2acb369722d9e
scanf("%d%d",&x,&y);
printf("%d",gcd(x,y));

return 0;
}