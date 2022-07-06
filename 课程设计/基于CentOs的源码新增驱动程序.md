# 基于centos的源码编译

## (1)Linux安装

参考文献：

## (2)搭配环境

~~~shell
// 搭配编译环境
sudo yum -y install bison
sudo yum -y install flex
sudo yum -y install gcc g++ gdb make
sudo yum -y install devtoolset-7-gcc*
sudo yum -y install centos-release-scl
sudo yum -y install bc
sudo yum -y install ncurses-devel
sudo yum -y install elfutils-libelf-devel
sudo yum -y install openssl-devel
sudo yum -y install zlib zlib-devel pcre pcre-devel gcc gcc-c++ openssl openssl-devel libevent libevent-devel perl unzip net-tools wget  
// 下载相应的版本源码 下载时间大约1小时 与网速和电脑性能有关
sudo yum -y install wget
sudo wget http://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.20.4.tar.xz
// 将linux文件放至指定目录下并且解压。
// 如果不是在root用户下 下列操作均加入sudo。
cp -r /home/ywx/linux-4.20.4.tar.xz /usr/src/kernels
sudo tar xvJf linux-4.20.4.tar.xz -C /usr/src/kernels/
// 查询已有内核版本
cd /boot/
ls
// 找到config-$(name -r)
// 将已有内核复制。
cd /usr/src/kernels/linux-4.20.4/
sudo cp /boot/config-$(name -r) .config
// 新建配置菜单。基于文本选单的配置界面
sudo make menuconfig
~~~

![image-20220705193256584](E:\ujs_meng\Operating_System\OperationSystem\课程设计\基于CentOs的源码新增驱动程序.assets\image-20220705193256584.png)

先选择Load->OK,Save->OK->Exit,Exit 

选择<Save>栏后配置就完成了，在源码根目录下会生成一个.config文件，这就是这次编译的参数配置，之前copy过来的.config文件变成了.config.old。

~~~shell
// 开始编译
/*
键入make -j4指令即可正式开始编译，为加快编译速度，允许4个任务同时进行，其中4是机器中CPU核的个数。编译过程通常比较慢长。性能较好约俩小时左右
*/
sudo make -j4
// 安装模块和内核
sudo make modules_install
sudo make install
// 更新引导文件
grub2-mkconfig -o /boot/grub2/grub.cfg
// 重启后 第一个是自行编译的 第二个是原来的。我们选择第一个即完成源码编译
~~~

![image-20220705193325281](E:\ujs_meng\Operating_System\OperationSystem\课程设计\基于CentOs的源码新增驱动程序.assets\image-20220705193325281.png)

新建一个文件夹simp_blkdev

~~~shell
mkdir simp_blkdev
cd simp_blkdev
vi simp_blkdev.c
~~~

simp_blkdev.c 文件的内容：

设置块设备大小尽量稍大点 不然256MB的文件写入系统就崩溃了- -但是也不能太大，嵌入式系统的内存有限 如果过大会出现不分配内存的现象。继而出现报错。

~~~c
#include <linux/module.h>
#include <linux/blkdev.h>

#define SIMP_BLKDEV_DISKNAME "simp_blkdev"          //块设备名
#define SIMP_BLKDEV_DEVICEMAJOR COMPAQ_SMART2_MAJOR //主设备号
#define SIMP_BLKDEV_BYTES (500*1024*1024)            // 块设备大小为500MB
#define SECTOR_SIZE_SHIFT 9

static struct gendisk *simp_blkdev_disk;// gendisk结构表示一个简单的磁盘设备
static struct block_device_operations simp_blkdev_fops = { //块设备操作，gendisk的一个属性
    .owner = THIS_MODULE,
};
static struct request_queue *simp_blkdev_queue;//指向块设备请求队列的指针
unsigned char simp_blkdev_data[SIMP_BLKDEV_BYTES];// 虚拟磁盘块设备的存储空间


/******************************************************
*
*   磁盘块设备数据请求的处理函数
*
******************************************************/
static void simp_blkdev_do_request(struct request_queue *q){
    struct request *req;// 正在处理的请求队列中的请求
    struct bio *req_bio;// 当前请求的bio
    struct bio_vec *bvec;// 当前请求的bio的段(segment)链表
    char *disk_mem;      // 需要读/写的磁盘区域
    char *buffer;        // 磁盘块设备的请求在内存中的缓冲区
    int i = 0;

    while((req = blk_fetch_request(q)) != NULL){
        // 判断当前req是否合法
        if((blk_rq_pos(req)<<SECTOR_SIZE_SHIFT) + blk_rq_bytes(req) > SIMP_BLKDEV_BYTES){
            printk(KERN_ERR SIMP_BLKDEV_DISKNAME":bad request:block=%llu, count=%u\n",(unsigned long long)blk_rq_pos(req),blk_rq_sectors(req));
            blk_end_request_all(req, -EIO);
            continue;
        }
        //获取需要操作的内存位置
        disk_mem = simp_blkdev_data + (blk_rq_pos(req) << SECTOR_SIZE_SHIFT);
        req_bio = req->bio;// 获取当前请求的bio

        switch (rq_data_dir(req)) {  //判断请求的类型
        case READ:
            // 遍历req请求的bio链表
            while(req_bio != NULL){
                //　for循环处理bio结构中的bio_vec结构体数组（bio_vec结构体数组代表一个完整的缓冲区）
                for(i=0; i<req_bio->bi_vcnt; i++){
                    bvec = &(req_bio->bi_io_vec[i]);
                    buffer = kmap(bvec->bv_page) + bvec->bv_offset;
                    memcpy(buffer, disk_mem, bvec->bv_len);
                    kunmap(bvec->bv_page);
                    disk_mem += bvec->bv_len;
                }
                req_bio = req_bio->bi_next;
            }
            __blk_end_request_all(req, 0);
            break;
        case WRITE:
            while(req_bio != NULL){
                for(i=0; i<req_bio->bi_vcnt; i++){
                    bvec = &(req_bio->bi_io_vec[i]);
                    buffer = kmap(bvec->bv_page) + bvec->bv_offset;
                    memcpy(disk_mem, buffer, bvec->bv_len);
                    kunmap(bvec->bv_page);
                    disk_mem += bvec->bv_len;
                }
                req_bio = req_bio->bi_next;
            }
            __blk_end_request_all(req, 0);
            break;
        default:
            /* No default because rq_data_dir(req) is 1 bit */
            break;
        }
    }
}


/******************************************************
*
*   模块的入口函数
*
******************************************************/
static int __init simp_blkdev_init(void){
    int ret;

    //1.添加设备之前，先申请设备的资源
    simp_blkdev_disk = alloc_disk(1);
    if(!simp_blkdev_disk){
        ret = -ENOMEM;
        goto err_alloc_disk;
    }

    //2.设置设备的有关属性(设备名，设备号，fops指针,请求队列,512B的扇区数)
    strcpy(simp_blkdev_disk->disk_name,SIMP_BLKDEV_DISKNAME);
    simp_blkdev_disk->major = SIMP_BLKDEV_DEVICEMAJOR;
    simp_blkdev_disk->first_minor = 0;
    simp_blkdev_disk->fops = &simp_blkdev_fops;
    // 将块设备请求处理函数的地址传入blk_init_queue函数，初始化一个请求队列
    simp_blkdev_queue = blk_init_queue(simp_blkdev_do_request, NULL);
    if(!simp_blkdev_queue){
        ret = -ENOMEM;
        goto err_init_queue;
    }
    simp_blkdev_disk->queue = simp_blkdev_queue;
    set_capacity(simp_blkdev_disk, SIMP_BLKDEV_BYTES>>9);

    //3.入口处添加磁盘块设备
    add_disk(simp_blkdev_disk);
    return 0;

    err_alloc_disk:
        return ret;
    err_init_queue:
        return ret;
}


/******************************************************
*
*   模块的出口函数
*
******************************************************/
static void __exit simp_blkdev_exit(void){
    del_gendisk(simp_blkdev_disk);// 释放磁盘块设备
    put_disk(simp_blkdev_disk);   // 释放申请的设备资源
    blk_cleanup_queue(simp_blkdev_queue);// 清除请求队列
}


module_init(simp_blkdev_init);// 声明模块的入口
module_exit(simp_blkdev_exit);// 声明模块的出口
~~~

Makefile 文件的内容：

~~~sh
ifeq ($(KERNELRELEASE),)
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
modules:
        $(MAKE) -C $(KDIR) M=$(PWD) modules
modules_install:
        $(MAKE) -C $(KDIR) M=$(PWD) modules_install
clean:
        rm -rf *.o *.ko .depend *.mod.o *.mod.c Module.* modules.*
.PHONY:modules modules_install clean
else
        obj-m := simp_blkdev.o
endif
~~~

版本3在Linux4.20.4版本下编译通过：

~~~she
sudo make
~~~

![image-20220705193530623](E:\ujs_meng\Operating_System\OperationSystem\课程设计\基于CentOs的源码新增驱动程序.assets\image-20220705193530623.png)

  ~~~shell
  sudo insmod simp_blkdev.ko // 块设备驱动模块插入
  lsblk // 查看块设备信息
  ~~~

![image-20220705193545526](E:\ujs_meng\Operating_System\OperationSystem\课程设计\基于CentOs的源码新增驱动程序.assets\image-20220705193545526.png)

块设备驱动模块插入成功

如果想要更改需先卸载设备

~~~shell
sudo rmmod simp_blkdev.ko // simp_blkdev.ko是设备名
~~~

## (3)实现功能：动态加载新的驱动&实现至少256MB数据的保存和读写

在块设备上建立ext3文件系统(此处可以选择其他文件系统，如：ext2、ext3、NTFS、xfs(CentOS7默认版本)等等)。

~~~shell
sudo mkfs.ext3 /dev/simp_blkdev # 在该块设备上建立ext3文件系统
~~~

挂载块设备

~~~shell
sudo mkdir -p /mnt/temp1
sudo mount -rw /dev/simp_blkdev /mnt/temp1/
sudo mount | grep simp_blkdev
lsmod // 查看挂载情况
~~~

![image-20220705193745821](E:\ujs_meng\Operating_System\OperationSystem\课程设计\基于CentOs的源码新增驱动程序.assets\image-20220705193745821.png)

在~目录下创建一个小于等于500MB的文件夹 后

~~~shell
sudo cp ~/xxx /home/mnt/temp1 // 将文件s复制进系统
df -H // 查看文件系统使用情况
~~~

![image-20220705193826298](E:\ujs_meng\Operating_System\OperationSystem\课程设计\基于CentOs的源码新增驱动程序.assets\image-20220705193826298.png)

~~~shell
1. #关于读，可使用cat命令 写可以使用vi命令
2. *#读文件的话 先使用cd命令进入到/mnt/temp1目录下 cat 文件名，即可完成读文件*
3. #写文件的话 先使用cd目录进入到/mnt/temp1目录下 vi 文件名，进入文件后 按i进入编辑模式 esc退出编辑模式 :wq保存退出
~~~

##  (4)动态卸载驱动

~~~shell
#删除挂载目录中的所有文件
rm -rf /mnt/temp1/*
#查看挂载结果，此时可以看到已用资源变为2%
df -H
#取消挂载
umount /mnt/temp1/ 
#查看模块的调用结果 可以发现调用数由1变为0
lsmod | grep simp_blkdev
#移除该模块
rmmod simp_blkdev
#查看模块的调用结果 此时模块已经查询不到了
lsmod | grep simp_blkdev
~~~

