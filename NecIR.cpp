#include "pxt.h"

//% color=50 weight=80
//% icon="\uf1eb"
namespace teddybotmIRV2 { 
int ir_code = 0x00;
int ir_addr = 0x00;
int data;

int logic_value(){//Judge logical value "0" and "1" sub-function
    uint32_t lasttime = system_timer_current_time_us();
    uint32_t nowtime;
    while(!uBit.io.P16.getDigitalValue());//Low wait
    nowtime = system_timer_current_time_us();
    if((nowtime - lasttime) > 400 && (nowtime - lasttime) < 700){//Low level 560us
        while(uBit.io.P16.getDigitalValue());//Is high just wait
        lasttime = system_timer_current_time_us();
        if((lasttime - nowtime)>400 && (lasttime - nowtime) < 700){//Then high level 560us
            return 0;
        }else if((lasttime - nowtime)>1500 && (lasttime - nowtime) < 1800){//Then high level 1.7ms
            return 1;
       }
    }
uBit.serial.printf("error\r\n");
    return -1;
}

void pulse_deal(){
    int i;
    ir_addr=0x00;//Cleared
    for(i=0; i<16;i++ )
    {
      if(logic_value() == 1)
      {
        ir_addr |=(1<<i);
      }
    }
    //Parse the command command in the remote control encoding
    ir_code=0x00;//Cleared
    for(i=0; i<16;i++ )
    {
      if(logic_value() == 1)
      {
        ir_code |=(1<<i);
      }
    }

}

void remote_decode(void){
    data = 0x00;
    uint32_t lasttime = system_timer_current_time_us();
    uint32_t nowtime;
    while(uBit.io.P16.getDigitalValue()){//High wait
        nowtime = system_timer_current_time_us();
        if((nowtime - lasttime) > 100000){//More than 100 ms, indicating that no key is pressed at this time
            ir_code = 0xff00;
            return;
        }
    }
    //If the duration of high level does not exceed 100ms
    lasttime = system_timer_current_time_us();
    while(!uBit.io.P16.getDigitalValue());//Low wait
    nowtime = system_timer_current_time_us();
    if((nowtime - lasttime) < 10000 && (nowtime - lasttime) > 8000){//9ms
        while(uBit.io.P16.getDigitalValue());//High wait
        lasttime = system_timer_current_time_us();
        if((lasttime - nowtime) > 4000 && (lasttime - nowtime) < 5000){//4.5ms, the infrared protocol header is received and it is newly sent data. Start parsing logic 0 and 1
            pulse_deal();
            //uBit.serial.printf("addr=0x%X,code = 0x%X\r\n",ir_addr,ir_code);
            data = ir_code;
            return;//ir_code;
        }else if((lasttime - nowtime) > 2000 && (lasttime - nowtime) < 2500){//2.25ms, which means the sent is consistent with the previous package
            while(!uBit.io.P16.getDigitalValue());//Low wait
            nowtime = system_timer_current_time_us();
            if((nowtime - lasttime) > 500 && (nowtime - lasttime) < 700){//560us
                //uBit.serial.printf("addr=0x%X,code = 0x%X\r\n",ir_addr,ir_code);
                data = ir_code;
                return;//ir_code;
            }
        }
    }
}

 //% 
int irCode(){
    remote_decode();
    return data;
}

//%
int readPulseIn(int status){
  uint32_t lasttime,nowtime,temp;
  if(status == 1){//HIGH
        lasttime = system_timer_current_time_us();
	while(!uBit.io.P2.getDigitalValue()){ 
           temp = system_timer_current_time_us();
           if((temp - lasttime) > 70000){
	     //uBit.serial.printf("time out 0 %d\r\n",(temp-lasttime));
	     return -1;
           }
        }
        lasttime = system_timer_current_time_us();
	while(uBit.io.P2.getDigitalValue()){
           if((system_timer_current_time_us() - lasttime) > 70000){
	     //uBit.serial.printf("time out 1");
	     return -1;
           }
        }
        nowtime = system_timer_current_time_us();
	
  }else{//LOW
	while(uBit.io.P2.getDigitalValue()){
           if((system_timer_current_time_us() - lasttime) > 70000){
	     //uBit.serial.printf("time out 3");
	     return -1;
           }
        }
	lasttime = system_timer_current_time_us();
	while(!uBit.io.P2.getDigitalValue()){
           if((system_timer_current_time_us() - lasttime) > 70000){
	     //uBit.serial.printf("time out 4");
	     return -1;
           }
	}
	nowtime = system_timer_current_time_us();
  }
  return (nowtime - lasttime);
}

}