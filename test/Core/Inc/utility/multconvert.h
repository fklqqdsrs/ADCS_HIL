#ifndef __MULTCONVERT_H__
#define __MULTCONVERT_H__
typedef union
{
  float number;
  uint8_t bytes[sizeof(float)];
} FLOATUNION_t;
typedef union
{
  long number;
  uint8_t bytes[sizeof(long)];
} LONGUNION_t;
typedef union
{
  uint16_t number;
  uint8_t bytes[sizeof(uint16_t)];
} UINT16UNION_t;
typedef union
{
  int number;
  uint8_t bytes[sizeof(int)];
} INTUNION_t;
typedef union
{
  int16_t number;
  uint8_t bytes[sizeof(int16_t)];
} INT16UNION_t;

template<class T>
T map(T val, T in_min, T in_max, T out_min, T out_max)
{
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

template<class T>
float get_const_map(T in_min, T in_max, T out_min, T out_max)
{
  return (float)(out_max - out_min) / (in_max - in_min);
}

#endif
