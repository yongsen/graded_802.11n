#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#define MAX 1024
#define FILE_LEN 64
#define MAX_LINE 128
#define LINE 8
#define WINDOW 20
#define ATMP_WINDOW 10
#define MCS_idx 08
#define DEV "/proc/net/dev"
#define WIRELESS "/proc/net/wireless"
#define INTERFACE "wlan2"
#define ACKFailure "/sys/kernel/debug/ieee80211/phy0/statistics/dot11ACKFailureCount"
#define FCSError "/sys/kernel/debug/ieee80211/phy0/statistics/dot11FCSErrorCount"
#define RTSFailure "/sys/kernel/debug/ieee80211/phy0/statistics/dot11RTSFailureCount"
#define RTSSuccess "/sys/kernel/debug/ieee80211/phy0/statistics/dot11RTSSuccessCount"
#define Failed "/sys/kernel/debug/ieee80211/phy0/statistics/failed_count"
#define Frame_dup "/sys/kernel/debug/ieee80211/phy0/statistics/frame_duplicate_count"
#define Multi_r "/sys/kernel/debug/ieee80211/phy0/statistics/multicast_received_frame_count"
#define Multi_t "/sys/kernel/debug/ieee80211/phy0/statistics/multicast_transmitted_frame_count"
#define Multi_re "/sys/kernel/debug/ieee80211/phy0/statistics/multiple_retry_count"
#define Received_frag "/sys/kernel/debug/ieee80211/phy0/statistics/received_fragment_count"
#define Retry "/sys/kernel/debug/ieee80211/phy0/statistics/retry_count"
#define Transmitted_frag "/sys/kernel/debug/ieee80211/phy0/statistics/transmitted_fragment_count"
#define Transmitted_f "/sys/kernel/debug/ieee80211/phy0/statistics/transmitted_frame_count"
#define Recv "/sys/kernel/debug/ieee80211/phy0/ath9k/recv"
#define Xmit "/sys/kernel/debug/ieee80211/phy0/ath9k/xmit"
#define Dump "/sys/kernel/debug/ieee80211/phy0/ath9k/dump_nfcal"
#define MACsample "/sys/kernel/debug/ieee80211/phy0/ath9k/samples"
#define RC_STATS "/sys/kernel/debug/ieee80211/phy0/netdev:wlan2/stations/ec:17:2f:f2:bb:9d/rc_stats"
#define OUT "/home/smws/pktinfo2/data/out"
typedef struct 
{
	unsigned rbytes;
	unsigned rpackets;
	unsigned tbytes;
	unsigned tpackets;
	int level;
	int retry;
	int misc;
}basic;
typedef struct 
{
	unsigned ACK_F;
	unsigned long FCS_E;
	unsigned RTS_F;
	unsigned RTS_S;
	unsigned failed;
	unsigned frame_dup;
	unsigned multi_r;
	unsigned multi_t;
	unsigned multi_re;
	unsigned received_frag;
	unsigned retry;
	unsigned transmitted_frag;
	unsigned transmitted_f;
}statistics;
typedef struct
{
	char HT;
	char MCS;
	char rate;
	int success;
	int retries;
	int xretries; 
	int per;
}rc_stats;
typedef struct
{
	int flag;
	float pdr_failed;
	float pdr_retry;
	unsigned pkt_transmitted;
	unsigned pkt_failed;
	unsigned pkt_retry;	
}pktresult;
typedef struct
{
	int pdrflag;
	int flag[FILE_LEN];
	int delt[FILE_LEN];
	float pdr[8];
	float interval_t[8];
	int interval_c[8];
	int rssi[8];
	float thrupt[3];	
	unsigned num[FILE_LEN][2];
	unsigned error[FILE_LEN][2];
}retry_flag;
typedef struct
{
	int ctl[3];
	int ext[3];
}recv_rssi;
char in_open(char *packet);
char out_open(char *output);
basic proc(char pkt, char sta, char out, char *name);
statistics count(char out);
int header(char out, char *timestamp);
int timestamp(char out, float timestamp);
void save_to_xml(char out , statistics sta_info, basic basic_info, float timestamp);
pktresult delivery(char out, statistics sta_info_old, statistics sta_info, basic basic_info, basic basic_old_info, float timeused);
recv_rssi recv_err(char *input);
retry_flag rate_stats(char out, retry_flag per_old, statistics sta_info_old, statistics sta_info, basic basic_info_old, basic basic_info, float timeused);
void result_save(char out, pktresult pkt_ratio, float timestamp);
int end(char out, char *timestamp);
