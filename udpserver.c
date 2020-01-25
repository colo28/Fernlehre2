#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/un.h>
#include <signal.h>

void usage()	{
	fprintf(stderr, "\nAufruf: ./udpclient [OPTION]... [WERT]...");
	fprintf(stderr, "\n\t --mode \tGeben Sie ip-udp oder unix mode an! Option ist verpflichtend\n");
	fprintf(stderr, "-p\t --port \tMit dieser Option können Sie den Port angeben\n");
	fprintf(stderr, "\t --ip \t\tMit dieser Option können Sie die IP-Adresse angeben\n");
}

int main(int argc, char **argv)	{

	short port=9000;
	char * end;
	char mode_string[50]={"empty"};	
	char ip_string[20]={"127.0.0.1"};
	struct option longopt[]={
		{"mode", required_argument, 0, 0},
		{"ip", required_argument, 0, 1},
		{"port", required_argument, 0, 'p'}
	};
	int o, option_index;
	while((o=getopt_long(argc, argv, "p:", longopt, &option_index))!=-1) {
			switch (o) {
				case 0:
					strcpy(mode_string, optarg);
					break;
				case 1:
					strcpy(ip_string, optarg);
					break;
				case 'p':
					port=strtol(optarg, &end, 10);
					printf("%d", port);	
					break;
				case '?':
					fprintf(stderr, "Es wurde eine unbekannte Option angegeben!\n");
					usage();
					exit(1);
					break;
				default:
					break;
			}
	}	
	if(!(strcmp(mode_string, "empty")))	{
			fprintf(stderr, "Die Option mode ist eine Pflichtoption!\n");
			usage();
			exit(1);
	}
	else {
		if(strcmp(mode_string, "ip-udp") && strcmp(mode_string, "unix"))	{
			fprintf(stderr, "Es wurde keine gültige Mode-Option angegeben!\n");
			usage();
			exit(1);
		}
	}	
				
	int s;
	struct sockaddr_in server;
	char puffer[256]="test";

	if(!(strcmp(mode_string, "ip-udp"))) {
	socklen_t receive_len;

	server.sin_family=AF_INET;
	int v=inet_pton(AF_INET, ip_string, &server.sin_addr);
		if(v==0)	{
			fprintf(stderr, "Keine gültige IP Adresse \n");
			usage();
			exit(1);
		}
		else if(v==-1)	{
			perror("inet_pton");
			exit(1);
		}
		else if(v!=1)	{
			fprintf(stderr, "Ungültiger Returncode");
			exit(1);
		}
	server.sin_port=htons(port);
	receive_len=sizeof(server);
	s=socket(AF_INET, SOCK_DGRAM, 0);
	if(s < 0)	{
		perror("socket");
	exit(1);
	}
	if ( bind(s, (const struct sockaddr *)&server, sizeof(server)) < 0 )	 { 
		perror("bind failed"); 
		exit(1); 
    } 
	while(1)	{
		recvfrom(s, &puffer, sizeof(puffer), 0,(struct sockaddr*) &server, &receive_len);
		printf("%s", puffer);
	}
	}
	else	{
		unlink("test");
		struct sockaddr_un server_unix;
		socklen_t receive_len_unix;
		s=socket(AF_UNIX, SOCK_DGRAM, 0);
		if(s<0)	{
			perror("socket");
			exit(1);
		}
		server_unix.sun_family=AF_UNIX;
		strcpy(server_unix.sun_path, "test");
		receive_len_unix=sizeof(server_unix);	
		bind(s, (struct sockaddr *) &server_unix, sizeof(struct sockaddr_un));
		while(1)	{
		//	read(s, puffer, 256);		
			recvfrom(s, &puffer, sizeof(puffer), 0,(struct sockaddr*) &server_unix, &receive_len_unix);

		printf("%s", puffer);
	}
	}
	
}
