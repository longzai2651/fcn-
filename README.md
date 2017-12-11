# fcn
free connect your private network from anywhere

# 1. fcn是什么

fcn[`free connect`]是一款傻瓜式的一键接入私有网络的工具, 由客户服务端和客户端组成

fcn使用fcn公网数据服务器以及数据加密技术，将用户客户端虚拟接入客户服务端私有网络

fcn = `fcn_server` + `fcn_client`

**注: FCNV3.0以前版本切换服务端存在BUG, FCN近期切换服务器IP, 可能导致无法连接服务端, 请重开服务端或更新服务端版本**

* FCN使用交流QQ群: `592512533` FCN交流论坛 http://bbs.xfconnect.com

* download FCN V3.0 FULL 百度网盘 https://pan.baidu.com/s/1pLsonhh

* download FCN V3.0 binary https://github.com/boywhp/fcn/releases/download/V3.0/FCN_V3.0.zip

* download FCN V3.0 嵌入式版本  https://github.com/boywhp/fcn/releases/download/V3.0/linux-embeded.zip

* FCN支持操作系统平台

|操作系统|文件名
|-------|---
| Windows XP以上操作系统 | windows/fcn_win.exe
| Linux 64bit | linux/fcn_x64
| Linux 32bit | linux/fcn_x86
| Linux 路由器 | linux-embedded/fcn_`mips/mipsel/arm/armhf`_uclib
| Linux arm | linux-embedded/`fcn-arm/armbian`

Linux openwrt/lede WR703N、华硕N14U、斐讯K2/K2P Openwrt/Padavan实测通过，openwrt/lede需自行安装`libopenssl`包

Linux arm/armbian 树莓派3、Orange Pi实测通过

* fcn接入原理示意图

![image](https://github.com/boywhp/fcn/raw/master/doc/FCN%E7%BD%91%E7%BB%9C%E7%A4%BA%E6%84%8F%E5%9B%BE.png)

* fcn接入 斐讯PSG1218/K2路由操作演示

![image](https://github.com/boywhp/fcn/blob/master/doc/fcn_k2_rt.gif)

# 2. fcn常用使用场景

FCN设计理念是, 用最简洁的界面操作, 提供一套完整的网络接入解决方案, 使用场景介绍见PPT

http://pan.baidu.com/s/1o8r2w7g

FCN Lan2Lan简介

http://weibo.com/ttarticle/p/show?id=2309404124768579250283

|使用场景|描述|实际操作
|-------|---|--
| 管理远程主机 | 无需公网IP对任意联网机器进行远程管理 | http://pan.baidu.com/s/1slpu4f7
| 远程接入Lan | 一键访问远程局域网网段 | http://pan.baidu.com/s/1gfP7ZCv
| 远程客户演示 | FCN服务端支持IP及端口访问限制, 可临时提供有限网络访问权限 | http://pan.baidu.com/s/1qXND5bY
| Lan2Lan | 通过FCN客户端代理本地局域网机器访问远程局域网 | http://pan.baidu.com/s/1slM5N6P
| 远程虚拟组网 | 通过FCN虚拟网卡透过互联网组建虚拟局域网 | --
| 网络代理池 | FCN链路聚合同时连接多个服务端, 出口IP随机切换 | --

# 3. fcn使用

## 3.1 运行客户服务端

首先配置fcn.conf配置文件, 注意目前测试帐户 `FCN_0000-FCN_9999`, 每个帐户限速100KB/s[点对点通信成功后无限制]

请用户随机挑选测试帐户，并且设置自己的唯一服务器名，以防止帐户冲突

|配置键值|描述
|-------|---
| [uid] | FCN_[0001-9999] 8字符FCN ID
| [name] | 服务器名，建议填写一个有意义的名称
| [psk] | 管理员账号密码hash或者明文密码，建议使用hash
| [cipher] | 指定加密算法【aes-256-cfb/aes-128-cfb/chacha20】，默认aes-256-cfb
| [authfile] | 用户列表文件名，用户列表文件使用fcn_win.exe获取
| [udp]| 0/1, 设置数据包通信类型  0:TCP 1:UDP，建议不填使用UDP
| [nat_nic] | 虚拟接入后连接的服务器网卡名, 建议不填
| [dhcp_ip/dhcp_mask/dhcp_dns] |  虚拟接入后DHCP网段, DHCP DNS服务器地址, 建议不填
| [uport]| 自定义udp通信端口, 默认5000，自定义[1000-2000], 建议不填
| [tport]| 自定义tcp通信端口, 默认8000，自定义[1000-2000], 建议不填
| [pport]| 自定义p2p通信端口, 除非服务端可做端口映射，否则不要填
| [fcn_svr]| 设置公网FCN服务器地址,默认s1.xfconnect.com, 建议不填
| [notun]| 0/1, 0：自动 1：强制应用层NAT，建议不填

tun驱动模式NAT,需要ROOT权限运行；应用层NAT模式，非ROOT权限无法收发ping包
```shell
./fcn_x64         # 应用层NAT模式
sudo ./fcn_x64    # tun驱动NAT模式
```
注:FCN服务端一个配置只能运行一个实体, 更改配置后, 需要kill掉旧的进程, 否则会提示错误

## 3.2 开机自启动[Thanks to 榭寄生], debian linux环境

* 建立启动脚本 fcn.sh, 内容如下:

```bash
#!/bin/sh
/home/pi/fcn-arm
```

* 添加执行权限 chmod +x fcn.sh

* 创建软链接 ln -s /home/pi/your_fcn_dir/fcn.sh /etc/init.d/fcn

* 添加自启动 update-rc.d fcn defaults 99

## 3.3 运行windows客户端

主界面添加服务器, 填写对应的连接参数, 连接, 成功后, windows客户端即接入了服务器对应局域网, 客户端/服务端参数对应如下

![image](https://github.com/boywhp/fcn/blob/master/doc/FCN%E7%BD%91%E7%BB%9C%E5%8F%82%E6%95%B0.png)

注:第一次连接时会自动安装虚拟网卡驱动,需用户确认同意

## 3.4 运行Linux客户端

Linux客户端、服务端功能已整合在同一个可执行中，程序通过命令行参数决定启动客户端或服务端功能，客户端最常见参数如下：
```bash
sudo ./fcn --uid FCN_0001 --svr SVR0001 --psk 'PASSWORD'
```
Linux命令行客户端支持参数如下：

|参数名|描述
|-------|---
|--uid | 对应服务端用户ID参数
|--psk | 对应服务端用户连接密码参数
|--svr | 对应服务端服务器名
|--usr | 对应服务端用户名
|--host | FCN公共服务器地址，默认s1.xfconnect.com，建议直接填写对应的ip地址
|--tun | 指定客户端虚拟网卡的名称，默认tun_fcn，建议多个FCN客户端时填写
|--tunip | 手工指定客户端虚拟网卡IP地址
|--tcp | 使用TCP链路，建议不填，使用UDP
|--vpn | 是否开启全局路由，默认接入服务端网卡网段，建议按需填写
|--fwd | 开启服务端局域网数据自动转发到虚拟网卡，建议按需开启。
|--nolog | FCN服务器不记录日志，默认开启日志记录到fcn.log文件
|--nodaemon | FCN服务器以控制台模式运行，默认后台执行。

# 4. fcn安全吗？

fcn使用了数字证书、tls以及数据加密技术，点对点通信技术, 用户网络数据全程加密，30分钟左右自动更新会话密钥，确保用户数据不会被截获解密或者中间人欺骗。

fcn公网服务器不会收集用户的任何网络数据，同时支持用户网络数据强制点对点通信。后期考虑开放用户加密接口，以便用户实现自定义的端到端私有加密。

## 4.1 fcn安全机制

* fcn公网服务器和fcn客户端之间通过TLS证书双向验证确保信道安全
* fcn用户服务端每30分钟向fcn公网服务器请求随机数
* fcn公网服务器使用真随机数发生器产生随机数, 并通过TLS连接安全传递给用户
* fcn客户端/服务端通过 随机码 + UID + PSK 计算出一个会话key
* fcn客户端/服务端使用会话key对通信数据包全程aes256加密
