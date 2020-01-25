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
	struct sockaddr_in dst;

	if(!(strcmp(mode_string, "ip-udp"))){
		s=socket(AF_INET, SOCK_DGRAM, 0);
		if(s < 0)	{
			perror("socket");
		exit(1);
		}
		dst.sin_family=AF_INET;
		dst.sin_port=htons(port);
		int v=inet_pton(AF_INET, ip_string, &dst.sin_addr);
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
		unlink("test");
		while(1)	{
			char in_data[256];
			if(fgets(in_data,256,stdin)==NULL)	{
				fprintf(stderr, "Daten können nicht gelesen werden\n");
				close(s);
				exit(1);
			}	
	sendto(s, in_data, sizeof(in_data),0, (const struct sockaddr*) &dst, sizeof(dst));
	}
	}
	else	{
		struct sockaddr_un dst_unix;
		s=socket(AF_UNIX, SOCK_DGRAM, 0);
		if(s < 0)	{
			perror("socket");
		exit(1);
		}
		dst_unix.sun_family=AF_UNIX;
		strcpy(dst_unix.sun_path, "test");	
		while(1)	{
			char in_data[256];
			if(fgets(in_data,256,stdin)==NULL)	{
				fprintf(stderr, "Daten können nicht gelesen werden\n");
				close(s);
				exit(1);
			}	
	if(sendto(s, in_data, sizeof(in_data),0, (const struct sockaddr*) &dst_unix, sizeof(struct sockaddr_un))<0) {
		perror("error sending");
	}
	}
	}

}
