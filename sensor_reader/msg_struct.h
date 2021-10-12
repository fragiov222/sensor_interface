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

#define LEN_VL53LX_INIT_MSG         5
#define LEN_VL53LX_DATA_MSG         324
#define VL53L5CX_RESOLUTION_64      64


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

#pragma pack(pop)

#endif // MSG_STRUCT_H
