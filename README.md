# fcn
free connect your private network from anywhere

# 1. fcn是什么

fcn[`free connect`]是一款傻瓜式的一键接入私有网络的工具, 由客户服务端和客户端组成

fcn使用fcn公网数据服务器以及数据加密技术，将用户客户端虚拟接入客户服务端私有网络

fcn = `fcn_server` + `fcn_client`

* FCN使用交流QQ群: `592512533`

* download FCN V2.1 binary

github https://github.com/boywhp/fcn/releases/download/V2.1/FCN_V2.1.zip

百度网盘 https://pan.baidu.com/s/1i5fv0yh

* download FCN V2.1.1 Beta binary

github https://github.com/boywhp/fcn/releases/download/V2.1.1/FCN_V2.1.1.zip

百度网盘 https://pan.baidu.com/s/1o8Ljqoq

* FCN操作视频

FCN windows桥接演示视频 http://v-wb.youku.com/v_show/id_XMjUxNTM5NTU1Ng==.html

FCN树莓派3启动服务端操作视频 http://my.tv.sohu.com/us/71649360/85990877.shtml
  
* fcn接入原理示意图

![image](https://github.com/boywhp/fcn/raw/master/FCN%E7%BD%91%E7%BB%9C%E7%A4%BA%E6%84%8F%E5%9B%BE.png)

## linux

目前支持linux x86 + x64 + arm + armbian + openwrt-mips平台

|文件位置|描述
|-------|---
|linux\ `server32` | 接入服务器x86平台bin
|linux\ `server` | 接入服务器x64平台bin
|linux\ `server-arm` | 接入服务器arm平台bin [PI3测试通过]
|linux\ `server-armbian` | 接入服务器armbian平台bin [Orange Pi测试通过]
|linux\ `server-openwrt` | 接入服务器openwrt mips平台bin [TPLINK WR703N测试通过]
|linux\ `server.conf` | 服务器配置文件

注：openwrt需安装kmod-tun组件

## windows

客户端功能支持windows x86/x64 XP SP3以上版本, 服务器功能需Windows Vista以上系统，由于桥接64位驱动使用sha2签名，Windows 7需安装`KB3033929`补丁

|文件位置|描述
|-------|---
|windows\ `fcn_win.exe` | FCN windows版， 整合服务器、客户端

# 2. fcn使用

## 2.1 运行客户服务端

首先配置server.conf配置文件, 注意目前测试帐户 `FCN_0000-FCN_9999`, 每个帐户限速100KB/s[点对点通信成功后无限制]

请用户随机挑选测试帐户，并且设置自己的唯一服务器名，以防止帐户冲突

|配置键值|描述
|-------|---
| [uid] | FCN_[0001-9999] 8字符用户ID *必填
| [name] | 服务器名,程序通过该名称标示服务器, 同一个uid不可重复，建议填写唯一标识
| [psk]| 连接密码, 由服务端启动时随机生成，建议不填
| [udp]| 0/1, 设置数据包通信类型  0:TCP 1:UDP，建议不填使用UDP
| [nat_nic] | 虚拟接入后连接的服务器网卡名, 建议不填
| [dhcp_ip/dhcp_mask/dhcp_dns] |  虚拟接入后DHCP网段, DHCP DNS服务器地址, 建议不填
| [uport]| 自定义udp通信端口, 默认5000，自定义[1000-2000], 建议不填
| [tport]| 自定义tcp通信端口, 默认8000，自定义[1000-2000], 建议不填
| [fcn_svr]| 设置公网FCN服务器地址,默认s1.xfconnect.com, 建议不填

由于需要操作底层网络数据转发,需要ROOT权限运行
```shell
./server      # ROOT用户直接运行
sudo ./server # 非ROOT用户使用sudo运行
```
注:FCN服务端只能运行一个实体, 更改配置后, 需要kill掉旧的进程, 否则会初始化失败错误

## 2.2 开机自启动[Thanks to 榭寄生], debian linux环境

* 建立启动脚本 fcn.sh, 内容如下:

```bash
#!/bin/sh
/home/pi/server-arm
```

* 添加执行权限 chmod +x fcn.sh
 
* 创建软链接 ln -s /home/pi/your_fcn_dir/fcn.sh /etc/init.d/fcn

* 添加自启动 update-rc.d fcn defaults 99

## 2.3 运行windows客户端

主界面添加服务器, 填写对应的连接参数, 连接, 成功后, windows客户端即接入了服务器对应局域网, 客户端/服务端参数对应如下

![image](https://github.com/boywhp/fcn/blob/master/FCN%E7%BD%91%E7%BB%9C%E5%8F%82%E6%95%B0.png)

注:第一次连接时会自动安装虚拟网卡驱动,需用户确认同意

# 3. fcn安全吗？

## 3.1 fcn通信安全机制

fcn使用了数字证书、tls以及aes 256bit加密技术，点对点通信技术, 用户网络数据全程加密，10分钟左右自动更新会话密钥，确保用户数据不会被截获解密或者中间人欺骗。

fcn公网服务器不会收集用户的任何网络数据，同时支持用户网络数据强制点对点通信。后期考虑开放用户加密接口，以便用户实现自定义的端到端私有加密。

## 3.2 fcn本地安全

fcn二进制文件发布前经过针对性的混淆加密处理，尽可能防止用户的加密配置文件被黑客攻击解密。

## 3.3 fcn是专业软件

fcn软件作者是boywhp，从事网络安全行业多年，是看雪安全论坛（bbs.pediy.com）高级会员，在业界发表过多篇网络安全技术文章。

当然作者也深知在网络攻防上，没有绝对的安全，因此在这里还请各位同行手下留情。

