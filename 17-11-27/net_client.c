/*
 *  ======== client.c ========
 *
 * TCP/IP Network Client example ported to use BIOS6 OS.
 *
 * Copyright (C) 2007, 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/
#include "drv.h"

// When USE_OLD_SERVERS set to zero, server daemon is used
#define USE_OLD_SERVERS 0

//---------------------------------------------------------------------------
// Title String
//
#pragma DATA_SECTION(VerStr, ".rodata");
const char *VerStr = "";

// Our NETCTRL callback functions
static void   NetworkOpen();
static void   NetworkClose();
static void   NetworkIPAddr( IPN IPAddr, uint IfIdx, uint fAdd );

// Fun reporting function
static void   ServiceReport( uint Item, uint Status, uint Report, HANDLE hCfgEntry );
/* Platform Information - we will read it form the Platform Library */

//---------------------------------------------------------------------------
// Configuration
//
char *HostName = "C6678";
extern char *IPAddr;
extern char *LocalIPMask;    // Not used when using DHCP
extern char *GatewayIP;    // Not used when using DHCP
char *DomainName  = "demo.net";         // Not used when using DHCP
char *DNSServer   = "0.0.0.0";          // Used when set to anything but zero

//volatile uint32_t sum = 0;
// Simulator EMAC Switch does not handle ALE_LEARN mode, so please configure the
// MAC address of the PC where you want to launch the webpages and initiate PING to NDK */

Uint8 clientMACAddress [6] = {0x00, 0x15, 0xE9, 0x85, 0x8A, 0x0A}; /* MAC address for my PC */

UINT8 DHCP_OPTIONS[] = { DHCPOPT_SERVER_IDENTIFIER, DHCPOPT_ROUTER };

void modify_ip(char* ip, char* ipmask, char* gatewayip)
{
    IPAddr = ip;
    LocalIPMask = ipmask;
    GatewayIP = gatewayip;
}
//
// Main Thread
//
//
Void ndk_stack(UArg a0, UArg a1)
{
    if(DNUM == MAST_CORE)
    {
        int               rc;
        HANDLE            hCfg;
        CI_SERVICE_TELNET telnet;
        CI_SERVICE_HTTP   http;

        //
        // THIS MUST BE THE ABSOLUTE FIRST THING DONE IN AN APPLICATION before
        //  using the stack!!
        //
        rc = NC_SystemOpen( NC_PRIORITY_HIGH, NC_OPMODE_INTERRUPT );
        if( rc )
        {
           printf("NC_SystemOpen Failed (%d)\n",rc);
            for(;;);
        }
        // Create a new configuration
        hCfg = CfgNew();
        if( !hCfg )
        {
            printf("Unable to create configuration\n");
            goto main_exit;
        }

        // We better validate the length of the supplied names
        if( strlen( DomainName ) >= CFG_DOMAIN_MAX ||
            strlen( HostName ) >= CFG_HOSTNAME_MAX )
        {
            printf("Names too long\n");
            goto main_exit;
        }

        // Add our global hostname to hCfg (to be claimed in all connected domains)
        CfgAddEntry( hCfg, CFGTAG_SYSINFO, CFGITEM_DHCP_HOSTNAME, 0,
                     strlen(HostName), (UINT8 *)HostName, 0 );

        // If the IP address is specified, manually configure IP and Gateway
        CI_IPNET NA;
        CI_ROUTE RT;
        IPN      IPTmp;

            // Setup manual IP address
        bzero( &NA, sizeof(NA) );
        NA.IPAddr  = inet_addr(IPAddr);
        NA.IPMask  = inet_addr(LocalIPMask);
        strcpy( NA.Domain, DomainName );
        NA.NetType = 0;

        // Add the address to interface 1
        CfgAddEntry( hCfg, CFGTAG_IPNET, 1, 0,
                               sizeof(CI_IPNET), (UINT8 *)&NA, 0 );

            // Add the default gateway. Since it is the default, the
            // destination address and mask are both zero (we go ahead
            // and show the assignment for clarity).
        bzero( &RT, sizeof(RT) );
        RT.IPDestAddr = 0;
        RT.IPDestMask = 0;
        RT.IPGateAddr = inet_addr(GatewayIP);

        // Add the route
        CfgAddEntry( hCfg, CFGTAG_ROUTE, 0, 0,
                               sizeof(CI_ROUTE), (UINT8 *)&RT, 0 );

            // Manually add the DNS server when specified
        IPTmp = inet_addr(DNSServer);
        if( IPTmp )
        CfgAddEntry( hCfg, CFGTAG_SYSINFO, CFGITEM_DHCP_DOMAINNAMESERVER,
                             0, sizeof(IPTmp), (UINT8 *)&IPTmp, 0 );

        // Specify TELNET service for our Console example
        bzero( &telnet, sizeof(telnet) );
        telnet.cisargs.IPAddr = INADDR_ANY;
        telnet.cisargs.pCbSrv = &ServiceReport;
        telnet.param.MaxCon   = 2;
        telnet.param.Callback = &ConsoleOpen;
        CfgAddEntry( hCfg, CFGTAG_SERVICE, CFGITEM_SERVICE_TELNET, 0,
                     sizeof(telnet), (UINT8 *)&telnet, 0 );

        // HTTP Authentication
        {
            CI_ACCT CA;

            // Name our authentication group for HTTP (Max size = 31)
            // This is the authentication "realm" name returned by the HTTP
            // server when authentication is required on group 1.
            CfgAddEntry( hCfg, CFGTAG_SYSINFO, CFGITEM_SYSINFO_REALM1,
                         0, 30, (UINT8 *)"DSP_CLIENT_DEMO_AUTHENTICATE1", 0 );

            // Create a sample user account who is a member of realm 1.
            // The username and password are just "username" and "password"
            strcpy( CA.Username, "username" );
            strcpy( CA.Password, "password" );
            CA.Flags = CFG_ACCTFLG_CH1;  // Make a member of realm 1
            rc = CfgAddEntry( hCfg, CFGTAG_ACCT, CFGITEM_ACCT_REALM,
                              0, sizeof(CI_ACCT), (UINT8 *)&CA, 0 );
        }

        // Specify HTTP service
        bzero( &http, sizeof(http) );
        http.cisargs.IPAddr = INADDR_ANY;
        http.cisargs.pCbSrv = &ServiceReport;
        CfgAddEntry( hCfg, CFGTAG_SERVICE, CFGITEM_SERVICE_HTTP, 0,
                     sizeof(http), (UINT8 *)&http, 0 );

        //
        // Configure IPStack/OS Options
        //

        // We don't want to see debug messages less than WARNINGS
        rc = DBG_INFO;
        CfgAddEntry( hCfg, CFGTAG_OS, CFGITEM_OS_DBGPRINTLEVEL,
                     CFG_ADDMODE_UNIQUE, sizeof(uint), (UINT8 *)&rc, 0 );

        //
        // This code sets up the TCP and UDP buffer sizes
        // (Note 8192 is actually the default. This code is here to
        // illustrate how the buffer and limit sizes are configured.)
        //

        // TCP Transmit buffer size
    //    rc = 8192;
        rc = 64*1024;
        CfgAddEntry( hCfg, CFGTAG_IP, CFGITEM_IP_SOCKTCPTXBUF,
                     CFG_ADDMODE_UNIQUE, sizeof(uint), (UINT8 *)&rc, 0 );

        // TCP Receive buffer size (copy mode)
    //    rc = 8192;
        rc = 64*1024;
        CfgAddEntry( hCfg, CFGTAG_IP, CFGITEM_IP_SOCKTCPRXBUF,
                     CFG_ADDMODE_UNIQUE, sizeof(uint), (UINT8 *)&rc, 0 );

        // TCP Receive limit (non-copy mode)
    //    rc = 8192;
        rc = 64*1024;
        CfgAddEntry( hCfg, CFGTAG_IP, CFGITEM_IP_SOCKTCPRXLIMIT,
                     CFG_ADDMODE_UNIQUE, sizeof(uint), (UINT8 *)&rc, 0 );

        // UDP Receive limit
        rc = 64*1024;
        CfgAddEntry( hCfg, CFGTAG_IP, CFGITEM_IP_SOCKUDPRXLIMIT,
                     CFG_ADDMODE_UNIQUE, sizeof(uint), (UINT8 *)&rc, 0 );

    #if 0
        // TCP Keep Idle (10 seconds)
        rc = 100;
        //   This is the time a connection is idle before TCP will probe
        CfgAddEntry( hCfg, CFGTAG_IP, CFGITEM_IP_TCPKEEPIDLE,
                     CFG_ADDMODE_UNIQUE, sizeof(uint), (UINT8 *)&rc, 0 );

        // TCP Keep Interval (1 second)
        //   This is the time between TCP KEEP probes
        rc = 10;
        CfgAddEntry( hCfg, CFGTAG_IP, CFGITEM_IP_TCPKEEPINTVL,
                     CFG_ADDMODE_UNIQUE, sizeof(uint), (UINT8 *)&rc, 0 );

        // TCP Max Keep Idle (5 seconds)
        //   This is the TCP KEEP will probe before dropping the connection
        rc = 50;
        CfgAddEntry( hCfg, CFGTAG_IP, CFGITEM_IP_TCPKEEPMAXIDLE,
                     CFG_ADDMODE_UNIQUE, sizeof(uint), (UINT8 *)&rc, 0 );
    #endif

        //
        // Boot the system using this configuration
        //
        // We keep booting until the function returns 0. This allows
        // us to have a "reboot" command.
        //
        do
        {
            rc = NC_NetStart( hCfg, NetworkOpen, NetworkClose, NetworkIPAddr );
        } while( rc > 0 );

        // Free the WEB files
    //    RemoveWebFiles();

        // Delete Configuration
        CfgFree( hCfg );

        // Close the OS
    main_exit:
        NC_SystemClose();
        //return(0);
    }
}


static HANDLE NDK_TSK_Handle=0;
extern void netUdpClientTask();
extern void netTcpRecv(void);
extern void netTcpClientTask();
extern void netTcpServerTask();
extern void udptest(void);
extern void boardcast_server(void);
extern void tcp(void);

//
// dtask_tcp_oobsrv() - TCP Data Server Daemon Function
// (SOCK_STREAMNC. port 999)
//
// Returns "1" if socket 's' is still open, and "0" if its been closed
//
char           gbuf[32768];
 int cnt=0, cnt1=0;
int dtask_tcp_oobsrv( SOCKET s, UINT32 unused )
{
    struct timeval to;
    int            i;


    (void)unused;
    uint64_t start;


    // Configure our socket timeout to be 5 seconds
    to.tv_sec  = 5;
    to.tv_usec = 0;
    //setsockopt( s, SOL_SOCKET, SO_SNDTIMEO, &to, sizeof( to ) );
    //setsockopt( s, SOL_SOCKET, SO_RCVTIMEO, &to, sizeof( to ) );

    for(;;)
    {
    	start = CSL_tscRead();
//		cnt = recv(s, gbuf, 16384, 0);
//		if(cnt < 0)
//			return 0;
//		cnt1 += cnt;

    	for(i=0; i<100000; i++)
    	{
    		cnt = send( s, gbuf, 32768, 0 );
    		if(cnt < 0)
    			return 0;
    	}

    	//printf("Speed is %fGbps\n", (float)(i*16384*8)/(CSL_tscRead() - start));
    }
    //fdClose( s );

    // Return "0" since we closed the socket
    return(0);
}
//
// NetworkOpen
//
// This function is called after the configuration has booted
//
static HANDLE hHello=0;
static void NetworkOpen()
{
	//HANDLE DaemonNew(uint Type, IPN LocalAddress, uint LocalPort, int(*pCb)(SOCKET,UINT32), uint Priority, uint StackSize, UINT32 Argument, uint MaxSpawn);
    // Create our local server
    //hHello = DaemonNew( SOCK_STREAM, 0, 7, dtask_tcp_oobsrv, OS_TASKPRINORM, OS_TASKSTKNORM, 0, 1 );
	NDK_TSK_Handle = TaskCreate( tcp, "udptest", OS_TASKPRINORM, 0x1400, 0, 0, 0 );
	//NDK_TSK_Handle = TaskCreate( netUdpServerTask, "netUdpServerTask", OS_TASKPRINORM, 0x1400, 0, 0, 0 );

	//NDK_TSK_Handle = TaskCreate( netTcpRecv, "netTcpRecv", OS_TASKPRINORM, 0x1400, 0, 0, 0 );
	//NDK_TSK_Handle = TaskCreate( netTcpClientTask, "netTcpClientTask", OS_TASKPRINORM, 0x1400, 0, 0, 0 );
}

//
// NetworkClose
//
// This function is called when the network is shutting down,
// or when it no longer has any IP addresses assigned to it.
//
static void NetworkClose()
{
    // Kill any active console
    ConsoleClose();
}


//
// NetworkIPAddr
//
// This function is called whenever an IP address binding is
// added or removed from the system.
//
static void NetworkIPAddr( IPN IPAddr, uint IfIdx, uint fAdd )
{
    static uint fAddGroups = 0;
    IPN IPTmp;

    if( fAdd )
        printf("Network Added: ");
    else
        printf("Network Removed: ");

    // Print a message
    IPTmp = ntohl( IPAddr );
    printf("If-%d:%d.%d.%d.%d\n", IfIdx,
            (UINT8)(IPTmp>>24)&0xFF, (UINT8)(IPTmp>>16)&0xFF,
            (UINT8)(IPTmp>>8)&0xFF, (UINT8)IPTmp&0xFF );

    // This is a good time to join any multicast group we require
    if( fAdd && !fAddGroups )
    {
        fAddGroups = 1;
//              IGMPJoinHostGroup( inet_addr("224.1.2.3"), IfIdx );
    }
}

//
// DHCP_reset()
//
// Code to reset DHCP client by removing it from the active config,
// and then reinstalling it.
//
// Called with:
// IfIdx    set to the interface (1-n) that is using DHCP.
// fOwnTask set when called on a new task thread (via TaskCreate()).
//
void DHCP_reset( uint IfIdx, uint fOwnTask )
{
    CI_SERVICE_DHCPC dhcpc;
    HANDLE h;
    int    rc,tmp;
    uint   idx;

    // If we were called from a newly created task thread, allow
    // the entity that created us to complete
    if( fOwnTask )
        TaskSleep(500);

    // Find DHCP on the supplied interface
    for(idx=1; ; idx++)
    {
        // Find a DHCP entry
        rc = CfgGetEntry( 0, CFGTAG_SERVICE, CFGITEM_SERVICE_DHCPCLIENT,
                          idx, &h );
        if( rc != 1 )
            goto RESET_EXIT;

        // Get DHCP entry data
        tmp = sizeof(dhcpc);
        rc = CfgEntryGetData( h, &tmp, (UINT8 *)&dhcpc );

        // If not the right entry, continue
        if( (rc<=0) || dhcpc.cisargs.IfIdx != IfIdx )
        {
            CfgEntryDeRef(h);
            h = 0;
            continue;
        }

        // This is the entry we want!

        // Remove the current DHCP service
        CfgRemoveEntry( 0, h );

        // Specify DHCP Service on specified IF
        bzero( &dhcpc, sizeof(dhcpc) );
        dhcpc.cisargs.Mode   = CIS_FLG_IFIDXVALID;
        dhcpc.cisargs.IfIdx  = IfIdx;
        dhcpc.cisargs.pCbSrv = &ServiceReport;
        CfgAddEntry( 0, CFGTAG_SERVICE, CFGITEM_SERVICE_DHCPCLIENT, 0,
                     sizeof(dhcpc), (UINT8 *)&dhcpc, 0 );
        break;
    }

RESET_EXIT:
    // If we are a function, return, otherwise, call TaskExit()
    if( fOwnTask )
        TaskExit();
}


void CheckDHCPOptions();

//
// Service Status Reports
//
// Here's a quick example of using service status updates
//
static char *TaskName[]  = { "Telnet","HTTP","NAT","DHCPS","DHCPC","DNS" };
static char *ReportStr[] = { "","Running","Updated","Complete","Fault" };
static char *StatusStr[] = { "Disabled","Waiting","IPTerm","Failed","Enabled" };
static void ServiceReport( uint Item, uint Status, uint Report, HANDLE h )
{
    printf( "Service Status: %-9s: %-9s: %-9s: %03d\n",
            TaskName[Item-1], StatusStr[Status],
            ReportStr[Report/256], Report&0xFF );

    //
    // Example of adding to the DHCP configuration space
    //
    // When using the DHCP client, the client has full control over access
    // to the first 256 entries in the CFGTAG_SYSINFO space.
    //
    // Note that the DHCP client will erase all CFGTAG_SYSINFO tags except
    // CFGITEM_DHCP_HOSTNAME. If the application needs to keep manual
    // entries in the DHCP tag range, then the code to maintain them should
    // be placed here.
    //
    // Here, we want to manually add a DNS server to the configuration, but
    // we can only do it once DHCP has finished its programming.
    //
    if( Item == CFGITEM_SERVICE_DHCPCLIENT &&
        Status == CIS_SRV_STATUS_ENABLED &&
        (Report == (NETTOOLS_STAT_RUNNING|DHCPCODE_IPADD) ||
         Report == (NETTOOLS_STAT_RUNNING|DHCPCODE_IPRENEW)) )
    {
        IPN IPTmp;

        // Manually add the DNS server when specified
        IPTmp = inet_addr(DNSServer);
        if( IPTmp )
            CfgAddEntry( 0, CFGTAG_SYSINFO, CFGITEM_DHCP_DOMAINNAMESERVER,
                         0, sizeof(IPTmp), (UINT8 *)&IPTmp, 0 );
#if 0
        // We can now check on what the DHCP server supplied in
        // response to our DHCP option tags.
        CheckDHCPOptions();
#endif

    }

    // Reset DHCP client service on failure
    if( Item==CFGITEM_SERVICE_DHCPCLIENT && (Report&~0xFF)==NETTOOLS_STAT_FAULT )
    {
        CI_SERVICE_DHCPC dhcpc;
        int tmp;

        // Get DHCP entry data (for index to pass to DHCP_reset).
        tmp = sizeof(dhcpc);
        CfgEntryGetData( h, &tmp, (UINT8 *)&dhcpc );

        // Create the task to reset DHCP on its designated IF
        // We must use TaskCreate instead of just calling the function as
        // we are in a callback function.
        TaskCreate( DHCP_reset, "DHCPreset", OS_TASKPRINORM, 0x1000,
                    dhcpc.cisargs.IfIdx, 1, 0 );
    }
}

void CheckDHCPOptions()
{
    char IPString[16];
    IPN  IPAddr;
    int  i, rc;

    // Now scan for DHCPOPT_SERVER_IDENTIFIER via configuration
    printf("\nDHCP Server ID:\n");
    for(i=1;;i++)
    {
        // Try and get a DNS server
        rc = CfgGetImmediate( 0, CFGTAG_SYSINFO, DHCPOPT_SERVER_IDENTIFIER,
                              i, 4, (UINT8 *)&IPAddr );
        if( rc != 4 )
            break;

        // We got something

        // Convert IP to a string:
        NtIPN2Str( IPAddr, IPString );
        printf("DHCP Server %d = '%s'\n", i, IPString);
    }
    if( i==1 )
        printf("None\n\n");
    else
        printf("\n");

    // Now scan for DHCPOPT_ROUTER via the configuration
    printf("Router Information:\n");
    for(i=1;;i++)
    {
        // Try and get a DNS server
        rc = CfgGetImmediate( 0, CFGTAG_SYSINFO, DHCPOPT_ROUTER,
                              i, 4, (UINT8 *)&IPAddr );
        if( rc != 4 )
            break;

        // We got something

        // Convert IP to a string:
        NtIPN2Str( IPAddr, IPString );
        printf("Router %d = '%s'\n", i, IPString);
    }
    if( i==1 )
        printf("None\n\n");
    else
        printf("\n");
}


