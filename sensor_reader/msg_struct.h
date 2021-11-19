#ifndef MSG_STRUCT_H
#define MSG_STRUCT_H

/*
 *******************************************************************************
 *> Constants and Macros
 *******************************************************************************
*/

#ifndef TRUE
#define TRUE                    1
#endif

#ifndef FALSE
#define FALSE                   0
#endif


#ifndef NULL
#define NULL                    0
#endif

// //////////////////////// MSGS_IDs ///////////////////////////////////

#define MAX_LEN_MSG                 4096

#define ID_VL53LX_INIT_MSG          1
#define ID_VL53LX_DATA_MSG          2

#define ID_VL53LX_CONFIG_MSG        111
#define ID_VL53LX_STOP_MSG          112

#define LEN_VL53LX_INIT_MSG         5
#define LEN_VL53LX_DATA_MSG         324
#define LEN_VL53LX_DATA_64_MSG      328
#define LEN_VL53LX_DATA_16_MSG      88
#define LEN_VL53LX_CONFIG_MSG       6
#define LEN_VL53LX_STOP_MSG         4

#define VL53L5CX_RESOLUTION_64      64
#define VL53L5CX_RESOLUTION_16      16


typedef signed char               int8;   // signed 8 bit
typedef unsigned char             uint8;   // unsigned 8 bit
typedef signed short int          int16;  // signed 16 bit
typedef unsigned short int        uint16;  // unsigned 16 bit
typedef signed int                int32;  // signed 32 bit
typedef unsigned int              uint32;  // unsigned 32 bit
typedef signed long int           int64;  // signed 64 bit
typedef unsigned long int         uint64;  // unsigned 64 bit

#pragma pack(push)
#pragma pack(1)

//dichiarazione della struttura dell'header comune a tutti i messaggi
typedef struct
{
    uint8 msg_id;
    uint16 msg_len;

} MSG_HEADER;

//dichiarazione della struttura dell'header comune a tutti i messaggi
typedef struct
{
    MSG_HEADER header;
    uint8 tof_id;
    uint8 tof_status;

} VL53LX_INIT_MSG;      //len 5

typedef struct
{
    uint8 range_status;
    uint16 range_val;
    uint16 sigma;

} VL53LX_OBJ_INFO;      //len 320

typedef struct
{
    MSG_HEADER header;
    uint8 id_tof;
    VL53LX_OBJ_INFO object_data[VL53L5CX_RESOLUTION_64];

} VL53LX_DATA_MSG;      //len 324

typedef struct
{
    MSG_HEADER header;
    uint8 id_tof;
    uint16 ms_get_new_data;
    VL53LX_OBJ_INFO object_data[VL53L5CX_RESOLUTION_16];
    uint16 ms_send_msg;

} VL53LX_DATA_16_MSG;      //len 84

typedef struct
{
    MSG_HEADER header;
    uint8 id_tof;
    uint16 ms_get_new_data;
    VL53LX_OBJ_INFO object_data[VL53L5CX_RESOLUTION_64];
    uint16 ms_send_msg;

} VL53LX_DATA_64_MSG;      //len 324

typedef struct
{
    MSG_HEADER header;
    uint8 resolution;
    uint8 frequency;
    uint8 sharpener;

} VL53LX_CONFIG_MSG;       //len 6

typedef struct
{
    MSG_HEADER header;
    uint8 stop;

} VL53LX_STOP_MSG;       //len 4

#pragma pack(pop)

#endif // MSG_STRUCT_H
