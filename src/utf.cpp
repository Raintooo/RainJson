#include "utf.h"


namespace UtfCodec
{

static unsigned parse_hex4(const char *str)
{
	unsigned h=0;
	if (*str>='0' && *str<='9') h+=(*str)-'0'; else if (*str>='A' && *str<='F') h+=10+(*str)-'A'; else if (*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;
	h=h<<4;str++;
	if (*str>='0' && *str<='9') h+=(*str)-'0'; else if (*str>='A' && *str<='F') h+=10+(*str)-'A'; else if (*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;
	h=h<<4;str++;
	if (*str>='0' && *str<='9') h+=(*str)-'0'; else if (*str>='A' && *str<='F') h+=10+(*str)-'A'; else if (*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;
	h=h<<4;str++;
	if (*str>='0' && *str<='9') h+=(*str)-'0'; else if (*str>='A' && *str<='F') h+=10+(*str)-'A'; else if (*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;
	return h;
}

std::string utf16le_to_utf8(const char *str, int n)
{
	if (str == nullptr){ return std::string(); }

    std::string ret;
    int len = 0;
    unsigned uc, uc2;
    char* dst = (char*)calloc(n, sizeof(char));
    char* u8str = dst;
    const char* ptr = str;

    const unsigned char firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

    while(u8str && *ptr)
    {
        if(*ptr == '\\')
        {
            ptr++;

            if(*ptr == 'u')
            {
                uc=parse_hex4(ptr+1);
                ptr+=4;	/* get the unicode char. */

                if ((uc>=0xDC00 && uc<=0xDFFF) || uc==0)	break;	/* check for invalid.	*/

                if (uc>=0xD800 && uc<=0xDBFF)	/* UTF16 surrogate pairs.	*/
                {
                    if (ptr[1]!='\\' || ptr[2]!='u')	break;	/* missing second-half of surrogate.	*/

                    uc2 = parse_hex4(ptr+3);
                    ptr+=6;

                    if (uc2<0xDC00 || uc2>0xDFFF)		break;	/* invalid second-half of surrogate.	*/

                    uc = 0x10000 + (((uc&0x3FF)<<10) | (uc2&0x3FF));
                }

                len = 4;
                if (uc<0x80) len=1;
                else if (uc<0x800) len=2;
                else if (uc<0x10000) len=3; 
                u8str += len;

                switch (len) {
                    case 4: *--u8str =((uc | 0x80) & 0xBF); uc >>= 6;
                    case 3: *--u8str =((uc | 0x80) & 0xBF); uc >>= 6;
                    case 2: *--u8str =((uc | 0x80) & 0xBF); uc >>= 6;
                    case 1: *--u8str =(uc | firstByteMark[len]);
                }

                u8str+=len;
            }
            else
            {
                *u8str++ = *ptr++;
            }
        }

        ptr++;
    }

    ret = dst;
    free(dst);
	
	return std::move(ret);
}

}