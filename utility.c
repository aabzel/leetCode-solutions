// C programming language handy functions


void explore_byte_order (void)
{
    unsigned short x = 1;       /* 0x0001 */
    int result;
    result = *((unsigned char *) &x);

    if (0 == result) {
        printf ("\nbig-endian");
    } else {
        // less significant byte first
        printf ("\nlittle-endian");     // esp32 is little endian
    }
}

uint16_t reverse_byte_order_uint16 (const uint16_t in2byteVal)
{
    uint16_t swapped = 0;
    swapped = (in2byteVal >> 8) | (in2byteVal << 8);
    return swapped;
}

uint32_t reverse_byte_order_uint32 (const uint32_t in4byteVal)
{
    uint32_t retval;
    retval = in4byteVal & 0xFF;
    retval = (retval << 8) | ((in4byteVal >> 8) & 0xFF);
    retval = (retval << 8) | ((in4byteVal >> 16) & 0xFF);
    retval = (retval << 8) | ((in4byteVal >> 24) & 0xFF);
    return retval;
}

uint8_t get_low_byte_from_word(const uint16_t word16bit)
{
    uint8_t result = 0;
    result = 0x00FF & word16bit;
    return result;
}

uint8_t get_hi_byte_from_word(const uint16_t word16bit)
{
    uint8_t result = 0;
    result = (0x00FF & (word16bit>>8));
    return result;
}

uint32_t pack_ipv4 (const uint8_t ipAddrByte1, const uint8_t ipAddrByte2,
                    const uint8_t ipAddrByte3, const uint8_t ipAddrByte4)
{
    uint8_t ipAddr[4] =
        { ipAddrByte1, ipAddrByte2, ipAddrByte3, ipAddrByte4 };
    uint32_t ipv4bytesLe, ipv4bytesBe;
    memcpy (&ipv4bytesLe, ipAddr, 4);
    ipv4bytesBe = reverse_byte_order_uint32 (ipv4bytesLe);
    return ipv4bytesBe;
}

int mac_bin_to_str (unsigned char *macBinIn, char *macStrOut, int sizeOfMacStr)
{
    if (macBinIn == NULL) {
        return 1;
    }
    if (macStrOut == NULL) {
        return 2;
    }

    if(sizeOfMacStr < 20){
    	return 3;
    }
    snprintf ((char *) macStrOut, sizeOfMacStr,
              "%02x:%02x:%02x:%02x:%02x:%02x",
              macBinIn[0], macBinIn[1], macBinIn[2],
              macBinIn[3], macBinIn[4], macBinIn[5]);

    return 0;
}


/* 0 - array if digit */
int digits_only (const char *s)
{
    while (*s) {
        if (valid_digit(*s++) != 0) {
        	return 1;
        }
    }

    return 0;
}


void uint16_to_str (const uint16_t inValue, char *outStr)
{
    if (outStr) {
        sprintf(outStr, "%04X", inValue);
    }
}

void print_array (const unsigned char* inArray,
                  const unsigned short arrayLen)
{
	printf("\n[");
	for (int i=0; i < arrayLen; i++) {
		printf ("%02x ",inArray[i]);
	}
	printf("]");
	printf("\n[");
	for (int i=0; i < arrayLen; i++) {
		printf("%c",inArray[i]);
	}
	printf("]");
}


/* return 0 if character contains only digit , else return 1*/
int valid_digit (const char character)
{
    if (('0' <= character)  && (character <= '9')) {
        return 0;
    } else {
        return 1;
    }
}


/* return 0 if character contains only hex digit , else return 1*/
int is_hex_digit (const char character) {
    if (('A' <= character && character <= 'F') || \
        ('a' <= character && character <= 'f') || \
        ('0' <= character && character <= '9')) {
        return 0;
    } else {
        return 1;
    }
}



/* return 0 if IP string is valid,
 * return positive number if error */
int is_valid_ip (char *inIpStr)
{
	//printf("\n is_valid_ip...\n");
	//printf("\n inIpStr: %s \n", inIpStr);
	int dotCnt = 0;
	int octetCnt = 0;
	int valOct[4] = {256,256,256,256};
	char curNumStr [10] = "";

	if (inIpStr == NULL){
		return 1;
	}
	int lenOfStr = strlen(inIpStr);
	//printf("\n lenOfStr: %d \n", lenOfStr);


	for (int j = 0; j < lenOfStr+1; j++) {

		char curChar = inIpStr[j];
		//printf("\n curChar: %c        \n", curChar);
		if ('.' == curChar) {
			dotCnt++;
			//printf("\n curNumStr: %s \n", curNumStr);
			valOct[octetCnt] = atoi(curNumStr);
			if (255 < valOct[octetCnt]) {
				return 4;
			}
			strcpy(curNumStr,"");
			octetCnt++;
		} else if ('\0' == curChar) {
			//printf("\n curNumStr: %s \n", curNumStr);
			valOct[octetCnt] = atoi(curNumStr);
			if (255 < valOct[octetCnt]) {
				return 4;
			}
			strcpy(curNumStr, "");
			octetCnt++;
		} else if ( !valid_digit(curChar) ) {
			char strDigit[2] = {curChar, '\0'};
			strcat(curNumStr, strDigit);
		} else {
			return 3;
		}
	}
	//printf("\n dotCnt: %d \n", dotCnt);
	if ( dotCnt != 3 )
		return 5;
	return 0;
}


/* return 0 if string is a regex  (0[x][0-9a-fA-F]{2}) ,
 * return positive number if error */
int is_valid_hex_byte (char const * const inByteStr) {
	int len;
	len = strlen(inByteStr);
	if (5<=len) {
		return 3;
	}
    if ('0' != inByteStr[0]) {
    	return 1;
    }
    if ('x' != inByteStr[1]) {
    	return 2;
    }
    if( is_hex_digit (inByteStr[2]) ) {
    	return 4;
    }
    if( is_hex_digit (inByteStr[3]) ) {
    	return 5;
    }

    return 0;
}


// ret 0 if arrays are equal
// ret 1.....size if arrays are not equal
int my_memcmp(const unsigned char *inArray1,
		      const unsigned char *inArray2,
			  int size)
{
	printf("\nsize of array: %d ", size);
	int res = 0;
	for (int i=0; i<size; i++) {
		if (inArray1[i]!=inArray2[i]) {
			printf("\ni %d",i);
			printf("\ninArray1[%d] %x(%c)",i,inArray1[i],inArray1[i]);
			printf("\ninArray2[%d] %x(%c)",i,inArray2[i],inArray2[i]);
			return i+1;
		}
	}
	return res;
}











