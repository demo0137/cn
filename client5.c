#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#define MAXLINE 80
char buffer[255] ;
#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT_NUM 6000
#define t_max_retransmission 3
typedef int bit_32_var ;
typedef char bit_8_var ;

typedef struct pdu_field
{
bit_32_var SN ;
bit_8_var data[MAXLINE] ;
bit_8_var status ;
}PDU_FIELD;
void str_cli ( FILE *fp , bit_32_var sfd )

{

bit_32_var no_of_data , counter_1 , counter_2 ;
static bit_32_var retransmission_counter , intial_readycheck_counter ;
PDU_FIELD send_data[MAXLINE] ,recv_data[MAXLINE] ;
printf ( "enter how many data you have to send : " ) ;
scanf ( "%d", &no_of_data ) ;
for ( counter_1 = 0 ; counter_1 < no_of_data ; counter_1++ )
{
printf ( "enter %d'th data : ",counter_1 ) ;
scanf ( "%s", send_data[counter_1].data ) ;
send_data[counter_1].SN = counter_1 ;
send_data[counter_1].status = 0 ;
}

for ( counter_1 = 0 ; counter_1 < no_of_data ; counter_1++ )
{
write ( sfd , &send_data[counter_1] , sizeof ( send_data[counter_1] ) ) ;
read ( sfd , &recv_data[counter_1] , sizeof ( recv_data[counter_1] ) ) ;

if ( counter_1 == 0 && ( strcmp ( recv_data[counter_1].data , "yes" ) != 0 ) )
{
  if ( intial_readycheck_counter == t_max_retransmission )
  {
  printf ( "client : receiver not there , better to exit : \n" ) ;
  exit ( 1 ) ;
  }
  intial_readycheck_counter++ ;
  printf ( "client : receiver is not ready : wait 10sec... \n" ) ;
  sleep ( 10 ) ;
  counter_1 = counter_1- 1 ;
}

if ( counter_1 > 0 )
{
if ( recv_data[counter_1].SN == send_data[counter_1].SN + 1 )
   printf ( "\n server responding = %s ", recv_data[counter_1].data ) ;
else
 {
 if ( retransmission_counter < t_max_retransmission )
   {
 printf ( "incorrect ack - sending the same data - \n" ) ;
 counter_1 -= 1 ;
 retransmission_counter++ ;
   }
 else
   {
    printf ( "Time out : sending next data \n " ) ;
retransmission_counter = 0 ;
   }
  
  }
}
}

write ( sfd , "Nothingtotransmit:\n" , 10 ) ;
printf ( "Transmission finished : \n" ) ;
exit ( 1 ) ;
}

int main ()

{

bit_32_var socket_fd ;
struct sockaddr_in config_client ;

socket_fd = socket ( AF_INET , SOCK_STREAM , 0 ) ;

if ( socket_fd < 0 )
{
printf ( "client : failed to create socket \n" ) ;
exit ( 1 ) ;
}

memset ( &config_client , 0 , sizeof ( struct sockaddr_in )) ;
config_client.sin_family = AF_INET ;
config_client.sin_port = htons ( SERVER_PORT_NUM ) ;
inet_aton ( SERVER_ADDR , &config_client.sin_addr ) ;
connect ( socket_fd , (struct sockaddr *)&config_client , sizeof ( config_client ) ) ;
printf ( "connect successfully\n" ) ;
system ( "clear" ) ;
str_cli ( stdin , socket_fd ) ;
exit ( 0 );
}
