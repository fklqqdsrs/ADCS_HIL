#ifndef _printo_
#define _printo_
extern "C"{

int _write(int le, char *ptr, int len)

{
	int DataIdx;
	for (DataIdx = 0; DataIdx < len; DataIdx++)

	{
		ITM_SendChar(*ptr++);

	}
	return len;

}

}
#endif
