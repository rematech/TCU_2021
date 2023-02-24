
#include "_include_header.h"

static uint8_t host_if_ch[HIL_MAX];
static uint8_t host_if_uart_ch=0;

static uint8_t hi_msg_buf[HIL_MAX][HI_MSG_MAX];
static uint8_t hi_msg_len[HIL_MAX];

static uint32_t hi_timeout[HIL_MAX];
static uint8_t hi_timeout_flag[HIL_MAX];

static HOST_IF_STATE_LIST hi_curr_state[HIL_MAX];

static uint8_t hi_seq_no=0;


const char* str_hi_ch[]=
{
    "RS232_MCU", 
};

static uint8_t hi_raw_data_echo=0;

uint8_t host_comm_seq=0;

void App_Host_Comm_Seq_Set(uint8_t seq)
{
    host_comm_seq = seq;
}

uint8_t App_Host_Comm_Seq_Get(void)
{
    return host_comm_seq;
}

void App_HI_Data_Echo(uint8_t en)
{
    hi_raw_data_echo = en;
}

void App_HI_Data_Clear(uint8_t hi_ch)
{
    if(hi_msg_len[hi_ch])
    {
        memset(hi_msg_buf[hi_ch],hi_msg_len[hi_ch],sizeof(hi_msg_buf[hi_ch]));
        hi_msg_len[hi_ch] = 0;
    }
    hi_timeout_flag[hi_ch] = FALSE;
}

void App_HI_Data_Update(uint8_t *data,uint32_t len)
{

}

uint32_t App_Host_IF_build_Packet(uint8_t *data, uint32_t len)
{
    uint32_t rlen;
   
    if(data != NULL)
    {
        HOST_IF_PACKET *hi_packet = (HOST_IF_PACKET*)data;
    
        hi_packet->header.packet_id = HI_PACKET_ID;
        hi_packet->header.seq = hi_seq_no;
        hi_packet->header.size = len;
        hi_packet->header.cs = Api_CheckSum_Encoding(&hi_packet->payload.opcode, hi_packet->header.size);
        
        rlen = PACKET_HEADER_LEN;
        rlen += len;
    }
    
    return rlen;
}

void App_Host_IF_Send_Msg(uint8_t opcode, uint8_t *data, uint32_t len)
{
    uint8_t buf[HI_MSG_MAX]={0};
    uint8_t msg_size;
    uint8_t cnt=0;
    bool send_flag = false;
    
    HOST_IF_PACKET *p = (HOST_IF_PACKET*)buf;

    p->header.unit_id = HIL_DEV_ID_TCU;
    
    switch(opcode)
    {
        case HIOP_TCU_REP_BOOT :
            p->payload.opcode = opcode;
            p->payload.data[cnt++] = SW_VER_MAJOR;
            p->payload.data[cnt++] = SW_VER_MINOR;
            p->payload.data[cnt++] = SW_VER_BUILD;

            cnt += PACKET_OPCODE_LEN;
            msg_size = App_Host_IF_build_Packet(buf,cnt);
            
            send_flag = true;

            p->payload.data[cnt++] = _ctrl_Mode[TEMP_CH_1];
        break;

        case HIOP_TCU_RES_DATA :        //3
            p->payload.opcode = opcode;//6
            p->payload.data[cnt++] = max31865_fault[TEMP_CH_1];
            p->payload.data[cnt++] = _ctrl_Mode[TEMP_CH_1];
//            p->payload.data[cnt++] = (pv_integer[TEMP_CH_1]>>8)&0x00FF;
            p->payload.data[cnt++] = pv_integer[TEMP_CH_1]&0x00FF;
            p->payload.data[cnt++] = pv_fraction[TEMP_CH_1];
            p->payload.data[cnt++] = curr_ssr_status[TEMP_CH_1];
            p->payload.data[cnt++] = pwm_percent_Integer[TEMP_CH_1];
            p->payload.data[cnt++] = alarm_status[TEMP_CH_1];

            p->payload.data[cnt++] = (_kp_integer[TEMP_CH_1]>>8)&0x00FF;//14
            p->payload.data[cnt++] = _kp_integer[TEMP_CH_1];
            p->payload.data[cnt++] = _kp_fraction[TEMP_CH_1];
            p->payload.data[cnt++] = (_ki_integer[TEMP_CH_1]>>8)&0x00FF;
            p->payload.data[cnt++] = _ki_integer[TEMP_CH_1];
            p->payload.data[cnt++] = _ki_fraction[TEMP_CH_1];
            p->payload.data[cnt++] = (_kd_integer[TEMP_CH_1]>>8)&0x00FF;
            p->payload.data[cnt++] = _kd_integer[TEMP_CH_1];
            p->payload.data[cnt++] = _kd_fraction[TEMP_CH_1];
            p->payload.data[cnt++] = _outputUpWeight_integer[TEMP_CH_1];
            p->payload.data[cnt++] = _outputUpWeight_fraction[TEMP_CH_1];
            p->payload.data[cnt++] = _outputDownWeight_integer[TEMP_CH_1];
            p->payload.data[cnt++] = _outputDownWeight_fraction[TEMP_CH_1];

            p->payload.data[cnt++] = max31865_fault[TEMP_CH_2];//27
            p->payload.data[cnt++] = _ctrl_Mode[TEMP_CH_2];
//            p->payload.data[cnt++] = (pv_integer[TEMP_CH_2]>>8)&0x00FF;
            p->payload.data[cnt++] = pv_integer[TEMP_CH_2]&0x00FF;
            p->payload.data[cnt++] = pv_fraction[TEMP_CH_2];
            p->payload.data[cnt++] = curr_ssr_status[TEMP_CH_2];
            p->payload.data[cnt++] = pwm_percent_Integer[TEMP_CH_2];
            p->payload.data[cnt++] = alarm_status[TEMP_CH_2];
            p->payload.data[cnt++] = (_kp_integer[TEMP_CH_2]>>8)&0x00FF;
            p->payload.data[cnt++] = _kp_integer[TEMP_CH_2];
            p->payload.data[cnt++] = _kp_fraction[TEMP_CH_2];
            p->payload.data[cnt++] = (_ki_integer[TEMP_CH_2]>>8)&0x00FF;
            p->payload.data[cnt++] = _ki_integer[TEMP_CH_2];
            p->payload.data[cnt++] = _ki_fraction[TEMP_CH_2];
            p->payload.data[cnt++] = (_kd_integer[TEMP_CH_2]>>8)&0x00FF;
            p->payload.data[cnt++] = _kd_integer[TEMP_CH_2];
            p->payload.data[cnt++] = _kd_fraction[TEMP_CH_2];
            p->payload.data[cnt++] = _outputUpWeight_integer[TEMP_CH_2];
            p->payload.data[cnt++] = _outputUpWeight_fraction[TEMP_CH_2];
            p->payload.data[cnt++] = _outputDownWeight_integer[TEMP_CH_2];
            p->payload.data[cnt++] = _outputDownWeight_fraction[TEMP_CH_2];

            cnt += PACKET_OPCODE_LEN;
            msg_size = App_Host_IF_build_Packet(buf,cnt);
            
            send_flag = true;
        break;
        
        case HIOP_TCU_RES_SET_SP :
            p->payload.opcode = opcode;
            p->payload.data[cnt++] = RET_CODE_SUC;
        
            cnt += PACKET_OPCODE_LEN;
            msg_size = App_Host_IF_build_Packet(buf,cnt);
            
            send_flag = true;
        break;

        case HIOP_TCU_RES_SET_ALARM :
            p->payload.opcode = opcode;
            p->payload.data[cnt++] = RET_CODE_SUC;
        
            cnt += PACKET_OPCODE_LEN;
            msg_size = App_Host_IF_build_Packet(buf,cnt);
            
            send_flag = true;
        break;
        
        case HIOP_TCU_REP_EVENT :
            p->payload.opcode = opcode;
            p->payload.data[cnt++] = RET_CODE_SUC;

            cnt += PACKET_OPCODE_LEN;
            msg_size = App_Host_IF_build_Packet(buf,cnt);
            
            send_flag = true;

        break;

        case HIOP_TCU_RES_SET_PID :
            p->payload.opcode = opcode;
            p->payload.data[cnt++] = RET_CODE_SUC;
        
            cnt += PACKET_OPCODE_LEN;
            msg_size = App_Host_IF_build_Packet(buf,cnt);
            
            send_flag = true;
        break;

    }

    if(send_flag == true)
    {
        send_flag = false;
        Hal_Uart_Send(host_if_ch[HIL_MCU],(const uint8_t*)buf,msg_size);
        
    }
}

void App_Host_IF_Send_Boot_Msg(void)
{
    App_Host_IF_Send_Msg(HIOP_TCU_REP_BOOT,NULL,0);
}

void App_Host_IF_Msg_Parser(uint8_t *data)
{
    float kp, ki, kd, up_wgt, down_wgt;

    PACKET_PAYLOAD *p = (PACKET_PAYLOAD*)data;
    
    dmsg(DL_DBG,"%s : %d\r\n",__func__,p->opcode);
    
    switch(p->opcode) 
    {
        case HIOP_TCU_RES_BOOT:     //01
            App_Host_Comm_Seq_Set(HCS_NORMAL);
            
            break;
            
        case HIOP_TCU_REQ_DATA:     //02
            App_Set_SSR_En(p->data[0]);
            
            App_Host_IF_Send_Msg(HIOP_TCU_RES_DATA,p->data,0);
            break;
        
        case HIOP_TCU_REQ_SET_SP:   //04
            App_Set_SP(TEMP_CH_1,p->data[0],p->data[1]);
            App_Set_SP(TEMP_CH_2,p->data[2],p->data[3]);
            App_Set_SP(TEMP_CH_3,p->data[4],p->data[5]);
            
            App_Host_IF_Send_Msg(HIOP_TCU_RES_SET_SP,p->data,0);
            break;

        case HIOP_TCU_REQ_SET_ALARM:    //06
            dev.alarm.lo_en = p->data[0];
            dev.alarm.lo_val = p->data[1];
            dev.alarm.hi_en = p->data[2];
            dev.alarm.hi_val = p->data[3];
            
            App_Host_IF_Send_Msg(HIOP_TCU_RES_SET_ALARM,p->data,0);
            
            break;
        
        case HIOP_TCU_RES_EVENT:

            break;

        case HIOP_TCU_REQ_SET_PID:
            // protocol : ch1, kpInt, kpFrac, kiInt, kdFrac, kdInt, kdFrac, up_wgtInt, up_wgtFrac, down_wgtInt, down_wgtFrac, ch2, ...
            // TEMP_CH_1
            IntCombine(&kp, p->data[0], p->data[1], p->data[2],2);
            IntCombine(&ki, p->data[3], p->data[4], p->data[5],2);
            IntCombine(&kd, p->data[6], p->data[7], p->data[8],2);
            IntCombineWgt(&up_wgt, p->data[9], p->data[10], 2);
            IntCombineWgt(&down_wgt, p->data[11], p->data[12], 2);
            PidSetTunings(&kp, &ki, &kd, &up_wgt, &down_wgt, TEMP_CH_1);

            // TEMP_CH_2
            IntCombine(&kp, p->data[13], p->data[14], p->data[15],2);
            IntCombine(&ki, p->data[16], p->data[17], p->data[18],2);
            IntCombine(&kd, p->data[19], p->data[20], p->data[21],2);
            IntCombineWgt(&up_wgt, p->data[22], p->data[23], 2);
            IntCombineWgt(&down_wgt, p->data[24], p->data[25], 2);
            PidSetTunings(&kp, &ki, &kd, &up_wgt, &down_wgt, TEMP_CH_2);

            App_Host_IF_Send_Msg(HIOP_TCU_RES_SET_PID, p->data, 0);

            break;

    }   
}

void App_Host_IF_Timeout_Update(uint8_t hi_ch)
{
    hi_timeout[hi_ch] = Api_Systick_Get();
    hi_timeout_flag[hi_ch] = TRUE;
}

uint8_t App_Host_IF_Timeout_Check(uint8_t hi_ch)
{
    uint32_t sys_tick = Api_Systick_Get();
    uint32_t check_time = hi_timeout[hi_ch];
    uint32_t elapsed = sys_tick - check_time;
    
    if(elapsed > HI_MSG_TIMEOUT_T)
        return 1; // Timeout !
    else 
        return 0;
}

static uint8_t App_Host_IF_Msg_Update(uint8_t hi_ch)
{
    uint8_t len;
    uint8_t str;
    
    len = Hal_Uart_Rcv_Len(host_if_ch[hi_ch]);
    if(len)
    {
        App_Host_IF_Timeout_Update(hi_ch);
       
        str = Hal_Uart_Get_Char(host_if_ch[hi_ch]);
        hi_msg_buf[hi_ch][hi_msg_len[hi_ch]] = str;
        hi_msg_len[hi_ch]++;
       

        if(hi_raw_data_echo) dmsg(DL_NON,"%02X ",str);

    }
    return len;
}

static void App_Host_IF_State_Machine(uint8_t hi_ch)
{
    HOST_IF_PACKET  *hi_packet;
    
    if(hi_timeout_flag[hi_ch] == TRUE)    
    {
        if(App_Host_IF_Timeout_Check(hi_ch))
        {
            App_HI_Data_Clear(hi_ch);
            hi_curr_state[hi_ch] = HIS_PACKET_ID_L;
            
            dmsg(DL_WRN,"HIF Time Out %d \r\n",hi_ch);
    
            return;
        }
    }
    
    App_Host_IF_Msg_Update(hi_ch);

    hi_packet = (HOST_IF_PACKET*)hi_msg_buf[hi_ch];
    
    switch(hi_curr_state[hi_ch])
    {
        case HIS_PACKET_ID_L:
            
            if((HI_PACKET_ID & 0xFF) == (hi_packet->header.packet_id & 0xFF))
            {
                hi_curr_state[hi_ch] = HIS_PACKET_ID_H;
            }
            else
            {
                App_HI_Data_Clear(hi_ch);
            }
        
            break;

        case HIS_PACKET_ID_H:
           
            if(HI_PACKET_ID == hi_packet->header.packet_id)
            {
                hi_curr_state[hi_ch] = HIS_PACKET_HEADER;
            }
            else
            {
                App_HI_Data_Clear(hi_ch);
                hi_curr_state[hi_ch] = HIS_PACKET_ID_L;
            }
           
            break;

        case HIS_PACKET_HEADER:
            
            if(hi_msg_len[hi_ch] >= PACKET_HEADER_LEN)
            {
                hi_curr_state[hi_ch] = HIS_PACKET_PAYLOAD;
            }
            break;

        case HIS_PACKET_PAYLOAD:

            if(hi_msg_len[hi_ch] >= (hi_packet->header.size + PACKET_HEADER_LEN ))
            {
                hi_curr_state[hi_ch] = HIS_PROCESS_MESSAGE;
            }
            break;

        case HIS_PROCESS_MESSAGE:

            dmsg(DL_DBG,"%s : %d \r\n",__func__,hi_curr_state[hi_ch]);

            if(Api_CheckSum_Decoding(&hi_packet->payload.opcode,hi_packet->header.size,hi_packet->header.cs) == 0)
            {
                dmsg(DL_DBG,"Type : %d \r\n",hi_packet->header.unit_id);
                switch(hi_packet->header.unit_id)
                {
                    case HIL_DEV_ID_TCU:
                        App_Host_IF_Msg_Parser(&hi_packet->payload.opcode);
                    break;

                    default:
                        dmsg(DL_ERR,"INVLAID UIIT TYPE.\r\n");
                    break;
                }
            }
            else
            {
                dmsg(DL_ERR,"%s : CS ERR\r\n",__func__);
            }
            
            App_HI_Data_Clear(hi_ch);
            hi_curr_state[hi_ch] = HIS_PACKET_ID_L;
        break;

        default:
        break;
    }
}

void App_Host_IF_Proc(void)
{
    uint8_t ch;

    for(ch = 0; ch < host_if_uart_ch ; ch++)
    {
        App_Host_IF_State_Machine(ch);
    }
}

void App_Host_IF_Init(uint8_t hi_ch,uint8_t uart_ch,uint8_t priority,uint32_t baudrate,uint8_t parity)
{
    host_if_ch[hi_ch] = uart_ch;

    host_if_uart_ch++;
    App_HI_Data_Clear(hi_ch);
    
    Hal_Uart_Init(host_if_ch[hi_ch],priority,baudrate,parity);
    dmsg(DL_INF,"Host IF:%s = Uart%d\r\n",str_hi_ch[hi_ch],host_if_ch[hi_ch]);

}


