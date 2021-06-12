
#include "Wire.h" // Init I2C

#define MPU_ADDR 0x68 // I2C address for the MPU-6050.
#define ACC_XYZ_ADDR 0X3B

float Deg_AccX, Deg_AccY;

int16_t acc_x, acc_y, acc_z; 

const double RadToDeg = 57.295779513082320876798154814105; 

void setup() {
  
  Serial.begin(9600); //Start serial communication. 
  
  Wire.begin(); // Assin Nano as I2C master (Leave the argument empty.). The slave will be MPU-6050. 
  Wire.beginTransmission(MPU_ADDR); // Get ready for the data transmission to the I2C slave device. 
  Wire.write(MPU_ADDR); //  Store the data which should be transmitted after Wire.endTransmission(). 0x6B: PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(); // Send data stored by Wire.write().  
  
}


void Get_Raw_ACC (){
    
    while (Wire.available()) // Return number of bytes 
  {
      acc_x = Wire.read() << 8 | Wire.read(); // Master reads 8bit data from Slave device. 
      acc_y = Wire.read() << 8 | Wire.read(); 
      acc_z = Wire.read() << 8 | Wire.read();
  }
}

void Request_ACC(){
  
  Wire.beginTransmission(MPU_ADDR); // Ready for the data transmisttion for MPU6050: https://www.arduino.cc/en/Reference/WireBeginTransmission
  Wire.write(ACC_XYZ_ADDR); // Store the data which should be transmitted after Wire.endTransmission(). 0x3B: ACCEL_XOUT (See the resister table of datasheet)
  Wire.endTransmission(false);  
  Wire.requestFrom(MPU_ADDR, 6); // Master requests 6 Byte from Slave. 
}

void Convert_RawToDeg(){

//Roll (X), Pitch (Y) equation 

Deg_AccX = atan(acc_y / sqrt(pow(acc_x,2) + pow(acc_z,2)))*RadToDeg; // Calculation for Roll(Left UpDown and Right UpDown)
Deg_AccY = atan(acc_x/sqrt(pow(acc_y,2)+pow(acc_z,2)))*RadToDeg; // Calculation for Pitch (Up and Down)

      Serial.print("Deg_AccX  = ");
      Serial.print(Deg_AccX );
      Serial.print("\t Deg_AccY = ");
      Serial.println(Deg_AccY);

}

void loop() {
  // put your main code here, to run repeatedly:

  Request_ACC();
  
  Get_Raw_ACC();
  
  Convert_RawToDeg(); // Conver to Radian 
  
  delay(500);

}
