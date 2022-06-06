#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
int n,r;
struct frame{
   char ack;
   int data;
}frm[10];
int sender(void);
void recvack(void);
void resend_sr(void);
void resend_gb(void);
void goback(void);
void selective(void);
int main(){
   int c;
   do{
       printf("1:Selective Repeat protocol\n2:Go Back N protocol\n0:Exit\n");
       printf("Enter choice:");
       scanf("%d",&c);
       switch(c)
       {
           case 1:selective();
           break;
           case 2:goback();
           case 0: exit(0);
           break;
       }
   }while(c>=1);

}    void goback(){
           sender();
           recvack();
           resend_gb();
           printf("\nAll frames sent successfully!\n");
       }
       void selective(){
           sender();
           recvack();
           resend_sr();
           printf("\nAll frames sent successfully!\n");
       }
       int sender(){
           int i;
           printf("\nEnter no. of frame to be sent:");
           scanf("%d",&n);
           for(i=1;i<=n;i++){
               printf("\n Enter data for frame [%d] ",i);
               scanf("%d",&frm[i].data);
               frm[i].ack='y';
           }
           return 0;
           }

       void recvack(){
           int i;
           rand();
           r=rand()%n;
           frm[r].ack='n';
           for(i=1;i<=n;i++){
               if(frm[i].ack=='n'){
                   printf("\n The frame number %d is not received\n",r);
               }
           }

       }
       void resend_sr(){
           printf("\nResending the frame %d",r);
           sleep(2);
           frm[r].ack='y';
           printf("\nThe received frame is %d",frm[r].data);
       }
       void resend_gb(){
           int i;
           printf("\nResending from frame %d",r);
           for(i=r;i<=n;i++){
               sleep(2);
               frm[i].ack='y';
               printf("\nReceived data of frame %d is %d",i,frm[i].data);
           }
       }