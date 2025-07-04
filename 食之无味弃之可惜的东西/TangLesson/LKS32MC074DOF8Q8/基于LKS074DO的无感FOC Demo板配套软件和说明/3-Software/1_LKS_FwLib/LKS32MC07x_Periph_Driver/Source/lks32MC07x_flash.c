 /**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_flash.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� FLASH������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��22�� <td>1.0     <td>HuangMG    <td>����
 * </table>
 */

#include "lks32mc07x_Flash.h"
#include "lks32mc07x_nvr.h"
/**
 * @brief flash����������������
 */
volatile u32 erase_flag = 0;
/**
 * @brief flash��̲�����������
 */
volatile u32 progm_flag = 0;


/**
 *@brief @b ��������:   void EraseSector(u32 adr, u16 nvr)
 *@brief @b ��������:   Flash��������������
 *@see���������ݣ�       ��
 *@param���������      
                        adr��       ������ַ  (һ������512�ֽ�) \n
                        nvr��       Ϊ0x800��ʱ�����NVR, Ϊ0ʱ������Flash \n
                        erase_flag�����ú���ǰ���븳ֵ0x9A0D361F������ִ�в����������������ܷ�
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
			����1��
           erase_flag = 0x9A0D361F;//��������
           EraseSector(0x00000000 , Flash_MAIN);//����flash��main���򼴴洢��������
           erase_flag = 0x00;//��������
      ����2��
           erase_flag = 0x9A0D361F;//��������
           EraseSector(0x00000000 , Flash_NVR);//����flash��NVR���򼴶�����1KB�û��洢��
           erase_flag = 0x00;//��������
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��22�� <td>1.0        <td>HuangMG        <td>����
 * </table>
 */
void EraseSector(u32 adr, u16 nvr)
{
	  u32 key1,key2,key3;
    key1 = 0x0;key2 = 0x0;key3 = 0x0;
	  SYS_PROTECT = 0x7A83;
    SYS_FLSE = 0x8FCA;
    if(erase_flag == 0x9A0D361F)
    { //get flash_cfg addr
      key1   = 0xB00C060A;
      key2   = 0x2A033015;
      key3   = erase_flag ^ key1 ^ key2;
      REG32(key3) &=~0x80008800;//clear erase_en, set to sector, clear nvr bit
      FLASH_ADDR = adr;     
      REG32(key3) |=(0x80000000|nvr);
      key1 = 0x0;key2 = 0x0;key3 = 0x0;
    }

    if(erase_flag == 0x9A0D361F)
    { //get flash_erase addr
      key1   = 0xB003A53A;
      key2   = 0x2A0C9335;
      key3   = erase_flag ^ key1 ^ key2;
      REG32(key3)= 0x7654DCBA;//trig sector erase, FLASH_ERASE
      FLASH_CFG &=~0x80008800;//clear erase_en, set to sector, clear nvr bit
      erase_flag = 0x00000000;
      key1 = 0x0;key2 = 0x0;key3 = 0x0;
    }
    FLASH_CFG &=~0x80008800;//clear erase_en, set to sector, clear nvr bit
    erase_flag = 0x00000000;
		SYS_FLSP = 0;
		SYS_PROTECT = 0;
    key1 = 0x0;key2 = 0x0;key3 = 0x0;
}

/**
 *@brief @b ��������:   int ProgramPage(u32 adr, u32 sz, u8 *buf, u16 nvr)
 *@brief @b ��������:   Flash��̲���
 *@see���������ݣ�       ��
 *@param���������      
                        adr��������ַ   \n
                        sz : ����ֽ�����(0~512�ֽ�) \n
                        buf: Ҫ��̵���������ָ�� \n
                        nvr��       Ϊ0x800��ʱ����NVR, Ϊ0ʱ�����Flash \n
                        progm_flag�����ú���ǰ���븳ֵ0x9AFDA40C������ִ�б�̣������������ܷ� 
 *@param���������      ��
 *@return�� �� ֵ��     1����̳ɹ���  0: ���ʧ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
      u8 Flash_Buff[2] = {0x12,0x23};
			����1��
           erase_flag = 0x9A0D361F;//��������
           EraseSector(0x00000000 , Flash_MAIN);//����flash��main���򼴴洢��������
           erase_flag = 0x00;//��������

           progm_flag = 0x9AFDA40C;//��̽���
           ProgramPage(0x00000000 ,2,Flash_Buff, Flash_MAIN);//���flash��main���򼴴洢��������
           erase_flag = 0x00;      //�������
      ����2��
           erase_flag = 0x9A0D361F;//��������
           EraseSector(0x00000000 , Flash_NVR);//����flash��NVR���򼴶�����1KB�û��洢��
           erase_flag = 0x00;//��������

           erase_flag = 0x9A0D361F;//��̽���
           EraseSector(0x00000000,2,Flash_Buff,Flash_NVR);  //���flash��NVR���򼴶�����1KB�û��洢��
           erase_flag = 0x00;      //�������
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��22�� <td>1.0        <td>HuangMG        <td>����
 * </table>
 */
int ProgramPage(u32 adr, u32 sz, u8 *buf, u16 nvr)
{
    volatile u32 Addr, size;
    volatile u8 *p;
    volatile u8 t_rlt = 1;

    u32 key1,key2,key3;
    /* Add your Code */
    key1 = 0x0;key2 = 0x0;key3 = 0x0;
	  SYS_PROTECT = 0x7A83;
    SYS_FLSP = 0x8F35;
    if(progm_flag == 0x9AFDA40C)
    {
      //get flash_cfg addr
      key1   = 0x2A5CA00F;
      key2   = 0xB0A30403;

      key3   = progm_flag ^ key1 ^ key2;
      REG32(key3) &=~0x80008000;
      FLASH_ADDR = adr;     
      REG32(key3) |=(0x08000000|nvr);
      progm_flag = 0;
      key1 = 0x0;key2 = 0x0;key3 = 0x0;

      p    = buf;
      size = (sz + 3) & ~3;                  // align the word
      Addr = adr;
      while(size)
      {
        FLASH_ADDR = (Addr & ~0x00000003);   // address alignment
        FLASH_WDATA= *p + (*(p+1)<<8) + (*(p+2)<<16) + (*(p+3)<<24);            // write data to flash

        p    += 4;
        size -= 4;                           // Go to next word
        Addr += 4;
      }

      p    = buf;
      size = (sz + 3) & ~3;                  // align the word
      Addr = adr;
      while(size)
      {
        u32 t_RData;
        FLASH_ADDR = (Addr & ~0x00000003);   // address alignment
        t_RData = *p + (*(p+1)<<8) + (*(p+2)<<16) + (*(p+3)<<24);
        if(t_RData != FLASH_RDATA)
        {
            t_rlt = 0;
        }

        p    += 4;
        size -= 4;
        Addr += 4;
      }

    }
    FLASH_CFG &= ~0x08000800;
    key1 = 0x0;key2 = 0x0;key3 = 0x0;
    progm_flag = 0;
		SYS_FLSP = 0;
		SYS_PROTECT = 0;
    return (t_rlt);
}


/**
 *@brief @b ��������:   u32 Read_Flash(uint32_t adr , u16 nvr)
 *@brief @b ��������:   ��ȡFLASH���ݺ���
 *@see���������ݣ�       ��
 *@param���������      
                        adr��       ������ַ  (һ������512�ֽ�) \n
                        nvr��       Ϊ0x800��ʱ�����NVR, Ϊ0ʱ������Flash \n
 *@param���������      ��
 *@return�� �� ֵ��     ��ȡ������ֵ
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
			����1��
           u32 Flash_main = 0;
           Flash_main = Read_Flash(0x00000000 , Flash_MAIN);//��ȡflash��main����0x00000000��ַһ��word����
      ����2��
           u32 Flash_nvr = 0;
           Flash_nvr = Read_Flash(0x00000000 , Flash_NVR);//��ȡflash��NVR����0x00000000��ַһ��word����
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��22�� <td>1.0        <td>HuangMG        <td>����
 * </table>
 */
u32 Read_Flash(uint32_t adr , u16 nvr)
{
   uint32_t data;
	 if(nvr == Flash_MAIN) //��ȡMIAN����
	 {
	   FLASH_CFG &= ~BIT11;
	   FLASH_ADDR = adr;
	   data = FLASH_RDATA;
     FLASH_CFG &= ~BIT11;  //�ָ�MIAN����		 
	 }else{
	   data =  Read_NVR(adr);
	 } 
	 return data;
}

/**
 *@brief @b ��������:   void Read_More_Flash(u32 adr, u32 sz, u32 *buf, u16 nvr)
 *@brief @b ��������:   ��ȡFLASH���ַ���ݺ���
 *@see���������ݣ�       ��
 *@param���������      
                        adr��       ������ַ  (һ������512�ֽ�) \n
                        nvr��Ϊ0x800����ȡMAIN���ݣ�Ϊ0x0����ȡNVR���� \n
                        buf:��ȡ���ݴ洢�ĵ�ַ \n
                        sz :��ȡsz����
 *@param���������      ��
 *@return�� �� ֵ��     ��ȡ������ֵ
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
           u32 Flash_Read_Buff[4] = {0};
			����1��   
           Read_More_Flash(0x00000000 ,4,Flash_Read_Buff,Flash_MAIN);//��ȡflash��main����0x00000000��ַ4��word����
      ����2��
           Read_More_Flash(0x00000000 ,4,Flash_Read_Buff,Flash_NVR);//��ȡflash��NVR����0x00000000��ַ4��word����
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��22�� <td>1.0        <td>HuangMG        <td>����
 * </table>
 */
void Read_More_Flash(u32 adr, u32 sz, u32 *buf, u16 nvr)
{
	 volatile u32 Addr = 0;
   volatile u32	size = 0;
   volatile u32 *p;
	 size = sz;
	 Addr = adr;
	 p  = buf;
	 while(size)
	 {	 
		 *p = Read_Flash(Addr , nvr);
		 while(!(FLASH_READY&BIT0)){};
	   size -= 1;  
		 Addr += 4;
		 p += 1;		 
	 }
}


