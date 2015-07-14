/*
 * Vansh Khanna, 2015
 * To be used with MBed.org.
 * Board: nRF51 DK
 * BLE Broadcasting Application. Using Groove IoT Sheild and corresponding Temperature Sensor.
 * NOTE: USE BLE AND NRF51 LIBRARY ON MBED
 * <This file is modified version of the source code provided by some other company for a slightly differnt
 * implementation>
 */
 //#define CONSOLE_LOG
 #include "mbed.h"
 //#include "BLEDevice.h"
//For scanning
#include "BLE.h"
#include "UUID.h"
#include "GattCharacteristic.h"
//For scanning

 /*
#ifdef CONSOLE_LOG
#define INFO(x, ...)    printf(x,        ##__VA_ARGS__);
#define INFO_NL(x, ...) printf(x "\r\n", ##__VA_ARGS__);
#else
#define INFO(x, ...)
#define INFO_NL(x, ...)
#endif
 */
Serial pc(USBTX, USBRX);
// name of the device
const static char DEVICE_NAME[] = "nRF51-DK";

const UUID nRF51_GATT_SERVICE     = UUID((uint8_t *)"f000aa20-0451-4000-b000-000000000000");
const UUID nRF51_GATT_CHAR_LED    = UUID((uint8_t *)"nRF51-DK led    ");

DigitalOut led1(P0_21);
DigitalOut led2(P0_22);
DigitalOut led3(P0_23);
DigitalOut led4(P0_24);

#define CHARACTERISTIC_COUNT 1
#define CHARACTERISTIC_LED   0

//Device for scanning Bluetooth
 // our bluetooth smart objects
BLEDevice           ble;
//Gatt Service not needed for now. //TODO
GattService        *gatt_service;
GattCharacteristic *gatt_characteristics[CHARACTERISTIC_COUNT];
uint8_t             gatt_char_value[CHARACTERISTIC_COUNT];


/* Advertisement callback. Stricly for scanning */
void advertisementCallback(const Gap::AdvertisementCallbackParams_t *params) {

    led1 = 0;
    //flag=1;
    //printf("Adv peerAddr: [%02x %02x %02x %02x %02x %02x] rssi %d, ScanResp: %u, AdvType: %u\r\n",
    //       params->peerAddr[5], params->peerAddr[4], params->peerAddr[3], params->peerAddr[2], params->peerAddr[1], params->peerAddr[0],
    //       params->rssi, params->isScanResponse, params->type);
           //printf("Device Found\n");
           printf("%02x%02x%02x%02x%02x%02x\n", params->peerAddr[5], params->peerAddr[4], params->peerAddr[3], params->peerAddr[2], params->peerAddr[1], params->peerAddr[0]);
#if DUMP_ADV_DATA
    for (unsigned index = 0; index < params->advertisingDataLen; index++) {
        printf("%02x ", params->advertisingData[index]);
    }
    printf("Device Found\r\n");
#endif /* DUMP_ADV_DATA */
}


void connectionCallback(Gap::Handle_t handle,
                  Gap::addr_type_t peerAddrType,
                  const Gap::address_t peerAddr,
                  const Gap::ConnectionParams_t * connectionParams)
{
  /*
  led1.write(1);
  led2.write(1);
  led3.write(1);
  led4.write(1);*/
    //return Gap::ConnectionEventCallback_t;
    pc.printf(">>Connected \n");
}

void disconnectionCallback(Gap::Handle_t handle,
                     Gap::DisconnectionReason_t disconnectReason)
{
  pc.printf(">>Disconnected\n");
  /*led1.write(0);
  led2.write(0);
  led3.write(0);
  led4.write(0);
  */

  ble.startAdvertising();
}

void onDataWritten(const GattWriteCallbackParams *context){
    if(context -> handle ==
        gatt_characteristics[CHARACTERISTIC_LED] -> getValueHandle())
    {
        pc.printf(">>Charac written\n");
        //led1.write(0);
        //led2.write(1);
        //led3.write(1);
        //led4.write(1);
        //pc.printf(context->data[0]);
        const uint8_t* value = context->data;
        uint8_t led_value = value[0];
        if((led_value & 0x01) == 0x01) led1.write(0);else led1.write(1);
        if((led_value & 0x02) == 0x02) led2.write(0);else led2.write(1);
        if((led_value & 0x03) ==0x03) {led3.write(0);led2.write(1);led1.write(1);}else led3.write(1);
        if((led_value & 0x04) == 0x04) led4.write(0);else led4.write(1);
    }
}

int main(){
    //pc.wait(1);
    gatt_characteristics[CHARACTERISTIC_LED] =
        new GattCharacteristic(
            nRF51_GATT_CHAR_LED,
            &gatt_char_value[CHARACTERISTIC_LED], 1, 1,
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE |
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE);

    gatt_service = new GattService(nRF51_GATT_SERVICE, gatt_characteristics, CHARACTERISTIC_COUNT);
    //Initialize BLE Device
    ble.init();

    ble.setDeviceName((uint8_t *)DEVICE_NAME);

    // configure our advertising type, payload and interval00
    ble.accumulateAdvertisingPayload(
          GapAdvertisingData::BREDR_NOT_SUPPORTED |
          GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.accumulateAdvertisingPayload(
          GapAdvertisingData::COMPLETE_LOCAL_NAME,
          (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));

    ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.setAdvertisingInterval(160); // 100ms

    /*
    led1.write(1);
    led3.write(1);
    led2.write(1);
    led4.write(1);
    */
    for(int i=0; i<10; i++)
        pc.printf("Hello World\n");
    ble.onConnection((Gap::ConnectionEventCallback_t)connectionCallback);
    ble.onDisconnection(disconnectionCallback);
    ble.gattServer().onDataWritten(onDataWritten);

    ble.addService(*gatt_service);

    ble.startAdvertising();

    /* Adding below for scanning */
        ble.setScanParams(500 /* scan interval */, 200 /* scan window */);
        ble.startScan(advertisementCallback);
    /**END OF PART FOR SCANNING**/

    for (;;)
    {
      ble.waitForEvent();
    }
}
