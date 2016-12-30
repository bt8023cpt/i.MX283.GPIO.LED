#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/version.h>

#include <asm/mach/arch.h>
#include <mach/hardware.h>
#include <mach/gpio.h>
#include <asm/gpio.h>

#include "../common/led_drv.h"

static int major;
static int minor;
struct cdev *led;
static dev_t devno;

/* gpio 1_23 */
static struct class *led_class;

#define DEVICE_NAME "led"

#define GPIO_LED1_PIN_NUM (2*32+7)

static int led_open(struct inode *inode, struct file *file)
{
        try_module_get (THIS_MODULE);

        /* 设置GPIO工作模式为输出模式 */
        gpio_direction_output(GPIO_LED1_PIN_NUM, 1);
        return 0;
}

static int led_release(struct inode *inode, struct file *file)
{
        module_put (THIS_MODULE);

        /* 设置GPIO工作模式为输出模式 */
        gpio_direction_output(GPIO_LED1_PIN_NUM, 1);
        return 0;
}

int led_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
        switch(cmd)
        {
        case LED_ON:
                gpio_set_value(GPIO_LED1_PIN_NUM, 1);
                break;

        case LED_OFF:
                gpio_set_value(GPIO_LED1_PIN_NUM, 0);
                break;
        }

        return 0;
}

struct file_operations led_fops =
{       .owner = THIS_MODULE, .open = led_open, .release = led_release, .unlocked_ioctl =
        led_ioctl};

static int __init led_init(void)
{
        gpio_free(GPIO_LED1_PIN_NUM);
        gpio_request(GPIO_LED1_PIN_NUM, "led_run");

        alloc_chrdev_region(&devno, minor, 1, "ledccccc");
        major = MAJOR(devno);
        led = cdev_alloc();
        cdev_init(led, &led_fops);
        led->owner = THIS_MODULE;
        cdev_add(led, devno, 1);
        led_class = class_create(THIS_MODULE, "led_class");
        device_create(led_class, NULL, devno, NULL, "led");
        return 0;
}

static void __exit led_exit(void)
{
        cdev_del(led);
        unregister_chrdev_region(devno, 1);
        device_destroy(led_class, devno);
        class_destroy(led_class);
}

module_init (led_init);
module_exit (led_exit);

MODULE_LICENSE("GPL");
