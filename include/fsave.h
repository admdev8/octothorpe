#include <stdint.h>

#ifdef _MSC_VER
#pragma pack(push,1)
#endif
struct FSAVE_info
{
	uint16_t	control_word;
	uint16_t	reserverd1;
	uint16_t	status_word;
	uint16_t	reserverd2;
	uint16_t	tag_word;
	uint16_t	reserverd3;
	uint32_t	instruction_pointer;
	uint16_t	code_segment;
	uint16_t	reserverd4;
	uint32_t	operand_address;
	uint16_t	data_segment;
	uint16_t	reserverd5;
	uint8_t		STx[8][10];
};
#ifdef _MSC_VER
#pragma pack(pop)
#endif
