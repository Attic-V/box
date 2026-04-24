#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "box/box.h"
#include "box/capture.h"
#include "box/posix.h"

struct style {
	char *edge_horizontal;
	char *edge_vertical;
	char *corner_NE;
	char *corner_NW;
	char *corner_SE;
	char *corner_SW;
};

static struct style style_single = {
	"─", "│",
	"┐", "┌", "┘", "└",
};

static void preprocess(char *buf)
{
	size_t i, j;

	char tmp[4096];
	size_t out = 0;

	for (i = 0; buf[i] != '\0' && out + 1 < sizeof(tmp); i++) {
		if (buf[i] == '\t') {
			for (j = 0; j < 8 && out + 1 < sizeof(tmp); j++) {
				tmp[out++] = ' ';
			}
		} else {
			tmp[out++] = buf[i];
		}
	}

	tmp[out] = '\0';
	strcpy(buf, tmp);
}

static size_t utf8_width(char *s, size_t bytes)
{
	mbstate_t st = {0};
	size_t width = 0;
	size_t i = 0;
	wchar_t wc;

	while (i < bytes) {
		size_t n = mbrtowc(&wc, s + i, bytes - i, &st);

		width += wcwidth(wc);
		i += n;
	}

	return width;
}

static void drawBox(char *buf)
{
	size_t i;

	char *p = buf;
	size_t width = 0;

	while (*p != '\0') {
		char *start = p;
		size_t len, w;

		while (*p != '\0' && *p != '\n') p++;
		len = p - start;
		w = utf8_width(start, len);

		if (w > width) width = w;
		if (*p == '\n') p++;
	}

	printf(style_single.corner_NW);
	for (i = 0; i < width + 2; i++) printf(style_single.edge_horizontal);
	printf(style_single.corner_NE);
	printf("\n");

	p = buf;
	while (*p != '\0') {
		size_t i;

		char *start = p;
		size_t len, w;

		while (*p != '\0' && *p != '\n') p++;
		len = p - start;
		w = utf8_width(start, len);

		printf(style_single.edge_vertical);
		printf(" ");
		fwrite(start, 1, len, stdout);

		for (i = w; i < width; i++) printf(" ");

		printf(" ");
		printf(style_single.edge_vertical);
		printf("\n");

		if (*p == '\n') p++;
	}

	printf(style_single.corner_SW);
	for (i = 0; i < width + 2; i++) printf(style_single.edge_horizontal);
	printf(style_single.corner_SE);
	printf("\n");
}

void box_begin(void)
{
	setlocale(LC_ALL, "");
	capture_begin();
}

void box_end(void)
{
	char *buf = capture_end();

	preprocess(buf);
	drawBox(buf);

	free(buf);
}
