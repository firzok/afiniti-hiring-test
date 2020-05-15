# afiniti-hiring-test

For details of this task please see [Task File.](https://github.com/firzok/afiniti-hiring-test/blob/master/Afiniti%20Engineering%20Hiring%20Test%20202005.pdf)

#### Language of choice
I chose `C` as the programming language because of its robustness, fast response, and most importantly
the availability of Net-SNMP Library and the little support in terms of examples/documentation it had. In terms of other languages, I researched a little bit on `go` but couldn't find any good library or examples, etc. I took a look at this [library](https://github.com/soniah/gosnmp) but based on the fact that it wasn't official and had little to no documentation I rejected it.

#### Architecture
Based on the little understanding I got from the Net-SNMP Library I made an SNMP Subagent implemented in the [daemon.c](https://github.com/firzok/afiniti-hiring-test/blob/master/daemon.c) file. This Subagent then registers different values from different sources to three different OIDs implemented in the [subagentObject.c](https://github.com/firzok/afiniti-hiring-test/blob/master/subagentObject.c) file.

#### Limitations
Based on the little time I had I was only able to run the Subagent as a root user.

#### Inferior Solutions
One solution I came up with was to use Python and [Pysnmp Library](http://snmplabs.com/pysnmp/). This would have taken less effort and time to develop but a Python program can never be compared to a C/C++ program in terms of its performance and the ability to scale.

#### Dependencies
* net-snmp
* libpq-fe [postgresql]

#### Running

1. Run the [database.sql](https://github.com/firzok/afiniti-hiring-test/blob/master/database.sql) file in your pgAdmin to setup the database.

2. Run this command in the project folder to clean previous build(if any)

    ```console
    foo@bar:~$ make clean
    ```

3. Run this command in the project folder to build

    ```console
    foo@bar:~$ make
    ```

4. Run this command to run

    ```console
    foo@bar:~$ sudo ./daemon -f
    ```

#### Testing

1. Run this command in another terminal to see the registered values in realtime

    ```console
    foo@bar:~$ watch snmpwalk -v 2c -c tutorial localhost 1.3.6.1.4.1.53864.2.4.1.1.2
    ```

2. Run these commands to get separate values

      1. Version number
            ```console
            foo@bar:~$ snmpget -v 2c -c tutorial localhost 1.3.6.1.4.1.53864.2.4.1.1.2.4
            SNMPv2-SMI::enterprises.53864.2.4.1.1.2.4 = STRING: "6.1.1"
            ```
      
      2. SNMP Signal Value
            ```console
            foo@bar:~$ snmpget -v 2c -c tutorial localhost 1.3.6.1.4.1.53864.2.4.1.1.2.1
            SNMPv2-SMI::enterprises.53864.2.4.1.1.2.1 = INTEGER: 50
            ```
      
      3. Size of `/var/log` in bytes
            ```console
            foo@bar:~$ snmpget -v 2c -c tutorial localhost 1.3.6.1.4.1.53864.2.4.1.1.2.0
            SNMPv2-SMI::enterprises.53864.2.4.1.1.2.0 = INTEGER: 123935211
            ```
      

#### Time spent
    Research: 1.5 days
    Development: 0.5 day
    
