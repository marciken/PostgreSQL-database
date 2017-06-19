// Server side C Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
/* libpq header file */ 
#include </home/maryamhafizahj/RPostgreSQL/src/libpq/libpq-fe.h> 
#define PORT 8080



int main(int argc, char const *argv[])
{

      char  query_string[256];         /* holds constructed SQL query */ 
      PGconn     *conn;                /* holds database connection */ 
      PGresult   *res;                 /* holds query result */ 
      int         i; 
          
 
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );

	conn = PQconnectdb("dbname=test123");    /* connect to the database */ 
 	if (PQstatus(conn) == CONNECTION_BAD) /* did the database connection fail? */ 
            { 
                fprintf(stderr, "Connection to database failed.\n"); 
                fprintf(stderr, "%s", PQerrorMessage(conn)); 
                exit(1); 
            } 
          
            sprintf(buffer,                   /* create an SQL query string */ 
                    "SELECT Name FROM STUDENT WHERE ID = '%s'", buffer); 
          
            res = PQexec(conn, query_string);            /* send the query */ 
          
            if (PQresultStatus(res) != PGRES_TUPLES_OK)  /* did the query fail? */ 
            { 
                fprintf(stderr, "SELECT query failed.\n"); 
                PQclear(res); 
                PQfinish(conn); 
                exit(1); 
            } 
          
            for (i = 0; i < PQntuples(res); i++) /* loop through all rows returned */ 
                printf("%s\n", PQgetvalue(res, i, 0)); /* print the value returned */

		send(new_socket , res , strlen(res) , 0 );
    		//printf("Hello message sent\n");
    		
          
            PQclear(res);                        /* free result */ 
          
            PQfinish(conn);                      /* disconnect from the database */ 
            return 0; 
          
        }
