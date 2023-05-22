#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "GPX.h"

#define _GPX_HEAD "<gpx version=\"1.1\" creator=\"GPX Lib\"\n xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n xmlns=\"http://www.topografix.com/GPX/1/1\"\n xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1\"\n>\n"
#define _GPX_TAIL "</gpx>\n"
#define _GPX_META_HEAD "<metadata>"
#define _GPX_META_TAIL "</metadata>\n"
#define _GPX_TRAK_HEAD "<trk>"
#define _GPX_TRAK_TAIL "</trk>\n"
#define _GPX_TRKSEG_HEAD "<trkseg>"
#define _GPX_TRKSEG_TAIL "</trkseg>\n"
#define _GPX_PT_HEAD "<TYPE lat=\""
#define _GPX_PT_TAIL "</TYPE>\n"

#define _GPX_NAME_HEAD "<name>"
#define _GPX_NAME_TAIL "</name>\n"
#define _GPX_DESC_HEAD "<desc>"
#define _GPX_DESC_TAIL "</desc>\n"
#define _GPX_SYM_HEAD "<sym>"
#define _GPX_SYM_TAIL "</sym>\n"
#define _GPX_ELE_HEAD "<ele>"
#define _GPX_ELE_TAIL "</ele>\n"
#define _GPX_SRC_HEAD "<src>"
#define _GPX_SRC_TAIL "</src>\n"
#define _GPX_TIME_HEAD "<time>"
#define _GPX_TIME_TAIL "</time>\n"

char _metaName[256];
char _metaDesc[256];
char _name[256];
char _desc[256];
char _ele[256];
char _sym[256];
char _src[256];
char _time[256];

char wrap_data[256];
char meta_data[1024];
char info_data[1024];
char pt_data[1024];

void replace_str(char *str, const char *old_str, const char *new_str)
{
    // 计算替换前的子串长度和替换后的子串长度
    size_t old_len = strlen(old_str);
    size_t new_len = strlen(new_str);

    char *pos;
    while ((pos = strstr(str, old_str)) != NULL)
    { // 反复查找
        // 将目标字符串向后移动
        memmove(pos + new_len, pos + old_len, strlen(pos + old_len) + 1);

        // 在原位置插入新字符串
        memcpy(pos, new_str, new_len);
    }
}
char *wrapCDATA(char *input)
{
    memset(wrap_data, 0, sizeof(wrap_data));
    strcat(wrap_data, "<![CDATA[");
    strcat(wrap_data, input);
    strcat(wrap_data, "]]>");
    return wrap_data;
}

char *getOpen()
{
    return _GPX_HEAD;
}

char *getClose()
{
    return _GPX_TAIL;
}

char *getMetaData()
{
    memset(meta_data, 0, sizeof(meta_data));
    strcat(meta_data, _GPX_META_HEAD);
    if (strlen(_metaName) > 0)
    {
        strcat(meta_data, _GPX_NAME_HEAD);
        strcat(meta_data, wrapCDATA(_metaName));
        strcat(meta_data, _GPX_NAME_TAIL);
    }
    if (strlen(_metaDesc) > 0)
    {
        strcat(meta_data, _GPX_DESC_HEAD);
        strcat(meta_data, wrapCDATA(_metaDesc));
        strcat(meta_data, _GPX_DESC_TAIL);
    }
    strcat(meta_data, _GPX_META_TAIL);
    return meta_data;
}

char *getTrakOpen()
{
    return _GPX_TRAK_HEAD;
}

char *getTrakClose()
{
    return _GPX_TRAK_TAIL;
}

char *getTrakSegOpen()
{
    return _GPX_TRKSEG_HEAD;
}

char *getTrakSegClose()
{
    return _GPX_TRKSEG_TAIL;
}

char *getInfo()
{
    memset(info_data, 0, sizeof(info_data));
    if (strlen(_name) > 0)
    {
        strcat(info_data, _GPX_NAME_HEAD);
        strcat(info_data, wrapCDATA(_name));
        strcat(info_data, _GPX_NAME_TAIL);
    }
    if (strlen(_desc) > 0)
    {
        strcat(info_data, _GPX_DESC_HEAD);
        strcat(info_data, wrapCDATA(_desc));
        strcat(info_data, _GPX_DESC_TAIL);
    }
    return info_data;
}

char *getPt(char *typ, char *lon, char *lat)
{
    memset(pt_data, 0, sizeof(pt_data));
    strcat(pt_data, _GPX_PT_HEAD);
    replace_str(pt_data, "TYPE", typ);
    // strcat(pt_data, "\" lat=\"");
    strcat(pt_data, lat);
    strcat(pt_data, "\" lon=\"");
    strcat(pt_data, lon);
    strcat(pt_data, "\">");

    if (strlen(_ele) > 0)
    {
        strcat(pt_data, _GPX_ELE_HEAD);
        strcat(pt_data, _ele);
        strcat(pt_data, _GPX_ELE_TAIL);
    }
    if (strlen(_time) > 0)
    {
        strcat(pt_data, _GPX_TIME_HEAD);
        strcat(pt_data, _time);
        strcat(pt_data, _GPX_TIME_TAIL);
    }
    if (strlen(_sym) > 0)
    {
        strcat(pt_data, _GPX_SYM_HEAD);
        strcat(pt_data, _sym);
        strcat(pt_data, _GPX_SYM_TAIL);
    }
    if (strlen(_src) > 0)
    {
        strcat(pt_data, _GPX_SRC_HEAD);
        strcat(pt_data, wrapCDATA(_src));
        strcat(pt_data, _GPX_SRC_TAIL);
    }
    char GPX_PT_TAIL[256];
    strcpy(GPX_PT_TAIL, _GPX_PT_TAIL);
    replace_str(GPX_PT_TAIL, "TYPE", typ);
    strcat(pt_data, GPX_PT_TAIL);

    return pt_data;
}

char *getPtWithEle(char *typ, char *lon, char *lat, char *ele)
{
    setEle(ele);
    return getPt(typ, lon, lat);
}

void setMetaName(char *name)
{
    strcpy(_metaName, name);
}

void setMetaDesc(char *desc)
{
    strcpy(_metaDesc, desc);
}

void setName(char *name)
{
    strcpy(_name, name);
}

void setDesc(char *desc)
{
    strcpy(_desc, desc);
}

void setEle(char *ele)
{
    strcpy(_ele, ele);
}

void setSym(char *sym)
{
    strcpy(_sym, sym);
}

void setSrc(char *src)
{
    strcpy(_src, src);
}

void setTime(char *time)
{
    strcpy(_time, time);
}
