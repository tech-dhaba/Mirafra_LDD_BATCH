#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>

#define DRIVER_NAME "rpi4b_spi_driver"
#define SPI_BUS 0  // SPI bus number (adjust as per your setup)
#define SPI_CS 0   // Chip Select number (adjust as per your setup)
#define SPI_MAX_SPEED 500000  // SPI speed in Hz (500kHz)

struct rpi4b_spi_dev {
    struct spi_device *spi;
};

static int rpi4b_spi_probe(struct spi_device *spi)
{
    struct rpi4b_spi_dev *dev;
    int ret;
    u8 tx_buf[2] = { 0x00, 0xFF };  // Example: Data to write
    u8 rx_buf[2] = { 0x00 };
    struct spi_transfer t = {
        .tx_buf = tx_buf,
        .rx_buf = rx_buf,
        .len = 2,
        .speed_hz = SPI_MAX_SPEED,
        .bits_per_word = 8,
    };
    struct spi_message m;

    dev_info(&spi->dev, "Probing SPI device\n");

    /* Allocate driver data */
    dev = devm_kzalloc(&spi->dev, sizeof(struct rpi4b_spi_dev), GFP_KERNEL);
    if (!dev) {
        dev_err(&spi->dev, "Failed to allocate memory\n");
        return -ENOMEM;
    }
    spi_set_drvdata(spi,dev);
    dev->spi = spi;

    /* Example SPI Transfer */
    spi_message_init(&m);
    spi_message_add_tail(&t, &m);

    ret = spi_sync(spi, &m);
    if (ret) {
        dev_err(&spi->dev, "SPI transfer failed: %d\n", ret);
        return ret;
    }

    dev_info(&spi->dev, "SPI transfer successful. RX Data: 0x%02x 0x%02x\n", rx_buf[0], rx_buf[1]);
    return 0;
}

static int rpi4b_spi_remove(struct spi_device *spi)
{
    dev_info(&spi->dev, "Removing SPI device\n");
    return 0;
}

/* SPI Device ID Table */
static const struct spi_device_id rpi4b_spi_id[] = {
    { "rpi4b_spi_device", 0 },
    { }
};

MODULE_DEVICE_TABLE(spi, rpi4b_spi_id);

/* SPI Driver Structure */
static struct spi_driver rpi4b_spi_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .owner = THIS_MODULE,
    },
    .probe = rpi4b_spi_probe,
    .remove = rpi4b_spi_remove,
    .id_table = rpi4b_spi_id,
};

static int __init rpi4b_spi_init(void)
{
    pr_info("Initializing %s\n", DRIVER_NAME);
    return spi_register_driver(&rpi4b_spi_driver);
}

static void __exit rpi4b_spi_exit(void)
{
    pr_info("Exiting %s\n", DRIVER_NAME);
    spi_unregister_driver(&rpi4b_spi_driver);
}

module_init(rpi4b_spi_init);
module_exit(rpi4b_spi_exit);

MODULE_AUTHOR("Vajid");
MODULE_DESCRIPTION("SPI Device Driver for Raspberry Pi 4B");
MODULE_LICENSE("GPL");
