#include "common.h"

int main(int argc,char *argv[])
{		
	int begin_flag = 1;
	char pkt, sta, out;
	float timeused;
	int i;
	
	char *temp; 
	char *ctime();
	
	struct timeval now, tstart, tend, tmodify;
	 
	// file start time
	gettimeofday(&now,NULL);
	temp = ctime(&now);
	if(temp[strlen(temp)-1] != '\0')
	{
		temp[strlen(temp)-1] = '\0';
	}

	printf("Start time: %s\n",temp);
	
	// open the out file and write the header of xml
	out = out_open(OUT);	
	//header(out, temp);

	// open teh proc file
	pkt = in_open(DEV);
	sta = in_open(WIRELESS);
	
	retry_flag ath_per_old;
	for(i=0;i<MAX_LINE;i++)
	{
		ath_per_old.delt[i] = 0;
		ath_per_old.num[i][0] = 0;
		ath_per_old.num[i][1] = 0;
	}
	retry_flag ath_per;
	
	basic basic_info_old = proc(pkt, sta, out, INTERFACE);
	//close(pkt);
	//close(sta);	
		
	statistics stats_info_old = count(out);
	statistics stats_info;
	basic basic_info;
	
	//pktresult pkt_ratio;
				
	gettimeofday(&tstart, NULL);
		
	while(1)
	{				
		//basic basic_info = proc(pkt, sta, out, "wlan0");	
		
		pkt = in_open(DEV);
		sta = in_open(WIRELESS);
		
		// get statistics
		stats_info = count(out);		
			
		// pkt_retry ratio
			if(begin_flag == 1)
			{
				tend = tstart;
				begin_flag = 0;
			}
			else
				gettimeofday(&tend, NULL);
			
			// time stamp in microsecond
			timeused = (tend.tv_sec-tstart.tv_sec)+0.000001*(tend.tv_usec-tstart.tv_usec);		
			//timeused = tend.tv_sec+0.000001*tend.tv_usec;		
						
			// get basic statistics
			basic_info = proc(pkt, sta, out, INTERFACE);	
			
			// get packet delivery ratio
			//pkt_ratio = delivery(out, stats_info_old, stats_info, basic_info_old, basic_info, timeused);
			
			ath_per = rate_stats(out, ath_per_old, stats_info_old, stats_info, basic_info_old, basic_info, timeused);
			
			// save the results to xml
			//save_to_xml(out, stats_info, basic_info, timeused);
			//result_save(out, pkt_ratio, timeused);
		
			if(ath_per.pdrflag == 1)
			{
				stats_info_old = stats_info;
				basic_info_old = basic_info;
			}
			for(i=0; i<MAX_LINE; i++)
			{				
				if(ath_per.flag[i] == 1)
				{
					ath_per_old.delt[i] = ath_per.delt[i];
					ath_per_old.num[i][0] = ath_per.num[i][0];
					ath_per_old.num[i][1] = ath_per.num[i][1];
				}
			}
			
		usleep(1000*1000);
						
		close(pkt);
		close(sta);			
	}	
	
	gettimeofday(&tmodify,NULL);
	temp = ctime(&tmodify);
	if(temp[strlen(temp)-1] != '\0')
	{
		temp[strlen(temp)-1] = '\0';
	}
	
	//end(out, temp);
	
	//statistics stats_info = count(out);

	return 0;
}
