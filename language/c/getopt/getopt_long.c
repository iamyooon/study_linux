#include <stdio.h>
#include <unistd.h>	//getopt()
#include <getopt.h>

static struct option long_options[] =
{
	{"node", required_argument, 0, 'n'},
	{"tc", required_argument, 0, 's'},
	{"mode", required_argument, 0, 'm'},
	{"role", required_argument, 0, 'r'},
	{"file", required_argument, 0, 'f'},
	{"txrx", required_argument, 0, 'x'},
	{"verbose", required_argument, 0, 'v'},
	{"logpath", required_argument, 0, 'l'},
	{"help", no_argument, 0, 'h'},
	{0, 0, 0, 0}
};


int parse_opt_long(int argc, char **argv)
{
        int c;
        opterr = 0;
	int option_index = 0;
	while ((c = getopt_long(argc, argv, "n:s:m:r:f:x:v:h:", long_options, &option_index)) != -1)
	{
		switch (c)
		{
			case 0:
				break;
			case 'n':
				printf("c=%c,optarg=%s\n",c,optarg);
				break;
			case 's':
				printf("c=%c,optarg=%s\n",c,optarg);
				break;
			case 'm':
				printf("c=%c,optarg=%s\n",c,optarg);
				break;
			case 'r':
				printf("c=%c,optarg=%s\n",c,optarg);
				break;
			case 'f':
				printf("c=%c,optarg=%s\n",c,optarg);
				break;
			case 'x':
				printf("c=%c,optarg=%s\n",c,optarg);
				break;
			case 'l':
				printf("c=%c,optarg=%s\n",c,optarg);
				break;
			case 'v':
				printf("c=%c,optarg=%s\n",c,optarg);
				break;
			case 'h':
				printf("c=%c,optarg=%s\n",c,optarg);
				break;
			default:
				break;
		}
	}
}

int main(int argc, char** argv)
{
	parse_opt_long(argc, argv);
}
