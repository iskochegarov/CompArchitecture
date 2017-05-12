#include "memory.h"


//reg_t reg;
//int16_t mem[MEM_SIZE];

int sc_memoryInit ()
{
    memset(mem, 0, MEM_SIZE * sizeof(mem[0]));
    return 0;
}

int sc_memorySet (u_int8_t address, int16_t value)
{
    if ((address < 100) && (address >= 0))
        mem[address] = value;
    else
    {
        sc_regSet(FLG_MEMORY_FAULT, 1);
        return ADDRESS_ERROR;
    }
    return 0;
}

int sc_memoryGet (u_int8_t address, int16_t *value)
{
    if ((address < 100) && (address >= 0))
        *value = mem[address];
    else
    {
        sc_regSet(FLG_MEMORY_FAULT, 1);
        return ADDRESS_ERROR;
    }
    return 0;
}

int sc_memorySave (char * filename)
{
    FILE *ptrsave = fopen(filename, "wb");
    if (ptrsave !=  NULL)
    {
        fwrite(mem, sizeof(mem[0]), MEM_SIZE, ptrsave);
        fclose(ptrsave);
    }
    else return FILE_ERROR;
    return 0;
}

int sc_memoryLoad (char * filename)
{
    FILE *ptrload = fopen(filename, "rb");
    if (ptrload != NULL)
    {
        fread(mem, sizeof(mem[0]), MEM_SIZE, ptrload);
        fclose(ptrload);
    }
    else return FILE_ERROR;
    return 0;
}

int sc_regInit (void)
{
    reg.accum = 0;
    reg.count = 0;
    reg.flg = 0b00000000;
    return 0;
}

int sc_regSet (int8_t regist, int8_t value)
{
    if (value == 1)
    {
        switch (regist)
        {
        case FLG_OVERFLOW:
        case FLG_ZERO_DIVISION:
        case FLG_MEMORY_FAULT:
        case FLG_TICK_IGNORE:
        case FLG_WRONG_COMMAND:
            reg.flg |= regist;
            break;
        default:
            return REG_ERROR;
        }
    }
    else if (value == 0)
    {
        switch (regist)
        {
        case FLG_OVERFLOW:
        case FLG_ZERO_DIVISION:
        case FLG_MEMORY_FAULT:
        case FLG_TICK_IGNORE:
        case FLG_WRONG_COMMAND:
            reg.flg &= ~regist;
            break;
        default:
            return REG_ERROR;
        }
    }
    else
        return REG_ERROR;
    return 0;
}

int sc_regGet (int8_t regist, int8_t * value)
{
    switch (regist)
    {
    case FLG_OVERFLOW:
    case FLG_ZERO_DIVISION:
    case FLG_MEMORY_FAULT:
    case FLG_TICK_IGNORE:
    case FLG_WRONG_COMMAND:
        *value = (reg.flg & regist) > 0 ? 1 : 0;
        break;
    default:
        return REG_ERROR;
    }
    return 0;
}

int sc_commandEncode (int8_t command, int8_t operand, int16_t * value)
{
    int16_t tmp = 0b0000000000000000;
    int width = 16;
    //FIXME: ПРОВЕРКА

    //printf("%02X", tmp);
    //printf("\n");
    tmp |= command;
    //printf("%02X", tmp);
    //printf("\n");
    tmp <<= 7;
    //printf("%02X", tmp);
    //printf("\n");
    tmp |= operand;
    //printf("%02X", tmp);
    //printf("\n");
    //tmp |= 0 << 14;
    //printf("%02X", tmp);
    //printf("\n");
    *value = tmp;
    return 0;
}


int sc_commandDecode (int16_t value, int8_t * command, int8_t * operand)
{
    if ((value & (1 << 14)) != 0)
    {
        sc_regSet(FLG_WRONG_COMMAND, 1);
        return COMMAND_ERROR;
    }
    //+проверка
    //*operand = (int8_t)(value);
    //*operand &= ~(1 << 7);  //убрать
    *operand = value & 127;
    *command = (int8_t)(value >> 7);
    return 0;
}
