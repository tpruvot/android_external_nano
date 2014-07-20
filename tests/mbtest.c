/**
 * Test for mblen and wcstombs functions
 * from src/utf8_mbfuncs.c
 *
 * tpruvot@github 2014
 */
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

size_t u8_strlen(char *buf);
size_t u8_strnlen(char *buf, size_t maxlen);

#define BUFFER_SIZE 50
#undef MB_CUR_MAX
#define MB_CUR_MAX 4

int main()
{
	int len, mbl;
	char *pmb = (char *) malloc(BUFFER_SIZE);
	char utf[] = "Ôhaï zéro Wörld\0";
	char *pu = utf;
	const wchar_t *pwc = L"Ôhaï zéro Wörld"; /* this "L" syntax is not compatible */
	wchar_t arr[BUFFER_SIZE] = L"\0";
	wchar_t *p;
	const wchar_t *cp = &arr[0];
	mbstate_t mbs;

	memset(pmb, 0, BUFFER_SIZE);

	/* simple func test */
	len = u8_strnlen(utf,BUFFER_SIZE);
	printf("'%s' has %d ext. chars\n", utf, len);

	/* initialize internal state variable */
	mbrlen(NULL, 0, &mbs);

	printf("Locale is %s\n", setlocale(LC_CTYPE, ""));


	/* test for our mblen.c */
	len = mblen("\xe6\x9e\x90", MB_CUR_MAX);
	printf("our mblen(\xe6\x9e\x90) = %d\n", len);
	len = mbrlen("\xe6\x9e\x90", MB_CUR_MAX, &mbs);
	printf("bionic mbrlen(\xe6\x9e\x90) = %d!\n", len);

	len = mbtowc(arr,"\xe6\x9e\x90", 3); // linux wc value is 0x6790
	printf("wide character (%d bytes): %08x\n", len, arr[0]);

	mbrlen(NULL, 0, &mbs);
	len = wctomb(pmb, arr[0]);
	printf("wctomb -> %s (ret=%d)\n", pmb, len);
	memset(pmb, 0, BUFFER_SIZE);

	printf("Converting to wchar_t string\n");
	len = mbstowcs(arr, utf, BUFFER_SIZE);
	printf("Characters converted %d\n", len);

	len = wcstombs(NULL, cp, BUFFER_SIZE); // len only
	printf("Converting to multibyte string, need %u bytes\n", len);
	len = wcsrtombs(pmb, &cp, BUFFER_SIZE, &mbs);
	printf("Characters converted %d, %s\n", len, pmb);

	printf("Hex value of first multibyte character: %02x%02x\n", pmb[0], pmb[1]);

	len = mblen(pmb, MB_CUR_MAX);
	printf("Length in bytes of first multibyte character: %u\n", len);

	free(pmb);

	return 0;
}

