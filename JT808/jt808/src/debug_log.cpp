#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

int debug_printf(int level, const char * file, const char * func, int line, const char * fmt, ...)
{
	if(1 == level)
	{
	}
	else if(6 == level)
	{
		struct tm *local;
		time_t tt;
		tzset();
		tt    = time(NULL);
		local = localtime(&tt);
		fprintf(stderr, "(%4d-%02d-%02d %02d:%02d:%02d ", local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);
	}

	fprintf(stderr, "%s,%s,%d) ", file, func, line);

	char sprint_buf[1000];
	memset(sprint_buf, 0, sizeof(sprint_buf));

	va_list args;
	va_start(args, fmt);
	vsnprintf(sprint_buf, sizeof(sprint_buf)-1, fmt, args);
	va_end(args);

	fprintf(stderr, sprint_buf);
	fprintf(stderr, "\n");

	return 0;
}


