#include <zephyr/kernel.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <bluetooth/scan.h>

static struct bt_conn *default_conn;

/** I change the code particularly for beacon.
 * Now it will only scan for the beacon and won't trigger the connection callback. 
*/
static void device_found(const bt_addr_le_t *addr, int8_t rssi, int8_t type, struct net_buf_simple *ad)
{ 

    /* We're only interested in Non-connectable and scannable advertising for beacon*/
	if (type != BT_GAP_ADV_TYPE_ADV_SCAN_IND) {
		return;
	}

    /** @brief  If the DATA LENGTH is not equal to 18, means it is not an beacon for accelerometer
     *  We can add more filter to undersatnd whether it is Beacon Frame for Accelerometer like adding flasg data
     *  UUId data
    */
    if (ad->data[7] != 18)
    {
        return;
    }    
    /** @brief Give the Battery level 
     *  Full = 100%     * 
    */
    int battery_level = ad->data[13];
    printk("Battery level: %d\n", battery_level);

    /* Print MAC address */
    char mac_addr[18];
    sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X", ad->data[20], ad->data[21], ad->data[22], ad->data[23], ad->data[24], ad->data[25]);
    printk("MAC address: %s\n", mac_addr);

    /**
     * @brief Check if the advertisement contains accelerometer data 
     * Convert accelerometer data from 8.8 fixed point to floating point 
    */
    if (ad->len >= 18 && ad->data[4] == 0x03 && ad->data[7] == 0x12) {
       float x = ((float)((int16_t)((ad->data[14]) | ad->data[15]))) / 256.0f;
        float y = ((float)((int16_t)((ad->data[16] ) | ad->data[17]))) / 256.0f;
        float z = ((float)((int16_t)((ad->data[18] ) | ad->data[19]))) / 256.0f;

        /* Convert to g-units */
        x = x * 9.81f;
        y = y * 9.81f;
        z = z * 9.81f;

        printk("Accelerometer values are X: %f ||Y: %f ||Z: %f\n",x ,y, z);

    /* Check if the tag is moving or stationary */
    if (x == 0.0 && y == -0.5 && z == 1.23) {
        printk("Tag is stationary\n");
    } else {
        printk("Tag is moving\n");        
    }
    }
}

void main(void)
{
    int err;

    /* Must be the called before any calls that require communication with the local Bluetooth hardware.*/
    err = bt_enable(NULL);
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return;
    }
    printk("Bluetooth initialized\n");


    /* Start scanning role*/
    err = bt_le_scan_start(BT_LE_SCAN_PASSIVE, device_found);
    if (err) {
        printk("Scanning failed to start (err %d)\n", err);
        return;
    }
    
    printk("Scanning started\n");

    while (1) {
        k_sleep(K_FOREVER);
    }
}
