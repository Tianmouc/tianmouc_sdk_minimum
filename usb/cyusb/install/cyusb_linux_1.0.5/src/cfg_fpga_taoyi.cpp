/************************************************************************************************
 * Program Name		:	08_cybulk.cpp							*
 * Author		:	V. Radhakrishnan ( rk@atr-labs.com )				*
 * License		:	LGPL Ver 2.1							*
 * Copyright		:	Cypress Semiconductors Inc. / ATR-LABS				*
 * Date written		:	April 3, 2012							*
 * Modification Notes	:									*
 * 												*
 * This program is a CLI program that does a bulk transfer using the bulkloop.hex file		*
 * downloaded to the FX2 device									*
\***********************************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <pthread.h>
#include <vector>
#include "../include/cyusb.h"

/********** Cut and paste the following & modify as required  **********/
static const char * program_name;
static const char *const short_options = "hvt:";
static const struct option long_options[] = {
		{ "help",	0,	NULL,	'h'	},
		{ "version",	0,	NULL,	'v'	},
		{ "timeout",    1,      NULL,   't',	},
		{ NULL,		0,	NULL,	 0	}
};

static int next_option;

static void print_usage(FILE *stream, int exit_code)
{
	fprintf(stream, "Usage: %s options\n", program_name);
	fprintf(stream, 
		"  -h  --help           Display this usage information.\n"
		"  -v  --version        Print version.\n"
		"  -t  --timeout	timeout in seconds, 0 for indefinite wait.\n");

	exit(exit_code);
}
/***********************************************************************/

static FILE *fp = stdout;
static int timeout_provided;
static int timeout = 0;
static cyusb_handle *h1 = NULL;

static void validate_inputs(void)
{
	if ( (timeout_provided) && (timeout < 0) ) {
	   fprintf(stderr,"Must provide a positive value for timeout in seconds\n");
	   print_usage(stdout, 1);
	}   
}

static void *reader(void *arg1)
{
	int r;
	int len=64;
	unsigned char buf[len];
	int transferred = 0;

	memset(buf,'\0',len);
	while (1) {
		printf("before transfer\n");
		r = cyusb_bulk_transfer(h1, 0x86, buf, len, &transferred, timeout * 1000);
		printf("after transfer\n");

		if ( r == 0 ) {
		
		   printf("%s", buf);
		   memset(buf,'\0',len);
		   continue;
		}
		else {
			cyusb_error(r);
			cyusb_close();
			return NULL;
		}
	} 

}

static void * writer(void *arg2)
{
	int r, nbr;
	unsigned char buf[64];
	int transferred = 0;

	memset(buf,'\0',64);
	while ( nbr = read(0,buf,64) ) {
		r = cyusb_bulk_transfer(h1, 0x04, buf, nbr, &transferred, timeout * 1000);
		
		if ( r == 0 ) {
			printf("transfer success\n");
			memset(buf,'\0',64);
			continue;
		}
		else {
			printf("transfer fail\n");
			cyusb_error(r);
			cyusb_close();
			return NULL;
		}
	} 

}

static void * writer_fpga_Cfg(void *arg2 )
{
	uint32_t adc_bit_prec= 8;
	 uint32_t out_interface = 0;
	const unsigned int SYS_STOP = 0x0; // sys stop
    const unsigned int SYS_RST = 0x1;// sys reset
    const unsigned int CONE_EN = 0x10; // cone enable
    const unsigned int PW_ON_GPIO = 0x200; //
    const unsigned int FPGA_MODE_CFG_HEAD = (0xdd << 24); //
    const unsigned int FPGA_MODE_CFG_TAIL = (0x96 << 24); //
    //add by taoyi!
    const unsigned int FPGA_MODE_CFG_READ_HEAD = (0xee << 24); //
    const unsigned int FPGA_MODE_CFG_READ_TAIL = (0x5a << 24); //
	unsigned int cfg = (adc_bit_prec + (out_interface << 7)) | CONE_EN | PW_ON_GPIO;
	unsigned int cfg_reset = (adc_bit_prec + 1 + (out_interface << 7)) | CONE_EN | PW_ON_GPIO;
	std::vector<unsigned int> cfg_data;
	//head
	cfg_data.push_back(0);
	cfg_data.push_back(0);
	cfg_data.push_back(FPGA_MODE_CFG_HEAD);
	// configuration data for FPGA mode 
	cfg_data.push_back(cfg);
	//tail
	cfg_data.push_back(FPGA_MODE_CFG_TAIL);
	cfg_data.push_back(0);
	cfg_data.push_back(0);
	//download cfg to usb 
	//for (auto i = 0; i < cfg_data.size(); i++) {
	long len = cfg_data.size() * 4;
	printf("cfg %x, cfg data size %d\n", cfg, len);
	
	int r, nbr;
	unsigned char buf[64];
	int transferred = 0;



	memcpy(buf, cfg_data.data(), len);


	printf("cyusb_bulk_transfer\n");
	r = cyusb_bulk_transfer(h1, 0x04, buf, len, &transferred, timeout * 1000);

	if (r == 0) {
			printf("Transfer success\n");
		} else {
			printf("Transfer failed\n");
			cyusb_error(r);
			cyusb_close();
			return NULL;
		}



	memset(buf,'\0',64);
	int* buf_i = (int*) buf;
	
	// while ( nbr = read(0,buf,64) ) {
	// 	r = cyusb_bulk_transfer(h1, 0x04, buf, nbr, &transferred, timeout * 1000);
	// 	if ( r == 0 ) {
	// 		memset(buf,'\0',64);
	// 		continue;
	// 	}
	// 	else {
	// 		cyusb_error(r);
	// 		cyusb_close();
	// 		return NULL;
	// 	}
	// } 

}

int main(int argc, char **argv)
{
	int r;
	char user_input = 'n';
	pthread_t tid1, tid2;

	program_name = argv[0];
	
	while ( (next_option = getopt_long(argc, argv, short_options, 
					   long_options, NULL) ) != -1 ) {
		switch ( next_option ) {
			case 'h': /* -h or --help  */
				  print_usage(stdout, 0);
			case 'v': /* -v or --version */
				  printf("%s (Ver 1.0)\n",program_name);
				  printf("Copyright (C) 2012 Cypress Semiconductors Inc. / ATR-LABS\n");
				  exit(0);
			case 't': /* -t or --timeout  */
				  timeout_provided = 1;
				  timeout = atoi(optarg);
				  break;
			case '?': /* Invalid option */
				  print_usage(stdout, 1);
			default : /* Something else, unexpected */
				  abort();
		}
	} 

	validate_inputs();


	

	r = cyusb_open();
	if ( r < 0 ) {
	   printf("Error opening library\n");
	   return -1;
	}
	else if ( r == 0 ) {
		printf("No device found\n");
		return 0;
	}
	if ( r > 1 ) {
		printf("More than 1 devices of interest found. Disconnect unwanted devices\n");
		return 0;
	}
	h1 = cyusb_gethandle(0);
	if ( cyusb_getvendor(h1) != 0x04b4 ) {
	  	printf("Cypress chipset not detected\n");
		cyusb_close();
	  	return 0;
	}
	r = cyusb_kernel_driver_active(h1, 0);
	if ( r != 0 ) {
	   printf("kernel driver active. Exitting\n");
	   cyusb_close();
	   return 0;
	}
	r = cyusb_claim_interface(h1, 0);
	if ( r != 0 ) {
	   printf("Error in claiming interface\n");
	   cyusb_close();
	   return 0;
	}
	else printf("Successfully claimed interface\n");
	//r = pthread_create(&tid1, NULL, reader, NULL);
	//r = pthread_create(&tid2, NULL, writer, NULL);






	r = pthread_create(&tid2, NULL, writer_fpga_Cfg, NULL);



	




	while ( 1) {
		pause();
	}


	cyusb_close();



	return 0;
}
