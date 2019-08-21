#pragma once

//extern "C" unsigned long far WriteDog(void);
//extern "C" unsigned long far ReadDog(void);


class CEncrypt
{
public:
	CEncrypt();
	~CEncrypt();

	static char s_chType;          // Licence类型： 1 .. 狗； 2 .. 网卡； 3 .. 日期
	static CString s_strData;      // Licence数据
	static CStringArray s_arrName; // Licence名称集合
	static CString s_strPwd;       // 交互密码

	CTime   m_LastTime;            // 上一次打开许可时间
	static CTimeSpan s_LastSpan;   // 系统时间与软件时间误差

	int     m_nKey[16][48];        // 密码位数组，临时变量

	// 时间转日期
	CTime TimeToDate(CTime& time);
	// 年月日转日期
	CTime YMDToDate(int nYear, int nMonth, int nDay);
	// 日期转年月日
	void DateToYMD(CTime& time, int& nYear, int& nMonth, int& nDay);
	// 格式化固定字符数组
	void FixChar(LPCSTR strText, char* outBuf, int len);
	// 获得网卡地址
	static int GetMacAddress(CStringArray *pArray);
	// 转换密码串到密码位
	void KeyStringToBit(LPCSTR strPwd);

	// 编码
	void Encrypt(char* inBuf, char* outBuf);
	// 解码
	void Decrypt(char* inBuf, char* outBuf);

	// 装入Licence数据
	BOOL Load(char* inBuf, BOOL bDog);

	//打开许可，先找狗，后找文件
	BOOL OpenLicence(LPCSTR strPwd,CString strDir);
	// 检测许可
	static int CheckLicence(LPCSTR strName);
	// 保存许可，先找狗，后找文件
	BOOL SaveLicence(LPCSTR strPwd);

};


/********************************************************************/
/**                     Microsoft LAN Manager                      **/
/**     Copyright (c) Microsoft Corporation. All rights reserved.  **/
/********************************************************************/
/* :ts=4 */

//** IPEXPORT.H - IP public definitions.
//
//  This file contains public definitions exported to transport layer and
//  application software.
//

#ifndef IP_EXPORT_INCLUDED
#define IP_EXPORT_INCLUDED  1

#if _MSC_VER > 1000
#pragma once
#endif

//
// IP type definitions.
//
typedef ULONG IPAddr;       // An IP address.
typedef ULONG IPMask;       // An IP subnet mask.
typedef ULONG IP_STATUS;    // Status code returned from IP APIs.

#ifndef s6_addr
//
// Duplicate these definitions here so that this file can be included by
// kernel-mode components which cannot include ws2tcpip.h, as well as
// by user-mode components which do.
//

typedef struct in6_addr {
    union {
        UCHAR       Byte[16];
        USHORT      Word[8];
    } u;
} IN6_ADDR;

#define in_addr6 in6_addr

//
// Defines to match RFC 2553.
//
#define _S6_un      u
#define _S6_u8      Byte
#define s6_addr     _S6_un._S6_u8

//
// Defines for our implementation.
//
#define s6_bytes    u.Byte
#define s6_words    u.Word

#endif

typedef struct in6_addr IPv6Addr;

/*INC*/

//
// The ip_option_information structure describes the options to be
// included in the header of an IP packet. The TTL, TOS, and Flags
// values are carried in specific fields in the header. The OptionsData
// bytes are carried in the options area following the standard IP header.
// With the exception of source route options, this data must be in the
// format to be transmitted on the wire as specified in RFC 791. A source
// route option should contain the full route - first hop thru final
// destination - in the route data. The first hop will be pulled out of the
// data and the option will be reformatted accordingly. Otherwise, the route
// option should be formatted as specified in RFC 791.
//

typedef struct ip_option_information {
    UCHAR   Ttl;                // Time To Live
    UCHAR   Tos;                // Type Of Service
    UCHAR   Flags;              // IP header flags
    UCHAR   OptionsSize;        // Size in bytes of options data
    PUCHAR  OptionsData;        // Pointer to options data
} IP_OPTION_INFORMATION, *PIP_OPTION_INFORMATION;

#if defined(_WIN64)

typedef struct ip_option_information32 {
    UCHAR   Ttl;
    UCHAR   Tos;
    UCHAR   Flags;
    UCHAR   OptionsSize;
    UCHAR * POINTER_32 OptionsData;
} IP_OPTION_INFORMATION32, *PIP_OPTION_INFORMATION32;

#endif // _WIN64

//
// The icmp_echo_reply structure describes the data returned in response
// to an echo request.
//

typedef struct icmp_echo_reply {
    IPAddr  Address;            // Replying address
    ULONG   Status;             // Reply IP_STATUS
    ULONG   RoundTripTime;      // RTT in milliseconds
    USHORT  DataSize;           // Reply data size in bytes
    USHORT  Reserved;           // Reserved for system use
    PVOID   Data;               // Pointer to the reply data
    struct ip_option_information Options; // Reply options
} ICMP_ECHO_REPLY, *PICMP_ECHO_REPLY;

#if defined(_WIN64)

typedef struct icmp_echo_reply32 {
    IPAddr  Address;
    ULONG   Status;
    ULONG   RoundTripTime;
    USHORT  DataSize;
    USHORT  Reserved;
    VOID * POINTER_32 Data;
    struct ip_option_information32 Options;
} ICMP_ECHO_REPLY32, *PICMP_ECHO_REPLY32;

#endif // _WIN64

typedef struct arp_send_reply {
    IPAddr  DestAddress;
    IPAddr  SrcAddress;
} ARP_SEND_REPLY, *PARP_SEND_REPLY;

typedef struct tcp_reserve_port_range {
    USHORT  UpperRange;
    USHORT  LowerRange;
} TCP_RESERVE_PORT_RANGE, *PTCP_RESERVE_PORT_RANGE;

#define MAX_ADAPTER_NAME 128

typedef struct _IP_ADAPTER_INDEX_MAP {
    ULONG   Index;
    WCHAR   Name[MAX_ADAPTER_NAME];
} IP_ADAPTER_INDEX_MAP, *PIP_ADAPTER_INDEX_MAP;

typedef struct _IP_INTERFACE_INFO {
    LONG    NumAdapters;
    IP_ADAPTER_INDEX_MAP Adapter[1];
} IP_INTERFACE_INFO,*PIP_INTERFACE_INFO;

typedef struct _IP_UNIDIRECTIONAL_ADAPTER_ADDRESS {
    ULONG   NumAdapters;
    IPAddr  Address[1];
} IP_UNIDIRECTIONAL_ADAPTER_ADDRESS, *PIP_UNIDIRECTIONAL_ADAPTER_ADDRESS;

typedef struct _IP_ADAPTER_ORDER_MAP {
    ULONG   NumAdapters;
    ULONG   AdapterOrder[1];
} IP_ADAPTER_ORDER_MAP, *PIP_ADAPTER_ORDER_MAP;

typedef struct _IP_MCAST_COUNTER_INFO {
    ULONG64 InMcastOctets;
    ULONG64 OutMcastOctets;
    ULONG64 InMcastPkts;
    ULONG64 OutMcastPkts;
} IP_MCAST_COUNTER_INFO, *PIP_MCAST_COUNTER_INFO;

//
// IP_STATUS codes returned from IP APIs
//

#define IP_STATUS_BASE              11000

#define IP_SUCCESS                  0
#define IP_BUF_TOO_SMALL            (IP_STATUS_BASE + 1)
#define IP_DEST_NET_UNREACHABLE     (IP_STATUS_BASE + 2)
#define IP_DEST_HOST_UNREACHABLE    (IP_STATUS_BASE + 3)
#define IP_DEST_PROT_UNREACHABLE    (IP_STATUS_BASE + 4)
#define IP_DEST_PORT_UNREACHABLE    (IP_STATUS_BASE + 5)
#define IP_NO_RESOURCES             (IP_STATUS_BASE + 6)
#define IP_BAD_OPTION               (IP_STATUS_BASE + 7)
#define IP_HW_ERROR                 (IP_STATUS_BASE + 8)
#define IP_PACKET_TOO_BIG           (IP_STATUS_BASE + 9)
#define IP_REQ_TIMED_OUT            (IP_STATUS_BASE + 10)
#define IP_BAD_REQ                  (IP_STATUS_BASE + 11)
#define IP_BAD_ROUTE                (IP_STATUS_BASE + 12)
#define IP_TTL_EXPIRED_TRANSIT      (IP_STATUS_BASE + 13)
#define IP_TTL_EXPIRED_REASSEM      (IP_STATUS_BASE + 14)
#define IP_PARAM_PROBLEM            (IP_STATUS_BASE + 15)
#define IP_SOURCE_QUENCH            (IP_STATUS_BASE + 16)
#define IP_OPTION_TOO_BIG           (IP_STATUS_BASE + 17)
#define IP_BAD_DESTINATION          (IP_STATUS_BASE + 18)

//
// Variants of the above using IPv6 terminology, where different
//

#define IP_DEST_NO_ROUTE            (IP_STATUS_BASE + 2)
#define IP_DEST_ADDR_UNREACHABLE    (IP_STATUS_BASE + 3)
#define IP_DEST_PROHIBITED          (IP_STATUS_BASE + 4)
#define IP_DEST_PORT_UNREACHABLE    (IP_STATUS_BASE + 5)
#define IP_HOP_LIMIT_EXCEEDED       (IP_STATUS_BASE + 13)
#define IP_REASSEMBLY_TIME_EXCEEDED (IP_STATUS_BASE + 14)
#define IP_PARAMETER_PROBLEM        (IP_STATUS_BASE + 15)

//
// IPv6-only status codes
//

#define IP_DEST_UNREACHABLE         (IP_STATUS_BASE + 40)
#define IP_TIME_EXCEEDED            (IP_STATUS_BASE + 41)
#define IP_BAD_HEADER               (IP_STATUS_BASE + 42)
#define IP_UNRECOGNIZED_NEXT_HEADER (IP_STATUS_BASE + 43)
#define IP_ICMP_ERROR               (IP_STATUS_BASE + 44)
#define IP_DEST_SCOPE_MISMATCH      (IP_STATUS_BASE + 45)

//
// The next group are status codes passed up on status indications to
// transport layer protocols.
//
#define IP_ADDR_DELETED             (IP_STATUS_BASE + 19)
#define IP_SPEC_MTU_CHANGE          (IP_STATUS_BASE + 20)
#define IP_MTU_CHANGE               (IP_STATUS_BASE + 21)
#define IP_UNLOAD                   (IP_STATUS_BASE + 22)
#define IP_ADDR_ADDED               (IP_STATUS_BASE + 23)
#define IP_MEDIA_CONNECT            (IP_STATUS_BASE + 24)
#define IP_MEDIA_DISCONNECT         (IP_STATUS_BASE + 25)
#define IP_BIND_ADAPTER             (IP_STATUS_BASE + 26)
#define IP_UNBIND_ADAPTER           (IP_STATUS_BASE + 27)
#define IP_DEVICE_DOES_NOT_EXIST    (IP_STATUS_BASE + 28)
#define IP_DUPLICATE_ADDRESS        (IP_STATUS_BASE + 29)
#define IP_INTERFACE_METRIC_CHANGE  (IP_STATUS_BASE + 30)
#define IP_RECONFIG_SECFLTR         (IP_STATUS_BASE + 31)
#define IP_NEGOTIATING_IPSEC        (IP_STATUS_BASE + 32)
#define IP_INTERFACE_WOL_CAPABILITY_CHANGE  (IP_STATUS_BASE + 33)
#define IP_DUPLICATE_IPADD          (IP_STATUS_BASE + 34)

#define IP_GENERAL_FAILURE          (IP_STATUS_BASE + 50)
#define MAX_IP_STATUS               IP_GENERAL_FAILURE
#define IP_PENDING                  (IP_STATUS_BASE + 255)


//
// Values used in the IP header Flags field.
//
#define IP_FLAG_DF      0x2         // Don't fragment this packet.

//
// Supported IP Option Types.
//
// These types define the options which may be used in the OptionsData field
// of the ip_option_information structure.  See RFC 791 for a complete
// description of each.
//
#define IP_OPT_EOL      0          // End of list option
#define IP_OPT_NOP      1          // No operation
#define IP_OPT_SECURITY 0x82       // Security option
#define IP_OPT_LSRR     0x83       // Loose source route
#define IP_OPT_SSRR     0x89       // Strict source route
#define IP_OPT_RR       0x7        // Record route
#define IP_OPT_TS       0x44       // Timestamp
#define IP_OPT_SID      0x88       // Stream ID (obsolete)
#define IP_OPT_ROUTER_ALERT 0x94  // Router Alert Option

#define MAX_OPT_SIZE    40         // Maximum length of IP options in bytes

#ifdef CHICAGO

// Ioctls code exposed by Memphis tcpip stack.
// For NT these ioctls are define in ntddip.h  (private\inc)

#define IOCTL_IP_RTCHANGE_NOTIFY_REQUEST   101
#define IOCTL_IP_ADDCHANGE_NOTIFY_REQUEST  102
#define IOCTL_ARP_SEND_REQUEST             103
#define IOCTL_IP_INTERFACE_INFO            104
#define IOCTL_IP_GET_BEST_INTERFACE        105
#define IOCTL_IP_UNIDIRECTIONAL_ADAPTER_ADDRESS        106

#endif


#endif // IP_EXPORT_INCLUDED




/*++

Copyright (c) Microsoft Corporation. All rights reserved.

Module Name:

    iptypes.h

--*/

#ifndef IP_TYPES_INCLUDED
#define IP_TYPES_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma warning(disable:4201)

#include <time.h>

// Definitions and structures used by getnetworkparams and getadaptersinfo apis

#define MAX_ADAPTER_DESCRIPTION_LENGTH  128 // arb.
#define MAX_ADAPTER_NAME_LENGTH         256 // arb.
#define MAX_ADAPTER_ADDRESS_LENGTH      8   // arb.
#define DEFAULT_MINIMUM_ENTITIES        32  // arb.
#define MAX_HOSTNAME_LEN                128 // arb.
#define MAX_DOMAIN_NAME_LEN             128 // arb.
#define MAX_SCOPE_ID_LEN                256 // arb.

//
// types
//

// Node Type

#define BROADCAST_NODETYPE              1
#define PEER_TO_PEER_NODETYPE           2
#define MIXED_NODETYPE                  4
#define HYBRID_NODETYPE                 8

//
// IP_ADDRESS_STRING - store an IP address as a dotted decimal string
//

typedef struct {
    char String[4 * 4];
} IP_ADDRESS_STRING, *PIP_ADDRESS_STRING, IP_MASK_STRING, *PIP_MASK_STRING;

//
// IP_ADDR_STRING - store an IP address with its corresponding subnet mask,
// both as dotted decimal strings
//

typedef struct _IP_ADDR_STRING {
    struct _IP_ADDR_STRING* Next;
    IP_ADDRESS_STRING IpAddress;
    IP_MASK_STRING IpMask;
    DWORD Context;
} IP_ADDR_STRING, *PIP_ADDR_STRING;

//
// ADAPTER_INFO - per-adapter information. All IP addresses are stored as
// strings
//

typedef struct _IP_ADAPTER_INFO {
    struct _IP_ADAPTER_INFO* Next;
    DWORD ComboIndex;
    char AdapterName[MAX_ADAPTER_NAME_LENGTH + 4];
    char Description[MAX_ADAPTER_DESCRIPTION_LENGTH + 4];
    UINT AddressLength;
    BYTE Address[MAX_ADAPTER_ADDRESS_LENGTH];
    DWORD Index;
    UINT Type;
    UINT DhcpEnabled;
    PIP_ADDR_STRING CurrentIpAddress;
    IP_ADDR_STRING IpAddressList;
    IP_ADDR_STRING GatewayList;
    IP_ADDR_STRING DhcpServer;
    BOOL HaveWins;
    IP_ADDR_STRING PrimaryWinsServer;
    IP_ADDR_STRING SecondaryWinsServer;
    time_t LeaseObtained;
    time_t LeaseExpires;
} IP_ADAPTER_INFO, *PIP_ADAPTER_INFO;

#ifdef _WINSOCK2API_

//
// The following types require Winsock2.
//

typedef enum {
    IpPrefixOriginOther = 0,
    IpPrefixOriginManual,
    IpPrefixOriginWellKnown,
    IpPrefixOriginDhcp,
    IpPrefixOriginRouterAdvertisement,
} IP_PREFIX_ORIGIN;

typedef enum {
    IpSuffixOriginOther = 0,
    IpSuffixOriginManual,
    IpSuffixOriginWellKnown,
    IpSuffixOriginDhcp,
    IpSuffixOriginLinkLayerAddress,
    IpSuffixOriginRandom,
} IP_SUFFIX_ORIGIN;

typedef enum {
    IpDadStateInvalid    = 0,
    IpDadStateTentative,
    IpDadStateDuplicate,
    IpDadStateDeprecated,
    IpDadStatePreferred,
} IP_DAD_STATE;

typedef struct _IP_ADAPTER_UNICAST_ADDRESS {
    union {
        ULONGLONG Alignment;
        struct { 
            ULONG Length;
            DWORD Flags;
        };
    };
    struct _IP_ADAPTER_UNICAST_ADDRESS *Next;
    SOCKET_ADDRESS Address;

    IP_PREFIX_ORIGIN PrefixOrigin;
    IP_SUFFIX_ORIGIN SuffixOrigin;
    IP_DAD_STATE DadState;

    ULONG ValidLifetime;
    ULONG PreferredLifetime;
    ULONG LeaseLifetime;
} IP_ADAPTER_UNICAST_ADDRESS, *PIP_ADAPTER_UNICAST_ADDRESS;

typedef struct _IP_ADAPTER_ANYCAST_ADDRESS {
    union {
        ULONGLONG Alignment;
        struct { 
            ULONG Length;
            DWORD Flags;
        };
    };
    struct _IP_ADAPTER_ANYCAST_ADDRESS *Next;
    SOCKET_ADDRESS Address;
} IP_ADAPTER_ANYCAST_ADDRESS, *PIP_ADAPTER_ANYCAST_ADDRESS;

typedef struct _IP_ADAPTER_MULTICAST_ADDRESS {
    union {
        ULONGLONG Alignment;
        struct {
            ULONG Length;
            DWORD Flags;
        };
    };
    struct _IP_ADAPTER_MULTICAST_ADDRESS *Next;
    SOCKET_ADDRESS Address;
} IP_ADAPTER_MULTICAST_ADDRESS, *PIP_ADAPTER_MULTICAST_ADDRESS;

//
// Per-address Flags
//
#define IP_ADAPTER_ADDRESS_DNS_ELIGIBLE 0x01
#define IP_ADAPTER_ADDRESS_TRANSIENT    0x02

typedef struct _IP_ADAPTER_DNS_SERVER_ADDRESS {
    union {
        ULONGLONG Alignment;
        struct {
            ULONG Length;
            DWORD Reserved;
        };
    };
    struct _IP_ADAPTER_DNS_SERVER_ADDRESS *Next;
    SOCKET_ADDRESS Address;
} IP_ADAPTER_DNS_SERVER_ADDRESS, *PIP_ADAPTER_DNS_SERVER_ADDRESS;

//
// Per-adapter Flags
//
#define IP_ADAPTER_DDNS_ENABLED            0x01
#define IP_ADAPTER_REGISTER_ADAPTER_SUFFIX 0x02
#define IP_ADAPTER_DHCP_ENABLED            0x04

//
// OperStatus values from RFC 2863
//
typedef enum {
    IfOperStatusUp = 1,
    IfOperStatusDown,
    IfOperStatusTesting,
    IfOperStatusUnknown,
    IfOperStatusDormant,
    IfOperStatusNotPresent,
    IfOperStatusLowerLayerDown
} IF_OPER_STATUS;

typedef struct _IP_ADAPTER_ADDRESSES {
    union {
        ULONGLONG Alignment;
        struct {
            ULONG Length;
            DWORD IfIndex;
        };
    };
    struct _IP_ADAPTER_ADDRESSES *Next;
    PCHAR AdapterName;
    PIP_ADAPTER_UNICAST_ADDRESS FirstUnicastAddress;
    PIP_ADAPTER_ANYCAST_ADDRESS FirstAnycastAddress;
    PIP_ADAPTER_MULTICAST_ADDRESS FirstMulticastAddress;
    PIP_ADAPTER_DNS_SERVER_ADDRESS FirstDnsServerAddress;
    PWCHAR DnsSuffix;
    PWCHAR Description;
    PWCHAR FriendlyName;
    BYTE PhysicalAddress[MAX_ADAPTER_ADDRESS_LENGTH];
    DWORD PhysicalAddressLength;
    DWORD Flags;
    DWORD Mtu;
    DWORD IfType;
    IF_OPER_STATUS OperStatus;
} IP_ADAPTER_ADDRESSES, *PIP_ADAPTER_ADDRESSES;

//
// Flags used as argument to GetAdaptersAddresses()
//
#define GAA_FLAG_SKIP_UNICAST      0x0001
#define GAA_FLAG_SKIP_ANYCAST      0x0002
#define GAA_FLAG_SKIP_MULTICAST    0x0004
#define GAA_FLAG_SKIP_DNS_SERVER   0x0008

#endif /* _WINSOCK2API_ */

//
// IP_PER_ADAPTER_INFO - per-adapter IP information such as DNS server list.
//

typedef struct _IP_PER_ADAPTER_INFO {
    UINT AutoconfigEnabled;
    UINT AutoconfigActive;
    PIP_ADDR_STRING CurrentDnsServer;
    IP_ADDR_STRING DnsServerList;
} IP_PER_ADAPTER_INFO, *PIP_PER_ADAPTER_INFO;

//
// FIXED_INFO - the set of IP-related information which does not depend on DHCP
//

typedef struct {
    char HostName[MAX_HOSTNAME_LEN + 4] ;
    char DomainName[MAX_DOMAIN_NAME_LEN + 4];
    PIP_ADDR_STRING CurrentDnsServer;
    IP_ADDR_STRING DnsServerList;
    UINT NodeType;
    char ScopeId[MAX_SCOPE_ID_LEN + 4];
    UINT EnableRouting;
    UINT EnableProxy;
    UINT EnableDns;
} FIXED_INFO, *PFIXED_INFO;

#pragma warning(default:4201)

#ifdef __cplusplus
}
#endif

#endif



/*++

Copyright (c) Microsoft Corporation. All rights reserved.

Module Name:

    iphlpapi.h

Abstract:
    Header file for functions to interact with the IP Stack for MIB-II and
    related functionality

--*/

#ifndef __IPHLPAPI_H__
#define __IPHLPAPI_H__

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// IPRTRMIB.H has the definitions of the strcutures used to set and get     //
// information                                                              //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#include <iprtrmib.h>

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// The GetXXXTable APIs take a buffer and a size of buffer.  If the buffer  //
// is not large enough, the APIs return ERROR_INSUFFICIENT_BUFFER  and      //
// *pdwSize is the required buffer size                                     //
// The bOrder is a BOOLEAN, which if TRUE sorts the table according to      //
// MIB-II (RFC XXXX)                                                        //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Retrieves the number of interfaces in the system. These include LAN and  //
// WAN interfaces                                                           //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


DWORD
WINAPI
GetNumberOfInterfaces(
    OUT PDWORD  pdwNumIf
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Gets the MIB-II ifEntry                                                  //
// The dwIndex field of the MIB_IFROW should be set to the index of the     //
// interface being queried                                                  //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
GetIfEntry(
    IN OUT PMIB_IFROW   pIfRow
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Gets the MIB-II IfTable                                                  //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
GetIfTable(
    OUT    PMIB_IFTABLE pIfTable,
    IN OUT PULONG       pdwSize,
    IN     BOOL         bOrder
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Gets the Interface to IP Address mapping                                 //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
GetIpAddrTable(
    OUT    PMIB_IPADDRTABLE pIpAddrTable,
    IN OUT PULONG           pdwSize,
    IN     BOOL             bOrder
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Gets the current IP Address to Physical Address (ARP) mapping            //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
GetIpNetTable(
    OUT    PMIB_IPNETTABLE pIpNetTable,
    IN OUT PULONG          pdwSize,
    IN     BOOL            bOrder
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Gets the IP Routing Table  (RFX XXXX)                                    //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
GetIpForwardTable(
    OUT    PMIB_IPFORWARDTABLE pIpForwardTable,
    IN OUT PULONG              pdwSize,
    IN     BOOL                bOrder
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Gets TCP Connection/UDP Listener Table                                   //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
GetTcpTable(
    OUT    PMIB_TCPTABLE pTcpTable,
    IN OUT PDWORD        pdwSize,
    IN     BOOL          bOrder
    );

DWORD
WINAPI
GetUdpTable(
    OUT    PMIB_UDPTABLE pUdpTable,
    IN OUT PDWORD        pdwSize,
    IN     BOOL          bOrder
    );


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Gets IP/ICMP/TCP/UDP Statistics                                          //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
GetIpStatistics(
    OUT  PMIB_IPSTATS   pStats
    );

DWORD
WINAPI
GetIpStatisticsEx(
    OUT  PMIB_IPSTATS   pStats,
    IN   DWORD          dwFamily
    );

DWORD
WINAPI
GetIcmpStatistics(
    OUT PMIB_ICMP   pStats
    );


DWORD
WINAPI
GetTcpStatistics(
    OUT PMIB_TCPSTATS   pStats
    );

DWORD
WINAPI
GetTcpStatisticsEx(
    OUT PMIB_TCPSTATS   pStats,
    IN  DWORD           dwFamily
    );

DWORD
WINAPI
GetUdpStatistics(
    OUT PMIB_UDPSTATS   pStats
    );

DWORD
WINAPI
GetUdpStatisticsEx(
    OUT PMIB_UDPSTATS   pStats,
    IN  DWORD           dwFamily
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Used to set the ifAdminStatus on an interface.  The only fields of the   //
// MIB_IFROW that are relevant are the dwIndex (index of the interface      //
// whose status needs to be set) and the dwAdminStatus which can be either  //
// MIB_IF_ADMIN_STATUS_UP or MIB_IF_ADMIN_STATUS_DOWN                       //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
SetIfEntry(
    IN PMIB_IFROW pIfRow
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Used to create, modify or delete a route.  In all cases the              //
// dwForwardIfIndex, dwForwardDest, dwForwardMask, dwForwardNextHop and     //
// dwForwardPolicy MUST BE SPECIFIED. Currently dwForwardPolicy is unused   //
// and MUST BE 0.                                                           //
// For a set, the complete MIB_IPFORWARDROW structure must be specified     //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
CreateIpForwardEntry(
    IN PMIB_IPFORWARDROW pRoute
    );

DWORD
WINAPI
SetIpForwardEntry(
    IN PMIB_IPFORWARDROW pRoute
    );

DWORD
WINAPI
DeleteIpForwardEntry(
    IN PMIB_IPFORWARDROW pRoute
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Used to set the ipForwarding to ON or OFF (currently only ON->OFF is     //
// allowed) and to set the defaultTTL.  If only one of the fields needs to  //
// be modified and the other needs to be the same as before the other field //
// needs to be set to MIB_USE_CURRENT_TTL or MIB_USE_CURRENT_FORWARDING as  //
// the case may be                                                          //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


DWORD
WINAPI
SetIpStatistics(
    IN PMIB_IPSTATS pIpStats
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Used to set the defaultTTL.                                              //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
SetIpTTL(
    UINT nTTL
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Used to create, modify or delete an ARP entry.  In all cases the dwIndex //
// dwAddr field MUST BE SPECIFIED.                                          //
// For a set, the complete MIB_IPNETROW structure must be specified         //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
CreateIpNetEntry(
    IN PMIB_IPNETROW    pArpEntry
    );

DWORD
WINAPI
SetIpNetEntry(
    IN PMIB_IPNETROW    pArpEntry
    );

DWORD
WINAPI
DeleteIpNetEntry(
    IN PMIB_IPNETROW    pArpEntry
    );

DWORD
WINAPI
FlushIpNetTable(
    IN DWORD   dwIfIndex
    );


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Used to create or delete a Proxy ARP entry. The dwIndex is the index of  //
// the interface on which to PARP for the dwAddress.  If the interface is   //
// of a type that doesnt support ARP, e.g. PPP, then the call will fail     //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
CreateProxyArpEntry(
    IN  DWORD   dwAddress,
    IN  DWORD   dwMask,
    IN  DWORD   dwIfIndex
    );

DWORD
WINAPI
DeleteProxyArpEntry(
    IN  DWORD   dwAddress,
    IN  DWORD   dwMask,
    IN  DWORD   dwIfIndex
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Used to set the state of a TCP Connection. The only state that it can be //
// set to is MIB_TCP_STATE_DELETE_TCB.  The complete MIB_TCPROW structure   //
// MUST BE SPECIFIED                                                        //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
SetTcpEntry(
    IN PMIB_TCPROW pTcpRow
    );


DWORD
WINAPI
GetInterfaceInfo(
    IN PIP_INTERFACE_INFO pIfTable,
    OUT PULONG            dwOutBufLen
    );

DWORD
WINAPI
GetUniDirectionalAdapterInfo(OUT PIP_UNIDIRECTIONAL_ADAPTER_ADDRESS pIPIfInfo,
                 OUT PULONG dwOutBufLen
                 );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Gets the "best" outgoing interface for the specified destination address //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
GetBestInterface(
    IN  IPAddr  dwDestAddr,
    OUT PDWORD  pdwBestIfIndex
    );

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Gets the best (longest matching prefix) route for the given destination  //
// If the source address is also specified (i.e. is not 0x00000000), and    //
// there are multiple "best" routes to the given destination, the returned  //
// route will be one that goes out over the interface which has an address  //
// that matches the source address                                          //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

DWORD
WINAPI
GetBestRoute(
    IN  DWORD               dwDestAddr,
    IN  DWORD               dwSourceAddr, OPTIONAL
    OUT PMIB_IPFORWARDROW   pBestRoute
    );

DWORD
WINAPI
NotifyAddrChange(
    OUT PHANDLE      Handle,
    IN  LPOVERLAPPED overlapped
    );


DWORD
WINAPI
NotifyRouteChange(
    OUT PHANDLE      Handle,
    IN  LPOVERLAPPED overlapped
    );


DWORD
WINAPI
GetAdapterIndex(
    IN LPWSTR  AdapterName,
    OUT PULONG IfIndex
    );

DWORD
WINAPI
AddIPAddress(
    IPAddr  Address,
    IPMask  IpMask,
    DWORD   IfIndex,
    PULONG  NTEContext,
    PULONG  NTEInstance
    );

DWORD
WINAPI
DeleteIPAddress(
    ULONG NTEContext
    );

DWORD
WINAPI
GetNetworkParams(
    PFIXED_INFO pFixedInfo, PULONG pOutBufLen
    );

DWORD
WINAPI
GetAdaptersInfo(
    PIP_ADAPTER_INFO pAdapterInfo, PULONG pOutBufLen
    );

#ifdef _WINSOCK2API_

//
// The following functions require Winsock2.
//

DWORD
WINAPI
GetAdaptersAddresses(
    ULONG Family,
    DWORD Flags,
    PVOID Reserved,
    PIP_ADAPTER_ADDRESSES pAdapterAddresses, 
    PULONG pOutBufLen
    );

#endif

DWORD
WINAPI
GetPerAdapterInfo(
    ULONG IfIndex, PIP_PER_ADAPTER_INFO pPerAdapterInfo, PULONG pOutBufLen
    );

DWORD
WINAPI
IpReleaseAddress(
    PIP_ADAPTER_INDEX_MAP  AdapterInfo
    );


DWORD
WINAPI
IpRenewAddress(
    PIP_ADAPTER_INDEX_MAP  AdapterInfo
    );

DWORD
WINAPI
SendARP(
    IPAddr DestIP,
    IPAddr SrcIP,
    PULONG pMacAddr,
    PULONG  PhyAddrLen
    );

BOOL
WINAPI
GetRTTAndHopCount(
    IPAddr DestIpAddress,
    PULONG HopCount,
    ULONG  MaxHops,
    PULONG RTT
    );

DWORD
WINAPI
GetFriendlyIfIndex(
    DWORD IfIndex
    );

DWORD
WINAPI
EnableRouter(
    HANDLE* pHandle,
    OVERLAPPED* pOverlapped
    );

DWORD
WINAPI
UnenableRouter(
    OVERLAPPED* pOverlapped,
    LPDWORD lpdwEnableCount OPTIONAL
    );
DWORD
WINAPI
DisableMediaSense(
    HANDLE *pHandle,
    OVERLAPPED *pOverLapped
    );

DWORD
WINAPI
RestoreMediaSense(
    OVERLAPPED* pOverlapped,
    LPDWORD lpdwEnableCount OPTIONAL
    );

DWORD
WINAPI
GetIpErrorString(
    IN IP_STATUS ErrorCode,
    OUT PWCHAR Buffer,
    IN OUT PDWORD Size
    );

#ifdef __cplusplus
}
#endif

#endif //__IPHLPAPI_H__
