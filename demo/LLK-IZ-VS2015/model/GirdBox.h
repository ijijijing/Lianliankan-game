#ifndef _GIRDBOX_H_
#define _GIRDBOX_H_

#include <cstdint>

#pragma pack(2)
typedef struct GirdBoxItem
{
	uint16_t type;
	uint16_t exist;
}GirdBoxItem, GirdBox;
#pragma pack()

#endif // !_GIRDBOX_H_

