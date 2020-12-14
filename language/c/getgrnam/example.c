#include <stdio.h>
#include <sys/types.h>
#include <grp.h>

int main(int argc, char** argv)
{
	//struct group {
	//   char   *gr_name;        // group name
	//   char   *gr_passwd;      // group password
	//   gid_t   gr_gid;         // group ID
	//   char  **gr_mem;         // NULL-terminated array of pointers
	//	                     // to names of group members */
	// };
	struct group *grp;
	int i;

	if (argc < 2)
		grp = getgrnam("root");
	else
		grp = getgrnam(argv[1]);

	if (grp == NULL) {
		printf("Not found\n");
		return -1;
	}

	printf("group name=%s,group_passwd=%s,group_gid=%d\n",
		grp->gr_name, grp->gr_passwd, grp->gr_gid);

	return 0;
}
