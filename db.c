#include <stdio.h>
#include <postgresql/libpq-fe.h>
#include <string.h>

int main()
{
    PGconn *conn;
    PGresult *res;

    conn = PQconnectdb("dbname=afinititest host=localhost user=firzok password=1212");

    if (PQstatus(conn) == CONNECTION_BAD)
    {
        puts("We were unable to connect to the database");
        PQclear(res);
        PQfinish(conn);
        return 0;
    }

    res = PQexec(conn,
                 "SELECT signalValue FROM snmpSignals ORDER BY signalTime DESC LIMIT 1");

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        puts("We did not get any data!");
        PQclear(res);
        PQfinish(conn);
        return 0;
    }

    printf("%s\n", PQgetvalue(res, 0, 0));

    PQclear(res);
    PQfinish(conn);
    return 0;
}