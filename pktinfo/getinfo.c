#include "common.h"

/******************************************************************************************
* 1. General: Open -> Extract -> Convert -> Save: 
* 	(1) count()
* 		/sys/kernel/debug/ieee80211/phy0/statistics
*  
* 2. Special: Open -> Extract -> Judge -> Convert -> Save:
* 	(1) rc_stats()
* 		/sys/kernel/debug/ieee80211/phy0/netdev:wlan0/stations/f4:ec:38:49:f5:aa/rc_stats
* 
* 	(2) recv()
* 		/sys/kernel/debug/ieee80211/phy0/ath9k/recv
* 
* 	(3) wireless() 
* 		/proc/net/wireless
* 		/proc/net/dev 
*********************************************************************************************/ 

char in_open(char *packet)
{
	char pkt;
	
	if((pkt=open(packet,O_RDONLY))==-1)
	{
		perror("fail to open");
		exit(1);
	}
	
	return pkt;
}

char out_open(char *output)
{ 
	char out;
	
	if((out=open(output, O_WRONLY | O_APPEND | O_CREAT))==-1)
	{
		perror("fail to open OUT");
		exit(1);
	}
	return out;
}

basic proc(char pkt, char sta, char out, char *name)
{
	char buf[MAX];
	char pkt_buf[MAX]="\0";
	char sta_buf[MAX]="\0";
	
	int m,n;
	int i=0;
	int j=0;
		
	char *p;
	char *s;
	char *pkt_info[20];
	char *sta_info[20];
	
	char *temp;
			
	// read input1 /proc/net/dev
	while((m=read(pkt,buf,MAX))>0)
	{		
		// split to one line
		buf[m]='\0';
		p=strstr(buf, name);
		s=strstr(p,"\n");
		m=s-p+1;
		strncpy(pkt_buf,p,m);
		
		// split to one word
		temp = pkt_buf;
		while((pkt_info[i]=strtok(temp, " ")) != NULL)
		{
			temp = NULL;
			i++;
		}				
	}
	
	// read input2 /proc/net/wireless
	while((n=read(sta,buf,MAX))>0)
	{		
		// split to one line
		buf[n]='\0';
		p=strstr(buf, name);
		s=strstr(p,"\n");
		n=s-p+1;
		strncpy(sta_buf,p,n);
		
		// split to one word
		temp = sta_buf;
		while((sta_info[j]=strtok(temp, " ")) != NULL)
		{
			temp = NULL;
			j++;
		}				
	}
	
	basic basic_info = {atoi(pkt_info[1]), atoi(pkt_info[2]), atoi(pkt_info[9]), atoi(pkt_info[10]), atoi(sta_info[3]), atoi(sta_info[8]), atoi(sta_info[9])};
					
	if((m < 0) | (n < 0))
	{
		perror("fail to creat");
		exit(1);
	}	

	return basic_info;
}

int header(char out, char *timestamp)
{
	char buf[MAX_LINE];	

	//sprintf(buf, "<Packet_info at the begin time of \"%s\">\n", timestamp);
	sprintf(buf, "<Packet_info>\n");
	write(out, buf, strlen(buf));
		
	sprintf(buf, "<fileinfo>\n<creattime>%s</creattime>\n</fileinfo>\n", timestamp);
	write(out, buf, strlen(buf));
	
	return 0;		
}

statistics count(char out)
{
	char buf[13][MAX];
	
	int m, n;
	
	statistics sta_info;
	
	// open and read 
	// ACKFailureCount
	if((m=open(ACKFailure,O_RDONLY))==-1)
	{
		perror("fail to open");
		exit(1);
	}
	while((n=read(m,buf[0],MAX))>0)
	{	
		if(buf[0][strlen(buf[0])-1] != '\0')
		{
			buf[0][strlen(buf[0])-1] = '\0';
		}
		//sprintf(xmlbuf, "<ACKFailure>%s</ACKFailure>\n", buf[0]);	
		//write(out, xmlbuf, strlen(xmlbuf));			
	}
	close(m);
	
	// FCSErrorCount
	if((m=open(FCSError,O_RDONLY))==-1)
	{
		perror("fail to open");
		exit(1);
	}
	while((n=read(m,buf[1],MAX))>0)
	{	
		if(buf[1][strlen(buf[1])-1] != '\0')
		{
			buf[1][strlen(buf[1])-1] = '\0';
		}
		//sprintf(xmlbuf, "<FCSError>%s</FCSError>\n", buf[1]);	
		//write(out, xmlbuf, strlen(xmlbuf));				
	}
	close(m);
	
	// RTSFailureCount
	if((m=open(RTSFailure,O_RDONLY))==-1)
	{
		perror("fail to open");
		exit(1);
	}
	while((n=read(m,buf[2],MAX))>0)
	{		
		if(buf[2][strlen(buf[2])-1] != '\0')
		{
			buf[2][strlen(buf[2])-1] = '\0';
		}		
		//sprintf(xmlbuf, "<RTSFailure>%s</RTSFailure>\n", buf[2]);	
		//write(out, xmlbuf, strlen(xmlbuf));					
	}
	close(m);
	
	// RTSSuccessCount
	if((m=open(RTSSuccess,O_RDONLY))==-1)
	{
		perror("fail to open");
		exit(1);
	}
	while((n=read(m,buf[3],MAX))>0)
	{		
		if(buf[3][strlen(buf[3])-1] != '\0')
		{
			buf[3][strlen(buf[3])-1] = '\0';
		}
		//sprintf(xmlbuf, "<RTSSuccess>%s</RTSSuccess>\n", buf[3]);	
		//write(out, xmlbuf, strlen(xmlbuf));		
	}
	close(m);
	
	// failed_count
	if((m=open(Failed,O_RDONLY))==-1)
	{
		perror("fail to open");
		exit(1);
	}
	while((n=read(m,buf[4],MAX))>0)
	{	
		if(buf[4][strlen(buf[4])-1] != '\0')
		{
			buf[4][strlen(buf[4])-1] = '\0';
		}
		//sprintf(xmlbuf, "<failed>%s</failed>\n", buf[4]);	
		//write(out, xmlbuf, strlen(xmlbuf));		
	}
	close(m);
	
	// frame_duplicate_count
	if((m=open(Frame_dup,O_RDONLY))==-1)
	{
		perror("fail to open");
		exit(1);
	}
	while((n=read(m,buf[5],MAX))>0)
	{	
		if(buf[5][strlen(buf[5])-1] != '\0')
		{
			buf[5][strlen(buf[5])-1] = '\0';
		}
		//sprintf(xmlbuf, "<frame_dup>%s</frame_dup>\n", buf[5]);	
		//write(out, xmlbuf, strlen(xmlbuf));			
	}
	close(m);
	
	// multicast_received_frame_count
	if((m=open(Multi_r,O_RDONLY))==-1)
	{
		perror("fail to open");
		exit(1);
	}
	while((n=read(m,buf[6],MAX))>0)
	{		
		if(buf[6][strlen(buf[6])-1] != '\0')
		{
			buf[6][strlen(buf[6])-1] = '\0';
		}		
		//sprintf(xmlbuf, "<multi_r>%s</multi_r>\n", buf[6]);	
		//write(out, xmlbuf, strlen(xmlbuf));				
	}
	close(m);
	
	// multicast_transmitted_frame_count
	if((m=open(Multi_t,O_RDONLY))==-1)
	{
		perror("fail to open");
		exit(1);
	}
	while((n=read(m,buf[7],MAX))>0)
	{		
		if(buf[7][strlen(buf[7])-1] != '\0')
		{
			buf[7][strlen(buf[7])-1] = '\0';
		}	
		//sprintf(xmlbuf, "<multi_t>%s</multi_t>\n", buf[7]);	
		//write(out, xmlbuf, strlen(xmlbuf));	
	}
	close(m);
	
	// Multi_re
	if((m=open(Multi_re,O_RDONLY))==-1)
	{
		perror("fail to open8");
		exit(1);
	}
	while((n=read(m,buf[8],MAX))>0)
	{		
		if(buf[8][strlen(buf[8])-1] != '\0')
		{
			buf[8][strlen(buf[8])-1] = '\0';
		}
		//sprintf(xmlbuf, "<Multi_re>%s</Multi_re>\n", buf[8]);	
		//write(out, xmlbuf, strlen(xmlbuf));		
	}
	close(m);
	
	// Received_frag
	if((m=open(Received_frag,O_RDONLY))==-1)
	{
		perror("fail to open9");
		exit(1);
	}
	while((n=read(m,buf[9],MAX))>0)
	{	
		if(buf[9][strlen(buf[9])-1] != '\0')
		{
			buf[9][strlen(buf[9])-1] = '\0';
		}
		//sprintf(xmlbuf, "<Received_frag>%s</Received_frag>\n", buf[9]);	
		//write(out, xmlbuf, strlen(xmlbuf));					
	}
	close(m);
		
	// Retry
	if((m=open(Retry,O_RDONLY))==-1)
	{
		perror("fail to open10");
		exit(1);
	}
	while((n=read(m,buf[10],MAX))>0)
	{	
		if(buf[10][strlen(buf[10])-1] != '\0')
		{
			buf[10][strlen(buf[10])-1] = '\0';
		}
		//sprintf(xmlbuf, "<Retry>%s</Retry>\n", buf[10]);	
		//write(out, xmlbuf, strlen(xmlbuf));					
	}
	close(m);
	
	// Transmitted_frag
	if((m=open(Transmitted_frag,O_RDONLY))==-1)
	{
		perror("fail to open11");
		exit(1);
	}
	while((n=read(m,buf[11],MAX))>0)
	{		
		if(buf[11][strlen(buf[11])-1] != '\0')
		{
			buf[11][strlen(buf[11])-1] = '\0';
		}		
		//sprintf(xmlbuf, "<Transmitted_frag>%s</Transmitted_frag>\n", buf[11]);	
		//write(out, xmlbuf, strlen(xmlbuf));		
	}
	close(m);
	
	// transmitted_frame_count
	if((m=open(Transmitted_f,O_RDONLY))==-1)
	{
		perror("fail to open12");
		exit(1);
	}
	while((n=read(m,buf[12],MAX))>0)
	{		
		if(buf[12][strlen(buf[12])-1] != '\0')
		{
			buf[12][strlen(buf[12])-1] = '\0';
		}	
		//sprintf(xmlbuf, "<Transmitted_frame>%s</Transmitted_frame>\n", buf[12]);	
		//write(out, xmlbuf, strlen(xmlbuf));						
	}
	close(m);
	
	// Save to struct sta_info		
	sta_info.ACK_F = atoi(buf[0]);
	sta_info.FCS_E = atoi(buf[1]);	
	sta_info.RTS_F = atoi(buf[2]);
	sta_info.RTS_S = atoi(buf[3]);
	sta_info.failed = atoi(buf[4]);	
	sta_info.frame_dup = atoi(buf[5]);
	sta_info.multi_r = atoi(buf[6]);
	sta_info.multi_t = atoi(buf[7]);
	sta_info.multi_re = atoi(buf[8]);
	sta_info.received_frag = atoi(buf[9]);	
	sta_info.retry = atoi(buf[10]);	
	sta_info.transmitted_frag = atoi(buf[11]);		
	sta_info.transmitted_f = atoi(buf[12]);		
		
	return sta_info;
}

// Read by line
char *ReadData(FILE *fp, char *buf)
{
    return fgets(buf, MAX, fp);
}

recv_rssi recv_err(char *input)
{
	char *recv_errbuf;
	char *recv_err_info[42][3];
	unsigned recv_info[42][3];
	char *recv_errtemp;
	char *recv_errline = NULL;
				
	int k;	
	int m; 	
	int p = 0;
	int q = 0;
		
	FILE *recv_errfp;
	recv_rssi rssi;	
	
	int CSI_flag = 0;
	
	if(CSI_flag == 1)
	{
			
		if((recv_errfp = fopen(Recv, "r")) == NULL)
		{
			printf("CANNOT OPEN recv");
			exit(1);
		} 
		
		recv_errbuf = (char*)malloc(30*sizeof(char));
		recv_errline = ReadData(recv_errfp, recv_errbuf);
			
		while(recv_errline)
		{	
			k = strlen(recv_errbuf)-1;			
			recv_errbuf[k] = '\0';	
				
			// split to one word
			recv_errtemp = recv_errbuf;
								
				while((recv_err_info[p][q]=strtok(recv_errtemp, " ")) != NULL)
				{							
					recv_errtemp = NULL;	
					recv_info[p][q] = atoi(recv_err_info[p][q]);				
					//printf("p: %d\t, q: %d\t, %s", p, q, recv_err_info[p][q]);
					//printf("\n");
															
					q++;
				}			
				
			recv_errline = ReadData(recv_errfp, recv_errbuf);
			q = 0;
			p++;
		}
		
		//printf("RSSI-CTL: %d, %d, %d", recv_info[7][2], recv_info[8][2], recv_info[9][2]);	
		//printf("\n");
		//printf("RSSI-EXT: %d, %d, %d", recv_info[10][2], recv_info[11][2], recv_info[12][2]);	
		//printf("\n");
		
		for(m=0;m<3;m++)
		{
			rssi.ctl[m] = recv_info[m+7][2];
		
			rssi.ext[m] = recv_info[m+10][2];
		}
					
		fclose(recv_errfp);	
		
	}
	else
	{
		for(m=0;m<3;m++)
		{
			rssi.ctl[m] = 0;
		
			rssi.ext[m] = 0;
		}
	}	
		
	return rssi;		
}

retry_flag rate_stats(char out, retry_flag per_old, statistics sta_info_old, statistics sta_info, basic basic_info_old, basic basic_info, float timeused)
{
	char *buf;
	char *rc_info[16];
	char *temp;
	char *line = NULL;
	char outbuf[MAX_LINE];

	char per_out[MAX_LINE][LINE];
	
	retry_flag delt_retry;
		
 	int l, m;	
 	
	int i = 0;
	int j = 0;
		
	FILE *fp;
	
	/***********************************************************/
	// packet-level delivery ratio
	float pdr_failed;
	float pdr_retry;
	
	unsigned pkt_transmitted;
	unsigned pkt_failed;
	unsigned pkt_retry;
	
	unsigned pkt_transmitted1;	
	unsigned pkt_retry1;
	float pdr_retry1;
	
	//// write packet-level PDR to one file
	pkt_failed = sta_info.failed - sta_info_old.failed;
	pkt_transmitted = sta_info.transmitted_f - sta_info_old.transmitted_f;
	pkt_retry = sta_info.retry - sta_info_old.retry;
	
	pkt_transmitted1 = basic_info.tpackets - basic_info_old.tpackets;
	pkt_retry1 = basic_info.retry - basic_info_old.retry;
	
	if((fp = fopen(RC_STATS, "r")) == NULL)
	{
		printf("CANNOT OPEN RC_STATS");
		exit(1);
	} 
	
	buf = (char*)malloc(100*sizeof(char));
	line = ReadData(fp, buf);
	
	while(line)
	{	
		l = strlen(buf)-1;			
		buf[l] = '\0';	
			 
		// split to one word
		temp = buf;
		        
		// Save per to different file		
		sprintf(per_out[j], "/home/smws/pktinfo2/data/%2d", (j-1));
		
		while((rc_info[i]=strtok(temp, " ")) != NULL)
		{
			temp = NULL;
			i++;
		}
		     
		// number of attemps
		if(i == 10)
		{
			delt_retry.delt[j] = atoi(rc_info[i-1]);
			delt_retry.num[j][0] = atoi(rc_info[i-2]);
			delt_retry.num[j][1] = atoi(rc_info[i-1]);
			
			delt_retry.error[j][0] = atoi(rc_info[i-4]);
			delt_retry.error[j][1] = atoi(rc_info[i-3]);
			
			delt_retry.thrupt[0] = atoi(rc_info[i-7]);
			delt_retry.thrupt[1] = atoi(rc_info[i-6]);
			delt_retry.thrupt[2] = atoi(rc_info[i-5]);			
		}
		else if (i == 9)
		{
			delt_retry.num[j][0] = atoi(rc_info[i-2]);
			delt_retry.num[j][1] = atoi(rc_info[i-1]);
						
			delt_retry.error[j][0] = atoi(rc_info[i-4]);
			delt_retry.error[j][1] = atoi(rc_info[i-3]);
			
			if(delt_retry.error[j][1] >= 100)
			{			
				delt_retry.delt[j] = atoi(rc_info[i-1]);
				delt_retry.thrupt[0] = atoi(rc_info[i-6]);
				delt_retry.thrupt[1] = atoi(rc_info[i-5]);
				delt_retry.thrupt[2] = atoi(rc_info[i-4]);			
			}
		}
		else 
		{
			delt_retry.delt[j] = -1;
		}
			 			
		if((delt_retry.delt[j] > 0))
		{	

			if(((delt_retry.delt[j] - per_old.delt[j]) > ATMP_WINDOW) && (per_old.delt[j] > 0))
			{
				/************************************************************************/
			
				if(pkt_transmitted < 0)
				{
					sta_info_old = sta_info;
					delt_retry.pdrflag = 0;
				}
				else if((pkt_transmitted > 0) & (basic_info.level > -100))
				{
					pdr_failed = 1.0 - (float)(pkt_failed)/(pkt_transmitted);
					pdr_retry = 1.0 - (float)(pkt_retry)/(pkt_transmitted);
					pdr_retry1 = 1.0 - (float)(pkt_retry1)/(pkt_transmitted1);
					
					if((pdr_retry >= 0) && (pdr_retry <= 1))
					{		  
						delt_retry.pdrflag = 1;
					}
				}	
						
				/************************************************************************/
				
				// 2. frame level PDR to MCS file							
				printf("idx %d\t %d\t %f\t %f\t %f\n", j-1, basic_info.level, delt_retry.thrupt[0], delt_retry.thrupt[1], timeused);
										
				if((m=open(per_out[j], O_WRONLY | O_APPEND | O_CREAT))==-1)
					{
						perror("fail to open frame level PER_OUT");
						exit(1);
			    	}
						  
					sprintf(outbuf, "%2d\t%d\t%f\t%f\t", j-1, basic_info.level, delt_retry.thrupt[1], delt_retry.thrupt[0]);		
					write(m, outbuf, strlen(outbuf));
					write(out, outbuf, strlen(outbuf));
					 	
					sprintf(outbuf, "%f\t%d\t%f\t", delt_retry.thrupt[2], delt_retry.num[j][1]-per_old.num[j][1], delt_retry.thrupt[1]-per_old.thrupt[1]);	
					write(m, outbuf, strlen(outbuf));
					write(out, outbuf, strlen(outbuf));
					
					sprintf(outbuf, "%f\t%f\n", pdr_retry, timeused);	
					write(m, outbuf, strlen(outbuf));
					write(out, outbuf, strlen(outbuf));

					close(m);							
			}
			delt_retry.flag[j] = 1;			
		}	
		
		// read data of next line
		line = ReadData(fp, buf);
		i = 0;
		j++;
	}
	
	fclose(fp);
	per_old = delt_retry;
	return delt_retry;
}

void save_to_xml(char out , statistics sta_info, basic basic_info, float timestamp)
{
	char xmlbuf[MAX_LINE];
	
	//Save to xml
	sprintf(xmlbuf, "<rawData ID=\"%f\">\n", timestamp);
	write(out, xmlbuf, strlen(xmlbuf));
	
	// received				
	sprintf(xmlbuf, "<rbytes>%u</rbytes>\n", basic_info.rbytes);
	write(out, xmlbuf, strlen(xmlbuf));
	sprintf(xmlbuf, "<rpackets>%u</rpackets>\n", basic_info.rpackets);
	write(out, xmlbuf, strlen(xmlbuf));	
	
	//transmited			
	sprintf(xmlbuf, "<tbytes>%u</tbytes>\n", basic_info.tbytes);
	write(out, xmlbuf, strlen(xmlbuf));
	sprintf(xmlbuf, "<tpackets>%u</tpackets>\n", basic_info.tpackets);
	write(out, xmlbuf, strlen(xmlbuf));	
	
	// statistic			
	sprintf(xmlbuf, "<level>%d</level>\n", basic_info.level);
	write(out, xmlbuf, strlen(xmlbuf));
	sprintf(xmlbuf, "<retry>%d</retry>\n", basic_info.retry);
	write(out, xmlbuf, strlen(xmlbuf));
	sprintf(xmlbuf, "<misc>%d</misc>\n", basic_info.misc);
	write(out, xmlbuf, strlen(xmlbuf));			
			
	sprintf(xmlbuf, "<ACKFailure>%u</ACKFailure>\n", sta_info.ACK_F);	
	write(out, xmlbuf, strlen(xmlbuf));	
	sprintf(xmlbuf, "<FCSError>%lu</FCSError>\n", sta_info.FCS_E);	
	write(out, xmlbuf, strlen(xmlbuf));			
	sprintf(xmlbuf, "<RTSFailure>%u</RTSFailure>\n", sta_info.RTS_F);	
	write(out, xmlbuf, strlen(xmlbuf));	
	sprintf(xmlbuf, "<RTSSuccess>%u</RTSSuccess>\n", sta_info.RTS_S);	
	write(out, xmlbuf, strlen(xmlbuf));			
	sprintf(xmlbuf, "<failed>%u</failed>\n", sta_info.failed);	
	write(out, xmlbuf, strlen(xmlbuf));			
	sprintf(xmlbuf, "<frame_dup>%u</frame_dup>\n", sta_info.frame_dup);	
	write(out, xmlbuf, strlen(xmlbuf));			
	sprintf(xmlbuf, "<multi_r>%u</multi_r>\n", sta_info.multi_r);	
	write(out, xmlbuf, strlen(xmlbuf));		
	sprintf(xmlbuf, "<multi_t>%u</multi_t>\n", sta_info.multi_t);	
	write(out, xmlbuf, strlen(xmlbuf));			
	sprintf(xmlbuf, "<Multi_re>%u</Multi_re>\n", sta_info.multi_re);	
	write(out, xmlbuf, strlen(xmlbuf));		
	sprintf(xmlbuf, "<Received_frag>%u</Received_frag>\n", sta_info.received_frag);	
	write(out, xmlbuf, strlen(xmlbuf));
	sprintf(xmlbuf, "<Retry>%u</Retry>\n", sta_info.retry);	
	write(out, xmlbuf, strlen(xmlbuf));	
	sprintf(xmlbuf, "<Transmitted_frag>%u</Transmitted_frag>\n", sta_info.transmitted_frag);	
	write(out, xmlbuf, strlen(xmlbuf));
	sprintf(xmlbuf, "<Transmitted_frame>%u</Transmitted_frame>\n", sta_info.transmitted_f);	
	write(out, xmlbuf, strlen(xmlbuf));
	
	sprintf(xmlbuf, "</rawData>\n");
	write(out, xmlbuf, strlen(xmlbuf));	
}

pktresult delivery(char out, statistics sta_info_old, statistics sta_info, basic basic_info_old, basic basic_info, float timeused)
{	
	int flag;
	float pdr_failed;
	float pdr_retry;
	
	unsigned pkt_transmitted;
	unsigned pkt_failed;
	unsigned pkt_retry;
	
	unsigned pkt_transmitted1;	
	unsigned pkt_retry1;
	float pdr_retry1;
	
	pktresult pdr_result;
	
	//char xmlbuf[MAX_LINE];
		
	pkt_failed = sta_info.failed - sta_info_old.failed;
	pkt_transmitted = sta_info.transmitted_f - sta_info_old.transmitted_f;
	pkt_retry = sta_info.retry - sta_info_old.retry;
	
	pkt_transmitted1 = basic_info.tpackets - basic_info_old.tpackets;
	pkt_retry1 = basic_info.retry - basic_info_old.retry;
	
	if(pkt_transmitted < 0)
	{
		sta_info_old = sta_info;
		flag = 0;
	}
	else if((pkt_transmitted > WINDOW) & (basic_info.level > -100))
	{
		pdr_failed = 1.0 - (float)(pkt_failed)/(pkt_transmitted);
		pdr_retry = 1.0 - (float)(pkt_retry)/(pkt_transmitted);
		pdr_retry1 = 1.0 - (float)(pkt_retry1)/(pkt_transmitted1);
		
		if(pdr_failed >= 0)
		{
			printf("%d\t %f\t %f\t %f\t %f\n", basic_info.level, pdr_retry, pdr_failed, pdr_retry1, timeused);
			//printf("%u\t %u\t %u\t %u\t %u\t %f\n", pkt_transmitted, pkt_retry, pkt_failed, pkt_transmitted1, pkt_retry1, timeused);
			
			//sprintf(xmlbuf, "%d\t %f\t %f\t %f\t %f\n", basic_info.level, pdr_retry, pdr_failed, pdr_retry1, timeused);	
			//write(out, xmlbuf, strlen(xmlbuf));	
			
			flag = 1;
			
			pktresult pdr_result = {flag, pdr_failed, pdr_retry, pkt_transmitted, pkt_failed, pkt_retry};
			return pdr_result;			
		}
	}
	
	return pdr_result;
}

void result_save(char out, pktresult pkt_ratio, float timestamp)
{
	char xmlbuf[MAX_LINE];
	
	//Save to xml
	sprintf(xmlbuf, "<rawData ID=\"%f\">\n", timestamp);
	write(out, xmlbuf, strlen(xmlbuf));
	
	sprintf(xmlbuf, "<Packet Transmitted>%u</Packet Transmitted>\n", pkt_ratio.pkt_transmitted);
	write(out, xmlbuf, strlen(xmlbuf));
	
	sprintf(xmlbuf, "<Packet Failed>%u</Packet Failed>\n", pkt_ratio.pkt_failed);
	write(out, xmlbuf, strlen(xmlbuf));
	sprintf(xmlbuf, "<Packet Retry>%u</Packet Retry>\n", pkt_ratio.pkt_retry);
	write(out, xmlbuf, strlen(xmlbuf));	
	
	sprintf(xmlbuf, "<Packet Failed Ratio>%f</Packet Failed Ratio>\n", pkt_ratio.pdr_failed);
	write(out, xmlbuf, strlen(xmlbuf));
	sprintf(xmlbuf, "<Packet Retry Ratio>%f</Packet Retry Ratio>\n", pkt_ratio.pdr_retry);
	write(out, xmlbuf, strlen(xmlbuf));	
	
	sprintf(xmlbuf, "</rawData>\n");
	write(out, xmlbuf, strlen(xmlbuf));	
}

int end(char out, char *timestamp)
{
	char buf[MAX_LINE];		
	
	sprintf(buf, "<fileinfo>\n<modifytime>%s</modifytime>\n</fileinfo>\n", timestamp);
	write(out, buf, strlen(buf));
	
	sprintf(buf, "</Packet_info>\n");
	write(out, buf, strlen(buf));	
		
	close(out);
	return 0;	
}
