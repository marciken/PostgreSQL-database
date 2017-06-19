
#include <stdio.h>
#include <stdlib.h>
#include </home/maryamhafizahj/RPostgreSQL/src/libpq/libpq-fe.h>

static void
exit_nicely(PGconn *conn, PGresult   *res)
{
    PQclear(res);
    PQfinish(conn);
    exit(1);
}

int
main(int argc, char* argv[])
{
    const char *conninfo = "sslmode=disable";
    PGconn     *conn;
    PGresult   *res;

    //------------------------------------------------------------------------
    // To create a new PostgreSQL database, connect with no database specified
    conn = PQconnectdb(conninfo);

    // Check to see that the backend connection was successfully made
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    res = PQexec(conn, "CREATE DATABASE test123");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "CREATE DATABASE failed: %s", PQerrorMessage(conn));
        exit_nicely(conn,res);
    }
    PQclear(res);

    PQfinish(conn); // Close connection

    //--------------------------------------------------------
    // Generate new connection to database adams created above
    conn = PQconnectdb("dbname=test123");
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to test123 database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    res = PQexec(conn, "CREATE TABLE STUDENT (Name char(20),ID char(8),Phone char(24))");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "CREATE TABLE failed: %s", PQerrorMessage(conn));
        exit_nicely(conn,res);
    }
    PQclear(res);

    PQfinish(conn); // Close connection

    //--------------------------------------------------------
    // Generate new connection to database adams created above
    conn = PQconnectdb("dbname=test123");
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to test123 database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    res = PQexec(conn, "CREATE TABLE STUDENT (Name char(20),ID char(10),Phone char(24))");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "CREATE TABLE failed: %s", PQerrorMessage(conn));
        exit_nicely(conn,res);
    }
    PQclear(res);

    char* sql;
    /*create SQL statement*/
    sql = "INSERT INTO STUDENT (Name , ID , Phone)"
          "VALUES ('Maryam Hafizah', '2016307359', '012-3456789');" 
          "INSERT INTO STUDENT (Name , ID , Phone)" 
          "VALUES ('Nazmi Faiz', '2016404040', '019-8765432');"  
          "INSERT INTO STUDENT (Name , ID , Phone)"  
          "VALUES ('Khai Bahar', '2016808080', '011-5647839');";

    res = PQexec(conn, sql);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "INSERT failed: %s", PQerrorMessage(conn));
        exit_nicely(conn,res);
    }
    PQclear(res);

    // close the connection to the database and cleanup
    PQfinish(conn);

    return 0;
}
