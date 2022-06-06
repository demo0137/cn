#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#define SUCCESS 1
#define FAILURE 0
#define NO_OF_FRAMES 10
typedef int BIT_32_VAR_INT ;
typedef char BIT_8_VAR_CHAR ;
void error ( BIT_8_VAR_CHAR *msg )
{
perror ( msg ) ;
exit ( 1 ) ;

}
typedef struct pdu_data
{
BIT_32_VAR_INT SN ;
BIT_8_VAR_CHAR data[80] ;
BIT_32_VAR_INT status ;
}PDU_DATA;

int main () {

 PDU_DATA send_data[10] , recv_data[10] ;
 BIT_32_VAR_INT socket_fd , newsocket_fd , port_no , client_len ;
//  char buffer[256] ;
 struct sockaddr_in serv_addr , cli_addr ;
 int sizeof_read_data , counter_1 ;
 //char buf[256] ;
 socket_fd = socket ( AF_INET , SOCK_STREAM , 0 ) ;
 if ( socket_fd < 0 )
 {
   error ( "error in socket opening " ) ;
 }

 bzero ( ( char * ) &serv_addr , sizeof ( serv_addr ) ) ;
 port_no = atoi ( "7100" ) ;

 serv_addr.sin_family = AF_INET ;
 serv_addr.sin_addr.s_addr = INADDR_ANY ;
 serv_addr.sin_port = htons ( 6000 ) ;

 if ( bind ( socket_fd , ( struct sockaddr * ) &serv_addr , sizeof ( serv_addr ) ) < 0 )
   error ( "error in binding" ) ;

 listen ( socket_fd , 5 ) ;

 client_len = sizeof ( cli_addr ) ;
 newsocket_fd = accept ( socket_fd , ( struct sockaddr * ) &cli_addr , &client_len ) ;

 if ( newsocket_fd < 0 )
   error ( "error in accept " ) ;

//  bzero ( buffer , 256 ) ;

 while ( 1 )
 {

   for ( counter_1 = 0 ; counter_1 < NO_OF_FRAMES ; counter_1++ )
{

   sizeof_read_data = read ( newsocket_fd , &recv_data[counter_1] , sizeof ( recv_data[counter_1]) ) ;

   if ( sizeof_read_data < 0 )
     error ( "error in reading from socket " ) ;
     
   printf ( "client sended : %s with SN %d :\n" , recv_data[counter_1].data , recv_data[counter_1].SN) ;
      
   printf ( "enter a reply to client : " ) ;
   scanf ( "%s",send_data[counter_1].data ) ;
      
   printf ( "\nenter a SN for next you want to receive : " ) ;
   scanf ( "%d",&send_data[counter_1].SN ) ;
  
   if ( send_data[counter_1].SN <= recv_data[counter_1].SN )
    counter_1 -= 1 ;
  
   send_data[counter_1].status = SUCCESS ;
   write ( newsocket_fd , &send_data[counter_1]  , sizeof ( send_data[counter_1]) ) ;
 }

 bzero ( &recv_data[counter_1].data , sizeof ( recv_data[counter_1].data ) )  ;
 read ( newsocket_fd , &recv_data[counter_1] , sizeof ( recv_data[counter_1] ) ) ;
 printf ( "it's end  Thank YOu : \n" ) ;
 exit ( 1 ) ;
}
return 0 ;

}
