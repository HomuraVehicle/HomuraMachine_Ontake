#ifndef HMR_I2CSHT75_INC
#define HMR_I2CSHT75_INC 0
/*
hmrI2C_SHT75 SHT75���W���[����I2C�̃S�~�Ǝ��K�i�p�̊֐��Q
v0_00/131102 iwahori

*/
#include"hmLib_v3_04/cstring.h"
#include"hmrType.h"

//������
void i2cSHT75_initialize(void);
//�I�[��
void i2cSHT75_finalize(void);
//�ʐM�J�n(IO=1����M�C0�����M)
boolian i2cSHT75_start(uint8 slave_address,boolian IO);
//�ʐM�I��
void i2cSHT75_stop(void);
//���M�֐�
boolian i2cSHT75_write(uint8 c);
//��M�֐�
uint8 i2cSHT75_read(boolian EndFlag);
//���x�E���x�f�[�^���p�ӂł�����
boolian i2cSHT75_data_can_recv(void);
#endif
