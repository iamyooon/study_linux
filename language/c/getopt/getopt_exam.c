#include <stdio.h>
#include <unistd.h>	//getopt()

int parse_opt(int argc, char **argv)
{
        int c;
        opterr = 0;
        while ((c = getopt(argc, argv, "a:b:c")) != -1){
                switch (c)
                {
                        case 'a':
                                printf("option=>%c arg=>%s\n", c, optarg);
                                break;
                        case 'b':
                                printf("option=>%c arg=>%s\n", c, optarg);
                                break;
                        case 'c':
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
