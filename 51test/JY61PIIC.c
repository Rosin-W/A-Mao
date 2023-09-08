sbit JY61P_SDA = P3^6;
sbit JY61P_SCL = P3^7;

unsigned char iic_ReadByte()
{
    unsigned char i = 0;
    unsigned char dat = 0;
 
    SDA = 1;//起始和发送一个字节之后SCL都是0
    delay_us(10);
    for(i = 0; i < 8; i++)//接收8个字节
    {
        SCL = 1;
        delay_us(10);
        dat<<=1;
        dat|=SDA;        //或运算, 读取数据是dat最低位
        dealy_us(10);
        SCL = 0;
        delay_us(10);
    }
    return dat;
}