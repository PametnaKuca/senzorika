#include "package.h"

short stringLength(char *str)
{
	char *tempStr = str;
	short len = 0;
	while(*(tempStr++) != '\0')
		len++;
	return len;
}

char* stringXOR(char *message)
{
		char xorString[2];
		char *tempMsg = message + 1;
		xorString[0] = *message;
		while(*tempMsg!='\0')
			xorString[0] = xorString[0] ^ *(tempMsg++);
		xorString[1] = '\0';
		return xorString;		
}

char* createPackage(uint8_t ID, uint8_t subID, uint8_t ctrl, char *data)
{
	char packageTemp[MAX_PACK_LEN + 1], *xorStr;
	short size = stringLength(data) + 4; // Message length + ID byte + SUB_ID byte + CTRL byte + XOR byte
	char protocolData[] = {(char)ID , (char)subID, (char)ctrl, '\0'};
	sprintf(packageTemp, "%.3d", size);
	strcat(packageTemp,protocolData);
	strcat(packageTemp,data);
	xorStr = stringXOR(packageTemp);
	strcat(packageTemp, xorStr);
	return packageTemp;
}
