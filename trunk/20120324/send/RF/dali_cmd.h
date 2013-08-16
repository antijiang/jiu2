#ifndef _DALI_CMD_CMD_H_
#define _DALI_CMD_CMD_H_

/*

#define DALI_CMD_OFF 0x00
#define DALI_CMD_UP   0x01
#define DALI_CMD_DOWN 0x02
#define DALI_CMD_STEP_UP 0x03
#define DALI_CMD_STEP_DOWN 0x04
#define DALI_CMD_RECALL_MAX_LEVEL 0x05
#define DALI_CMD_RECALL_MIN_LEVEL 0x06
#define DALI_CMD_STEP_OFF  0x07
#define DALI_CMD_STEP_ON 0x08

#define DALI_CMD_GOTO_SCENE 0x10
#define DALI_CMD_REMOVE_SCENE 0x50

#define DALI_CMD_ADD_GROUP	0x60
#define DALI_CMD_REMOVE_GROUP 0x70
#define DALI_CMD_STORE_DTR_AS_SHORT_ADDR 0x80
#define DALI_CMD_QUERY_RAND_ADDR_H 0xC2
#define DALI_CMD_QUERY_RAND_ADDR_M 0xC3
#define DALI_CMD_QUERY_RAND_ADDR_L 0xC4

//SPCIAL COMMAND
#define DALI_CMD_TERMINATE 0xA100
#define DALI_CMD_DTR  0xA300
#define DALI_CMD_INITIALISE	0xA500
#define DALI_CMD_RANDOMISE 0xA700
#define DALI_CMD_COMPARE 0xA900
#define DALI_CMD_WITHDRAW 0xAB00
#define DALI_CMD_SEARCHADDRH 0xB100
#define DALI_CMD_SEARCHADDRM 0xB300
#define DALI_CMD_SEARCHADDRL 0xB500
#define DALI_CMD_PROGRAM 0xB700
#define DALI_CMD_VERIFY 0xB900
#define DALI_CMD_QUERY_SHORT_ADD  0xBB00
#define DALI_CMD_PHYSICL_SELECTION 0xBD00

*/
#define		DALI_YES		0xff	// 返回yes 正确应答
#define		DALI_NO			0	       //无应答
#define	    DALI_ACK		1		 //表示总线有活动但数据无效f_dalirx=0的
#define		DALI_ACT_FLAG	0x8000   //大力总线有活动，但竞争或干扰

// Indirect arc power control commands
#define DALI_OFF                          0
#define DALI_UP                           1
#define DALI_DOWN                         2
#define DALI_STEP_UP                      3
#define DALI_STEP_DOWN                    4
#define DALI_RECALL_MAX                   5
#define DALI_RECALL_MIN                   6
#define DALI_DOWN_AND_OFF                 7
#define DALI_ON_AND_UP                    8
#define DALI_GO_TO_SCENE                  16            // 16 - 31

// General configuration commands
#define DALI_RESET                        32
#define DALI_STORE_ACTUAL                 33
       
// Arc power parameters settings
#define DALI_STORE_MAX                    42
#define DALI_STORE_MIN                    43
#define DALI_STORE_FAILURE                44
#define DALI_STORE_POWER_ON               45
#define DALI_STORE_FADE_TIME              46
#define DALI_STORE_FADE_RATE              47
#define DALI_STORE_SCENE                  64            // 64 - 79
        
// System parameters settings
#define DALI_REMOVE_SCENE                 80            // 80 - 95
#define DALI_ADD_TO_GROUP                 96            // 96 - 111
#define DALI_REMOVE_GROUP                 112           // 112 - 127
#define DALI_STORE_SHORT_ADD              128
        
// Queries related to status information
#define DALI_QUERY_STATUS                 144
#define DALI_QUERY_BALLAST                145
#define DALI_QUERY_LFAILURE               146
#define DALI_QUERY_POWER_ON               147
#define DALI_QUERY_LIMIT_ERR              148
#define DALI_QUERY_RESET                  149
#define DALI_QUERY_MISSING                150
#define DALI_QUERY_VERSION                151
#define DALI_QUERY_CONTENT                152
#define DALI_QUERY_DEVICE                 153
#define DALI_QUERY_MINIMUM                154
#define DALI_QUERY_PFAILURE               155
        
// Queries related to arc power parameters settings
#define DALI_QUERY_ACTUAL                 160
#define DALI_QUERY_MAX_LEVEL              161
#define DALI_QUERY_MIN_LEVEL              162
#define DALI_QUERY_POWER_L                163
#define DALI_QUERY_FAILURE_L              164
#define DALI_QUERY_FADE                   165

// Queries related to system parameters settings
#define DALI_QUERY_SCENE                  176           // 176 - 191
#define DALI_QUERY_GR_0_7                 192
#define DALI_QUERY_GR_8_15                193
#define DALI_QUERY_ADDRESS_H              194
#define DALI_QUERY_ADDRESS_M              195
#define DALI_QUERY_ADDRESS_L              196
       
// Terminate special processes
#define DALI_TERMINATE                    0xA1          // command 256
        
// Download information to the dtr
#define DALI_DATA_TRANS_REG               0xA3

// Addressing commands
#define DALI_INITIALISE                   0xA5
#define DALI_RANDOMISE                    0xA7
#define DALI_COMPARE                      0xA9
#define DALI_WITHDRAW                     0xAB
#define DALI_SEARCHADDRH                  0xB1
#define DALI_SEARCHADDRM                  0xB3
#define DALI_SEARCHADDRL                  0xB5
#define DALI_PROGRAM_SHORT                0xB7
#define DALI_VERIFY_SHORT                 0xB9
#define DALI_QUERY_SHORT                  0xBB
#define DALI_PHYSICAL_SEL                 0xBD
#define DALI_ENABLE_TYPE                  0xC1

#define	DALI_CMD_FOLLOW_BIT		(0x01)
#define	DALI_GROUP_ADDR  			0x80
#define	DALI_BROADCAST_ADDR  		0xff
#define	INII_ADDR_ALL	0
#define	INII_ADDR_NONE	0xff    //未分的分配
#define	INII_ADDR_SAME	0xfe   //相同地址再重新分配
#define	INII_ADDR_ASIGN	0x01
#define	DALI_MAX_LAMP	64
#define	DALI_MAX_GROUP	16

#define	DALI_SPECIAL_ADDR_REG	
#define	DALI_SPECIAL_CMD_REG	

void DCMD_Off(BYTE BSGaddr);

void     DCMD_SetDTR(BYTE dat);
void	    DCMD_SetLevel(BYTE BSGaddr,BYTE level);
void	    DCMD_StoreDTR(BYTE BSGaddr,BYTE cmd,BYTE dat);
void     DCMD_SetFadeTime(BYTE BSGaddr,BYTE fadet);
void     DCMD_SetFadeRate(BYTE BSGaddr,BYTE fade);
void     DCMD_RecallMax(BYTE BSGaddr);
void     DCMD_RecallMin(BYTE BSGaddr);
BYTE 	DCMD_GotoScene(BYTE BSGaddr,BYTE SceneNo);
void DCMD_DimUpDown(BYTE BSGaddr,BYTE IsUp);
void DCMD_DimStepUpDown(BYTE BSGaddr,BYTE IsUp);
void DCMD_DimStepUpDown_OnOff(BYTE BSGaddr,BYTE IsUp);
 void    DCMD_StoreScenFromActualLevel(BYTE BSGaddr,BYTE SceNo);

void DAPI_RandomAllocateAllAddress(void);
BOOL DAPI_AllocateOneAddress(BYTE saddr);

void DaliPhysicalSelect(void);

BYTE 	DCMD_AddToGroup(BYTE BSGaddr,BYTE grp)	;
BYTE 	DCMD_RemoveFromGroup(BYTE BSGaddr,BYTE grp);
BYTE 	DCMD_QueryScene(BYTE saddr,BYTE SceneNo);
BYTE 	DCMD_QueryBallast(BYTE saddr);
WORD 	DCMD_QueryGRPW(BYTE Saddr);
BYTE 	DCMD_QueryActuaLevel(BYTE saddr);
BYTE 	DCMD_QueryMissingShortAddress(BYTE saddr);
void DAPI_RandomAllocateNoneAddress( );
void 
DAPI_RandomAllocateSameAddress(BYTE addr);
#endif

