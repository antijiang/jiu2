				   /*这个程序是用在浩普公司酒精探测器汽车信号处理的控制软件设计要求
是：
 酒精检测器软件工作原理
这个酒精检测器是为了解决"酒后驾车"而设计，让驾驶者，嘴吹传感器，没
有喝酒就可以开动汽车，否则就无法开动汽车。一个酒精传感器，和一个驻集
体话筒放在一起，吹气的时候话筒电路发出一系列方波，酒精传感器，的输出
电压会按照一定的规律变化，如果检测到话筒电路的方波，同时有检测到酒精
传感器没有酒精信号，就发射信号开动汽车电路。

酒精传感器工作原理
发送模块：
当按扭按下，单片机被复位，开始工作，完成以后关闭所有电源，进入HALT
状态。整机电流小于10微安。
接收模块：
通电开始工作，断开汽车电路，使汽车无法工作，知道接收到，发射模块
的正确信号以后，才闭合汽车电路，汽车才能工作。

本项目的几个主要单元电路
1 话筒电路。由一个三极管构成。用单片机去检测那个端口的电平，如果
发现了很多高，和很多低，就说明是嘴吹到，这样检测的间隔时间定为
100HZ，如果在检测的3秒内，检测到低电平超过10次，就可以认为这个
信号是嘴吹的效果。
2 酒精探测电路。这个电路由一个LED酒精传感器实现，先对该传感器，功
电7秒，然后变成100HZ方波，再3秒开始检测，如果信号，电压在正常范围
就是没有酒精，否则，就是有问题。
3 射频编码发射电路。手持机，在检测到吹了而且没有酒精以后，发送一个
信号给控制盒，如果发送码和，以前盒内的学习码相同，就开通汽车。
4 射频码学习和识别电路。控制盒收到射频码，如果这个时候有学习按扭，就
将接收到的码保存在EEP中。
5 射频发射和接收电路。要求无线通讯距离10米足够了。低电压发射，直放
式接收。
6 编码方案，为了提高这个项目的可扩展性，使用24位的数据方案。3字
节，最后4位为功能键功能位，前20位为编码号码位。20位可以表达到100万
个不同的编码。烧写到发送单片机内。

编程原理：
1 麦克风信号计数原理，这个信号接在外部中断端口，外部中断对此进行计数
测量完毕，检查计数值，如果计数值大于10表示是嘴吹的效果。
2 以10MS为单位，做延时计数，开始测量，前7秒一直供电，3秒为100HZ方波，
然后允许嘴吹，5秒后，开始测量，如果测量到的传感器电压小于预定值，而且
麦克风计数大于10，就认为，酒精超标。如果计数合格，但是电压正常，就
认为，没有问题，就发送开车信号。
3 信号采样方案，对2.5V进行采样,并对传感器电压进行采样,计算其比值,比值大
说明电压大,比值小说明电压小.
4 为了降低产品的电流，和成本使用内部RC震荡器，这样晶震的频率是8M
使用了8分频的选项，将系统时钟定位为1M，这样单片机的电流将
为1MA左右。
这个版本，是已经启动了，学习发射功能的版本
这个版本以前是用有源蜂鸣器,这里用无源蜂鸣器,来做出有酒精
和没有酒精的不同的音调
这个版本的程序，修改了，吹起延时时间，避免摔发送器，来
实现通过
电路上这样改动以后，在同样的吹气条件下，单片机接收到的脉冲数变成，10倍

但是如果用户连续敲打，发现还是会产生足够的脉冲数，无法和吹起区分

又把程序做了改动，将接受吹气的时间缩短到1秒，错过这个时间，吹气，不起作用，敲打也不起作用，就是红灯刚亮的那一秒，这样，只够敲打一次，就可以区分开，是敲打，还是吹气了，吹气的次数大于，敲打。

这个程序，在话筒电路滤波的电阻，取值，由原来的104降低到102
这样脉冲数不会大幅度的减少防止敲击造成通过

在这个程序中，增加了对吹气的判断，吹气计数分为6个段
要求其中有4段的计数值大于6，而且总和大于50，这样可以
降低对吹气的压力要求，但是增加了时间的要求

产品装壳以后麦克风的灵敏度变成非常低，所以做降低灵敏度处理

为了解决吹一次不必能通过的问题，在这个程序里做了这样的
功能，在吹的时候会一直延时到9秒，如果吹过了，就跳出延时
否则在这个时间内吹过都是有效的

这个程序将传感器的加热时间从8秒改为6秒

这个程序在电路中加入了NTC电阻，吹气的时候，测量前后阻值的变化
如果阻值有向35度变化，就认为是吹气造成的

NTC在调试的时候要稳定工作，需要供电稳定，
 射频发射接收学问很大，电池供电，电压降低，发射功率降低，容易造成
 接收距离的变化，为了提高稳定性，在发射码前增加了前导脉冲，保证
 码的第一个上升沿精确，需加入适当的滤波电路，

为了提高产品的一致性，和对接收板的容错能力，准备使用发射器晶体振荡器
提高接收的准确性，可以这样做接收算法，首先对前导码，进行计数，6个码
计数个数超过3个就认为可以接收，否则不做接收，这样就可以滤掉，不能
接收的信号

*/
#include <iom48v.h>
#include <eeprom.h>
#include <macros.h>
#define sleep()     asm("sleep")
#define set_bit(x,y)  (x|=(1<<y))
#define clr_bit(x,y)  (x&=~(1<<y))
#define dealarm  3
#define NOP()   asm("nop")  // NULL_operation()
#define maikeshu 18   

//portb定义

#define hongled  0  //红发光管输出
#define lvled    1  //绿发光管输出
#define maikefeng    2   //麦克风电源输出
//portd定义
#define jiujingyuan  0  //酒精传感器电源
#define buzzy    1      //蜂鸣器输出

#define jishu    2  //麦克风计数输入
#define shepin    4  //射频发射输出
//portC定义
#define vin  0           //酒精传感器电源输入
#define jiujingin    1      //酒精传感器输入
#define fasong    4      //学习发送

#define biaoding    5   //酒精标定

#define alarm  4
#define shandeng  0
#define alarmout  2
#define dealarmout  1
#define jiujingbaonongdu  910  
#define qishuge  10 
#define  addcout 0  
//酒精的报警浓度，999最高，
unsigned int adc_rel,maikefengcount,qichang,qiwen1,qiwen2;//AD转换结果
//unsigned int adcout1,adcout2;
unsigned char adc_mux, adcover;
unsigned char i,j,k,iw,jw;
   unsigned char  state,qishu;
   //unsigned char  timefen;
   long jiujingsheding;
   long jiujingnongdu,adcout1,adcout2;;
   //这两个数字的设置不能大于255，第一个数字可以为奇数，后一个数字必须为偶数
const unsigned char sendcode[2]={102,114};//射频发送码

const unsigned char er[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char T0T,T0C;
/*              微秒级延时程序	  		*/
void delay_us(int time)
     {     
      do
      	{
         time--;
    	}	
      while (time>1);
     }     
/*              毫秒级延时程序  		*/	 
void delay_ms(unsigned int time1)
     {
      while(time1!=0)
          {		
           delay_us(250);
           time1--;
          }
     }	
//-------------
//在这里使用定时器0，来做音调的发生器，因为TIMER2有异步时钟功能
//也许将来可以用到
//----------------------------
void timer0_init(void)
{
 TCCR0B|= (1<<CS01);//8分频
 //OCR2=0;TCNT2
T0T=0xff-30;
T0C=0xff-5;

 TIMSK0|=(1<<TOIE0)|(1<<OCIE0A);//溢出和输出比较中断有效
}
void timer0_close(void)
{
TIMSK0&=~(1<<TOIE0);
TIMSK0&=~(1<<OCIE0A);
clr_bit(PORTD,buzzy) ;// 关开蜂鸣,因为这个函数，是响完以后调用
//对于无源蜂鸣器,在这里关闭非常有意义
}
#pragma interrupt_handler timer0_ovf_isr:iv_TIMER0_OVF
void timer0_ovf_isr(void)
{
    TCNT0=T0T;
    clr_bit(PORTD,buzzy) ;// 关开蜂鸣
    OCR0A=T0C;
}
#pragma interrupt_handler timer0_COMP_isr:iv_TIMER0_COMPA
void timer0_COMP_isr(void)
{
    set_bit(PORTD,buzzy) ;//开蜂鸣
}   	 
//------------------------------------
//----------------------------wei>tou
void xiajiangyin(unsigned char tou,unsigned char wei)
{
unsigned char i;
 timer0_init();
 
 for(i=tou;i<wei;i++)
{
    
 T0T=0xff-i;
 T0C=0xff-10;

 
 delay_ms(3);
 }
 
 timer0_close();
 //clr_bit(PORTA,speaker);
}
//----------------------------tuo>wei
void shangshengyin(unsigned char tou,unsigned char wei)
{
unsigned char i;
 timer0_init();
 
 for(i=tou;i>wei;i--)
{
    
 T0T=0xff-i;
  T0C=0xff-10;

 
 delay_ms(3);
 }
 
 timer0_close();
 //clr_bit(PORTA,speaker);
}    
     
     
//-----------------------------
//端口初试化程序
void portint(void){
PORTD=0X00;
  DDRD=0X13;//D口为输入，输出口，
    DDRB=0X07;//B口为输出口
    
   DDRC=0X00;//C口为输入口，作为水位的测量，和温度的测量口，电源监测
   PORTC=0XF8;//打开端口上拉电阻
  }  
     
//----------------------------- 
void adc_init(void)
{
 //DDRC=0x00;
 //portint();
 //PORTC=0x00; 
 ADCSRA = 0x00; 
 ADMUX =0x00; 
 ADMUX =0x40|(adc_mux&0x0f);//选择AVCC为基准
 ACSR  =(1<<ACD);//关闭模拟比较器
 ADCSRA=(1<<ADEN)|(1<<ADSC)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0) ;//32分频
}
//ADC完成中断
#pragma interrupt_handler adc_isr:iv_ADC     
void adc_isr(void)
{
 adc_rel=ADC&0x3ff;
 adcover=1;
  //adc_rel1=ADCL;
 //adc_rel=ADCH;
 //ADMUX =(1<<REFS1)|(1<<REFS0)|(adc_mux&0x0f);
 //nop();
 //
 //ADMUX=adc_mux&0x0f;//选择out部AVCC为基准(1<<REFS0)|(1<<REFS1)|
 //ADCSRA|=(1<<ADSC);//启动AD转换ADCSRA|=(1<<ADSC)|(1<<ADIE);//启动AD转换
}
//ADC结果转换成电压值
//--------------------------------------------------
//INT0用来做麦克风的计数判断
void int_init(void)//在这里初始化所有的外部中断
{
EICRA=0x01;//外部中断0,下降沿中断

EIMSK=0x01;//外部中断0使能
}
//-------------------------------
//这里做外部中断0服务程序,外部中断上升沿引起中断,
//平时震动传感器出于低电平
#pragma interrupt_handler int0_isr:iv_INT0

void int0_isr(void)
{
//clr_bit(PORTD,kong1);//} //have chenged

maikefengcount++;

} 


//-------------------
//这个子程序产生100HZ电源信号,
void g100hz(unsigned char time){
unsigned int i;
  for(i=0;i<10*time;i++)//在这里做电平宽度检测
          {
    if(maikefengcount>50)	{
clr_bit(PORTD,buzzy) ;//开蜂鸣
   }  
    	  
  set_bit(PORTD,jiujingyuan) ;//开酒精传感器电源
  delay_us(620);
  clr_bit(PORTD,jiujingyuan) ;//关酒精传感器电源
  delay_us(620);
  if(i%100==0){
  set_bit(PORTB,lvled) ;//开绿灯
  }
  else if(i%100==50){
   clr_bit(PORTB,lvled) ;//开绿灯
  }
  
  }
  clr_bit(PORTB,lvled) ;//开绿灯
}
//这个子程序产生100HZ电源信号,
void g100hz1(unsigned int time){//qichang,qishu
unsigned int i,i5;
unsigned int stk;
qishu=0;//清气数
stk=0;
  int_init();
  for(i=0;i<10*time;i++)//在这里做电平宽度检测
          {
    
    if(time<40) {  
  set_bit(PORTD,jiujingyuan) ;//开酒精传感器电源
  delay_us(620);
  clr_bit(PORTD,jiujingyuan) ;//关酒精传感器电源
  delay_us(620);
  }
  else{
  delay_us(620);
    clr_bit(PORTD,jiujingyuan) ;//关酒精传感器电源
    delay_us(620);
  }
  
  
  if(i<10*time){
  
  
  if(i%50==0){
  set_bit(PORTB,lvled) ;//开绿灯
  }
  else if(i%50==25){
   clr_bit(PORTB,lvled) ;//开绿灯
  }
  }
  else{
  clr_bit(PORTB,lvled) ;//开绿灯
  }
  /*
  if((i>time/2&&i<time)||(i>3*time/2&&i<2*time)||(i>5*time/2&&i<3*time)||(i>7*time/2&&i<4*time))
  {
  int_init();//maikefengcount
  }
  else{
  EIMSK=0x00;//外部中断关闭
  
  }
  */
 
  
  //在这段时间如果计数数字超过了5就认为连续
  i5=i%3;
  
  if(i5==0)
  {

  if(maikefengcount>qichang+addcout){
  qishu++;
  }
  qichang=maikefengcount;//记录起数
  
  }
  
  
  
  
  
  
  /*
 
  if(i==7*time/2)
  {

  if(maikefengcount>qichang+addcout){
  qishu++;
  }
  qichang=maikefengcount;//记录起数
  
  }
  if(i==8*time/2)
  {

  if(maikefengcount>qichang+addcout){
  qishu++;
  }
  qichang=maikefengcount;//记录起数
  
  }
  if(i==9*time/2)
  {

  if(maikefengcount>qichang+addcout){
  qishu++;
  }
  qichang=maikefengcount;//记录起数
  
  }
  */
  //--------------
  /*if(i%50>1){//这里开麦克风，关蜂鸣器
  
  //set_bit(PORTB,maikefeng) ;//开麦克风电源
  clr_bit(PORTD,buzzy) ;//关蜂鸣 
   SEI();
  }
  else {
  CLI();
  if(maikefengcount>maikeshu)   {
clr_bit(PORTD,buzzy) ;//开蜂鸣
   } 
   else{ 
   //set_bit(PORTB,maikefeng) ;//guan麦克风电源
  set_bit(PORTD,buzzy) ;//kai蜂鸣  
    }
  }
  */
  
  if(maikefengcount>maikeshu&&qishu>qishuge){
  
  set_bit(PORTD,buzzy) ;//开蜂鸣
  delay_ms(50);
    clr_bit(PORTD,buzzy) ;//开蜂鸣
     
  
  clr_bit(PORTB,lvled) ;//开绿灯
   clr_bit(PORTB,hongled) ;//关红灯
   break;
     }
  
  }
  clr_bit(PORTB,lvled) ;//开绿灯
  clr_bit(PORTD,buzzy) ;//开蜂鸣
}
//----------------------
//----------------------
//射频信号发送子程序,这个子程序负责发送一个保存好射频编码
//当学习键按下,为了保证稳定性，可以采取100HZ发射，但是
//这个通讯的距离很近，信号的质量是很好的，所以可以采取
//1K，为了稳定性还是使用100HZ的发射频率
//为了实现发送信号即能开机，又能关机的效果，决定这个发射信号
//如果和学习码相同就开机，如果是FF，FF，FF就表示关机，
//这样可以实现一个效果，是如果没有喝酒上车，吹了可以开车
//在车上喝了酒，再吹就会关掉。这个功能可以考虑要和不要
//但是这里做出来。
void shepinfasong(char kaiguan)
{unsigned char i,j;
unsigned char data[2];
if(kaiguan==1)//表示开机
{
data[0]=sendcode[0];
data[1]=sendcode[1];
}
else{
data[0]=sendcode[0];
data[1]=sendcode[1]-1;
}

set_bit(PORTD,shepin) ;
    delay_us(1000);
clr_bit(PORTD,shepin) ;
    delay_us(1000);
set_bit(PORTD,shepin) ;
    delay_us(1000);
clr_bit(PORTD,shepin) ;
   delay_us(1000);
set_bit(PORTD,shepin) ;
    delay_us(1000);
clr_bit(PORTD,shepin) ;
    delay_us(1000);
   
for(j=0;j<2;j++)//
          {
for(i=0;i<8;i++)//
          {
    	  
  k=data[j]&er[i]; 
   if(k==0){//1/3高，2/3低表示0
   clr_bit(PORTD,shepin) ;
     delay_us(2000);
   set_bit(PORTD,shepin) ;
    delay_us(2000);
     clr_bit(PORTD,shepin) ;
     delay_us(2000);
   
     }
     else{
     clr_bit(PORTD,shepin) ;
     delay_us(1000);
      set_bit(PORTD,shepin) ;
    delay_us(4000);
    clr_bit(PORTD,shepin) ;
     delay_us(1000);
     
     
     }
    	  
          }
          }
}
//--------------------------
void gotosleep(void){
  PORTD=0X00;
  DDRD=0X00;//D口为输入，输出口，
  DDRB=0X00;//B口为输出口
  PORTB=0X00;
   DDRC=0X00;//C口为输入口，作为水位的测量，和温度的测量口，电源监测
   PORTC=0X00;//打开端口上拉电阻
   //关闭端口
 
   //关闭A/D
   ADCSRA=0 ;
   //关闭内部基准电压
   ADMUX=0;
   //设置为掉电电模式
   SMCR=(1<<SE)|(1<<SM1);
  sleep();
   
   
}
//MAIN程序
void main(void)
{
 unsigned int i,j;
  char      mj,ki,mp;
  int ntc;
 int time;
 portint();
 
DDRB=0x07;//5 is in put
 PORTB=0x00;
 jiujingsheding=EEPROMread(1);//开机以后读取标定数据
 
maikefengcount=0;
 EEPROMwrite(1,20);
 delay_ms(20);
 maikefengcount=0;
jiujingsheding=EEPROMread(1);//开机以后读取标定数据
  mj=0;
  mp=0;
  ki=0;
jiujingsheding=jiujingbaonongdu;
  //delay_ms(30);//2.2MS延时
 //while(1)
 // {//delay_ms(8000);//这个延时为0.5秒延时
  //---------------
  //程序在这里开始一个测试过程
  SEI();
  
  shangshengyin(50,20);
set_bit(PORTB,maikefeng) ;//关麦克风电源
  //xiajiangyin(15,50);
  
  
  set_bit(PORTD,jiujingyuan) ;//开酒精传感器电源
       delay_us(2480);
  adcover=0;
       adc_mux=0;
   adc_init();
  ADCSRA|=(1<<ADSC)|(1<<ADIE);//a/d begin启动AD转换
  for(i=0;i<1000;i++)//在这里做电平宽度检测
          {
       delay_us(10);
       if( adcover==1){
       break;
       }
           }
//程序到这里已经测试到2.5V的电源电压
     adcout1=adc_rel;
    if(adcout1>840){//803表示3.12V，853为3V
     for(i=0;i<3;i++)//在这里做电平过低报警
          {
     //set_bit(PORTD,buzzy) ;//开蜂鸣
     set_bit(PORTB,hongled) ;//开红灯
     set_bit(PORTB,lvled) ;//开红灯
     shangshengyin(40,20);
    clr_bit(PORTB,hongled) ;//开红灯
     clr_bit(PORTB,lvled) ;//开红灯
     xiajiangyin(15,50);
     //clr_bit(PORTD,buzzy) ;//开蜂鸣
    
     //delay_ms(50);//7000MS延时
     
     }
     //clr_bit(PORTD,buzzy) ;//开蜂鸣
     while(1)	
  {
   gotosleep();
    gotosleep();
   
    //delay_us(3);
  } //WHILE 函数
     }
  CLI();
  
     set_bit(PORTB,lvled) ;//开绿灯
     
      set_bit(PORTD,jiujingyuan) ;//开酒精传感器电源
//程序在这里原来有一个鸣响程序    
      //set_bit(PORTD,buzzy) ;//开蜂鸣
      //delay_ms(35);
    //clr_bit(PORTD,buzzy) ;//关蜂鸣  
    //delay_ms(35);
      
      //delay_ms(1000);//7000MS延时
      
       for(i=0;i<10;i++)//在这里做电平宽度检测
          {
          set_bit(PORTB,lvled) ;//开绿灯
      delay_ms(35);
       clr_bit(PORTB,lvled) ;//开绿灯
       delay_ms(35);
        }
      
    //在这里做一个判断，如果有学习码发送键按下，就做编码发送，以供学习之用  
      //mp=PINC&0x08;
      mp=0x08;
      if(mp==0){
      
      shepinfasong(1);//由于前面有个2秒的延时，这个发送动作每2秒会动作一次
     
     //set_bit(PORTD,buzzy) ;//开蜂鸣
     //delay_ms(50);//7000MS延时
     //clr_bit(PORTD,buzzy) ;//开蜂鸣
    
     
     
     while(1)	
  {
   gotosleep();
    gotosleep();
   
    //delay_us(3);
  } //WHILE 函数
      }
      else{
    // delay_ms(2000);//7000MS延时
     for(i=0;i<35;i++)//在这里做电平宽度检测
          {
          set_bit(PORTB,lvled) ;//开绿灯
      delay_ms(35);
       clr_bit(PORTB,lvled) ;//开绿灯
       delay_ms(35);
        }
    //maikefengcount=0;
   g100hz(30);
   SEI();//开中断开始计数，只有在这个时候以后吹气才有作用
  //--------------------------- 
  //在这里加入一个程序测量NTC电阻的阻值,如果温度小于35度
//就做下面的测量 ,在这里记录测量到的电阻的阻值,然后,在吹气
//完成以后再次测量电阻的阻值,如果这两次测量,阻值,有明显的变小
//就认为,有吹的动作
//set_bit(PORTB,maikefeng) ;//开麦克风电源
//delay_ms(350);//给与一定的稳定时间
    adcover=0;
       adc_mux=2;
   adc_init();
  ADCSRA|=(1<<ADSC)|(1<<ADIE);//a/d begin启动AD转换
  for(i=0;i<1000;i++)//在这里做电平宽度检测
          {
       delay_us(10);
       if( adcover==1){
       break;
       }
           }
   adcout1=adc_rel;
  //----------------------------- 
   xiajiangyin(20,50);
   
   
   maikefengcount=0;//清计数器
    set_bit(PORTB,hongled) ;//开红灯
    //set_bit(PORTD,buzzy) ;//开蜂鸣
     // delay_ms(35);
    //clr_bit(PORTD,buzzy) ;//关蜂鸣  
    //
    set_bit(PORTB,maikefeng) ;//开麦克风电源
    //int_init();//在这里打开,麦克风计数qichang,qishu
    //delay_ms(35);
   g100hz1(1200);//程序在这里等待吹气
   g100hz(20);//程序在这里延时2秒等待气体反应完毕
   //-----------------------------
   //程序在这里测量NTC的阻值,看看有没有变小,这个判据
   //将作为这次测试的一个依据
   set_bit(PORTB,maikefeng) ;//开麦克风电源
delay_ms(35);//给与一定的稳定时间
    adcover=0;
       adc_mux=2;
   adc_init();
  ADCSRA|=(1<<ADSC)|(1<<ADIE);//a/d begin启动AD转换
  for(i=0;i<1000;i++)//在这里做电平宽度检测
          {
       delay_us(10);
       if( adcover==1){
       break;
       }
           }
  adcout2=adc_rel;
   adcout2=adcout1-adcout2;
   if(adcout1<560)//如果温度超过35度，就通过
   {
    adcout2=10;
   }
     adcout2=10;
  // else{
  // ntc=adcout2;
  // }
   //如果吹气以后温度没有提高,或者测试前温度大于35度就休眠
   //否则继续
   
   if(adcout2<2)
   {
   while(1) 
  {
   gotosleep();
    gotosleep();
   
    //delay_us(3);
  } //WHILE 函数
   
   }
  //上面的程许，测量了吹气的温度，如果吹前温度大于35度，就
//不做测量通过,否则吹气以后数字必须下降足够的数值,也就是 
//温度传感器必须,温度有所提高 
   
   
   
   
   //-----------------------------
    //delay_ms(1000);
   //程序到这里开始检测酒精传感器
    set_bit(PORTD,jiujingyuan) ;//开酒精传感器电源
       delay_us(2480);
       //delay_ms(10);
    //--------------------------   
        adcover=0;
       adc_mux=0;
   adc_init();
  ADCSRA|=(1<<ADSC)|(1<<ADIE);//a/d begin启动AD转换
  for(i=0;i<1000;i++)//在这里做电平宽度检测
          {
       delay_us(10);
       if( adcover==1){
       break;
       }
           }
//程序到这里已经测试到2.5V的电源电压
      adcout1=adc_rel;
    /*if(adcout1>803){//803表示3.12V，853为3V
     for(i=0;i<3;i++)//在这里做电平宽度检测
          {
     set_bit(PORTD,buzzy) ;//开蜂鸣
     set_bit(PORTB,hongled) ;//开红灯
     set_bit(PORTB,lvled) ;//开红灯
     delay_ms(50);//7000MS延时
     //clr_bit(PORTD,buzzy) ;//开蜂鸣
     clr_bit(PORTB,hongled) ;//开红灯
     clr_bit(PORTB,lvled) ;//开红灯
     delay_ms(50);//7000MS延时
     
     }
     clr_bit(PORTD,buzzy) ;//开蜂鸣
     gotosleep();
     }*/
  //--------------------------   
        adcover=0;
       adc_mux=1;
       
   adc_init();
   delay_us(480);
  ADCSRA|=(1<<ADSC)|(1<<ADIE);//a/d begin启动AD转换
  for(i=0;i<1000;i++)//在这里做电平宽度检测
          {
       delay_us(10);
       if( adcover==1){
       break;
       }
           }
//程序到这里已经测试到2.5V的电源电压
     clr_bit(PORTD,buzzy) ;//开蜂鸣
      clr_bit(PORTD,jiujingyuan) ;//关酒精传感器电源
     adcout2=adc_rel;
    jiujingnongdu=1000*adcout2/adcout1;
//--------------------------------------
 
// 上面的程序已经测量完毕，下面的程序是做射频的信号发送
  mp=PORTC&0x10;//标定扭，是PC4
   
    if(mp==0){
     // EEPROMwrite( 1, jiujingnongdu);//将测量到的数据保存到EEP
       set_bit(PORTB,lvled) ;//开绿灯
      set_bit(PORTB,hongled) ;//开绿灯
      delay_ms(500);//7000MS延时
      clr_bit(PORTB,lvled) ;//开绿灯
      clr_bit(PORTB,hongled) ;//开绿灯
       delay_ms(500);//7000MS延时
       set_bit(PORTB,lvled) ;//开绿灯
      set_bit(PORTB,hongled) ;//开绿灯
      delay_ms(500);//7000MS延时
      clr_bit(PORTB,lvled) ;//开绿灯
      clr_bit(PORTB,hongled) ;//开绿灯
       delay_ms(500);//7000MS延时
      
    }
   else{//在这里检测到有酒精，而且吹了
   if(maikefengcount>maikeshu&&jiujingnongdu<jiujingsheding&&qishu>qishuge)
    {
     set_bit(PORTB,hongled) ;//开红灯
     clr_bit(PORTB,lvled) ;//开绿灯
    shepinfasong(2);//发送锁车信息
    delay_ms(500);
    shepinfasong(2);//发送锁车信息
    delay_ms(500);
    shepinfasong(2);//发送锁车信息
    for(i=0;i<3;i++)//在这里做有酒精报警
          {
     xiajiangyin(20,50);
     delay_ms(50);//7000MS延时
     
     
     }
     clr_bit(PORTB,hongled) ;//开红灯
    }
    //在这里检测到吹了但是没有酒精
    else if(maikefengcount>maikeshu&&qishu>qishuge){
     clr_bit(PORTB,hongled) ;//开红灯
     set_bit(PORTB,lvled) ;//开绿灯
    shepinfasong(1);//发送开车信息
    delay_ms(500);
    shepinfasong(1);
    delay_ms(500);
    shepinfasong(1);
    for(i=0;i<2;i++)//在这里做无酒精报警
          {
     shangshengyin(60,20);
     delay_ms(50);//7000MS延时
     
     
     }
     delay_ms(700);//7000MS延时
     clr_bit(PORTB,lvled) ;//开绿灯
    }
    NOP();
    
    }
    //作完上面的程序程序进入休眠状态。
     
    gotosleep();
    gotosleep();
    gotosleep();
    //delay_ms(4000);//7000MS延时
    
    
    }
while(1)    
  {
   gotosleep();
    gotosleep();
   
    //delay_us(3);
  } //WHILE 函数
  
  
}//MAin  函数
