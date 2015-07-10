/* 开始例行公事 这是一个模块所必须的*/
#ifndef __KERNEL__
#	define __KERNEL__       //这个宏定义用来标识改程序为内核程序
#endif
#ifndef __MODULE
#	define __MODULE__          //用来标示该内核程序为模块化的程序
#endif

//#include <linux/config.h>
#include <linux/module.h>

MODULE_LICENSE("GPL"); //版权许可证声明
/*结束例行公事 */

#include "linux/uaccess.h"    // copy_to_user(), copy_from_user
#include "linux/kernel.h"    //printk()在这个文件里
#include "linux/fs.h"         //struct file_operation,register_chardev(). unregister_chardev()
#include "linux/errno.h"
#include "linux/init.h"       //module_init() module_exit()
#include "linux/types.h"
#include "linux/kdev_t.h"
#define MAX_SIZE 1024

/*  设备的入口点  */
static int scull_open(struct inode *inode, struct file *file);
static int scull_release(struct inode *inode, struct file *file);
static ssize_t scull_read(struct file *file, char __user *user, size_t t, loff_t *f);
static ssize_t scull_write(struct file *file, const char __user *user, size_t t, loff_t *f);

static char message[MAX_SIZE] = "-------congratulations--------!";
static int device_num = 0;//设备号
static int counter = 0;//计数用
static int mutex = 0;//互斥用

static char* devName = "mydev";//设备名

struct file_operations char_fops = {
 	open: scull_open, 
	release: scull_release,
	read: scull_read,
	write: scull_write,
};  //每个驱动程序中该结构的实例都是用标准的c语法声明的，一般是将新的操作添加在该结构的末尾，这样对驱动程序简单的进行一次重新编译，这些操作都会被赋予NULL值，因此也就选择默认的行为

/* 注册模块 */
int init_module()
{
	int result;//用来保存申请到的主设备号
	/* 函数中第一个参数是告诉系统，新	注册的设备的主设备号由系统分配，
	 * 第二个参数是新设备注册时的设备名字，
	 * 第三个参数是指向file_operations的指针，
	 * 当用设备号为0创建时，系统一个可以用的设备号创建模块 */
	result = register_chrdev(0, devName, &char_fops);
	if (result < 0)
	{
		printk("regist failure!\n");
		return -1;
	}
	else
	{
		printk("the device has been registered!\n");
		device_num = result;
		printk("<1>the virtual device's major number %d.\n", device_num);
		printk("<1>Or you can see it by using\n");
		printk("<1>------more /proc/devices-------\n");
		printk("<1>To talk to the driver,create a dev file with\n");
		printk("<1>------'mknod /dev/mydev c %d 0'-------\n", device_num);
		printk("<1>Use \"rmmode\" to remove the module\n");

		return 0;
	}
}
/* 注销模块，函数名很特殊 */
void cleanup_module()
{
	unregister_chrdev(device_num, devName);  //注销以后设备就不存在了
	printk("unregister it success!\n");
}

static int scull_open(struct inode *inode, struct file *file)
{
	//若是早期的操作系统直接写上MOD_INC_USE_COUNT后直接return 0 即可
        if(mutex)
                return -EBUSY;
        mutex = 1;//上锁
	printk("<1>main  device : %d\n", MAJOR(inode->i_rdev));
	printk("<1>slave device : %d\n", MINOR(inode->i_rdev));
	printk("<1>%d times to call the device\n", ++counter);
	try_module_get(THIS_MODULE);
	return 0;
}
/* 每次使用完后会release */
static int scull_release(struct inode *inode, struct file *file)
{
	//若是早期的操作系统直接写上MOD_DEC_USE_COUNT后直接return 0 即可
	printk("Device released!\n");
	module_put(THIS_MODULE);
        mutex = 0;//开锁
	return 0;
}

static ssize_t scull_read(struct file *file, char __user *user, size_t t, loff_t *f)
{
	if(copy_to_user(user,message,sizeof(message)))
	{
		return -EFAULT;
	}
	return sizeof(message);
}

static ssize_t scull_write(struct file *file, const char __user *user, size_t t, loff_t *f)
{
	if(copy_from_user(message,user,sizeof(message)))
	{
		return -EFAULT;
	}
	return sizeof(message);
}

