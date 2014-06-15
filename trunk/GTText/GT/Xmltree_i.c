/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Oct 28 18:03:55 1999
 */
/* Compiler settings for D:\Dev\xmltree\Xmltree.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IXMLDOMDocumentEventsSink = {0xB30149CF,0x22AD,0x43EA,{0xA4,0x12,0x3A,0xEA,0x62,0xBF,0x25,0xFC}};


const IID LIBID_XmltreeLib = {0x67EC031D,0x694F,0x4DDF,{0x83,0x8A,0xCD,0x74,0x58,0xDF,0xF2,0x5E}};


const CLSID CLSID_XMLDOMDocumentEventsSink = {0xF3F146CE,0xAE61,0x44CC,{0x9E,0x02,0x4B,0x06,0xDA,0x10,0xAB,0xE4}};


#ifdef __cplusplus
}
#endif

