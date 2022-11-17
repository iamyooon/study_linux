#include <sys/types.h>  // getaddrinfo()
#include <sys/socket.h> // getaddrinfo()
#include <netdb.h>	// getaddrinfo()
#include <string.h> // memset()
#include <stdio.h>

// 출처: https://techlog.gurucat.net/293 [제임스딘딘의 Tech & Life:티스토리]
int main(int argc,char *argv[])
{
	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo;  // 결과를 저장할 변수

	memset(&hints, 0, sizeof(hints)); // hints 구조체의 모든 값을 0으로 초기화
	hints.ai_family = AF_UNSPEC;     // IPv4와 IPv6 상관하지 않고 결과를 모두 받겠다

	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	status = getaddrinfo("www.google.com", "80", &hints, &servinfo);
	printf("servinfo->ai_socktype=%d\n", servinfo->ai_socktype);

	hints.ai_socktype = SOCK_DGRAM; 
	status = getaddrinfo("www.google.com", "80", &hints, &servinfo);
	printf("servinfo->ai_socktype=%d\n", servinfo->ai_socktype);

	return 0;
}


