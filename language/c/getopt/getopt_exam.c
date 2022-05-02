#include <stdio.h>
#include <unistd.h>	//getopt()

int parse_opt(int argc, char **argv)
{
        int c;
        opterr = 0;
        while ((c = getopt(argc, argv, "a:e:d:")) != -1){
                switch (c)
                {
                        case 'a':
                                printf("option=>%c arg=>%s\n", c, optarg);
                                break;
                        case 'e':
                                printf("option=>%c arg=>%s\n", c, optarg);
                                break;
                        case 'd':
                                printf("option=>%c arg=>%s\n", c, optarg);
                                break;
                        default:
                                printf("what! [%s]\n",optarg);
                }
        }
}

int main(int argc, char** argv)
{
	parse_opt(argc, argv);
}
