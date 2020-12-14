#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>

int main(int argc, char** argv)
{
	//  struct passwd {
	//  char   *pw_name;       /* username */
	//  char   *pw_passwd;     /* user password */
	//  uid_t   pw_uid;        /* user ID */
	//  gid_t   pw_gid;        /* group ID */
	//  char   *pw_gecos;      /* user information */
	//  char   *pw_dir;        /* home directory */
	//  char   *pw_shell;      /* shell program */
	//  };
	struct passwd *pw;
	int i;

	if (argc < 2)
		pw = getpwnam("root");
	else
		pw = getpwnam(argv[1]);

	if (pw == NULL) {
		printf("Not found\n");
		return -1;
	}

	printf("user name=%s,user pw=%s,user id=%d"
		" group id=%d, user info=%s, home dir=%s, sell program=%s\n",
		pw->pw_name, pw->pw_passwd, pw->pw_uid,
		pw->pw_gid, pw->pw_gecos, pw->pw_dir, pw->pw_shell);

	return 0;
}
