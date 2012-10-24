#include "common.h"

status getinfo(char* output)
{
	char input_X;
	char input_P;
	int m;
	int n;
	
	char buf[MAX];
	char *temp;	
	char *x_temp[MAX];	
	char *p_temp[MAX];
	
	status basic_info;
			
	int i = 0;
	int j = 0;
	
	// Open the input and output file
	
	if((input_X=open(X,O_RDONLY))==-1)
	{
		perror("fail to open DATA_X");
		exit(1);
	}
	
	if((input_P=open(P,O_RDONLY))==-1)
	{
		perror("fail to open DATA_P");
		exit(1);
	}
		
	//Read the input file
			
	while((m=read(input_X,buf,MAX))>0)
	{		
		// split to one line
		buf[m]='\0';	
		
		// split to one word
		temp = buf;
				
		while((x_temp[i]=strtok(temp, " ")) != NULL)
		{
			basic_info.x[i] = atoi(x_temp[i]);
			temp = NULL;			
			i++;	
		}			
	}
	
	while((n=read(input_P,buf,MAX))>0)
	{		
		// split to one line
		buf[n]='\0';	
		
		// split to one word
		temp = buf;
				
		while((p_temp[j]=strtok(temp, " ")) != NULL)
		{
			basic_info.p[j] = atof(p_temp[j]);
			temp = NULL;			
			j++;	
		}			
	}
	
	close(input_X);
	close(input_P);
	
	return basic_info;
}

// Calculate PDR of fixed, weighted and sliding method
DSWA DSWA_calculate(status basic)	
{
	DSWA DSWA_info;
	
	// Weighting factor of interval
	float w[8] = {0.2,0.4,0.6,0.8,1,1,1,1,};
	float t[8] = {0.8,0.9,0.8,0.9,0.8,0.8,0.9,0.9};
	float p[8];
	int s[8] = {187,185,181,183,184,184,188,181};
	float beta;
	 
	unsigned Ws = 0;
	float Ps = 0.0;
	unsigned Sum = 0;
	
	int i = 0;
	int j = 0;
	int k = 0;
	
	char out;
	char outbuf[MIN];
	
	// Open the output file
	if((out=open(OUT, O_WRONLY | O_CREAT))==-1)
	{
		perror("fail to open OUT");
		exit(1);
	}
		
	p[0] = 1 + t[0];
	for(i = 1; i < 8; i++)
	{
		if((t[i] - t[i-1]) >= 0)
		{
			p[i] = 1 + (t[i]-t[i-1]) / t[i-1];
			//p[i] = 1 + (t[i]-t[i-1]);
		}
		else if((t[i] - t[i-1]) < 0)
		{
			p[i] = 0.98 + 0.98 * (t[i]-t[i-1]) / t[i-1];
			//p[i] = 1 + (t[i]-t[i-1]);
		}
	}
	
	DSWA_info.Ws[0] = WINDOW;
		
	for(i = 1; i < 10000; i++)
	{
		if(Ws < TOTAL)
		{
			// Window length
			for(j = 0; j < 8; j++)
			{
				Ps = Ps + w[j] * p[j] * s[j];
				beta = beta + w[j] * fabs(p[j] - 1);
			}
			
			DSWA_info.Ws[i] = (int)Ps / 6;
			beta = beta / 6;
			
			if(DSWA_info.Ws[i] < 5)
			{
				DSWA_info.Ws[i] = DSWA_info.Ws[i-1];
			}
			
			// Packet Delivery
			for(k = Ws; k < Ws + DSWA_info.Ws[i]; k++)
			{
				Sum = Sum + basic.x[k];
			}
			DSWA_info.Ps[i] = (float)Sum / DSWA_info.Ws[i];
					
			DSWA_info.Error[i] = DSWA_info.Ps[i] - basic.p[Ws + DSWA_info.Ws[i]];
			
			// Update the PDR[8]
			for(j = 0; j < 7; j++)
			{
				p[j] = p[j+1];
				s[j] = s[j+1];
			}
			
			if(DSWA_info.Ps[i-1] != 0)
			{
				if((DSWA_info.Ps[i] - DSWA_info.Ps[i-1]) >= 0)
				{
					p[j] = 1 + (DSWA_info.Ps[i] - DSWA_info.Ps[i-1]) / DSWA_info.Ps[i-1];
					//p[j] = 1 + (DSWA_info.Ps[i] - DSWA_info.Ps[i-1]);
				}
				else if((DSWA_info.Ps[i] - DSWA_info.Ps[i-1]) < 0)
				{
					p[j] = 0.98 + 0.98 * (DSWA_info.Ps[i] - DSWA_info.Ps[i-1]) / DSWA_info.Ps[i-1];
					//p[j] = 1 + (DSWA_info.Ps[i] - DSWA_info.Ps[i-1]);
				}
			}
			else if(DSWA_info.Ps[i-1] == 0)
			{
				p[j] = p[j-1];
			}

			s[j] = DSWA_info.Ws[i];
			
			printf("%d\t%d\t%f\t%f\t%f\t%f\n", Ws, DSWA_info.Ws[i], DSWA_info.Ps[i], DSWA_info.Error[i], beta, p[j]-1);
			
			sprintf(outbuf, "%d\t%d\t%f\t%f\t%f\t%f\n", Ws, DSWA_info.Ws[i], DSWA_info.Ps[i], DSWA_info.Error[i], beta, p[j]-1);	
			write(out, outbuf, strlen(outbuf));	
			
			Ws = Ws + DSWA_info.Ws[i];
			Ps = 0;
			Sum = 0;
		}
	}
	
	close(out);
	
	return DSWA_info;
}	

int main(int argc,char *argv[])
{
	status basicinfo;
	DSWA DSWAinfo;
	
	basicinfo = getinfo(OUT);
	DSWAinfo = DSWA_calculate(basicinfo);
	
	
	return 0;
}
