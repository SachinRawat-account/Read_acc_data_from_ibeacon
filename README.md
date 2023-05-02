# To ingest accelerometer data from a BLE Tag and detect whether the tag is moving or stationary

A BLE beacon with an accelerometer will be broadcasting the accelerometer data. Reads this accelerometer data and then detect whether the tag is moving or stationary.

Example of raw packets with iBeacon and Accelerometer broadcasts:

1)0x0201060303E1FF1216E1FFA10364FFF4000FFF003772A33F23AC
2)0x0201061AFF4C00021553594F4F4B534F4349414C444953544500000000E8
3)0x0201060303E1FF1216E1FFA10364FFF60011FF003772A33F23AC
4)0x0201061AFF4C00021553594F4F4B534F4349414C444953544500000000E8
5)0x0201060303E1FF1216E1FFA10364FFF40011FF033772A33F23AC
6)0x0201061AFF4C00021553594F4F4B534F4349414C444953544500000000E8

![image](https://user-images.githubusercontent.com/117103811/235597634-4bd660cb-defb-4a8c-9c1f-dd6620098ce9.png)
**Beacon Frame definition for Accelerometer**

This is based on **Zephyr OS** and the SDK is **Nordic SDK**.
To run this code you need to install the [nordic SDK](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/1.9.1/nrf/getting_started.html)
Use the IDE [VS code](https://code.visualstudio.com/) and install all of the recommended extension(recommended by getting started). 
