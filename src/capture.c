#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "box/capture.h"
#include "box/posix.h"

#define BUF_SIZE 4096

struct frame {
	int stdout_prev;
	int pipefd[2];

	struct frame *prev;
};

static struct frame *frame = NULL;

void pushFrame(void)
{
	struct frame *f = malloc(sizeof(*f));

	f->prev = frame;
	frame = f;
}

void popFrame(void)
{
	struct frame *f = frame;

	frame = f->prev;
	free(f);
}

void capture_begin(void)
{
	fflush(stdout);

	pushFrame();

	pipe(frame->pipefd);

	frame->stdout_prev = dup(STDOUT_FILENO);
	dup2(frame->pipefd[1], STDOUT_FILENO);

	close(frame->pipefd[1]);
}

char *capture_end(void)
{
	char *buf = malloc(BUF_SIZE);
	ssize_t captureLen;

	fflush(stdout);

	dup2(frame->stdout_prev, STDOUT_FILENO);
	close(frame->stdout_prev);

	captureLen = read(frame->pipefd[0], buf, BUF_SIZE - 1);
	close(frame->pipefd[0]);

	if (captureLen < 0) {
		perror("read");
		free(buf);
		popFrame();
		return strdup("");
	}

	buf[captureLen] = '\0';
	popFrame();
	return buf;
}
