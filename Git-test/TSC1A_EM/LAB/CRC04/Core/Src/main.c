/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "fatfs_sd.h"
#include "ff.h"
#include "diskio.h"

#include <stdio.h>
#include "stm32F7xx_hal.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma location=0x2007c000
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
#pragma location=0x2007c0a0
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

#elif defined ( __CC_ARM )  /* MDK ARM Compiler */

__attribute__((at(0x2007c000))) ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
__attribute__((at(0x2007c0a0))) ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

#elif defined ( __GNUC__ ) /* GNU Compiler */
ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT] __attribute__((section(".RxDecripSection"))); /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT] __attribute__((section(".TxDecripSection")));   /* Ethernet Tx DMA Descriptors */

#endif

ETH_TxPacketConfig TxConfig;

CRC_HandleTypeDef hcrc;

ETH_HandleTypeDef heth;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ETH_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_SPI1_Init(void);
static void MX_CRC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

FATFS fs; // file system

FIL fil;// file
FRESULT fresult; // to storing result of each operation
char buffer[1024];// store the data which can read/write
char checkbuff[1024];

uint32_t crcValue;
char uartbuff[1024];


#define MAXFILE 1000
#define MAX_FILENAME_LENGTH 30

char Afilename[MAXFILE][MAX_FILENAME_LENGTH];
uint32_t Acrc[MAXFILE];

char Afilename_in_index[MAXFILE][MAX_FILENAME_LENGTH];

UINT br, bw; // file read/write count (counter)

// 0.0 capacity check var
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;


void send_uart (char *string){

	uint8_t len = strlen (string);

	//1.1.1 transmit in blocking mode
	// HAL_UART_Transmit(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout)
	HAL_UART_Transmit(&huart3,(uint8_t *) string, len , 2000);
}

//void find_txtfile (void)
void find_txtfile (const TCHAR* namsakhun){

	DIR dir; // directory
	FILINFO fno; // file info
	//path = ""; //list

	char surname[5];

	sprintf(surname,"*.%s",namsakhun);
	send_uart(surname);

	fresult = f_opendir(&dir, "");

		 if ( fresult != FR_OK){

			 sprintf(buffer, "fresult = %d f_opendir \r\n", fresult);
			 send_uart(buffer);

		 }
		 else if ( fresult == FR_OK){

			 do{

				 //fresult = f_findfirst(&dir, &fno, "", surname);
				 fresult = f_findfirst(&dir, &fno, "index", surname);


					 while( fresult == FR_OK && fno.fname[0] ){
						 sprintf(buffer,"\r %s \r\n",fno.fname);
						 send_uart(buffer);
						 fresult = f_findnext(&dir,&fno);

					 }
		 }while (fno.fname[0] != 0);
		 }
		 f_closedir(&dir);

}

// aka. �?ลัวชื่อไฟล์ซ้ำ
int is_dup_filename (char *filename, int count){
	//if (filename != "index.txt"){
	for (int k = 0; k <= count; k++) {
	        if (strcmp(Afilename[k], filename) == 0) {
	            return 1; // Duplicate found
	        }
	}return 0; // No duplicate

 	 //}
}

int is_filename_in_index (char *filename, int count_in_index){
	//if (filename != "index.txt"){
	for (int k = 0; k <= count_in_index ; k++) {
	        if (strcmp(Afilename_in_index[k], filename) == 0) {
	            return 1; // Duplicate found
	        }
	}return 0; // No duplicate

 	 //}
}

int check_filename_w_index (const char *filename){

	fresult = f_open(&fil,filename, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

	  if ( fresult == FR_OK){
		  send_uart("index.txt opened leaw ka :P \n");
	    		}
	  if ( fresult != FR_OK){

		  send_uart("Failed to open index file T_T \r\n");
		  return -1;
	  }

	  int count = 0;
	  //f_read(&fil , buffer, sizeof(buffer) , &br);
	  while (f_gets(Afilename_in_index[count], MAX_FILENAME_LENGTH, &fil)) {

		  Afilename_in_index[count][strcspn(Afilename_in_index[count], "\n")] = '\0';
		          count++;
		          if (count >= MAXFILE) break;

	  }

	  f_close(&fil);
}

uint8_t i = 0;
uint8_t j = 0;



void find_txtfile_w_crc3 (const TCHAR* namsakhun){

	DIR dir; // directory
	FILINFO fno; // file info


	char surname[5];

	sprintf(surname,"*.%s",namsakhun);
	send_uart(surname);

	fresult = f_opendir(&dir, "");

		 if ( fresult != FR_OK){

			 sprintf(buffer, "fresult = %d f_opendir \r\n", fresult);
			 send_uart(buffer);

		 }
		 else if ( fresult == FR_OK && fno.fname != "index.txt" ){

			 do{

				 	 	 fresult = f_findfirst(&dir, &fno, "", surname);

				 	 	 /* ********************************** */
						 while( fresult == FR_OK && fno.fname[0] ){



							 if (fresult != FR_OK || fno.fname[0] == 0) {
										 if (fno.fname[0] == 0) {
											 send_uart("No more files.\r\n");
										 } else {
											 sprintf(buffer, "Error finding first file: %d\r\n", fresult);
											 send_uart(buffer);
										 }
										 break;
									 }

							 if (is_dup_filename(fno.fname,i)== 0){

								 sprintf(buffer," %s \r\n",fno.fname);
								 send_uart(buffer);

								 if ( i < MAXFILE && i == j){
									 //strncpy( Afilename[i] , fno.fname , fno.fsize );

									 strncpy(Afilename[i], fno.fname, sizeof(Afilename[i]) - 1);
									 Afilename[i][sizeof(Afilename[i]) - 1] = '\0';


									 sprintf(buffer,"File name array = %s \r\n", Afilename[i]);
									 send_uart(buffer);
									 sprintf(buffer,"position i = %d \r\n", i);
									 send_uart(buffer);

									 i++ ;

								 } else {
										send_uart("Maximum file limit reached.\r\n");
										break;
									}

								 fresult = f_open(&fil, fno.fname, FA_READ);
								 crcValue = 0xFFFFFFFF;


								 while ((fresult = f_read(&fil, buffer, sizeof(buffer), &br)) == FR_OK && br > 0) {

									crcValue = HAL_CRC_Accumulate(&hcrc, (uint32_t *)buffer, br / sizeof(uint32_t));
								 }

								 crcValue ^= 0xFFFFFFFF;

								 /* 0x%08X : to transform DEC to HEX
								  *
								  * 0x ; hex format
								  * % ; format spec
								  * 08 ; padded w 0, min of 8 char
								  * X ; uppercase hexadecimal number
								  *
								  * */

								 sprintf(uartbuff, "CRC32: 0x%08X\r\n", crcValue);
								 send_uart(uartbuff);

								 if ( j < MAXFILE && j == i-1){


										Acrc[j] = crcValue;

										sprintf(buffer,"CRC array = %u \r\n", Acrc[j]);
										send_uart(buffer);
										sprintf(buffer,"position j = %d \r\n", j);
										send_uart(buffer);

										j++ ;

								} else {
									send_uart("Maximum file limit reached.\r\n");
									break;
								}
							 }
						 f_close(&fil);

						 fresult = f_findnext(&dir,&fno);

					 }


		 }while (fno.fname[0] != 0);
		 }

		 f_closedir(&dir);

}

char Cfilename[MAXFILE][MAX_FILENAME_LENGTH];
uint32_t Ccrc[MAXFILE];

int line[100];
int crc_buff;


void check_crc_from_index(void){

	int count = 0;
	FRESULT res;

	res = f_open(&fil, "index.txt", /*FA_OPEN_ALWAYS | */ FA_READ /*| FA_WRITE*/);
	if (res != FR_OK) {
					sprintf(buffer, "Error opening output file: %d\r\n", res);
					send_uart(buffer);
					return;
				}

	while(f_gets(buffer, sizeof(buffer) ,&fil)){
			send_uart(buffer);

			// remove \n from the end of a string
			buffer[strcspn(buffer, "\n")] = '\0';

			// split file name and crc
			char *Cfilename_ = strtok(buffer, ", ");
			char *Ccrc_ = strtok(NULL, ", ");

			// filename >.<
			if (Cfilename_ != NULL) {

						sprintf(checkbuff,"Cfilename_ : %s\r\n", Cfilename_); // suppose to show string
						send_uart(checkbuff);
			}else { send_uart("Error: filename not found\r\n");}

			// CRC >.<
			if (Ccrc_ != NULL) {

						sprintf(checkbuff,"Ccrc_ : %s\r\n", Ccrc_); // suppose to show string
						send_uart(checkbuff);

						// crc's buffer : string to hex
						crc_buff = (int) strtol(Ccrc_,NULL,16);
						sprintf(checkbuff,"%X\r\n", crc_buff);
						send_uart(checkbuff);

			}else { send_uart("Error: CRC value not found\r\n");}

			// add to the clone array
			if(Cfilename_&& Ccrc != NULL){

				strncpy(Cfilename[count], Cfilename_, MAX_FILENAME_LENGTH - 1);
				Cfilename[count][MAX_FILENAME_LENGTH - 1] = '\0'; //
				Ccrc[count] = crc_buff;

				count++;
			}

	}

	for (int i = 0; i < count; i++) {
			sprintf(buffer,"Name: %s, CRC: %d\r\n", Cfilename[i], Ccrc[i]);
			send_uart(buffer);
	}

	f_close(&fil);

	// compare array btw Afilename w Cfilename
	for (int i = 0; i <= count; i++) {
		for(int j = 0; j<= count; j++){
			if(Ccrc[i] == Acrc[j]){
				sprintf(buffer,"sump jaaaaa, Acrc = %d , Ccrc = %d \r\n",Acrc[j],Ccrc[i]);
				send_uart(buffer);
				break;

			}else if(Ccrc[i] != Acrc[j]){
				send_uart("no sump ka\r\n");
				if(j == count){
					sprintf(buffer,"still no sump jaaaaa, Ccrc = %d \r\n",Ccrc[i]);
					send_uart(buffer);
				}

			}
		}
	}


}

uint8_t hrs_r_gmt;
uint8_t min_r;
uint8_t sec_r;

uint8_t exact_date;
uint8_t exact_month;

char hrs_r_gmt_char[2];
char min_r_char[2];
char sec_r_char[2];

char exact_date_char[2];
char exact_month_char[2];

char timestamp_now[20];

void utc2rtc_ja(uint32_t UTC_now){

	// compare array btw Afilename w Cfilename
	// started date : 07-30-2024

	uint32_t m = 63072000; // year 1972 in UTC
	uint32_t fouryears_sec = 126230400; // all sec in 4 years

	//uint32_t UTC_now = 1722837758;

	// n = remaining sec started in 1972
	uint32_t n = UTC_now - m;

	sprintf(buffer,"n = %u \r\n",n);
	send_uart(buffer);

	double tt = (double)n / (double)fouryears_sec ; // tt = total round in 4 years

	// find remaining round
	double rr = (double)tt - (int)tt ;


	sprintf(buffer,"rr = %2f \r\n",rr);
	send_uart(buffer);

	//comparing w sec in 4 years
	uint32_t sec4y_rr = rr* fouryears_sec ;

	//total day remaining
	////// aka. convert sec4y_rr (sec) -> day
	double days4y_rr = (double)sec4y_rr / (double)(60*60*24) ;

	////////////////////////////////////////////////

	//decimal day remaining
	double decday4y_rr = (double)days4y_rr - (int)days4y_rr ;

	uint8_t GMT_7 = 7;


	////////////////////////////////////////////////

	//only day remaining
	uint32_t onlyday4y_rr = (int)days4y_rr ;

	uint16_t thisyear = 1972 + (4*(int)tt);

	///////////////////////////////////////////////

	// convert remaining days in 4year(decday4y_rr) to hour
	double hrs_r = decday4y_rr * 24.0;

	//// hour + GMT
	hrs_r_gmt = (int)hrs_r + GMT_7;

	// convert remaining days in 4year(decday4y_rr) to minute
	double min_rr = ((double)hrs_r - (int)hrs_r)*60 ;
	min_r = (int)min_rr;

	// convert remaining days in 4year(min_r) to sec
	sec_r = ((double)min_rr - (int)min_rr)*60 ;

	/////////////////
	// Aug 05, 2024 editing

	int a = onlyday4y_rr;;

	uint16_t days_in_months[12];


	if (thisyear % 4 == 0) {
	        // Leap year
	        days_in_months[0] = 31;  // January
	        days_in_months[1] = 29;  // February
	        days_in_months[2] = 31;  // March
	        days_in_months[3] = 30;  // April
	        days_in_months[4] = 31;  // May
	        days_in_months[5] = 30;  // June
	        days_in_months[6] = 31;  // July
	        days_in_months[7] = 31;  // August
	        days_in_months[8] = 30;  // September
	        days_in_months[9] = 31;  // October
	        days_in_months[10] = 30; // November
	        days_in_months[11] = 31; // December
	    } else {
	        // Non-leap year
	        days_in_months[0] = 31;  // January
	        days_in_months[1] = 28;  // February
	        days_in_months[2] = 31;  // March
	        days_in_months[3] = 30;  // April
	        days_in_months[4] = 31;  // May
	        days_in_months[5] = 30;  // June
	        days_in_months[6] = 31;  // July
	        days_in_months[7] = 31;  // August
	        days_in_months[8] = 30;  // September
	        days_in_months[9] = 31;  // October
	        days_in_months[10] = 30; // November
	        days_in_months[11] = 31; // December
	    }

	    // Iterate through months to find the exact month and date
	    for (uint8_t month = 0; month < 12; month++) {
	        if (a < days_in_months[month]) {
	            exact_date = a + 1;
	            exact_month = month + 1;
	            break ;

	        }
	        a -= days_in_months[month];
	    }

//	    uint8_t hrs_r_gmt;
//	    uint8_t min_r;
//	    uint8_t sec_r;
//
//	    uint8_t exact_date;
//	    uint8_t exact_month;

	    if(exact_month < 10){sprintf(exact_month_char,"0%u",exact_month);}
	    		else {sprintf(exact_month_char, "%u", exact_month);}

	    if(exact_date < 10){sprintf(exact_date_char,"0%u",exact_date);}
	    		else {sprintf(exact_date_char, "%u", exact_date);}

	    if(hrs_r_gmt < 10){sprintf(hrs_r_gmt_char,"0%u",hrs_r_gmt);}
	    		else {sprintf(hrs_r_gmt_char, "%u", hrs_r_gmt);}

	    if(min_r < 10){sprintf(min_r_char,"0%u",min_r);}
	    	    		else {sprintf(min_r_char, "%u", min_r);}

	    if(sec_r < 10){sprintf(sec_r_char,"0%u",sec_r);}
	    	    	    else {sprintf(sec_r_char, "%u", sec_r);}




	///////
	sprintf(buffer,"now : %u/%s/%s , %s:%s:%s \r\n",thisyear,exact_month_char,exact_date_char,hrs_r_gmt_char,min_r_char,sec_r_char);
	send_uart(buffer);

	sprintf(timestamp_now,"%u%s%s_%s-%s-%s",thisyear,exact_month_char,exact_date_char,hrs_r_gmt_char,min_r_char,sec_r_char);
	send_uart(timestamp_now);

}

void create_new_file_w_timestamp( const char* log_buff , const char* surname_timestamp){

	char buffer_name[30];


	sprintf(buffer_name,"%s_%s.%s",log_buff,timestamp_now,surname_timestamp);

	fresult = f_open(&fil, buffer_name, FA_OPEN_ALWAYS | FA_READ | FA_WRITE );

	strcpy (buffer, "this is the noob text from noob ziang\r\n");

		  fresult = f_write(&fil, buffer , bufsize(buffer), &bw);


	f_close(&fil);
	bufclear();

}

void choose_file_name( int chosen_num){

	char buffer_num[100];

	sprintf(buffer_num,"chosen number : %d \r\nfilename : %s \r\nCRC32 value : 0x%08X\r\n  ",chosen_num,Afilename[chosen_num],Acrc[chosen_num]);
	send_uart(buffer_num);

	bufclear_custom(buffer_num);


}






int bufsize (char *buf){ // bufsize is use to determine the size of buffer up to which it is filled
	int m = 0;
	while (*buf++ != '\0'){
		m++;
	}
	return m;
}

void add_2_indexFile (const char *output_filename){
	// we have to add arrays to index file in the final step kaaa :P
	// arrays =  Afilename, Acrc



	 	FIL output_file;
	    FRESULT res;
	    UINT bw;
	    //if(output_filename != "index.txt"){
			res = f_open(&output_file, output_filename, FA_WRITE | FA_READ);
			if (res != FR_OK) {
				sprintf(buffer, "Error opening output file: %d\r\n", res);
				send_uart(buffer);
				return;
			}

			for (int k = 0; k < MAXFILE; k++) {
				if (strlen(Afilename[k]) == 0 ) {
					break; // No more filenames to write
				}
				sprintf(buffer, "%u, %s, 0x%08X\r\n",k, Afilename[k], Acrc[k]);

				send_uart(buffer);



					res = f_write(&output_file, buffer, strlen(buffer), &bw);
					if (res != FR_OK) {
						sprintf(buffer, "Error writing to output file: %d\r\n", res);
						send_uart(buffer);
						break;
					}
			}

			f_close(&output_file);

	   // }

}


void add_22_indexFile (const char *output_filename){
	// we have to add arrays to index file in the final step kaaa :P
	// arrays =  Afilename, Acrc



	 	FIL output_file;
	    FRESULT res;
	    UINT bw;

	    res = f_open(&output_file, output_filename, FA_WRITE | FA_READ);
	    if (res != FR_OK) {
	        sprintf(buffer, "Error opening output file: %d\r\n", res);
	        send_uart(buffer);
	        return;
	    }

	    for (int k = 0; k < MAXFILE; k++) {
	        if (strlen(Afilename[k]) == 0 ) {
	            break; // No more filenames to write
	        }



	        if (is_dup_filename("index.txt",i)== 0){

	        		sprintf(buffer, "%s, 0x%08X\r\n", Afilename[k], Acrc[k]);

	        		send_uart(buffer);

					res = f_write(&output_file, buffer, strlen(buffer), &bw);
					if (res != FR_OK) {
						sprintf(buffer, "Error writing to output file: %d\r\n", res);
						send_uart(buffer);
						break;
					}
	        }

	    }

	    f_close(&output_file);



}


// 1.3 clear buffer
void bufclear (void){
	/*
	for (int i = 0; i<1024; i++){
		buffer[i] = '\0';
	}*/

	memset(buffer,0, sizeof(buffer));

}

void bufclear_custom (char BUFF){
	/*
	for (int i = 0; i<1024; i++){
		buffer[i] = '\0';
	}*/

	memset(BUFF,0, sizeof(BUFF));

}



void mount_sd_card(void){

		fresult = f_mount(&fs, "" , 0);
	  	  if (fresult != FR_OK){
	  		  send_uart("error in mounting SD CARD \r\n");
	  	  }
	  	  else{
	  		  send_uart("SD card mounted successfully kaa ^_^ \r\n");
	  	  }


	  	 // FRESULT f_getfree (const TCHAR* path, DWORD* nclst, FATFS** fatfs);	/* Get number of free clusters on the drive */

	  		  f_getfree("",&fre_clust, &pfs);

	  		  total = (uint32_t)( (pfs ->n_fatent - 2) * pfs ->csize * 0.5);
	  		  sprintf (buffer, "SD CARD Total Size: \t%lu\r\n",total);
	  		  // %lu is unsigned int or unsigned long
	  		  send_uart(buffer);
	  		  bufclear();

	  		  free_space = (uint32_t)(fre_clust * pfs ->csize * 0.5);
	  		  sprintf (buffer,"SD CARD Free space: \t%lu\r\n",free_space);
	  		  send_uart(buffer);

	  		  bufclear();

}


void delete_file_in_sd_card(const char *filename_wanna_delete, const char *namsakhun_wanna_delete){

	char buff[30];
	sprintf(buff,"%s.%s",filename_wanna_delete,namsakhun_wanna_delete);
	fresult = f_unlink(buff);
	if ( fresult == FR_OK ){
		sprintf(buffer,"%s removed successfully \r\n",buff);
		send_uart(buffer);}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_SPI1_Init();
  MX_FATFS_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */

  // ************* - reset state - *******************************************

      	 // 2. mount SD card
      	  	  mount_sd_card();

      	 // 3. BLANK index.txt

      	  	  // remove index.txt
     	  	  delete_file_in_sd_card("index","txt");

     	  	  //delete_file_in_sd_card("log_20240813_16-01-34","txt");



    		  // create an index.txt file
    		  fresult = f_open(&fil,"index.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
    		  	  if ( fresult == FR_OK ){send_uart("index.txt opened leaw ka :P \n");}

    		  f_read(&fil , buffer, sizeof(buffer) , &br);

    		  f_close(&fil); // don't forget to close the file
    		  bufclear();

      // *******************


      //find_txtfile("txt");

    //  bufclear();
    //  find_txtfile_w_crc3("txt");
    //  bufclear();


    /*  for (int z = 0; z <= i ; z++) {

      			 sprintf(buffer,"Afilename = %d \r\n", Afilename[z]);
      			 send_uart(buffer);

      		 }*/


    //  add_2_indexFile("index.txt");

      // ************ - Record State - *****************************************

       //  1. create a file
    	  // how to create new file w timestamp
    //	  /*1)*/ utc2rtc_ja(1723620005); // <- put utc at here********
    //	  /*2)*/ create_new_file_w_timestamp("log","txt");

      // 2. put data in the file
    	  // **additional, assume it's done

      // 3. scan file inside a SD card -> files's CRC
    	  find_txtfile_w_crc3("txt");
    	  bufclear();

    	  // put list of files into the index.txt
    	  add_2_indexFile("index.txt");

      // 4. choose file wanna choose
    	  choose_file_name(3);

    //*/

      // P'new's hidden func
      //check_crc_from_index();



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

   static uint8_t MACAddr[6];

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
  heth.Init.TxDesc = DMATxDscrTab;
  heth.Init.RxDesc = DMARxDscrTab;
  heth.Init.RxBuffLen = 1524;

  /* USER CODE BEGIN MACADDRESS */

  /* USER CODE END MACADDRESS */

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
  /* USER CODE BEGIN ETH_Init 2 */

  /* USER CODE END ETH_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
