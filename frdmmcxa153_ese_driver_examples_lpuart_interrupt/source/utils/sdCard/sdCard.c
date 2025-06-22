/*
 * sdCard.c
 *
 *  Created on: 18 May 2025
 *      Author: henri
 */
#include "../source/utils/sdCard/FatFs/ff.h"		/* Declarations of FatFs API */
#include "../source/utils/sdCard/sdCard.h"		/* Declarations of FatFs API */
#include "../source/utils/comProtocols/Lpuart/lpuart0_interrupt.h"
#include "../source/utils/comProtocols/Lpuart/lpuart2_interrupt.h"
#include "../source/utils/gps/gps.h"
#include <stdlib.h>
#include <stdio.h>

int u = 1;
FATFS FatFs;    // File system object
FIL Fil;        // File object
UINT bw, br;    // bw = bytes written, br = bytes read
FRESULT fr;
//char readBuf[512];  // Buffer to read data into
char settingsReadBuf[512];  // Buffer to read data into


void sdInit()
{
	fr = f_mount(&FatFs, "", 0);		// Give a work area to the default drive
	    	    	if (fr != FR_OK) {
	    	    		    // Handle mount failure
	    	    		    printf("Mount failed with error code %d\n", fr);
	    	    		    while (1);
	    	    		}
	    	    	f_unlink(PATHNAME);

}
void sdWrite()
{
	fr = f_open(&Fil, SETFILE, FA_WRITE | FA_OPEN_ALWAYS);	//Create a file
	printf("%d",fr);
	    	    	if (fr == FR_OK) {
	        	    	if (fr == FR_OK) {
	        	    		f_write(&Fil, "11.1,N,12,E,13.1,N,12,E,11.1,N,12,E,11.1,N,12,E\r\n",16  , &bw);	// Write data to the fil
	        	    		f_printf(&Fil, " %d \n\r", u);
	    	    		}
	    	    		fr = f_close(&Fil);
	    	    	}
}
void sdSettings()
{
fr = f_open(&Fil, SETFILE, FA_WRITE | FA_OPEN_ALWAYS );//| FA_OPEN_APPEND);	//Create a file
if (fr == FR_OK) {
	        	    	if (fr == FR_OK) {
	        	    		int done = 0;
	        	    		while(!done)
	        	    	{
	        	    		    			char c = lpuart0_getchar();
	        	    		    			if(c == '#') //handshake by laptop to declare its done
	        	    		    			{done = 1;}
	        	    		    			else{
	        	    		    			f_printf("%c,", c);
	        	    		    			}
	        	    		    			//needs to print 4 locations
	    	    		}
	    	    		fr = f_close(&Fil);
	    	    	}
}
}
void sdReadSettings(coordinates_t* target, int level)
{
	disableLpuart2();
	char buf[512];
	sdBufWrite(SETFILE,buf,sizeof(buf)/sizeof(char));
    char *ptr = &buf[0];

    char *token; // Tokenize the NMEA sentence
    int field = 0;
    token = strsep(&ptr, ",");
    while ((token) != NULL) {


    	if (field ==  0 + (level * 4)) {
    	    target->lat = atof(token);
    	} else if (field == 1 + (level * 4 )) {
    	    target->latDir = token[0];
    	} else if (field == 2 + (level * 4)) {
    	    target->lon = atof(token);
    	} else if (field == 3 + (level * 4)) {
    	    target->lonDir = token[0];
    	}

		    	    			        //field 6:valid fix, field 7: number of satellites, field 8 horizontal dillution, field 9: altitude, field 10: geoidal seperation magnitude, field 11:geoidal seperation unit, field 12: differential GPS, field 13: stuff for error detection
	field++;
	token = strsep(&ptr, ",");
	}
    enableLpuart2();
}
void sdLog(uint8_t temp, coordinates_t* pos)
{
	fr = f_open(&Fil, LOGFILE, FA_WRITE | FA_OPEN_ALWAYS | FA_OPEN_APPEND);	//Create a file
	    	    	if (fr == FR_OK) {
	    	    		fr = f_lseek(&Fil, f_size(&Fil));
	        	    	if (fr == FR_OK) {
	        	    		f_printf(&Fil, "%lf,%c,%lf,%c,%d\n\r", pos->lat,pos->latDir,pos->lon, pos->lonDir, temp);
	    	    		}
	    	    		fr = f_close(&Fil);
	    	    	}
}
void sdRead(char* filename)
{
	char readBuf[512];  // Buffer to read data into
	fr = f_open(&Fil, filename, FA_READ);
	int amount = f_size(&Fil) / sizeof(readBuf);
	amount++;
	if (fr == FR_OK) {
		  for(int u = 0; u < amount; u++)
		    	    		{
		  do
		  {
		  f_read(&Fil,readBuf,sizeof(readBuf) - 1, &br );
		  for (int i = 0; i<br; i++)
		  {
		    printf("%c", readBuf[i]);
		  }

		  }while (br == sizeof(readBuf));
		  }
		  printf("\n");
		  f_close(&Fil);
	}
}
void sdReadLogs()
{
	sdRead(LOGFILE);
}



void sdBufWrite(char *filename, char *buffer, int size) {
	fr = f_open(&Fil, filename, FA_READ);


	if (fr == FR_OK) {
		f_read(&Fil, buffer, size-1, &br);
		buffer[br] = '\0';
		//printf("%s",buffer);
		f_close(&Fil);
	}
}
