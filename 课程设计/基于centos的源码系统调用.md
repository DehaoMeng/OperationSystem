# 基于centos的源码编译及系统调用

## (1)Linux 安装

参考文献:

## (2) 环境搭建

~~~shell
// 下载相应的版本源码 下载时间大约1小时 与网速和电脑性能有关
sudo wget http://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.20.4.tar.xz
~~~

~~~shell
// 搭配编译环境
sudo yum -y install ncurses-devel
sudo yum -y install bc
sudo yum -y install bison
sudo yum -y install flex
sudo yum -y install gcc g++ gdb make
sudo yum -y install centos-release-scl
sudo yum -y install devtoolset-7-gcc*
// 将linux文件放至指定目录下并且解压。
// 如果 不是在root用户下 下列操作均加入sudo
cp -r ~/linux-4.20.4.tar.xz /usr/src/kernels
sudo tar xvJf linux-4.20.4.tar.xz -C /usr/src/kernels/
cd /usr/src/kernels/linux-4.20.4/kernel
vi sys.c
在末尾#endif前面加入函数
SYSCALL_DEFINE1(cube,int,num){
    int result = num*num*num;
    return result;
}
// 添加声明
cd /usr/src/kernels/linux-4.20.4/arch/x86/include/asm/
vi syscalls.h
asmlinkage long sys_cube(long n);
// 加一个系统调用的id
cd /usr/src/kernels/linux-4.20.4/arch/x86/entry/syscalls
vi syscall_64.tbl // 这里也有可能是32
# 在里面添加自己的系统调用号 这里的333 根据自己的情况改
（335  64 cube __x64_sys_cube）

// 查询已有内核版本
cd /boot/
ls
// 找到config-$(name -r)
// 将已有内核复制。
cd /usr/src/kernels/linux-4.20.4/

sudo cp /boot/config-$(name -r) .config

sudo make menuconfig
~~~

syscalls.h插入位置

![image-20220705194420657](E:\ujs_meng\Operating_System\OperationSystem\课程设计\基于centos的源码系统调用.assets\image-20220705194420657.png)

menuconfig界面

![image-20220705194213703](E:\ujs_meng\Operating_System\OperationSystem\课程设计\基于centos的源码系统调用.assets\image-20220705194213703.png)选择<Save>栏后配置就完成了，在源码根目录下会生成一个.config文件，这就是这次编译的参数配置，之前copy过来的.config文件变成了.config.old。

~~~shell
sudo yum -y install elfutils-libelf-devel
sudo yum -y install openssl-devel
sudo yum -y install zlib zlib-devel pcre pcre-devel gcc gcc-c++ openssl openssl-devel libevent libevent-devel perl unzip net-tools wget
// 开始编译
/*
键入make -j4指令即可正式开始编译，为加快编译速度，允许4个任务同时进行，其中4是机器中CPU核的个数。编译过程通常比较慢长。性能较好约俩小时左右
*/
// 安装模块和内核
make modules_install
make install
// 更新引导文件
grub2-mkconfig -o /boot/grub2/grub.cfg
// 重启后 第一个是自行编译的 第二个是原来的。我们选择第一个即完成源码编译
~~~

![image-20220705193325281](E:\ujs_meng\Operating_System\OperationSystem\课程设计\基于centos的源码系统调用.assets\image-20220705193325281.png)

重启系统后输入

## (3)系统调用实现三次方功能

~~~shell
vim cubic.c
// 输入以下代码
#include<stdio.h>
#include<linux/kernel.h>
#include<sys/syscall.h>
#include<unistd.h>
int main(){
        double n;
        long s;
        scanf("%lf",&n);
        s=n*100;
        if(s<0){
                s = -syscall(335,-s);
        }
        else{
                s = syscall(335,s);
        }
        n = s;
        n = n / syscall(335,100);
        printf("%lf",n);
        return 0;
}
// 保存
gcc cubic.c

~~~

 测试用例分别为：4，-4，3.14，-3.14		 ![image-20220705194447398](E:\ujs_meng\Operating_System\OperationSystem\课程设计\基于centos的源码系统调用.assets\image-20220705194447398.png)
