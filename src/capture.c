#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "box/capture.h"

static int savedStdout = -1;
static int pipefd[2];

static char buf[4096];
static size_t captureLen = 0;

void capture_begin(void)
{
	fflush(stdout);

	pipe(pipefd);

	savedStdout = dup(STDOUT_FILENO);
	dup2(pipefd[1], STDOUT_FILENO);

	close(pipefd[1]);
}

char *capture_end(void)
{
	fflush(stdout);

	dup2(savedStdout, STDOUT_FILENO);
	close(savedStdout);

	captureLen = read(pipefd[0], buf, sizeof(buf) - 1);
	close(pipefd[0]);

	if (captureLen < 0) {
		captureLen = 0;
	}
	buf[captureLen] = '\0';
	return buf;
}
