#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#define _GNU_SOURCE
#include <dlfcn.h>


char *signame[]={"INVALID", "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL"
                ,"SIGTRAP", "SIGABRT", "SIGBUS", "SIGFPE", "SIGKILL"
                ,"SIGUSR1", "SIGSEGV", "SIGUSR2", "SIGPIPE", "SIGALRM"
                ,"SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT", "SIGSTOP"
                ,"SIGTSTP", "SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU"
                ,"SIGXFSZ", "SIGVTALRM", "SIGPROF", "SIGWINCH", "SIGPOLL"
                ,"SIGPWR", "SIGSYS","Stack Overflow", NULL};

void handler(int sig) {
        void *array[32];
        size_t size;

        // get void*'s for all entries on the stack
        size = backtrace(array, 32);
        // print out all the frames to stderr
        fprintf(stderr, "\n");
        fprintf(stderr, "============================================\n");
        fprintf(stderr, "[Wemake] Catch %s signal \n", signame[sig]);
        fprintf(stderr, "============================================\n");
        fprintf(stderr, "Backtrace:\n");
        backtrace_symbols_fd(array, size, STDERR_FILENO);

        fprintf(stderr, "\n");
        fprintf(stderr, "Memory map:\n");
        /* Now the link map.  */
        int mapfd = open ("/proc/self/maps", O_RDONLY);
        if (mapfd != -1)
        {
                char buf[256];
                ssize_t n;

                while ((n = read (mapfd, buf, sizeof (buf))) > 0){
                        buf[n]='\0';
                        fprintf(stderr, "%s\n",buf);
                }

                close (mapfd);
        }
        exit(1);
}

static int __attribute__((constructor)) segfault_init(void)
{
        struct sigaction sa;
        const char *signals;
        int error;

        sigemptyset(&sa.sa_mask);
        sa.sa_sigaction = &handler;
        sa.sa_flags = SA_RESTART | SA_SIGINFO | SA_ONSTACK;

        /*
         * Configure the signal handlers.
         */
        sigaction(SIGSEGV, &sa, NULL);
        sigaction(SIGBUS, &sa, NULL);
        sigaction(SIGILL, &sa, NULL);
        sigaction(SIGABRT, &sa, NULL);
        sigaction(SIGFPE, &sa, NULL);
        sigaction(SIGSYS, &sa, NULL);

        return (error);
}

void __stack_chk_fail(void)
{
        printf("*** stack smashing detected ***:\n");
        handler(32);
}

