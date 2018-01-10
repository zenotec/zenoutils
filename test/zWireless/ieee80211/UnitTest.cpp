/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>

#include <zutils/zLog.h>

#include "UnitTest.h"
#include "Ieee80211Test.h"

/* Radiotap header */
const uint8_t radiotap[] =
{
    0x00, 0x00, 0x26, 0x00, 0x2f, 0x40, 0x00, 0xa0, /* ..&./@.. */
    0x20, 0x08, 0x00, 0xa0, 0x20, 0x08, 0x00, 0x00, /*  ... ... */
    0x55, 0x66, 0x96, 0x31, 0x00, 0x00, 0x00, 0x00, /* Uf.1.... */
    0x10, 0x0c, 0x3c, 0x14, 0x40, 0x01, 0xe2, 0x00, /* ..<.@... */
    0x00, 0x00, 0xe2, 0x00, 0xc1, 0x01              /* ...... */
};
const size_t radiotap_len = sizeof(radiotap);

/* Radiotap header + Beacon Frame (233 bytes) */
const uint8_t beacon[233] =
{
    0x00, 0x00, 0x26, 0x00, 0x2f, 0x40, 0x00, 0xa0, /* ..&./@.. */
    0x20, 0x08, 0x00, 0xa0, 0x20, 0x08, 0x00, 0x00, /*  ... ... */
    0x55, 0x66, 0x96, 0x31, 0x00, 0x00, 0x00, 0x00, /* Uf.1.... */
    0x10, 0x0c, 0x3c, 0x14, 0x40, 0x01, 0xe2, 0x00, /* ..<.@... */
    0x00, 0x00, 0xe2, 0x00, 0xc1, 0x01, 0x80, 0x00, /* ........ */
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x00, 0x11, /* ..m..G.. */
    0x22, 0x33, 0x44, 0x55, 0xa0, 0xfd, 0x3b, 0xf0, /* m..G..;. */
    0x5f, 0xfe, 0x38, 0x00, 0x00, 0x00, 0x64, 0x00, /* _.8...d. */
    0x11, 0x00, 0x00, 0x08, 0x4c, 0x4c, 0x57, 0x69, /* ....LLWi */
    0x66, 0x69, 0x35, 0x30, 0x01, 0x08, 0x8c, 0x12, /* fi50.... */
    0x98, 0x24, 0xb0, 0x48, 0x60, 0x6c, 0x03, 0x01, /* .$.H`l.. */
    0x24, 0x05, 0x04, 0x01, 0x02, 0x00, 0x00, 0x07, /* $....... */
    0x0a, 0x44, 0x45, 0x20, 0x24, 0x08, 0x14, 0x64, /* .DE $..d */
    0x0b, 0x1a, 0x00, 0x30, 0x14, 0x01, 0x00, 0x00, /* ...0.... */
    0x0f, 0xac, 0x04, 0x01, 0x00, 0x00, 0x0f, 0xac, /* ........ */
    0x04, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x02, 0x0c, /* ........ */
    0x00, 0x3b, 0x02, 0x74, 0x00, 0x2d, 0x1a, 0xce, /* .;.t.-.. */
    0x01, 0x1b, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x3d, 0x16, 0x24, 0x05, 0x00, 0x00, 0x00, /* .=.$.... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x7f, 0x08, 0x04, 0x00, 0x00, 0x02, 0x00, /* ........ */
    0x00, 0x00, 0x40, 0xdd, 0x18, 0x00, 0x50, 0xf2, /* ..@...P. */
    0x02, 0x01, 0x01, 0x80, 0x00, 0x03, 0xa4, 0x00, /* ........ */
    0x00, 0x27, 0xa4, 0x00, 0x00, 0x42, 0x43, 0x5e, /* .'...BC^ */
    0x00, 0x62, 0x32, 0x2f, 0x00, 0xb7, 0x66, 0xbf, /* .b2/..f. */
    0x79                                            /* y */
};
const size_t beacon_len = sizeof(beacon);

/* Radiotap header + Probe Request frame + FCS (137 bytes) */
const uint8_t probe_req[137] =
{
    0x00, 0x00, 0x26, 0x00, 0x2f, 0x40, 0x00, 0xa0, /* ..&./@.. */
    0x20, 0x08, 0x00, 0xa0, 0x20, 0x08, 0x00, 0x00, /*  ... ... */
    0xb7, 0x59, 0xdc, 0x31, 0x00, 0x00, 0x00, 0x00, /* .Y.1.... */
    0x10, 0x0c, 0x3c, 0x14, 0x40, 0x01, 0xb5, 0x00, /* ..<.@... */
    0x00, 0x00, 0xb5, 0x00, 0x9b, 0x01, 0x40, 0x00, /* ......@. */
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* ........ */
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0xff, 0xff, /* ..;..... */
    0xff, 0xff, 0xff, 0xff, 0xd0, 0xba, 0x00, 0x00, /* ........ */
    0x01, 0x08, 0x02, 0x04, 0x0b, 0x0c, 0x12, 0x16, /* ........ */
    0x18, 0x24, 0x2d, 0x1a, 0x62, 0x01, 0x03, 0x00, /* .$-.b... */
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x08, 0xe0, 0xe1, 0x09, 0x00, 0x32, 0x04, /* ......2. */
    0x30, 0x48, 0x60, 0x6c, 0x7f, 0x09, 0x00, 0x00, /* 0H`l.... */
    0x00, 0x00, 0x01, 0x00, 0x00, 0x40, 0x00, 0xbf, /* .....@.. */
    0x0c, 0xb0, 0x71, 0xc0, 0x33, 0xf0, 0xff, 0x49, /* ..q.3..I */
    0x02, 0xf0, 0xff, 0x49, 0x02, 0xe4, 0x19, 0x58, /* ...I...X */
    0x68                                            /* h */
};
const size_t probe_req_len = sizeof(probe_req);

/* Radiotap header + Probe Response frame + FCS (429) */
const uint8_t probe_resp[429] =
{
    0x00, 0x00, 0x26, 0x00, 0x2f, 0x40, 0x00, 0xa0, /* ..&./@.. */
    0x20, 0x08, 0x00, 0xa0, 0x20, 0x08, 0x00, 0x00, /*  ... ... */
    0x5c, 0x7d, 0xb5, 0x01, 0x30, 0x02, 0x00, 0x00, /* \}..0... */
    0x10, 0x02, 0x6c, 0x09, 0xa0, 0x00, 0xb0, 0x00, /* ..l..... */
    0x00, 0x00, 0xaf, 0x00, 0xa9, 0x01, 0x50, 0x00, /* ......P. */
    0x3a, 0x01, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, /* :...;... */
    0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x00, 0x11, /* ..C.9F.. */
    0x22, 0x33, 0x44, 0x55, 0xa0, 0x3f, 0x30, 0x98, /* C.9F.?0. */
    0x66, 0xc3, 0x88, 0x09, 0x00, 0x00, 0x64, 0x00, /* f.....d. */
    0x11, 0x14, 0x00, 0x08, 0x4c, 0x4c, 0x57, 0x69, /* ....LLWi */
    0x66, 0x69, 0x35, 0x30, 0x01, 0x08, 0x82, 0x84, /* fi50.... */
    0x0b, 0x16, 0x24, 0x30, 0x48, 0x6c, 0x03, 0x01, /* ..$0Hl.. */
    0x01, 0x2a, 0x01, 0x00, 0x32, 0x04, 0x0c, 0x12, /* .*..2... */
    0x18, 0x60, 0x30, 0x14, 0x01, 0x00, 0x00, 0x0f, /* .`0..... */
    0xac, 0x04, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, /* ........ */
    0x01, 0x00, 0x00, 0x0f, 0xac, 0x02, 0x0c, 0x00, /* ........ */
    0x0b, 0x05, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x46, /* ....,..F */
    0x05, 0x32, 0x08, 0x01, 0x00, 0x00, 0x2d, 0x1a, /* .2....-. */
    0xad, 0x09, 0x17, 0xff, 0xff, 0xff, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x3d, 0x16, 0x01, 0x08, 0x11, 0x00, /* ..=..... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x4a, 0x0e, 0x14, 0x00, 0x0a, 0x00, /* ..J..... */
    0x2c, 0x01, 0xc8, 0x00, 0x14, 0x00, 0x05, 0x00, /* ,....... */
    0x19, 0x00, 0x7f, 0x08, 0x05, 0x00, 0x08, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x40, 0xdd, 0x06, 0x00, 0x14, /* ...@.... */
    0x6c, 0x00, 0x00, 0x00, 0xdd, 0x7e, 0x00, 0x50, /* l....~.P */
    0xf2, 0x04, 0x10, 0x4a, 0x00, 0x01, 0x10, 0x10, /* ...J.... */
    0x44, 0x00, 0x01, 0x02, 0x10, 0x3b, 0x00, 0x01, /* D....;.. */
    0x03, 0x10, 0x47, 0x00, 0x10, 0xfc, 0xff, 0x06, /* ..G..... */
    0xf4, 0xd2, 0xe3, 0x1c, 0x6e, 0x73, 0x72, 0xd7, /* ....nsr. */
    0xb3, 0x00, 0x6f, 0x08, 0xde, 0x10, 0x21, 0x00, /* ..o...!. */
    0x0d, 0x4e, 0x45, 0x54, 0x47, 0x45, 0x41, 0x52, /* .NETGEAR */
    0x2c, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x10, 0x23, /* , Inc..# */
    0x00, 0x05, 0x52, 0x37, 0x30, 0x30, 0x30, 0x10, /* ..R7000. */
    0x24, 0x00, 0x05, 0x52, 0x37, 0x30, 0x30, 0x30, /* $..R7000 */
    0x10, 0x42, 0x00, 0x02, 0x33, 0x32, 0x10, 0x54, /* .B..32.T */
    0x00, 0x08, 0x00, 0x06, 0x00, 0x50, 0xf2, 0x04, /* .....P.. */
    0x00, 0x01, 0x10, 0x11, 0x00, 0x09, 0x6d, 0x69, /* ......mi */
    0x63, 0x72, 0x6f, 0x6e, 0x65, 0x74, 0x31, 0x10, /* cronet1. */
    0x08, 0x00, 0x02, 0x20, 0x08, 0x10, 0x3c, 0x00, /* ... ..<. */
    0x01, 0x03, 0x10, 0x49, 0x00, 0x06, 0x00, 0x37, /* ...I...7 */
    0x2a, 0x00, 0x01, 0x20, 0xdd, 0x1e, 0x00, 0x90, /* *.. .... */
    0x4c, 0x04, 0x08, 0xbf, 0x0c, 0xb2, 0x59, 0x82, /* L.....Y. */
    0x0f, 0xea, 0xff, 0x00, 0x00, 0xea, 0xff, 0x00, /* ........ */
    0x00, 0xc0, 0x05, 0x00, 0x01, 0x00, 0x00, 0x00, /* ........ */
    0xc3, 0x02, 0x00, 0x02, 0xdd, 0x09, 0x00, 0x10, /* ........ */
    0x18, 0x02, 0x00, 0x00, 0x1c, 0x00, 0x00, 0xdd, /* ........ */
    0x18, 0x00, 0x50, 0xf2, 0x02, 0x01, 0x01, 0x88, /* ..P..... */
    0x00, 0x03, 0xa4, 0x00, 0x00, 0x27, 0xa4, 0x00, /* .....'.. */
    0x00, 0x42, 0x43, 0xbc, 0x00, 0x62, 0x32, 0x66, /* .BC..b2f */
    0x00, 0xb1, 0x37, 0xb0, 0xc2                    /* ..7.. */
};
const size_t probe_resp_len = sizeof(probe_resp);

/* Radiotap header + Association Request frame + FCS (170) */
const uint8_t ass_req[170] =
{
    0x00, 0x00, 0x26, 0x00, 0x2f, 0x40, 0x00, 0xa0, /* ..&./@.. */
    0x20, 0x08, 0x00, 0xa0, 0x20, 0x08, 0x00, 0x00, /*  ... ... */
    0x90, 0xeb, 0xc5, 0x01, 0x30, 0x02, 0x00, 0x00, /* ....0... */
    0x10, 0x02, 0x6c, 0x09, 0xa0, 0x00, 0xb6, 0x00, /* ..l..... */
    0x00, 0x00, 0xb5, 0x00, 0xad, 0x01, 0x00, 0x00, /* ........ */
    0x3a, 0x01, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, /* :...!7.. */
    0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x00, 0x11, /* LfAR"... */
    0x22, 0x33, 0x44, 0x55, 0x30, 0x77, 0x31, 0x04, /* !7..0w1. */
    0x0a, 0x00, 0x00, 0x04, 0x47, 0x57, 0x30, 0x32, /* ....GW02 */
    0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, /* ......$0 */
    0x48, 0x6c, 0x32, 0x04, 0x0c, 0x12, 0x18, 0x60, /* Hl2....` */
    0x21, 0x02, 0x02, 0x14, 0x24, 0x02, 0x01, 0x0b, /* !...$... */
    0x30, 0x14, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, /* 0....... */
    0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, 0x01, 0x00, /* ........ */
    0x00, 0x0f, 0xac, 0x02, 0x80, 0x00, 0x2d, 0x1a, /* ......-. */
    0xad, 0x01, 0x17, 0xff, 0xff, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0xdd, 0x09, 0x00, 0x10, 0x18, 0x02, /* ........ */
    0x00, 0x00, 0x10, 0x00, 0x00, 0xdd, 0x07, 0x00, /* ........ */
    0x50, 0xf2, 0x02, 0x00, 0x01, 0x00, 0x00, 0x41, /* P......A */
    0x2d, 0x60                                      /* -` */
};
const size_t ass_req_len = sizeof(ass_req);

/* Radiotap header + Association Response frame + FCS (176) */
const uint8_t ass_resp[176] =
{
    0x00, 0x00, 0x26, 0x00, 0x2f, 0x40, 0x00, 0xa0, /* ..&./@.. */
    0x20, 0x08, 0x00, 0xa0, 0x20, 0x08, 0x00, 0x00, /*  ... ... */
    0x31, 0xf7, 0xc5, 0x01, 0x30, 0x02, 0x00, 0x00, /* 1...0... */
    0x10, 0x02, 0x6c, 0x09, 0xa0, 0x00, 0xb8, 0x00, /* ..l..... */
    0x00, 0x00, 0xb7, 0x00, 0xb2, 0x01, 0x10, 0x00, /* ........ */
    0x3a, 0x01, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, /* :.LfAR". */
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x21, 0x22, /* ..!7.... */
    0x23, 0x24, 0x25, 0x26, 0x50, 0x53, 0x31, 0x04, /* !7..PS1. */
    0x00, 0x00, 0x01, 0xc0, 0x01, 0x08, 0x82, 0x84, /* ........ */
    0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, 0x32, 0x04, /* .....$2. */
    0x30, 0x48, 0x60, 0x6c, 0x2d, 0x1a, 0xed, 0x19, /* 0H`l-... */
    0x1b, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x3d, 0x16, 0x01, 0x00, 0x15, 0x00, 0x00, 0x00, /* =....... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x7f, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x40, 0xdd, 0x18, 0x00, 0x50, 0xf2, 0x02, /* .@...P.. */
    0x01, 0x01, 0x80, 0x00, 0x03, 0xa4, 0x00, 0x00, /* ........ */
    0x27, 0xa4, 0x00, 0x00, 0x42, 0x43, 0x5e, 0x00, /* '...BC^. */
    0x62, 0x32, 0x2f, 0x00, 0x05, 0xab, 0x23, 0x71  /* b2/...#q */
};
const size_t ass_resp_len = sizeof(ass_resp);

/* Radiotap header + RTS Frame (58 bytes) */
const uint8_t rts[58] =
{
    0x00, 0x00, 0x26, 0x00, 0x2f, 0x40, 0x00, 0xa0, /* ..&./@.. */
    0x20, 0x08, 0x00, 0xa0, 0x20, 0x08, 0x00, 0x00, /*  ... ... */
    0x28, 0xe1, 0x9b, 0x31, 0x00, 0x00, 0x00, 0x00, /* (..1.... */
    0x10, 0x30, 0x3c, 0x14, 0x40, 0x01, 0xb5, 0x00, /* .0<.@... */
    0x00, 0x00, 0xb0, 0x00, 0x9b, 0x01, 0xb4, 0x00, /* ........ */
    0x74, 0x00, 0x74, 0x1b, 0xb2, 0xf2, 0x22, 0xfd, /* t.t...". */
    0x5c, 0xf7, 0xe6, 0x82, 0x37, 0x33, 0x5c, 0xb0, /* \...73\. */
    0xe4, 0xff                                      /* .. */
};
const size_t rts_len = sizeof(rts);

/* Radiotap header + CTS Frame (52 bytes) */
/* Frame (52 bytes) */
const uint8_t cts[52] =
{
    0x00, 0x00, 0x26, 0x00, 0x2f, 0x40, 0x00, 0xa0, /* ..&./@.. */
    0x20, 0x08, 0x00, 0xa0, 0x20, 0x08, 0x00, 0x00, /*  ... ... */
    0xf6, 0x6c, 0x9b, 0x31, 0x00, 0x00, 0x00, 0x00, /* .l.1.... */
    0x10, 0x30, 0x3c, 0x14, 0x40, 0x01, 0xb0, 0x00, /* .0<.@... */
    0x00, 0x00, 0xb1, 0x00, 0x98, 0x01, 0xc4, 0x00, /* ........ */
    0x66, 0x00, 0x74, 0x1b, 0xb2, 0xf2, 0x22, 0xfd, /* f.t...". */
    0xfb, 0x6f, 0x5c, 0x66                          /* .o\f */
};
const size_t cts_len = sizeof(cts);

/* Radiotap header + ACK Frame (52 bytes) */
const uint8_t ack[52] = {
    0x00, 0x00, 0x26, 0x00, 0x2f, 0x40, 0x00, 0xa0, /* ..&./@.. */
    0x20, 0x08, 0x00, 0xa0, 0x20, 0x08, 0x00, 0x00, /*  ... ... */
    0x65, 0x06, 0x9f, 0x31, 0x00, 0x00, 0x00, 0x00, /* e..1.... */
    0x10, 0x30, 0x3c, 0x14, 0x40, 0x01, 0xae, 0x00, /* .0<.@... */
    0x00, 0x00, 0xae, 0x00, 0x9c, 0x01, 0xd4, 0x00, /* ........ */
    0x00, 0x00, 0x5c, 0xf7, 0xe6, 0x82, 0x37, 0x33, /* ..\...73 */
    0xac, 0x13, 0x5a, 0x0a                          /* ..Z. */
};
const size_t ack_len = sizeof(ack);

/* Authentication Frame */
const uint8_t auth[67] = {
    0x00, 0x00, 0x1a, 0x00, 0x2f, 0x48, 0x00, 0x00,
    0x70, 0xa5, 0xa4, 0x05, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x02, 0x6c, 0x09, 0xa0, 0x00, 0xdd, 0x00,
    0x00, 0x00, 0xb0, 0x00, 0x3a, 0x01, 0x04, 0xf0,
    0x21, 0x37, 0xec, 0xb2, 0x4c, 0x66, 0x41, 0x52,
    0x22, 0xa5, 0x04, 0xf0, 0x21, 0x37, 0xec, 0xb2,
    0xb0, 0xd4, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0xdd, 0x09, 0x00, 0x10, 0x18, 0x02, 0x00, 0x00,
    0x10, 0x00, 0x00
};
const size_t auth_len = sizeof(auth);

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("Ieee80211Test.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::DBG);

  // Test all classes
  UTEST_INIT();

  // Frame class unit tests
  UTEST_TEST(Ieee80211Test_RadiotapDefaults, 0);
  UTEST_TEST(Ieee80211Test_RadiotapGetSet, 0);
  UTEST_TEST(Ieee80211Test_RadiotapAssemble, 0);
  UTEST_TEST(Ieee80211Test_RadiotapDisassemble, 0);

  // Frame class unit tests
  UTEST_TEST(Ieee80211Test_FrameDefaults, 0);
  UTEST_TEST(Ieee80211Test_FrameGetSet, 0);
  UTEST_TEST(Ieee80211Test_FrameAssemble, 0);
  UTEST_TEST(Ieee80211Test_FrameDisassemble, 0);

  // Management Frame class unit tests
  UTEST_TEST(Ieee80211Test_ManagementFrameDefaults, 0);
  UTEST_TEST(Ieee80211Test_ManagementFrameGetSet, 0);
  UTEST_TEST(Ieee80211Test_ManagementFrameAssemble, 0);
  UTEST_TEST(Ieee80211Test_ManagementFrameDisassemble, 0);

  // Beacon class unit tests
  UTEST_TEST(Ieee80211Test_BeaconDefaults, 0);
  UTEST_TEST(Ieee80211Test_BeaconGetSet, 0);
  UTEST_TEST(Ieee80211Test_BeaconAssemble, 0);
  UTEST_TEST(Ieee80211Test_BeaconDisassemble, 0);

  // Probe Request class unit tests
  UTEST_TEST(Ieee80211Test_ProbeRequestDefaults, 0);
  UTEST_TEST(Ieee80211Test_ProbeRequestGetSet, 0);
  UTEST_TEST(Ieee80211Test_ProbeRequestAssemble, 0);
  UTEST_TEST(Ieee80211Test_ProbeRequestDisassemble, 0);

  // Probe Response class unit tests
  UTEST_TEST(Ieee80211Test_ProbeResponseDefaults, 0);
  UTEST_TEST(Ieee80211Test_ProbeResponseGetSet, 0);
  UTEST_TEST(Ieee80211Test_ProbeResponseAssemble, 0);
  UTEST_TEST(Ieee80211Test_ProbeResponseDisassemble, 0);

  // Association Request class unit tests
  UTEST_TEST(Ieee80211Test_AssociationRequestDefaults, 0);
  UTEST_TEST(Ieee80211Test_AssociationRequestGetSet, 0);
  UTEST_TEST(Ieee80211Test_AssociationRequestAssemble, 0);
  UTEST_TEST(Ieee80211Test_AssociationRequestDisassemble, 0);

  // Association Response class unit tests
  UTEST_TEST(Ieee80211Test_AssociationResponseDefaults, 0);
  UTEST_TEST(Ieee80211Test_AssociationResponseGetSet, 0);
  UTEST_TEST(Ieee80211Test_AssociationResponseAssemble, 0);
  UTEST_TEST(Ieee80211Test_AssociationResponseDisassemble, 0);

  // Authentication class unit tests
  UTEST_TEST(Ieee80211Test_AuthenticationDefaults, 0);
  UTEST_TEST(Ieee80211Test_AuthenticationGetSet, 0);
  UTEST_TEST(Ieee80211Test_AuthenticationAssemble, 0);
  UTEST_TEST(Ieee80211Test_AuthenticationDisassemble, 0);

  UTEST_FINI();

  // Exit
  exit(0);

}