/*

argv[0] contains filename;
argv[1] contains server_ipaddress;
argv[2] contains portno


*/


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>


void error(const char *msg)
{
	perror(msg);
	exit(0);
}
int mul(int rbit)
{
  int r = 1;
  for(int i=0;i<rbit;i++)  //2^r >= messageLength + r +1 --->Formula to calulate r 
  {
    r = 2*r;
  }
  return r;
}

/// This function will add parity bits to our data
void addParityBits(int data[], int messageLength, int rbit, int Array[])
{
  //We are making use of even parity here i.e if the count is 4 then make rbit as 0 as evenparity already exits if count is 3 make the rbit as 1 as now 3+1 = 4 and hence even parity
  int count = 0;
  int p = 0;
  int q = 1;
  int s = 1;
  int r = 0;
  for(int i=(messageLength+rbit);i>=1;i--)// the value of i will start for the end of the messageLength+rbit
  {                                       // lets say our messageLength is 7 and hence rbits == 4  and hence total length will be 11    
    if(data[i] == 7 || s == mul(r))                     // we start from 11 i.e for this loop its like 1 and 7 will be stored here from earlier
    {                                    //1 2   3  4  5 6   7 8   9 10 11
      count = 0;                  ///     11 10  9  8  7  6  5 4   3  2  1
      
      for(int j=i;j>=1;)           //for i=2, j=2 then we will continue inner for loop for 2 rounds as q=2 and parity bit is 2 so we have to do for 10-9 6-5 i.e 2-3 6-7
      {
        for(int k=0;k<q;k++)
        {
          if(data[j] == 1)
          {
            count++;
          }
          printf("j is %d\n",j);
          j--;     // we start from 10 so for first j=9 and for second j=8 but our next bits are 6 and 5 so we subtract q
        }
        j = j-q;
      }
      q = q*2;
      printf("Count for position %d is %d\n",i,count);
      if(count%2 == 0)
      {
        data[i] = 0;
        Array[p] = 0;
        p++;
      }
      else
      {
        data[i] = 1;
        Array[p] = 1;
        p++;
      }
      r++;

    }
    s++;
  }
  
 
}

int binaryToDecimal(int a[],int rbit)
{
  int r =1;
  int x = 0;
  for(int i=0;i<rbit;i++)
  {
    x = r*a[i]+x;
    r = r*2;
  }
  return x;
}


int main(int argc, char *argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	int messageLength;
	int data[255];
	int temp;
	int rbit;
	int errorbit;
	int copyData[100];
	int Array[100];
	if(argc < 3)
	{
		fprintf(stderr, "usage %s hostname port \n", argv[0]);
		exit(1);
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM,0 );
	if(sockfd < 0)
	{
		error("Error opening the socket");
	}
	server = gethostbyname(argv[1]);
	if(server == NULL)
	{
		fprintf(stderr, "Error, no such host");

	}
	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char*) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr))<0)
	{
		error("Connection failed");
	}
	// while(1)
	// {
	// 	bzero(buffer, 255);
	// 	fgets(buffer, 255, stdin);
	// 	n = write(sockfd, buffer, strlen(buffer));
	// 	if(n < 0)
	// 	{
	// 		error("Error on writing");
	// 	}
	// 	bzero(buffer, 255);
	// 	n = read(sockfd, buffer, 255);
	// 	if(n < 0)
	// 	{
	// 		error("Erron on reading");
			
	// 	}
	// 	printf("Server: %s", buffer);
	// 	int i = strcmp("Bye", buffer);
	// 	if(i == 0) break;
	// }
	bzero(data, 255);
	int count=1;
	while(temp != -1)
	{
	 n = recv(sockfd, &temp, sizeof(temp), 0);
	 if(temp == 1 || temp == 0)
	 {

	 printf("%d ",temp);
	 data[count] = temp;
	 count++;
	 }
	}
	count = count -1;
	n = recv(sockfd, &rbit, sizeof(rbit),0);
	//printf("%d\n",rbit);
	printf("\n_____________________________Introducing Error in the array___________________________\n");
  printf("\n");




  	printf("\n%d\n", count);

  printf("<---------------Message---------------------->\n");
  for(int i=1;i<=(count);i++)
  {
    printf("%d   ",data[i]);
  }
  printf("\n");
  for(int i=1;i<=(count);i++)
  {
    printf("-   ");
  }
  printf("\n");
  for(int i=(count);i>=1;i--)
  {
    if((int)i/2>=5)
    {
      
    printf("%d  ",i);
    }
    else
    {
      printf("%d   ",i);
    }
  }




  printf("\n");


  //Introduce the error by taking input from the user
  printf("<-------------------------------------------->");
  printf("\n");
  printf("\nEnter the bit where you want to put error::");
  scanf("%d",&errorbit);
  if(data[((count+1)-errorbit)] == 0)
  {
    data[((count+1)-errorbit)] = 1;
  }
  else
  {
    data[((count+1)-errorbit)] = 0;
  }








  //Printing the error introduced array;
  printf("Error introduced message--->\n");
  for(int i=1;i<=(count);i++)
  {
    printf("%d",data[i]);
  }
  printf("\n");





  ///For copying data into copy data;
  for(int i=1;i<=(count);i++)
  {
    copyData[i] = data[i];
  }


  addParityBits(data,(count-rbit),rbit,Array);



  //Printing the parity bits of the error data
  printf("The parity bits for the error messages are::\n");
  for(int i=0;i<rbit;i++)
  {
    printf("%d",Array[i]);
  }
  printf("\n");







  int i = binaryToDecimal(Array,rbit);
  printf("The error was introduced at position %d", i);
  printf("\n\n");
  //Correcting the error that was introduced in the array
  if(copyData[count+1-i]==1)
  {
    copyData[count+1-i] = 0;
  }
  else
  {
    copyData[count+1-i] = 1;
  }
  printf("\n");
  printf("\nCorrected Data--->\n");
  for(int i=1;i<=(count);i++)
  {
    printf("%d",copyData[i]);
  }
	
	close(sockfd);
	return 0;

}

