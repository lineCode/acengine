# acengine

## 目录说明:
### baseapp  
逻辑服务器进程  

### bin   
可执行文件  
存放所有的可执行文件，以及管理脚本，（启动，关闭，重启）  

### common  
公共组件 （网络模块，配置文件读取）  

### dbapp  
数据库操作相关  

### gateapp     
网关服   
负责消息的转发，负载均衡，消息包的合法判断  

### loginapp   
登录服  
验证登录信息，创建默认账号等  


### masterapp   
管理服  
管理所有的进程——重启，心跳包检测  

### test  
各个小模块的测试代码，演示使用  

### 联系我：  
**jxdeng3264@163.com**   


### 命名规范
	https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/contents/

## 需要具备的功能

	多线程
	解析脚本
	数据库支持	
	封装消息的打包
