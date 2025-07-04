/**
 * @file
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_dsp.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� DSP�������� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2015��11��5�� <td>1.0     <td>Howlet      <td>����
 * </table>
 *@par �޸���־:DSP���ú����������жϴ�����¸�λDSP����
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��12�� <td>2.0     <td>ywx   <td>�޸�
 * </table>
 */

#include "lks32mc07x_dsp.h"

/**
 *@brief @b ��������:   void DSP_Init(void)
 *@brief @b ��������:   DSPʹ�ܺ͹ر�
 *@see���������ݣ�       SYS_ModuleClockCmd()
 *@param���������       ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��12�� <td>2.0     <td>ywx   <td>�޸�
 * </table>
 */
void DSP_Init(void)
{
    SYS_ModuleClockCmd(SYS_Module_DSP0, ENABLE); /* DSPʱ��ʹ��*/
}

/**
 *@brief @b ��������:   s16 DSP_sin(u16 angle)
 *@brief @b ��������:   DSP sin���㺯��
 *@see���������ݣ�
 *@param���������      angle�� ����Ƕ�
 *@param���������      ��
 *@return�� �� ֵ��     sin�Ƕ�ֵ
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��12�� <td>2.0     <td>ywx   <td>�޸�
 * </table>
 */
s16 DSP_sin(u16 angle)
{
    s16 answer;
    int32_t flg = __get_PRIMASK();
    __disable_irq();
    DSP_REG->SC = BIT2 | BIT1;
    DSP_REG->THETA = angle;
    answer = DSP_REG->SIN;
    __set_PRIMASK(flg);
    return answer;
}

/**
 *@brief @b ��������:  s16 DSP_cos(u16 angle)
 *@brief @b ��������:   DSP cos���㺯��
 *@see���������ݣ�
 *@param���������      angle�� ����Ƕ�
 *@param���������      ��
 *@return�� �� ֵ��     cos�Ƕ�ֵ
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��12�� <td>2.0     <td>ywx   <td>�޸�
 * </table>
 */
s16 DSP_cos(u16 angle)
{
    s16 answer;
    int32_t flg = __get_PRIMASK();
    __disable_irq();
    DSP_REG->SC = BIT2 | BIT1;
    DSP_REG->THETA = angle;
    answer = DSP_REG->COS;
    __set_PRIMASK(flg);
    return answer;
}

/**
 *@brief @b ��������:  s16 u16 DSP_rms(s16 a,s16 b)
 *@brief @b ��������:   DSP �������������㺯��
 *@see���������ݣ�
 *@param���������      a�� ������
 *                     b:����
 *@param���������      ��
 *@return�� �� ֵ��     ����������
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��12�� <td>2.0     <td>ywx   <td>�޸�
 * </table>
 */
u16 DSP_rms(s16 a, s16 b)
{
    u16 answer;
    int32_t flg = __get_PRIMASK();
    __disable_irq();
    DSP_REG->DID = a;
    DSP_REG->DIS = b;
    answer = DSP_REG->REM;
    __set_PRIMASK(flg);
    return answer;
}

/**
 *@brief @b ��������:  DSP_arctan(s16 x,s16 y)
 *@brief @b ��������:   DSP �����м��㺯��
 *@see���������ݣ�
 *@param���������      x����ĸ
 *                     y:����
 *@param���������      ��
 *@return�� �� ֵ��     ������ֵ
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��12�� <td>2.0     <td>ywx   <td>�޸�
 * </table>
 */
s16 DSP_arctan(s16 x, s16 y)
{
    s16 answer;
    int32_t flg = __get_PRIMASK();
    __disable_irq();
    DSP_REG->SC = BIT1;
    DSP_REG->X = x;
    DSP_REG->Y = y;
    answer = DSP_REG->ARCTAN;
    __set_PRIMASK(flg);
    return answer;
}

/**
 *@brief @b ��������: s32 DSP_div(s32 a,s32 b)
 *@brief @b ��������:   DSP ��������
 *@see���������ݣ�
 *@param���������      x��������
 *                     y:����
 *@param���������      ��
 *@return�� �� ֵ��     ��������
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��12�� <td>2.0     <td>ywx   <td>�޸�
 * </table>
 */
s32 DSP_div(s32 a, s32 b)
{
    s32 answer;
    int32_t flg = __get_PRIMASK();
    __disable_irq();
    DSP_REG->DID = a;
    DSP_REG->DIS = b;
    answer = DSP_REG->QUO;
    __set_PRIMASK(flg);
    return answer;
}

/**
 *@brief @b ��������: s16 DSP_mod(s32 x,s16 y)
 *@brief @b ��������:   DSP ����������ģ
 *@see���������ݣ�
 *@param���������      x��x��
 *                     y: y��
 *@param���������      ��
 *@return�� �� ֵ��     ������ģ
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��12�� <td>2.0     <td>ywx   <td>�޸�
 * </table>
 */
s16 DSP_mod(s32 x, s16 y)
{
    s16 answer;
    int32_t flg = __get_PRIMASK();
    __disable_irq();
    DSP_REG->X = x;
    DSP_REG->Y = y;
    answer = DSP_REG->MOD;
    __set_PRIMASK(flg);
    return answer;
}

/**
 *@brief @b ��������: u16 DSP_sqrt(u32 val)
 *@brief @b ��������:   ��ƽ������
 *@see���������ݣ�
 *@param���������      val ����������
 *@param���������      ��
 *@return�� �� ֵ��     ������Ī
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��12�� <td>2.0     <td>ywx   <td>�޸�
 * </table>
 */
u16 DSP_sqrt(u32 val)
{
    u16 answer;
    int32_t flg = __get_PRIMASK();
    __disable_irq();
    DSP_REG->RAD = val;
    answer = DSP_REG->SQRT;
    __set_PRIMASK(flg);
    return answer;
}
