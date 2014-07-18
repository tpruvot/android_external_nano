/**
 * Test for mblen and wcstombs functions
 */
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

/* from mblen.c */
size_t u8_strlen(char *buf);
size_t u8_strnlen(char *buf, size_t maxlen);

void print_mb(const char* ptr)
{
	wchar_t wc;
	const char* end = ptr + strlen(ptr);
	mbstate_t state;
	int len;

	mbrlen(NULL, 0, &state);

	while((len = mbrtowc(&wc, ptr, end - ptr, &state)) > 0) {
		//printf("Next %d bytes are the character %lc\n", len, wc);
		printf("%lc", wc);
		ptr += len;
	}
	printf("\n");
}

#define BUFFER_SIZE 50
#undef MB_CUR_MAX
#define MB_CUR_MAX 4

int main()
{
	int len, mbl;
	char *pmb = (char *) malloc(BUFFER_SIZE);
	char utf[] = "Ôhaï zéro Wörld";
	char *pu = utf;
	wchar_t pwc[] = L"Ôhaï zéro Wörld";
	wchar_t arr[BUFFER_SIZE] = L"\0";
	wchar_t *p;
	const wchar_t *cp = pwc;
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

	printf("Converting to multibyte string with bionic\n");

	mbrlen(NULL, 0, &mbs);
	len = wcsrtombs(pmb, &cp, BUFFER_SIZE, &mbs);
	printf("Characters converted %d, %s\n", len, utf);

	print_mb(pmb);

	len = 0;
	mbrlen(NULL, 0, &mbs); p = arr;
	while((mbl = mbtowc(p, pu, len)) > 0) {
		pu+=mbl; len++; p++;
	}
	printf("wide character strdata (%d): %ls\n", len, arr);

	printf("Hex value of first multibyte character: %04x\n", *pmb);

	len = mblen(pmb, MB_CUR_MAX);
	printf("Length in bytes of multibyte character %04x: %u\n", *pmb, len);

	free(pmb);

	return 0;
}

