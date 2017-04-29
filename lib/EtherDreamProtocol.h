#pragma once

//#include <type_traits>

using namespace std;

//typedef unsigned int uint32_t;
//typedef unsigned short uint16_t;
//typedef short int16_t;
//typedef char uint8_t;

#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )

PACK(
	struct dac_point {
		uint16_t control;
		int16_t x;
		int16_t y;
		uint16_t r;
		uint16_t g;
		uint16_t b;
		uint16_t i;
		uint16_t u1;
		uint16_t u2;
	};
)

PACK(
	struct dac_status {
		uint8_t protocol;
		uint8_t light_engine_state;
		uint8_t playback_state;
		uint8_t source;
		uint16_t light_engine_flags;
		uint16_t playback_flags;
		uint16_t source_flags;
		uint16_t buffer_fullness;
		uint32_t point_rate;
		uint32_t point_count;
	};
)

PACK(
	struct dac_broadcast{
		uint8_t mac_address[6];
		uint16_t hw_revision;
		uint16_t sw_revision;
		uint16_t buffer_capacity;
		uint32_t max_point_rate;
		struct dac_status status;
	};
)

/* 'b' (0x62) */
PACK(
	struct begin_command {
		uint8_t command;
		uint16_t low_water_mark;
		uint32_t point_rate;
	};
)

/* 'q' (0x74) */
PACK(
	struct queue_command {
		uint8_t command;
		uint32_t point_rate;
	};
)

/* 'd' (0x64) */
PACK(
	struct data_command {
		uint8_t command;
		uint16_t npoints;
		struct dac_point data[];
	};
)

/* 'd' (0x64) */
PACK(
	struct data_command_header {
		uint8_t command;
		uint16_t npoints;
	};
)

PACK(
	struct dac_response {
		uint8_t response;
		uint8_t command;
		struct dac_status dac_status;
	};
)