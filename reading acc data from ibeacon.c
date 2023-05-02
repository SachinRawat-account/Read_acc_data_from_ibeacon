#include <zephyr.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/gatt.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_driver.h>
#include <bluetooth/conn.h>

static struct bt_conn *default_conn;


static void device_found(const bt_addr_le_t *addr, s8_t rssi, u8_t type, struct net_buf_simple *ad)
{
    if (type != BT_LE_ADV_IND && type != BT_LE_ADV_DIRECT_IND) {
        return;
    }

    if (ad->len < 19) {
        return;
    }

    /**
     * @brief Check if the advertisement contains accelerometer data 
     * Convert accelerometer data from 8.8 fixed point to floating point 
    */
    if (buf->len >= 18 && buf->data[4] == 0x03 && buf->data[7] == 0x12) {
       float x = ((float)((int16_t)((buf->data[15] << 8) | buf->data[14]))) / 256.0f;
        float y = ((float)((int16_t)((buf->data[17] << 8) | buf->data[16]))) / 256.0f;
        float z = ((float)((int16_t)((buf->data[19] << 8) | buf->data[18]))) / 256.0f;

        /* Convert to g-units */
        x = x * 9.81f;
        y = y * 9.81f;
        z = z * 9.81f;

    /* Check if the tag is moving or stationary */
    if (x == 0.0 && y == -0.5 && z == 1.23) {
        printk("Tag is stationary\n");
    } else {
        printk("Tag is moving\n");
        printk("Accelerometer values are X: %f ||Y: %f ||Z: %f\n",x ,y, z);
    }
    }
}

static struct bt_le_scan_cb scan_callbacks = {
    .recv = device_found,
};

static void connected(struct bt_conn *conn, uint8_t err)
{
    if (err) {
        printk("Connection failed (err %u)\n", err);
    } else {
        printk("Connected\n");
        default_conn = bt_conn_ref(conn);
    }
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    printk("Disconnected (reason %u)\n", reason);
    if (default_conn) {
        bt_conn_unref(default_conn);
        default_conn = NULL;
    }
}

static struct bt_conn_cb conn_callbacks = {
    .connected = connected,
    .disconnected = disconnected,
};

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

    /* Connection callback*/
    bt_conn_cb_register(&conn_callbacks);

    /* Start scanning role*/
    err = bt_le_scan_start(BT_LE_SCAN_PASSIVE, &scan_callbacks);
    if (err) {
        printk("Scanning failed to start (err %d)\n", err);
        return;
    }
    
    printk("Scanning started\n");

    while (1) {
        k_sleep(K_FOREVER);
    }
}
