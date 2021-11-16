#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef EXT_XMLPARSE_XMLPARSER_H_
#define EXT_XMLPARSE_XMLPARSER_H_

bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,bool *value);
bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,int  *value);
bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,unsigned int *value);
bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,long *value);
bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,unsigned long *value);
bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,double *value);
bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,char *value,const int ilen=0);


#endif // EXT_XMLPARSE_XMLPARSER_H_