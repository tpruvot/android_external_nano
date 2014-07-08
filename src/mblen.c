/**
 * Add some of the missing implementation of mb functions in bionic
 *
 * tpruvot@github - 2014
 *
 ***********************/

#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>

static mbstate_t mbs = {0};

int mblen(const char *s, size_t n)
{
	int buf_mb_len = 1;

	if (s == NULL) {
		/* No support for state dependent encodings. */
		memset(&mbs, 0, sizeof(mbs));
		return (0);
	}

	if ((*s & 0xF0) == 0xF0)
		buf_mb_len = 4; // start of 4-char utf series
	else if ((*s & 0xF0) == 0xE0)
		buf_mb_len = 3; // start of 3-char
	else if ((*s & 0xF0) == 0xC0)
		buf_mb_len = 2; // start of 2-char
	else if ((*s & 0xC0) == 0x80)
		buf_mb_len = 0; // utf hidden part

	return (buf_mb_len);
}

/* mb char -> wchar */
int mbtowc(wchar_t * __restrict__ wc, const char * __restrict__ src, size_t n)
{
	int len = mblen(src, n);
	if (!wc) return 0;

	//mbrtowc(wc, src, n, &mbs);

	*wc = 0;
	if (len > 0)
		memcpy((char*) wc, src, len);

	return len;
}

/* wchar -> mb char */
int wctomb(char * __restrict__ s, wchar_t wc)
{
	int len;
	if (!s) return 0;

	s[0] = wc & 0xFF;
	if (s[0]) s[1] = (wc & 0xFF00) >> 8;
	if (s[1]) s[2] = (wc & 0xFF0000) >> 16;
	if (s[2]) s[3] = (wc & 0xFF000000) >> 24;
	if (s[3]) s[4] = '\0';

	len = mblen(s, 4);
	return len;
}
