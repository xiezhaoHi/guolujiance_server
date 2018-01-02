#ifndef _PROTOCOL_DATA_DESC_H_
#define _PROTOCOL_DATA_DESC_H_


// 工作状态
typedef enum _E_STATE
{
    NORMAL      = 0,// 正常
    ALARM_LOW   = 1,// 低段报警
    ALARM_HIGH  = 2,// 高段报警
}E_STATE;


// 气体名称
typedef enum _E_GAS_NAME
{
    NONE = 0x00,
    EX   = 0x01,
    C0   = 0x02,
    O2   = 0x03,
    H2   = 0x04,
    CH4  = 0x05,
    C3H8 = 0x06,
    CO2  = 0x07,
    O3   = 0x08,
    H2S  = 0x09,
    SO2  = 0x0A,
    NH3  = 0x0B,
    C12  = 0x0C,
    ETO  = 0x0D,
    HCL  = 0x0E,
    PH3  = 0x0F,

    HBR  = 0x10,
    HCN  = 0x11,
    ASH3 = 0x12,
    HF   = 0x13,
    BR2  = 0x14,
    NO   = 0x15,
    NO2  = 0x16,
    NOX  = 0x17,
    CLO2 = 0x18,
    SIH4 = 0x19,
    CS2  = 0x1A,
    F2   = 0x1B,
    B2H6 = 0x1C,
    GEH4 = 0x1D,
    N2   = 0x1E,
    THT  = 0x1F,

    C2H2 = 0x20,
    C2H4 = 0x21,
    CH2O = 0x22,
    LPG  = 0x23,
    HC   = 0x24,
    C6H6 = 0x25,
    H2O2 = 0x26,
}E_GAS_NAME;


// 测量单位
typedef enum _E_MEASUREMENT
{
    PERCENT_LEL = 0x00,// %LEL
    PERCENT_VOL = 0x01,// %VOL
    PPM         = 0x02,// PPM
    PPB         = 0x03,// PPb
    NONE_       = 0x04,// 无单位
};


// 传入气体类型编码，获取气体名称显示字符串
inline char * GetGasDisplayNameByCode(int code)
{
    typedef struct _T_GAS_CODE_NAME_PAIR
    {
        int     code;
        char  * name;
    }T_GAS_CODE_NAME_PAIR;
    
    static T_GAS_CODE_NAME_PAIR pairs[] = {
        {NONE, "NONE"},
        {EX, "EX"},
        {C0, "C0"},
        {O2, "O2"},
        {H2, "H2"},
        {CH4,"CH4"},
        {C3H8, "C3H8"},
        {CO2, "CO2"},
        {O3, "O3"},
        {H2S, "H2S"},
        {SO2, "SO2"},
        {NH3, "NH3"},
        {C12, "C12"},
        {ETO, "ETO"},
        {HCL, "HCL"},
        {PH3, "PH3"},

        {HBR, "HBR"},
        {HCN, "HCN"},
        {ASH3, "ASH3"},
        {HF, "HF"},
        {BR2, "BR2"},
        {NO, "NO"},
        {NO2, "NO2"},
        {NOX, "NOX"},
        {CLO2, "CLO2"},
        {SIH4, "SIH4"},
        {CS2, "CS2"},
        {F2, "F2"},
        {B2H6, "B2H6"},
        {GEH4, "GEH4"},
        {N2, "N2"},
        {THT, "THT"},

        {C2H2, "C2H2"},
        {C2H4, "C2H4"},
        {CH2O, "CH2O"},
        {LPG,  "LPG"},
        {HC, "HC"},
        {C6H6, "C6H6"},
        {H2O2, "H2O2"},
    };
    
    static int count = sizeof(pairs) / sizeof(T_GAS_CODE_NAME_PAIR);
    for (int i = 0; i < count; i++) {
        if (pairs[i].code == code) {
            return pairs[i].name;
        }
    }

    return "NONE";
}

// 传入单位编码，获取单位显示字符串
inline char * GetMeasurementDisplayNameByCode(int code)
{
    typedef struct _T_MEASUREMENT_CODE_NAME_PAIR
    {
        int     code;
        char  * name;
    }T_MEASUREMENT_CODE_NAME_PAIR;

    static T_MEASUREMENT_CODE_NAME_PAIR pairs[] = {
        {PERCENT_LEL, "%LEL"},
        {PERCENT_VOL, "%VOL"},
        {PPM, "PPM"},
        {PPB, "PPb"},
        {NONE_, "NONE"},
    };

    static int count = sizeof(pairs) / sizeof(T_MEASUREMENT_CODE_NAME_PAIR);
    for (int i = 0; i < count; i++) {
        if (code == pairs[i].code) {
            return pairs[i].name;
        }
    }

    return "NONE";
}

#endif//!_PROTOCOL_DATA_DESC_H_