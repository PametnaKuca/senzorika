#include "package.h"

short stringLength(char *str)
{
	char *tempStr = str;
	short len = 0;
	while(*(tempStr++) != '\0')
		len++;
	return len;
}

char* createPackage(uint8_t ID, uint8_t subID, uint8_t ctrl, char *data)
{
	char packageTemp[MAX_PACK_LEN + 1];
	short size = stringLength(data) + 4; // Message length + ID byte + SUB_ID byte + CTRL byte + XOR byte
	char protocolData[] = {(char)ID , (char)subID, (char)ctrl, '\0'};
	char xorByte[] = {63, '\0'};
	sprintf(packageTemp, "%.3d", size);
	strcat(packageTemp,protocolData);
	strcat(packageTemp,data);
	strcat(packageTemp, xorByte);
	return packageTemp;
}
