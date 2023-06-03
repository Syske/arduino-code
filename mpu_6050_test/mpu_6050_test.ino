// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class
// 10/7/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2011-10-07 - initial release

/* ============================================
  I2Cdev device library code is placed under the MIT license
  Copyright (c) 2011 Jeff Rowberg

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
  ===============================================
*/

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

#define LED_PIN 13
bool blinkState = false;

unsigned long now, lastTime = 0;
float dt;                                   //微分时间

float aax = 0, aay = 0, aaz = 0, agx = 0, agy = 0, agz = 0; //角度变量
long axo = 0, ayo = 0, azo = 0;             //加速度计偏移量
long gxo = 0, gyo = 0, gzo = 0;             //陀螺仪偏移量

float pi = 3.1415926;
float AcceRatio = 16384.0;                  //加速度计比例系数
float GyroRatio = 131.0;                    //陀螺仪比例系数

uint8_t n_sample = 8;                       //加速度计滤波算法采样个数
float aaxs[8] = {0}, aays[8] = {0}, aazs[8] = {0};         //x,y轴采样队列
long aax_sum, aay_sum, aaz_sum;                     //x,y轴采样和

float a_x[10] = {0}, a_y[10] = {0}, a_z[10] = {0} , g_x[10] = {0} , g_y[10] = {0}, g_z[10] = {0}; //加速度计协方差计算队列
float Px = 1, Rx, Kx, Sx, Vx, Qx;           //x轴卡尔曼变量
float Py = 1, Ry, Ky, Sy, Vy, Qy;           //y轴卡尔曼变量
float Pz = 1, Rz, Kz, Sz, Vz, Qz;           //z轴卡尔曼变量

void setup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
  Wire.begin();

  // initialize serial communication
  // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
  // it's really up to you depending on your project)
  Serial.begin(115200);

  // initialize device
  Serial.println("Initializing I2C devices...");

  accelgyro.initialize();                 //初始化

  unsigned short times = 200;             //采样次数
  for (int i = 0; i < times; i++)
  {
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); //读取六轴原始数值
    axo += ax; ayo += ay; azo += az;      //采样和
    gxo += gx; gyo += gy; gzo += gz;
  }
  axo /= times; ayo /= times; azo /= times; //计算加速度计偏移
  gxo /= times; gyo /= times; gzo /= times; //计算陀螺仪偏移
  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  // configure Arduino LED for
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  unsigned long now = millis();             //当前时间(ms)
  dt = (now - lastTime) / 1000.0;           //微分时间(s)
  lastTime = now;                           //上一次采样时间(ms)
  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // these methods (and a few others) are also available
  //accelgyro.getAcceleration(&ax, &ay, &az);
  //accelgyro.getRotation(&gx, &gy, &gz);

  float accx = ax / AcceRatio;              //x轴加速度
  float accy = ay / AcceRatio;              //y轴加速度
  float accz = az / AcceRatio;              //z轴加速度

  aax = atan(accy / accz) * (-180) / pi;    //y轴对于z轴的夹角
  aay = atan(accx / accz) * 180 / pi;       //x轴对于z轴的夹角
  aaz = atan(accz / accy) * 180 / pi;       //z轴对于y轴的夹角

  aax_sum = 0;                              // 对于加速度计原始数据的滑动加权滤波算法
  aay_sum = 0;
  aaz_sum = 0;
  for (int i = 1; i < n_sample; i++)
  {
    aaxs[i - 1] = aaxs[i];
    aax_sum += aaxs[i] * i;
    aays[i - 1] = aays[i];
    aay_sum += aays[i] * i;
    aazs[i - 1] = aazs[i];
    aaz_sum += aazs[i] * i;
  }
  aaxs[n_sample - 1] = aax;
  aax_sum += aax * n_sample;
  aax = (aax_sum / (11 * n_sample / 2.0)) * 9 / 7.0; //角度调幅至0-90°
  aays[n_sample - 1] = aay;                      //此处应用实验法取得合适的系数
  aay_sum += aay * n_sample;                     //本例系数为9/7
  aay = (aay_sum / (11 * n_sample / 2.0)) * 9 / 7.0;
  aazs[n_sample - 1] = aaz;
  aaz_sum += aaz * n_sample;
  aaz = (aaz_sum / (11 * n_sample / 2.0)) * 9 / 7.0;

  float gyrox = - (gx - gxo) / GyroRatio * dt; //x轴角速度
  float gyroy = - (gy - gyo) / GyroRatio * dt; //y轴角速度
  float gyroz = - (gz - gzo) / GyroRatio * dt; //z轴角速度
  agx += gyrox;                             //x轴角速度积分
  agy += gyroy;                             //x轴角速度积分
  agz += gyroz;
  /* kalman start */
  Sx = 0; Rx = 0;
  Sy = 0; Ry = 0;
  Sz = 0; Rz = 0;
  for (int i = 1; i < 10; i++)
  { //测量值平均值运算
    a_x[i - 1] = a_x[i];                    //即加速度平均值
    Sx += a_x[i];
    a_y[i - 1] = a_y[i];
    Sy += a_y[i];
    a_z[i - 1] = a_z[i];
    Sz += a_z[i];
  }
  a_x[9] = aax;
  Sx += aax;
  Sx /= 10;                                 //x轴加速度平均值
  a_y[9] = aay;
  Sy += aay;
  Sy /= 10;                                 //y轴加速度平均值
  a_z[9] = aaz;
  Sz += aaz;
  Sz /= 10;

  for (int i = 0; i < 10; i++)
  {
    Rx += sq(a_x[i] - Sx);
    Ry += sq(a_y[i] - Sy);
    Rz += sq(a_z[i] - Sz);
  }
  Rx = Rx / 9;                              //得到方差
  Ry = Ry / 9;
  Rz = Rz / 9;
  Px = Px + 0.0025;                         // 0.0025在下面有说明...
  Kx = Px / (Px + Rx);                      //计算卡尔曼增益
  agx = agx + Kx * (aax - agx);             //陀螺仪角度与加速度计速度叠加
  Px = (1 - Kx) * Px;                       //更新p值

  Py = Py + 0.0025;
  Ky = Py / (Py + Ry);
  agy = agy + Ky * (aay - agy);
  Py = (1 - Ky) * Py;
  Pz = Pz + 0.0025;
  Kz = Pz / (Pz + Rz);
  agz = agz + Kz * (aaz - agz);
  Pz = (1 - Kz) * Pz;

  /* kalman end */

  Serial.print(agx); Serial.print(",");
  Serial.print(agy); Serial.print(",");
  Serial.print(agz); Serial.println();
}
