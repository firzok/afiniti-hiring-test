#include <stdlib.h>
#include "subagentObject.h"
#include <postgresql/libpq-fe.h>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

void init_version()
{

    static char version[10] = "6.1.1";
    static oid nstAgentSubagentObject_oid[] = {1, 3, 6, 1, 4, 1, 53864, 2, 4, 1, 1, 2, 4};

    static netsnmp_watcher_info watcher_info;

    netsnmp_handler_registration *reginfo;

    snmp_log(LOG_INFO, "Initalizing Version instance.\n");

    //Creates a handler registration structure with a new MIB handler.
    reginfo = netsnmp_create_handler_registration("version", NULL,
                                                  nstAgentSubagentObject_oid,
                                                  OID_LENGTH(nstAgentSubagentObject_oid),
                                                  HANDLER_CAN_RONLY);

    int watcher_flags = WATCHER_MAX_SIZE;

    netsnmp_init_watcher_info6(&watcher_info, version, strlen(version),
                               ASN_OCTET_STR, watcher_flags,
                               sizeof(version), NULL);

    //netsnmp_register_watched_instance is used to register the static string variable "version"
    netsnmp_register_watched_instance(reginfo, &watcher_info);

    snmp_log(LOG_INFO, "Done initalizing Version instance.\n\n");
}

static long logSize;
FILE *ls;
int handler_logSize(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    snmp_log(LOG_INFO, "Log size accessed.\n");

    switch (reqinfo->mode)
    {
    case MODE_GET:
        ls = popen("du -sb /var/log | grep -oE '^\\s*[0-9]+'", "r");
        char sizeofLog[256];

        char *ret = fgets(sizeofLog, sizeof(sizeofLog), ls);
        pclose(ls);

        logSize = strtol(sizeofLog, NULL, 10);

        //snmp_set_var_typed_value is used to update the value of "logSize"
        snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
                                 &logSize,
                                 sizeof(logSize));
        break;
    default:
        snmp_log(LOG_ERR, "unknown mode (%d) in handle_totalClients\n", reqinfo->mode);
        return SNMP_ERR_GENERR;
    }
    return SNMP_ERR_NOERROR;
}

void init_log_size()
{
    /*
     * Getting the size of /var/log folder in bytes
     */
    FILE *ls = popen("du -sb /var/log | grep -oE '^\\s*[0-9]+'", "r");
    char sizeofLog[256];

    char *ret = fgets(sizeofLog, sizeof(sizeofLog), ls);
    pclose(ls);

    logSize = strtol(sizeofLog, NULL, 10);

    static oid log_size_oid[] = {1, 3, 6, 1, 4, 1, 53864, 2, 4, 1, 1, 2, 0};

    snmp_log(LOG_INFO, "Initalizing Log Size instance.\n");

    /*
    * the line below registers the "logSize" variable above as
    * accessible.
    * 
    * handler_logSize is a callback function defined above that 
    * be called everytime "signalValue" is accessed, it also acts 
    * as an updater of this value.
    */
    netsnmp_register_long_instance("logSize", log_size_oid, OID_LENGTH(log_size_oid), &logSize, handler_logSize);

    snmp_log(LOG_INFO, "Done initalizing Log Size instance.\n\n");
}

static long signalValue;

void update_signal_value()
{
    /*
     * Getting the latest value of signalValue from DB and setting it
     * In case of any error default value is set to 0
     */

    PGconn *conn;
    PGresult *res;

    conn = PQconnectdb("dbname=afinititest host=localhost user=firzok password=1212");

    if (PQstatus(conn) == CONNECTION_BAD)
    {
        snmp_log(LOG_ERR, "Unable to connect to database\n");
        PQclear(res);
        PQfinish(conn);
        signalValue = 0;
    }
    else
    {
        res = PQexec(conn,
                     "SELECT signalValue FROM snmpSignals ORDER BY signalTime DESC LIMIT 1");

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            snmp_log(LOG_ERR, "Did not get signalValue from database\n");
            PQclear(res);
            PQfinish(conn);
            signalValue = 0;
            return;
        }
        else
        {
            signalValue = atoi(PQgetvalue(res, 0, 0));
        }

        PQclear(res);
        PQfinish(conn);
    }
}

int handler_signalValue(netsnmp_mib_handler *handler, netsnmp_handler_registration *reginfo, netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests)
{
    snmp_log(LOG_INFO, "Signal Value accessed.\n");

    switch (reqinfo->mode)
    {
    case MODE_GET:
        update_signal_value();

        //snmp_set_var_typed_value is used to update the value of "signalValue"
        snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
                                 &signalValue,
                                 sizeof(signalValue));
        break;
    default:
        snmp_log(LOG_ERR, "unknown mode (%d) in handler_signalValue\n", reqinfo->mode);
        return SNMP_ERR_GENERR;
    }
    return SNMP_ERR_NOERROR;
}

void init_db_signalValue()
{
    update_signal_value();

    static oid signalValue_oid[] = {1, 3, 6, 1, 4, 1, 53864, 2, 4, 1, 1, 2, 1};

    snmp_log(LOG_INFO, "Initalizing Signal Value instance.\n");

    /*
    * the line below registers the "signalValue" variable above as
    * accessible.
    * 
    * handler_signalValue is a callback function defined above that 
    * be called everytime "signalValue" is accessed, it also acts 
    * as an updater of this value.
    */
    netsnmp_register_long_instance("signalValue", signalValue_oid, OID_LENGTH(signalValue_oid), &signalValue, handler_signalValue);

    snmp_log(LOG_INFO, "Done initalizing Signal Value instance.\n\n");
}